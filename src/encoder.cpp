#include "Arduino.h"
#include "configuration.h"
#include "LCD.h"
#include "timers.h"

//Boton
#define TIME_MENU 3000000/TIME_TIMER //3s
#define TIME_SENS 200000/TIME_TIMER   //0.2s

#define INTERRUPTION_TIME_GUARD 40 //ms

uint16_t tiempo_pulsado=0;
uint8_t incremento_enc=10; //Incremento del encoder

//Encoder
volatile byte aFlag = 0; // let's us know when we're expecting a rising edge on pinA to signal that the encoder has arrived at a detent
volatile byte bFlag = 0; // let's us know when we're expecting a rising edge on pinB to signal that the encoder has arrived at a detent (opposite direction to when aFlag is set)

uint16_t old_timeA=0;
uint16_t old_timeB=0;

void move_encoder(int increase);

//===========================================================================
//============================= POSITIONS ===================================
//===========================================================================


//===========================================================================
//============================= FUNCTIONS ===================================
//===========================================================================

unsigned int enc_button_read(void)
{
  unsigned int menu_intro = false;
  if (digitalRead(SW_ENC_PIN)==0)
  {
    tiempo_pulsado++;
    if (tiempo_pulsado>TIME_MENU)
    {
      menu_intro=true;
      tiempo_pulsado=0;
      Serial.println("Entro en Menu");
    } 
  }
  else
  {
    if (tiempo_pulsado>=TIME_SENS) //Dejo de pulsar
    {        
        if(incremento_enc==1)
        {
          incremento_enc=10; //Coarse
        }
        else
        {
          incremento_enc=1;  //Fine
        }
        LCD_refresh();      
    }
    tiempo_pulsado=0;
  }
  return menu_intro;
}


void Read_EncA()
{
  boolean A_val;
  boolean B_val;
  
  if((millis()-old_timeA)>INTERRUPTION_TIME_GUARD)
  {
    old_timeA=millis();     
    if (bFlag) 
    {
      A_val = digitalRead(DT_ENC_PIN);
      B_val = digitalRead(CLK_ENC_PIN);

      if((A_val == 0) or (B_val == 0))
      {
        move_encoder(+1);
        LCD_refresh();
      }      
      bFlag = 0; //reset flags for the next turn
      aFlag = 0; //reset flags for the next turn

      reset_sleep_timer();
    }
    else 
    {
      aFlag = 1;
    }
  }
  
}


void Read_EncB()
{
  boolean A_val;
  boolean B_val;  
  if((millis()-old_timeB)>INTERRUPTION_TIME_GUARD)
  {
    old_timeB=millis(); 
  
    if(aFlag) //check that we have both pins at detent (HIGH) and that we are expecting detent on this pin's rising edge
    { 
        A_val = digitalRead(DT_ENC_PIN);
        B_val = digitalRead(CLK_ENC_PIN);
  
        if((A_val == 0) or (B_val == 0))
        {
          move_encoder(-1);
          LCD_refresh();
        }  
        reset_sleep_timer();
        
        bFlag = 0; //reset flags for the next turn
        aFlag = 0; //reset flags for the next turn
    }
    else
    {
       bFlag = 1; //signal that we're expecting pinB to signal the transition to detent from free rotation
    }
  }
  else
  {
    /*NOTING*/
  }

 }

 unsigned int get_incremento_encoder(void)
 {
  return incremento_enc;
 }
