#include "APP/SAFETY/safety_policy.h"

SafetyDecision_t SAFETY_EvaluateFrontDistance(u16 distance_cm, u32 sensor_age_ms)
{
    if (sensor_age_ms > SAFETY_SENSOR_TIMEOUT_MS)
    {
        return SAFETY_STOP_SENSOR;
    }

    /* A zero distance is treated as invalid, never as a physical obstacle. */
    if (distance_cm == 0U)
    {
        return SAFETY_STOP_SENSOR;
    }

    if (distance_cm <= SAFETY_STOP_DISTANCE_CM)
    {
        return SAFETY_STOP_OBSTACLE;
    }

    if (distance_cm <= SAFETY_CAUTION_DISTANCE_CM)
    {
        return SAFETY_CAUTION;
    }

    return SAFETY_CLEAR;
}

SafetyDecision_t SAFETY_EvaluateHeartbeat(u32 heartbeat_age_ms)
{
    if (heartbeat_age_ms > SAFETY_HEARTBEAT_TIMEOUT_MS)
    {
        return SAFETY_STOP_LINK;
    }

    return SAFETY_CLEAR;
}
