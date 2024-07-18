#include<stdio.h>
#include<stdint.h>


enum BombSignals { /* all signals for the Bomb FSM */
UP_SIG,
DOWN_SIG,
ARM_SIG,
TICK_SIG
};
/*....................................................................*/
enum BombStates { /* all states for the Bomb FSM */
SETTING_STATE,
TIMING_STATE
};
/*.....................................................................*/
typedef struct EventTag {
 uint16_t sig; /* signal of the event */
/* add event parameters by derivation from the Event structure... */
} Event;
 typedef struct TickEvtTag {
 Event super; /* derive from the Event structure */
 uint8_t fine_time; /* the fine 1/10 s counter */
} TickEvt;
/*.....................................................................*/
 typedef struct Bomb1Tag { /* the Bomb FSM */
 uint8_t state; /* the scalar state-variable */
 uint8_t timeout; /* number of seconds till explosion */
 uint8_t code; /* currently entered code to disarm the bomb */
 uint8_t defuse; /* secret defuse code to disarm the bomb */
} Bomb1;
/* macro for taking a state transition */
 #define TRAN(target_) (me->state = (uint8_t)(target_))
/*....................................................................*/
void Bomb1_ctor(Bomb1 *me, uint8_t defuse) { /* the "constructor" */
me->defuse = defuse; /* the defuse code is assigned at instantiation */
}
/*.....................................................................*/
void Bomb1_init(Bomb1 *me) { /* initial transition */
me->timeout = INIT_TIMEOUT;/* timeout is initialized in initial tran. */
TRAN(SETTING_STATE);
}
/*.....................................................................*/
void Bomb1_dispatch(Bomb1 *me, Event const *e) { /* dispatching */
switch (me->state) {
case SETTING_STATE: {
switch (e->sig) {
case UP_SIG: { /* internal transition with a guard */
if (me->timeout < 60) { /* guard condition */
++me->timeout;
BSP_display(me->timeout);
}
break;
}
case DOWN_SIG: { /* internal transition with a guard */
if (me->timeout > 1) {
--me->timeout;