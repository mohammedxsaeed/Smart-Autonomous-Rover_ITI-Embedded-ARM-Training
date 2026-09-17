#include "LIB/STD_TYPES.h"
#include "LIB/UTILS.h"
#include "MCAL/RCC/MRCC_interface.h"
#include "MCAL/GPIO/GPIO_interface.h"
#include "MCAL/TIM/TIM_interface.h"
#include "MCAL/USART/USART_interface.h"
#include "MCAL/SYSTICK/SYSTICK_interface.h"
#include "MCAL/SPI/SPI_interface.h"
#include "HAL/MOTOR_DRIVER/MOTOR_DRIVER_interface.h"
#include "HAL/BTM/BTM_interface.h"
#include "HAL/ULTRASONIC/ULTRASONIC_interface.h"
#include "HAL/SERVO/SERVO_interface.h"
#include "HAL/BUZZER/BUZZER_interface.h"
#include "HAL/STP/STP_interface.h"
#include "HAL/TFT/ST7735_interface.h"
#include "OS_Scheduler/OS_interface.h"
#include "HAL/SAFETY/safety_policy.h"
#include <math.h>

#define MODE_MANUAL_BT 1
#define MODE_PHONE_AUTONOMY 2
#define DIR_STOP_LED 0
#define DIR_FORWARD 1
#define DIR_BACKWARD 2
#define DIR_LEFT 3
#define DIR_RIGHT 4
#define PHONE_HEARTBEAT_TIMEOUT_TICKS 20U

#define CM_PER_TICK_AT_SPEED   1.0f
#define TICKS_FOR_CM(cm)       ((u8)((cm) / CM_PER_TICK_AT_SPEED))

u8 Current_Mode = MODE_MANUAL_BT;
u8 Current_Direction = DIR_STOP_LED;
u8 Robot_Speed = 50;
u16 Current_Distance = 400;
u8 Servo_Pos = 90;
s8 Servo_Direction = 3;
static u8 Phone_Command_Age = PHONE_HEARTBEAT_TIMEOUT_TICKS;
static u8 DisplayFrame[8];

const u8 Font5x7[28][5] = {
    {0x00, 0x00, 0x00, 0x00, 0x00}, // Space (Index 0)
    {0x7D, 0x00, 0x00, 0x00, 0x00}, // ! (Index 1)
    {0x7E, 0x11, 0x11, 0x11, 0x7E}, // A (Index 2)
    {0x7F, 0x49, 0x49, 0x49, 0x36}, // B
    {0x3E, 0x41, 0x41, 0x41, 0x22}, // C
    {0x7F, 0x41, 0x41, 0x22, 0x1C}, // D
    {0x7F, 0x49, 0x49, 0x49, 0x41}, // E
    {0x7F, 0x09, 0x09, 0x09, 0x01}, // F
    {0x3E, 0x41, 0x49, 0x49, 0x7A}, // G
    {0x7F, 0x08, 0x08, 0x08, 0x7F}, // H
    {0x00, 0x41, 0x7F, 0x41, 0x00}, // I
    {0x20, 0x40, 0x41, 0x3F, 0x01}, // J
    {0x7F, 0x08, 0x14, 0x22, 0x41}, // K
    {0x7F, 0x40, 0x40, 0x40, 0x40}, // L
    {0x7F, 0x02, 0x0C, 0x02, 0x7F}, // M
    {0x7F, 0x04, 0x08, 0x10, 0x7F}, // N
    {0x3E, 0x41, 0x41, 0x41, 0x3E}, // O
    {0x7F, 0x09, 0x09, 0x09, 0x06}, // P
    {0x3E, 0x41, 0x51, 0x21, 0x5E}, // Q
    {0x7F, 0x09, 0x19, 0x29, 0x46}, // R
    {0x46, 0x49, 0x49, 0x49, 0x31}, // S
    {0x01, 0x01, 0x7F, 0x01, 0x01}, // T
    {0x3F, 0x40, 0x40, 0x40, 0x3F}, // U
    {0x1F, 0x20, 0x40, 0x20, 0x1F}, // V
    {0x3F, 0x40, 0x38, 0x40, 0x3F}, // W
    {0x63, 0x14, 0x08, 0x14, 0x63}, // X
    {0x07, 0x08, 0x70, 0x08, 0x07}, // Y
    {0x61, 0x51, 0x49, 0x45, 0x43}  // Z (Index 27)
};

u8 ScrollBuffer[150];
u16 ScrollBufferLength = 0;
u16 CurrentScrollIndex = 0;

