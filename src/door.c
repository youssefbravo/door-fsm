#include "door.h"

/* Small helper: packs a state and a reason into one result.
   'static' = only this file can use it. */
static door_result_t result(door_state_t state, door_reason_t reason)
{
    door_result_t r = { state, reason };
    return r;
}

/*
 * door_next: the door's rules.
 * Inputs:  current state, one event, and whether the outside swing zone is clear.
 * Output:  the next state + the reason (REASON_NONE when nothing went wrong).
 */
door_result_t door_next(door_state_t state, door_event_t event, bool outside_clear)
{
    /* Rule shared by every resting state: never START opening into a blocked zone.
     * Stay where you are and report why. */
    if (event == EV_CMD_OPEN && !outside_clear &&
        (state == DOOR_UNKNOWN || state == DOOR_CLOSED || state == DOOR_STOPPED)) {
        return result(state, REASON_OUTSIDE_BLOCKED);
    }

    switch (state) {

    case DOOR_UNKNOWN:
        if (event == EV_SWITCH_CLOSED) return result(DOOR_CLOSED, REASON_NONE);
        if (event == EV_SWITCH_OPEN)   return result(DOOR_OPEN, REASON_NONE);
        if (event == EV_CMD_OPEN)      return result(DOOR_OPENING, REASON_NONE);
        if (event == EV_CMD_CLOSE)     return result(DOOR_CLOSING, REASON_NONE);
        return result(state, REASON_NONE);

    case DOOR_CLOSED:
        if (event == EV_CMD_OPEN)      return result(DOOR_OPENING, REASON_NONE);
        return result(state, REASON_NONE);

    case DOOR_OPENING:
        /* Swinging out: the outside zone is what we could hit. */
        if (event == EV_OUTSIDE_BLOCKED) return result(DOOR_STOPPED, REASON_OUTSIDE_BLOCKED);
        if (event == EV_SWITCH_OPEN)     return result(DOOR_OPEN, REASON_NONE);
        if (event == EV_TIMEOUT)         return result(DOOR_FAULT, REASON_TIMEOUT);
        return result(state, REASON_NONE);

    case DOOR_OPEN:
        if (event == EV_CMD_CLOSE)     return result(DOOR_CLOSING, REASON_NONE);
        return result(state, REASON_NONE);

    case DOOR_CLOSING:
        /* Coming down through the doorway: the beam is what we could hit. */
        if (event == EV_BEAM_BLOCKED)  return result(DOOR_STOPPED, REASON_BEAM_BLOCKED);
        if (event == EV_SWITCH_CLOSED) return result(DOOR_CLOSED, REASON_NONE);
        if (event == EV_TIMEOUT)       return result(DOOR_FAULT, REASON_TIMEOUT);
        return result(state, REASON_NONE);

    case DOOR_STOPPED:
        /* Only a fresh command moves it again. */
        if (event == EV_CMD_OPEN)      return result(DOOR_OPENING, REASON_NONE);
        if (event == EV_CMD_CLOSE)     return result(DOOR_CLOSING, REASON_NONE);
        return result(state, REASON_NONE);

    case DOOR_FAULT:
        return result(state, REASON_NONE);
    }

    return result(DOOR_FAULT, REASON_NONE);
}

const char *door_state_name(door_state_t state)
{
    switch (state) {
    case DOOR_UNKNOWN: return "UNKNOWN";
    case DOOR_CLOSED:  return "CLOSED";
    case DOOR_OPENING: return "OPENING";
    case DOOR_OPEN:    return "OPEN";
    case DOOR_CLOSING: return "CLOSING";
    case DOOR_STOPPED: return "STOPPED";
    case DOOR_FAULT:   return "FAULT";
    }
    return "INVALID";
}

/* Text the OLED will show the user */
const char *door_reason_name(door_reason_t reason)
{
    switch (reason) {
    case REASON_NONE:            return "OK";
    case REASON_OUTSIDE_BLOCKED: return "Outside blocked";
    case REASON_BEAM_BLOCKED:    return "Doorway blocked";
    case REASON_TIMEOUT:         return "End switch timeout";
    }
    return "INVALID";
}