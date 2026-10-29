#include "unity.h"
#include "door.h"

void setUp(void) {}
void tearDown(void) {}

/* ---- Direction rules (your 4 from before, now checking .state) ---- */

static void test_beam_stops_closing_door(void)
{
    door_result_t r = door_next(DOOR_CLOSING, EV_BEAM_BLOCKED, true);
    TEST_ASSERT_EQUAL(DOOR_STOPPED, r.state);
}

static void test_outside_stops_opening_door(void)
{
    door_result_t r = door_next(DOOR_OPENING, EV_OUTSIDE_BLOCKED, true);
    TEST_ASSERT_EQUAL(DOOR_STOPPED, r.state);
}

static void test_beam_notstopping_opening_door(void)
{
    door_result_t r = door_next(DOOR_OPENING, EV_BEAM_BLOCKED, true);
    TEST_ASSERT_EQUAL(DOOR_OPENING, r.state);
}

static void test_outside_notstopping_closing_door(void)
{
    door_result_t r = door_next(DOOR_CLOSING, EV_OUTSIDE_BLOCKED, true);
    TEST_ASSERT_EQUAL(DOOR_CLOSING, r.state);
}

/* ---- New: refuse to start opening into a blocked zone ---- */

static void test_closed_door_refuses_to_open_when_outside_blocked(void)
{
    door_result_t r = door_next(DOOR_CLOSED, EV_CMD_OPEN, false);
    TEST_ASSERT_EQUAL(DOOR_CLOSED, r.state);
    TEST_ASSERT_EQUAL(REASON_OUTSIDE_BLOCKED, r.reason);
}

static void test_closed_door_opens_when_outside_clear(void)
{
    door_result_t r = door_next(DOOR_CLOSED, EV_CMD_OPEN, true);
    TEST_ASSERT_EQUAL(DOOR_OPENING, r.state);
    TEST_ASSERT_EQUAL(REASON_NONE, r.reason);
}

static void test_stopped_door_refuses_to_open_when_outside_blocked(void)
{
    door_result_t r = door_next(DOOR_STOPPED, EV_CMD_OPEN, false);
    TEST_ASSERT_EQUAL(DOOR_STOPPED, r.state);
    TEST_ASSERT_EQUAL(REASON_OUTSIDE_BLOCKED, r.reason);
}

static void test_beam_stop_reports_doorway_reason(void)
{
    door_result_t r = door_next(DOOR_CLOSING, EV_BEAM_BLOCKED, true);
    TEST_ASSERT_EQUAL(REASON_BEAM_BLOCKED, r.reason);
    TEST_ASSERT_EQUAL_STRING("Doorway blocked", door_reason_name(r.reason));
}
/* ---- Startup: trust only real evidence ---- */

static void test_unknown_learns_closed_from_switch(void)
{
    door_result_t r = door_next(DOOR_UNKNOWN, EV_SWITCH_CLOSED, true);
    TEST_ASSERT_EQUAL(DOOR_CLOSED, r.state);
    TEST_ASSERT_EQUAL(REASON_NONE, r.reason);
}

static void test_unknown_learns_open_from_switch(void)
{
    door_result_t r = door_next(DOOR_UNKNOWN, EV_SWITCH_OPEN, true);
    TEST_ASSERT_EQUAL(DOOR_OPEN, r.state);
    TEST_ASSERT_EQUAL(REASON_NONE, r.reason);
}

/* ---- Timeouts: every moving state has a safety net ---- */

static void test_timeout_while_opening_is_fault(void)
{
    door_result_t r = door_next(DOOR_OPENING, EV_TIMEOUT, true);
    TEST_ASSERT_EQUAL(DOOR_FAULT, r.state);
    TEST_ASSERT_EQUAL(REASON_TIMEOUT, r.reason);
}

static void test_timeout_while_closing_is_fault(void)
{
    door_result_t r = door_next(DOOR_CLOSING, EV_TIMEOUT, true);
    TEST_ASSERT_EQUAL(DOOR_FAULT, r.state);
    TEST_ASSERT_EQUAL(REASON_TIMEOUT, r.reason);
}

/* ---- FAULT is sticky ---- */

static void test_fault_ignores_open_command(void)
{
    door_result_t r = door_next(DOOR_FAULT, EV_CMD_OPEN, true);
    TEST_ASSERT_EQUAL(DOOR_FAULT, r.state);
}

/* ---- Commands move the door when they should ---- */

static void test_stopped_door_resumes_on_close_command(void)
{
    door_result_t r = door_next(DOOR_STOPPED, EV_CMD_CLOSE, true);
    TEST_ASSERT_EQUAL(DOOR_CLOSING, r.state);
    TEST_ASSERT_EQUAL(REASON_NONE, r.reason);
}

static void test_open_door_closes_on_command(void)
{
    door_result_t r = door_next(DOOR_OPEN, EV_CMD_CLOSE, true);
    TEST_ASSERT_EQUAL(DOOR_CLOSING, r.state);
    TEST_ASSERT_EQUAL(REASON_NONE, r.reason);
}
int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_beam_stops_closing_door);
    RUN_TEST(test_outside_stops_opening_door);
    RUN_TEST(test_beam_notstopping_opening_door);
    RUN_TEST(test_outside_notstopping_closing_door);
    RUN_TEST(test_closed_door_refuses_to_open_when_outside_blocked);
    RUN_TEST(test_closed_door_opens_when_outside_clear);
    RUN_TEST(test_stopped_door_refuses_to_open_when_outside_blocked);
    RUN_TEST(test_beam_stop_reports_doorway_reason);
    RUN_TEST(test_unknown_learns_closed_from_switch);
    RUN_TEST(test_unknown_learns_open_from_switch);
    RUN_TEST(test_timeout_while_opening_is_fault);
    RUN_TEST(test_timeout_while_closing_is_fault);
    RUN_TEST(test_fault_ignores_open_command);
    RUN_TEST(test_stopped_door_resumes_on_close_command);
    RUN_TEST(test_open_door_closes_on_command);
    return UNITY_END();
}