void Matrix_PrintString(char* str)
{
    ScrollBufferLength = 0;
    CurrentScrollIndex = 0;

    for(u8 i=0; i<8; i++) ScrollBuffer[ScrollBufferLength++] = 0x00;

    while(*str)
    {
        u8 charIndex = 0;
        if(*str == ' ') charIndex = 0;
        else if(*str == '!') charIndex = 1;
        else if(*str >= 'A' && *str <= 'Z') charIndex = *str - 'A' + 2;

        for(u8 col = 0; col < 5; col++)
        {
            ScrollBuffer[ScrollBufferLength++] = Font5x7[charIndex][col];
        }
        ScrollBuffer[ScrollBufferLength++] = 0x00;
        str++;
    }

    for(u8 i=0; i<8; i++) ScrollBuffer[ScrollBufferLength++] = 0x00;
}
static void StopRover(void)
{
    MOTOR_SHIELD_Stop();
    Current_Direction = DIR_STOP_LED;
}

static u8 ForwardAllowed(void)
{
    if (Current_Distance == 0 || Current_Distance <= SAFETY_STOP_DISTANCE_CM)
    {
        StopRover();
        BUZZER_PlayAlert();
        return 0;
    }
    return 1;
}

void Buzzer_OS_Task(void)
{
    BUZZER_Task();
}
void App_ControlTask(void)
{
    u8 data;
    u8 speed_changed = 0;

    if (BTM_IsDataAvailable())
    {
        data = BTM_ReceiveData();
         Phone_Command_Age = 0;

        if (data == 'F' || data == 'f')
        {
            Current_Mode = MODE_PHONE_AUTONOMY;
            StopRover();
            BUZZER_PlayStartup();
            BTM_SendString((u8 *)"{\"event\":\"mode\",\"mode\":\"PHONE_AUTONOMY\"}\r\n");
            return;
        }

        if (data == 'M' || data == 'm')
        {
            Current_Mode = MODE_MANUAL_BT;
            StopRover();
            BUZZER_PlayModeSwitch();
            BTM_SendString((u8 *)"{\"event\":\"mode\",\"mode\":\"MANUAL\"}\r\n");
            return;
        }

        if (data >= '0' && data <= '9')
        {
            Robot_Speed = (data == '0') ? 100 : (data - '0') * 10;
            speed_changed = 1;
        }

        switch (data)
        {
            case 'W': case 'w':
                if (ForwardAllowed())
                {
                    MOTOR_SHIELD_MoveForward(Robot_Speed);
                    Current_Direction = DIR_FORWARD;
                }
                break;
            case 'S': case 's':
                MOTOR_SHIELD_MoveBackward(Robot_Speed);
                Current_Direction = DIR_BACKWARD;
                BUZZER_PlayReversing();
                break;
            case 'A': case 'a':
                MOTOR_SHIELD_TurnLeft(Robot_Speed);
                Current_Direction = DIR_LEFT;
                break;
            case 'D': case 'd':
                MOTOR_SHIELD_TurnRight(Robot_Speed);
                Current_Direction = DIR_RIGHT;
                break;
            case 'Q': case 'q':
                MOTOR_SHIELD_TurnLeft(Robot_Speed);
                Current_Direction = DIR_LEFT;
                break;
            case 'E': case 'e':
                MOTOR_SHIELD_TurnRight(Robot_Speed);
                Current_Direction = DIR_RIGHT;
                break;
            case 'P': case 'p':
                StopRover();
                break;
            case '+':
                if (Robot_Speed <= 90) Robot_Speed += 10;
                speed_changed = 1;
                break;
            case '-':
                if (Robot_Speed >= 10) Robot_Speed -= 10;
                speed_changed = 1;
                break;
            default:
                break;
        }

        if (speed_changed && Current_Direction != DIR_STOP_LED)
        {
            if (Current_Direction == DIR_FORWARD && !ForwardAllowed()) return;
            if (Current_Direction == DIR_FORWARD) MOTOR_SHIELD_MoveForward(Robot_Speed);
            else if (Current_Direction == DIR_BACKWARD) MOTOR_SHIELD_MoveBackward(Robot_Speed);
            else if (Current_Direction == DIR_LEFT) MOTOR_SHIELD_TurnLeft(Robot_Speed);
            else if (Current_Direction == DIR_RIGHT) MOTOR_SHIELD_TurnRight(Robot_Speed);
        }
    }

    if (Current_Mode == MODE_PHONE_AUTONOMY)
    {
        if (Phone_Command_Age < PHONE_HEARTBEAT_TIMEOUT_TICKS) Phone_Command_Age++;
        if (Phone_Command_Age >= PHONE_HEARTBEAT_TIMEOUT_TICKS) StopRover();
    }
}

void App_RadarTask(void)
{
    Servo_Pos += Servo_Direction;
    if (Servo_Pos >= 150 || Servo_Pos <= 30) {
        Servo_Direction = -Servo_Direction;
    }
    SERVO_SetAngle(Servo_Pos);

    // Send instant radar frame to Mobile App for real-time canvas rendering
    BTM_SendString((u8 *)"{\"event\":\"radar\",\"angle\":");
    BTM_SendNumber(Servo_Pos);
    BTM_SendString((u8 *)",\"distance\":");
    BTM_SendNumber(Current_Distance);
    BTM_SendString((u8 *)"}\r\n");
}

