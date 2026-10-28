#include "unity.h"   // the test framework
#include "door.h"    // your code: door_next, the states, the events

/* Unity runs setUp before every test and tearDown after.
   We have nothing to prepare, but Unity requires both to exist. */
void setUp(void) {}
void tearDown(void) {}

/* ---- Test 1: the beam stops a closing door ---- */
static void test_beam_stops_closing_door(void)
{
    door_state_t result = door_next(DOOR_CLOSING, EV_BEAM_BLOCKED);  // ask
    TEST_ASSERT_EQUAL(DOOR_STOPPED, result);                         // check
}

/* ---- Test 2: the outside sensor stops an opening door ---- */
static void test_outside_stops_opening_door(void)
{
    door_state_t result = door_next(DOOR_OPENING, EV_OUTSIDE_BLOCKED);
    TEST_ASSERT_EQUAL(DOOR_STOPPED, result);
}

/* ---- Test 3: YOUR TURN ----
   The beam must NOT stop an opening door.
   Start: DOOR_OPENING   Event: EV_BEAM_BLOCKED   Expected: DOOR_OPENING */
   static void test_beam_notstopping_opening_door(void){
    door_state_t result = door_next(DOOR_OPENING, EV_BEAM_BLOCKED);
    TEST_ASSERT_EQUAL(DOOR_OPENING, result);

   }


/* ---- Test 4: YOUR TURN ----
   The outside sensor must NOT stop a closing door.
   Start: DOOR_CLOSING   Event: EV_OUTSIDE_BLOCKED   Expected: DOOR_CLOSING */
 static void test_outside_notstopping_closing_door(void){
    door_state_t result = door_next(DOOR_CLOSING, EV_OUTSIDE_BLOCKED);
    TEST_ASSERT_EQUAL(DOOR_CLOSING, result);

   }

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_beam_notstopping_opening_door);
    RUN_TEST(test_outside_notstopping_closing_door);
    RUN_TEST(test_beam_stops_closing_door);
    RUN_TEST(test_outside_stops_opening_door);
    return UNITY_END();
}