#include "Arduino.h"
#include "configuration.h"



//===========================================================================
//============================= POSITIONS ===================================
//===========================================================================


//===========================================================================
//============================= FUNCTIONS ===================================
//===========================================================================

double main_sensor_temp_measurement(void)
{
  uint16_t temp = 0;
  double d_temp = 0;
    
  //Adquisición de temperatura
#if defined(Hakko)
   
    // Discard first reading
    analogRead(PTC_Hakko);
    
    // Read temperature four times
    temp += analogRead(PTC_Hakko);
    temp += analogRead(PTC_Hakko);
    temp += analogRead(PTC_Hakko);
    temp += analogRead(PTC_Hakko);
    
    // Four readings average
    temp = (temp >> 2);
    
    // Get the value in degrees celsius
    d_temp = ((EQUATION_M*temp) + EQUATION_N);
    //Serial.println(temp);
#elif defined(Weller)
       
      
      // Discard first reading
      analogRead(TC);
      
      // Read temperature four times
      temp += analogRead(TC);
      temp += analogRead(TC);
      temp += analogRead(TC);
      temp += analogRead(TC);
      
      // Four readings average
      temp = (temp >> 2);
      // Get the value in degrees celsius
      d_temp = ((EQUATION_M*temp) + EQUATION_N); 
#endif
    return d_temp;
}

double ambient_sensor_temp_measurement(void)
{
  return ((EQUATION_CJC_M*analogRead(CJC)) + EQUATION_CJC_N);
  //Serial.println(CJ_temp);
}
