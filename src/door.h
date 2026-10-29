#ifndef DOOR_H
#define DOOR_H

#include <stdbool.h>   /* gives C the bool type: true / false */

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
    EV_BEAM_BLOCKED,      /* laser beam in the doorway broken (inside) */
    EV_OUTSIDE_BLOCKED,   /* HC-SR04 sees something in the swing zone */
    EV_TIMEOUT
} door_event_t;

/* NEW: why something happened, for the display and logs */
typedef enum {
    REASON_NONE,
    REASON_OUTSIDE_BLOCKED,
    REASON_BEAM_BLOCKED,
    REASON_TIMEOUT
} door_reason_t;

/* NEW: a struct bundles two values so one function can return both */
typedef struct {
    door_state_t  state;
    door_reason_t reason;
} door_result_t;

/* CHANGED: returns a result, takes a third input */
door_result_t door_next(door_state_t state, door_event_t event, bool outside_clear);
const char *door_state_name(door_state_t state);
const char *door_reason_name(door_reason_t reason);   /* NEW */

#endif