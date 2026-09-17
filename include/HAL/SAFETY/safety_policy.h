#ifndef SAFETY_POLICY_H
#define SAFETY_POLICY_H

#include "LIB/STD_TYPES.h"

/* Safety limits are intentionally conservative and must be tuned on the real rover. */
#define SAFETY_STOP_DISTANCE_CM       30U
#define SAFETY_CAUTION_DISTANCE_CM    60U
#define SAFETY_HEARTBEAT_TIMEOUT_MS   1000U
#define SAFETY_SENSOR_TIMEOUT_MS       250U

/* Safety decision returned to the autonomy/controller layer. */
typedef enum
{
    SAFETY_CLEAR = 0,
    SAFETY_CAUTION,
    SAFETY_STOP_SENSOR,
    SAFETY_STOP_LINK,
    SAFETY_STOP_OBSTACLE,
    SAFETY_STOP_INVALID_COMMAND
} SafetyDecision_t;

SafetyDecision_t SAFETY_EvaluateFrontDistance(u16 distance_cm, u32 sensor_age_ms);
SafetyDecision_t SAFETY_EvaluateHeartbeat(u32 heartbeat_age_ms);

#endif
