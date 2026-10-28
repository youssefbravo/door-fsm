#ifndef DOOR_H
#define DOOR_H

typedef enum {
    DOOR_UNKNOWN,
    DOOR_CLOSED,
    DOOR_OPENING,
    DOOR_OPEN,
    DOOR_CLOSING,
    DOOR_STOPPED,
    DOOR_FAULT
} door_state_t;

typedef enum {
    EV_CMD_OPEN,
    EV_CMD_CLOSE,
    EV_SWITCH_OPEN,
    EV_SWITCH_CLOSED,
     EV_BEAM_BLOCKED,      
    EV_OUTSIDE_BLOCKED,   
    EV_TIMEOUT
} door_event_t;

door_state_t door_next(door_state_t state, door_event_t event);
const char *door_state_name(door_state_t state);

#endif