#include "LIB/STD_TYPES.h"
#include "LIB/UTILS.h"
#include "MCAL/GPIO/GPIO_interface.h"
#include "MCAL/SYSTICK/SYSTICK_interface.h"
#include "HAL/BUZZER/BUZZER_config.h"
#include "HAL/BUZZER/BUZZER_interface.h"

typedef enum {
    BUZZER_IDLE = 0,
    BUZZER_STARTUP,
    BUZZER_REVERSING,
    BUZZER_ALERT,
    BUZZER_MODESWITCH
} Buzzer_State_t;

static Buzzer_State_t Current_Sound = BUZZER_IDLE;
static u16 Step_Counter = 0;
static u16 Tone_Timer = 0;

void BUZZER_Init(void)
{
    GPIO_InitConfig_t BuzzerPin = {
        .Mode  = OUTPUT,
        .OType = PUSH_PULL,
        .Speed = SPEED_MEDIUM,
        .Pupd  = NONE
    };
    BuzzerPin.Pin = BUZZER_PIN;
    GPIO_Init(BUZZER_PORT, &BuzzerPin);
    GPIO_SetPinValue(BUZZER_PORT, BUZZER_PIN, LOW);
}

void BUZZER_On(void)
{
    GPIO_SetPinValue(BUZZER_PORT, BUZZER_PIN, HIGH);
}

void BUZZER_Off(void)
{
    GPIO_SetPinValue(BUZZER_PORT, BUZZER_PIN, LOW);
}

void BUZZER_Beep(u32 Copy_u32DelayMs)
{
    GPIO_SetPinValue(BUZZER_PORT, BUZZER_PIN, HIGH);
    Delay_ms(Copy_u32DelayMs);
    GPIO_SetPinValue(BUZZER_PORT, BUZZER_PIN, LOW);
}

static void Tone(u32 frequency, u32 duration_ms)
{
    if (frequency == 0) {
        Delay_ms(duration_ms);
        return;
    }

    u32 period_us = 1000000 / frequency;
    u32 half_period = period_us / 2;
    u32 cycles = (duration_ms * 1000) / period_us;

    for (u32 i = 0; i < cycles; i++)
    {
        GPIO_SetPinValue(BUZZER_PORT, BUZZER_PIN, HIGH);
        Delay_us(half_period);
        GPIO_SetPinValue(BUZZER_PORT, BUZZER_PIN, LOW);
        Delay_us(half_period);
    }
}

void BUZZER_SoundStartup(void)
{
    Tone(800, 200);
    Delay_ms(50);
    Tone(1200, 200);
    Delay_ms(50);
    Tone(1600, 300);
}

void BUZZER_SoundReversing(void)
{
    Tone(1000, 300);
    Delay_ms(200);
    Tone(1000, 300);
    Delay_ms(200);
}

void BUZZER_SoundObstacleAlert(void)
{
    Tone(2000, 150);
    Delay_ms(80);
    Tone(2000, 150);
    Delay_ms(80);
    Tone(2000, 250);
}

void BUZZER_SoundModeSwitch(void)
{
    Tone(1500, 200);
    Delay_ms(100);
    Tone(2000, 200);
}

void BUZZER_PlayStartup(void)
{
    if (Current_Sound == BUZZER_IDLE) {
        Current_Sound = BUZZER_STARTUP;
        Step_Counter = 0;
        Tone_Timer = 0;
    }
}

void BUZZER_PlayReversing(void)
{
    if (Current_Sound == BUZZER_IDLE) {
        Current_Sound = BUZZER_REVERSING;
        Step_Counter = 0;
        Tone_Timer = 0;
    }
}

void BUZZER_PlayAlert(void)
{
    if (Current_Sound == BUZZER_IDLE) {
        Current_Sound = BUZZER_ALERT;
        Step_Counter = 0;
        Tone_Timer = 0;
    }
}

void BUZZER_PlayModeSwitch(void)
{
    if (Current_Sound == BUZZER_IDLE) {
        Current_Sound = BUZZER_MODESWITCH;
        Step_Counter = 0;
        Tone_Timer = 0;
    }
}

void BUZZER_Task(void)
{
    if (Current_Sound == BUZZER_IDLE) {
        return;
    }

    switch (Current_Sound)
    {
        case BUZZER_STARTUP:
            if (Step_Counter == 0)      { GPIO_SetPinValue(BUZZER_PORT, BUZZER_PIN, HIGH); }
            else if (Step_Counter == 2) { GPIO_SetPinValue(BUZZER_PORT, BUZZER_PIN, LOW); }
            else if (Step_Counter == 5) { GPIO_SetPinValue(BUZZER_PORT, BUZZER_PIN, HIGH); }
            else if (Step_Counter == 7) { GPIO_SetPinValue(BUZZER_PORT, BUZZER_PIN, LOW); }
            else if (Step_Counter == 10){ GPIO_SetPinValue(BUZZER_PORT, BUZZER_PIN, HIGH); }
            else if (Step_Counter == 15){
                GPIO_SetPinValue(BUZZER_PORT, BUZZER_PIN, LOW);
                Current_Sound = BUZZER_IDLE;
            }
            break;

        case BUZZER_REVERSING:
            if (Step_Counter == 0)      { GPIO_SetPinValue(BUZZER_PORT, BUZZER_PIN, HIGH); }
            else if (Step_Counter == 4) { GPIO_SetPinValue(BUZZER_PORT, BUZZER_PIN, LOW); }
            else if (Step_Counter == 12){
                Step_Counter = 0;
                return;
            }
            break;

        case BUZZER_ALERT:
            if (Step_Counter % 2 == 0) {
            	GPIO_TogglePin(BUZZER_PORT, BUZZER_PIN);
            }
            if (Step_Counter >= 10) {
                GPIO_SetPinValue(BUZZER_PORT, BUZZER_PIN, LOW);
                Current_Sound = BUZZER_IDLE;
            }
            break;

        case BUZZER_MODESWITCH:
            if (Step_Counter == 0)      { GPIO_SetPinValue(BUZZER_PORT, BUZZER_PIN, HIGH); }
            else if (Step_Counter == 2) { GPIO_SetPinValue(BUZZER_PORT, BUZZER_PIN, LOW); }
            else if (Step_Counter == 4) { GPIO_SetPinValue(BUZZER_PORT, BUZZER_PIN, HIGH); }
            else if (Step_Counter == 6) {
                GPIO_SetPinValue(BUZZER_PORT, BUZZER_PIN, LOW);
                Current_Sound = BUZZER_IDLE;
            }
            break;

        default:
            Current_Sound = BUZZER_IDLE;
            break;
    }

    Step_Counter++;
}