#define RADAR_CX        64
#define RADAR_CY        159
#define RADAR_MAX_CM    100

static void Radar_DrawBackground(void)
{
    ST7735_FillScreen(BLACK);

    ST7735_DrawCircle(RADAR_CX, RADAR_CY, 35,  GREEN);
    ST7735_DrawCircle(RADAR_CX, RADAR_CY, 70,  GREEN);
    ST7735_DrawCircle(RADAR_CX, RADAR_CY, 105, GREEN);
    ST7735_DrawCircle(RADAR_CX, RADAR_CY, 140, GREEN);

    ST7735_DrawLine(0, RADAR_CY, 127, RADAR_CY, GREEN);
    ST7735_DrawLine(RADAR_CX, RADAR_CY, RADAR_CX, 0, GREEN);
    ST7735_DrawLine(RADAR_CX, RADAR_CY, 20, 20, GREEN);
    ST7735_DrawLine(RADAR_CX, RADAR_CY, 108, 20, GREEN);

    ST7735_DrawString(1, RADAR_CY - 39, "35", GREEN, BLACK, 1);
    ST7735_DrawString(1, RADAR_CY - 74, "70", GREEN, BLACK, 1);
    ST7735_DrawString(1, RADAR_CY - 109, "105", GREEN, BLACK, 1);
}

void App_TFTRadarTask(void) {
    static s8  Local_s8LastDir   = 1;
    static u8  Local_u8BlipValid = 0;
    static s16 Local_s16OldBlipX = 0, Local_s16OldBlipY = 0;
    static s16 Local_s16OldLineX = RADAR_CX, Local_s16OldLineY = RADAR_CY; // Track old line

    if ((Servo_Direction > 0 && Local_s8LastDir <= 0) ||
        (Servo_Direction < 0 && Local_s8LastDir >= 0))
    {
        Radar_DrawBackground();
        Local_u8BlipValid = 0;
    }
    Local_s8LastDir = Servo_Direction;
    float angle_rad = Servo_Pos * (3.14159f / 180.0f);
    u16 Local_u16ClampedDist = (Current_Distance > RADAR_MAX_CM) ? RADAR_MAX_CM : Current_Distance;

    s16 Local_s16LineX = RADAR_CX - (s16)(Local_u16ClampedDist * 1.4f * cos(angle_rad));
    s16 Local_s16LineY = RADAR_CY - (s16)(Local_u16ClampedDist * 1.4f * sin(angle_rad));

    if (Local_s16LineX < 0)   Local_s16LineX = 0;
    if (Local_s16LineX > 127) Local_s16LineX = 127;
    if (Local_s16LineY < 0)   Local_s16LineY = 0;
    if (Local_s16LineY > 159) Local_s16LineY = 159;

    ST7735_DrawLine(RADAR_CX, RADAR_CY, (u8)Local_s16OldLineX, (u8)Local_s16OldLineY, BLACK);

    if (Local_u8BlipValid)
    {
        ST7735_DrawFilledCircle(Local_s16OldBlipX, Local_s16OldBlipY, 2, BLACK);
        Local_u8BlipValid = 0;
    }
    ST7735_DrawLine(RADAR_CX, RADAR_CY, (u8)Local_s16LineX, (u8)Local_s16LineY, GREEN);
    Local_s16OldLineX = Local_s16LineX;
    Local_s16OldLineY = Local_s16LineY;
    if (Current_Distance > 0 && Current_Distance <= RADAR_MAX_CM)
    {
        ST7735_DrawFilledCircle(Local_s16LineX, Local_s16LineY, 2, RED);
        Local_s16OldBlipX = Local_s16LineX;
        Local_s16OldBlipY = Local_s16LineY;
        Local_u8BlipValid = 1;

        char Local_s8DistText[7];
        u16  Local_u16D = Current_Distance;

        Local_s8DistText[0] = (Local_u16D >= 100) ? ('0' + (Local_u16D / 100) % 10) : ' ';
        Local_s8DistText[1] = (Local_u16D >= 10)  ? ('0' + (Local_u16D / 10) % 10)  : ' ';
        Local_s8DistText[2] = '0' + (Local_u16D % 10);
        Local_s8DistText[3] = 'C';
        Local_s8DistText[4] = 'M';
        Local_s8DistText[5] = ' ';
        Local_s8DistText[6] = '\0';
        ST7735_DrawString(2, 2, Local_s8DistText, GREEN, BLACK, 1);
    }
}
void App_TelemetryTask(void)
{
    BTM_SendString((u8 *)"{\"distance\":");
    BTM_SendNumber(Current_Distance);

    BTM_SendString((u8 *)",\"angle\":");
    BTM_SendNumber(Servo_Pos);

    BTM_SendString((u8 *)",\"speed\":");
    BTM_SendNumber(Robot_Speed);

    BTM_SendString((u8 *)",\"direction\":\"");

    switch (Current_Direction)
    {
        case DIR_FORWARD:
            BTM_SendString((u8 *)"FORWARD");
            break;

        case DIR_BACKWARD:
            BTM_SendString((u8 *)"BACKWARD");
            break;

        case DIR_LEFT:
            BTM_SendString((u8 *)"LEFT");
            break;

        case DIR_RIGHT:
            BTM_SendString((u8 *)"RIGHT");
            break;

        default:
            BTM_SendString((u8 *)"STOP");
            break;
    }

    BTM_SendString((u8 *)"\",\"mode\":\"");

    if (Current_Mode == MODE_PHONE_AUTONOMY)
    {
        BTM_SendString((u8 *)"PHONE_AUTONOMY");
    }
    else
    {
        BTM_SendString((u8 *)"MANUAL");
    }

    BTM_SendString((u8 *)"\"}\r\n");
}

