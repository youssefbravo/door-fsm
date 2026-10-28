#include "door.h"

/*
 * door_next: the door's rules.
 * Input:  the current state + one event.
 * Output: the next state.
 * No hardware, no global variables: same inputs always give the same output.
 */
door_state_t door_next(door_state_t state, door_event_t event)
{
    switch (state) {

    case DOOR_UNKNOWN:
        /* Just powered on: we don't know where the door is.
         * An end switch is evidence; a command starts a move. */
        if (event == EV_SWITCH_CLOSED) {
            return DOOR_CLOSED;
        }
        if (event == EV_SWITCH_OPEN) {
            return DOOR_OPEN;
        }
        if (event == EV_CMD_OPEN) {
            return DOOR_OPENING;
        }
        if (event == EV_CMD_CLOSE) {
            return DOOR_CLOSING;
        }
        return state;

    case DOOR_CLOSED:
        /* Only an open command moves a closed door. */
        if (event == EV_CMD_OPEN) {
            return DOOR_OPENING;
        }
        return state;

    case DOOR_OPENING:
        /* Moving up. Done when the open switch fires.
         * No switch in time = something broke. */
        if (event == EV_OUTSIDE_BLOCKED) {
            return DOOR_STOPPED;  /* safety stop */
        }
        if (event == EV_SWITCH_OPEN) {
            return DOOR_OPEN;
        }
        if (event == EV_TIMEOUT) {
            return DOOR_FAULT;
        }
        return state;  /* commands ignored while moving */

    case DOOR_OPEN:
        /* Only a close command moves an open door. */
        if (event == EV_CMD_CLOSE) {
            return DOOR_CLOSING;
        }
        return state;

    case DOOR_CLOSING:
        /* Moving down: the one direction that can trap something. */
        if (event == EV_SWITCH_CLOSED) {
            return DOOR_CLOSED;
        }
       
         if(event == EV_BEAM_BLOCKED){
            return DOOR_STOPPED;
        }
        if (event == EV_TIMEOUT) {
            return DOOR_FAULT;
        }
        return state;

    case DOOR_STOPPED:
        /* Stopped by an obstacle. Clearing the obstacle does NOT restart it:
         * only a fresh command moves the door again. */
        if (event == EV_CMD_OPEN) {
            return DOOR_OPENING;
        }
        if (event == EV_CMD_CLOSE) {
            return DOOR_CLOSING;
        }
        return state;

    case DOOR_FAULT:
        /* Sticky: a human must check the hardware first. */
        return state;
    }

    /* Only reached if state holds a value outside the enum
     * (memory corruption, a bad cast). Fail safe: treat it as a fault. */
    return DOOR_FAULT;
}

/* Turns a state into text for logs and the display. */
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
    return "INVALID";  /* never return NULL: a caller printing it would crash */
}