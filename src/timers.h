/**
 * LCD.h
 *
 * Library to control the LCD Screen:
 *
 */
#ifndef TIMERS_H
#define TIMERS_H

//===========================================================================
//============================= OTHERS ======================================
//===========================================================================
#define TIME_TIMER 200000

//===========================================================================
//======================== FUNCTION DECLARATIONS ============================
//===========================================================================
void reset_sleep_timer(void);
bool check_sleep_timer(void);
//===========================================================================
//============================= END MODULE ==================================
//===========================================================================
#endif //TIMERS
