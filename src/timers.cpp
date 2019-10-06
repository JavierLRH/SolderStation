#include "Arduino.h"
#include "configuration.h"


unsigned long on_time =0; //Time refresh when a comand is entered.

//===========================================================================
//============================= FUNCTIONS ===================================
//===========================================================================

void reset_sleep_timer(void)
{
  on_time=millis();
}

bool check_sleep_timer(void)
{
  if((millis() - on_time) >= TURN_OFF_TIME)//on_time load the value of millis when a temp is inserterd
  {
    return true;
  }
  else
  {
    return false;
  }
}