void App_DisplayTask(void)
{
    static u8 scroll_timer = 0;

    static u8 last_direction = 255;
    if (Current_Direction != last_direction)
    {
        switch (Current_Direction)
        {
            case DIR_FORWARD:  Matrix_PrintString("FORWARD"); break;
            case DIR_BACKWARD: Matrix_PrintString("BACKWARD"); break;
            case DIR_LEFT:     Matrix_PrintString("LEFT"); break;
            case DIR_RIGHT:    Matrix_PrintString("RIGHT"); break;
            case DIR_STOP_LED: Matrix_PrintString("WARNING! OBSTACLE DETECTED"); break;
        }
        last_direction = Current_Direction;
    }

    for(u8 i = 0; i < 8; i++)
    {
        if((CurrentScrollIndex + i) < ScrollBufferLength)
            DisplayFrame[i] = ScrollBuffer[CurrentScrollIndex + i];
        else
            DisplayFrame[i] = 0x00;
    }


    scroll_timer++;
    if (scroll_timer >= 2) // 10ms * 5 = 50ms
    {
        scroll_timer = 0;
        CurrentScrollIndex++;
        if (CurrentScrollIndex >= ScrollBufferLength - 8)
        {
            CurrentScrollIndex = 0;
        }
    }
}
void App_UltrasonicTask(void) {
    Current_Distance = ULTRASONIC_GetDistance();
    if (Current_Distance == 0) Current_Distance = 400;
    if (Current_Distance <= SAFETY_STOP_DISTANCE_CM )   StopRover();
}

int main(void)
{
    MRCC_init();
    MRCC_EN_peripheral_CLK(AHB1_BUS, GPIOA_EN);
    MRCC_EN_peripheral_CLK(AHB1_BUS, GPIOB_EN);
    MRCC_EN_peripheral_CLK(APB1_BUS, APB1_TIM2);
    MRCC_EN_peripheral_CLK(APB1_BUS, APB1_TIM4);
    MRCC_EN_peripheral_CLK(APB1_BUS, APB1_TIM3);
    MRCC_EN_peripheral_CLK(APB2_BUS, APB2_USART1);
    MRCC_EN_peripheral_CLK(APB2_BUS, APB2_SPI1);
    MRCC_EN_peripheral_CLK(APB2_BUS, APB2_TIM1);

    BUZZER_Init();
    BTM_Init();
    MOTOR_SHIELD_Init();
    TIM_PWM_Init(TIM2);
    ULTRASONIC_Init();
    SERVO_Init();
    SERVO_SetAngle(90);
    STP_Init();
    STP_StartAutoRefresh(DisplayFrame, 8);
    ST7735_Init();
    Radar_DrawBackground();

    BTM_SendString((u8 *)"System Ready. 'M' for BT, 'F' for Autopilot\r\n");
    BTM_SendString((u8 *)"{\"event\":\"ready\",\"mode\":\"MANUAL\"}\r\n");
    BUZZER_PlayStartup();

    OS_CreateTask(0, 30,  Buzzer_OS_Task,   0);
    OS_CreateTask(1, 50,  App_ControlTask,  5);
    OS_CreateTask(2, 10,  App_DisplayTask,  10);
    OS_CreateTask(3, 100, App_TelemetryTask, 20);
    OS_CreateTask(4, 50,  App_RadarTask,    25);
    OS_CreateTask(5, 50,  App_UltrasonicTask, 30);
    OS_CreateTask(6, 100, App_TFTRadarTask, 35);

    Start_OS();

    while (1)
    {
        OS_Update();
    }
}
