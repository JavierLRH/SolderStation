#include <TimerOne.h>
#include <PID_v1.h>

#include "configuration.h"
#include "LCD.h"
#include "sensor.h"
#include "encoder.h"
#include "timers.h"

//States
#define OFF_INTRO 0
#define OFF 1
#define ON_INTRO 2
#define ON 3
#define BASE_INTRO 4
#define BASE 5
#define MENU_INTRO 6
#define MENU 7


uint8_t state=OFF_INTRO;



//Global Variables
double TC_temp=0; //Temp in the termocuple
double PTC_H_temp=0;
double PTC_W_temp=0;
double CJ_temp=0;
double main_sensor_temp=0;

double set_temp =400; //Desired temp
double base_temp=100; //Temp when the handle is in the base
double output=255; //Output of the PID


uint8_t menu_pos=0;


//Start PID
PID heater(&main_sensor_temp, &output, &set_temp, KP_VAL,KI_VAL, KD_VAL, DIRECT);//REVERSE SI SE USA UN PWM NO INVERTIDO

PID base(&main_sensor_temp, &output, &base_temp, KP_VAL,KI_VAL, KD_VAL, DIRECT);


//functions
void Read_EncA(void);
void Read_EncB(void);
void ISR_Timer1(void);
void State_Machine(void);


void setup() 

{
  Timer1.initialize(TIME_TIMER);//200ms
  Timer1.attachInterrupt(ISR_Timer1);
  
  Serial.begin(9600);
  Serial.println("Puerto configurado");
  
  //define pins//
  
  //Heater
  pinMode(HEATER_PIN, OUTPUT);
  
  OCR4D=0; //Dutty cicle Salida invertida que se vuelve a invertir con los transistores
  TCCR4C = ((1 << 0)|(1 << 2) | (0 << 3)); //Configurar PWM Timer4D
  TCCR4B=((1<<2)|(1<<1)); //Arrancar timer 1KHz
  //analogWrite(HEATER_PIN, 255); //Output off
  
  
  
  
  //Analog input defined by default
  
  //encoder
  pinMode(SW_ENC_PIN, INPUT_PULLUP);
  pinMode(CLK_ENC_PIN, INPUT);
  pinMode(DT_ENC_PIN, INPUT);
  
  attachInterrupt(digitalPinToInterrupt(DT_ENC_PIN),Read_EncA,FALLING);
  attachInterrupt(digitalPinToInterrupt(CLK_ENC_PIN),Read_EncB,FALLING);
  
  //Base
  pinMode(BASE_PIN, INPUT_PULLUP);
  
  
  
  
  
  
  //PID SETUP//
  heater.SetMode(AUTOMATIC);
  heater.SetSampleTime(TIME_TIMER/1000);
  heater.SetOutputLimits(PWM_MIN, PWM_MAX);
  
  base.SetMode(AUTOMATIC);
  base.SetSampleTime(TIME_TIMER/1000);
  base.SetOutputLimits(PWM_MIN, PWM_MAX);
  
  
  
  //LCD SETUP//
  LCD_init();

}

void loop()

{     
    State_Machine();//Maquina de estados 
}


void State_Machine()
{
switch  (state)
    {
      
    case ON_INTRO: 
      LCD_clear();
      state=ON;    
    break;
       
    case ON:
     //PID         
     heater.Compute(); //Calcule the value of the output each sample time  
     OCR4D=output;//DUTTY CICLE     
     //analogWrite(HEATER_PIN, output);

     //LCD
     LCD_update((unsigned int) CJ_temp,(unsigned int) main_sensor_temp, 
                (unsigned int) output,(unsigned int) set_temp, 
                get_incremento_encoder());
     
     //Base mode
      if (digitalRead(BASE_PIN)==0) //The handle is place in the base 
      {
          state=BASE;  
      } 
      
      // Auto turn-off
      if(check_sleep_timer() == true)
      {
        state = OFF_INTRO;      
      }  
      
    break;
    
    case OFF_INTRO:
      LCD_OFF_screen();
      state=OFF;
    break;  
      
    case OFF:
        OCR4D=0;
        //analogWrite(HEATER_PIN, 255); //Output off  
    break;
    
    case BASE:
      //sensor_temp=readTemp(); //Read te temperature of the termocouple
                             //Display handle temp
              
      base.Compute(); //Calcule the value of the output WITH THE BASE TEMP
      OCR4D=output; //duty cicle
      //analogWrite(HEATER_PIN, output);
      
      // Auto turn-off
      if(check_sleep_timer() == true)
      {
        state = OFF_INTRO;      
      }  
        
      if (digitalRead(BASE_PIN)==1) //The handle isn`t place in the base  
       state=ON;
    break;
         
    case MENU:
      OCR4D=0;//Output off
      //analogWrite(HEATER_PIN, 255); //Output off
    break;
    }
}


void ISR_Timer1()
{
  //Serial.println(state);
  switch (state)
  {
    case ON:
      //UPDATE TIME LCD
      LCD_timer_update();
      
      //Handle temperature
      main_sensor_temp = main_sensor_temp_measurement();      
      //Temperatura ambiente
      CJ_temp= ambient_sensor_temp_measurement();
  
      //Encoder button fn  
      if (enc_button_read() == true)
      {
        state=MENU_INTRO;
      }
      else
      {
        //Se realizan funciones internas
      }
      
    break;
  }
}

void move_encoder(int increase)
{  
  switch(state)
      {
        case MENU:
          menu_pos +=increase;
        break;
         
       case ON:
        set_temp= set_temp +=(int)(increase*get_incremento_encoder());
      
        if(set_temp<MIN_TEMP)
          set_temp=MIN_TEMP;
        else if (set_temp>MAX_TEMP)
          set_temp=MAX_TEMP;
        else
        /*NOTHING*/
       break;
      
      case OFF:
       state=ON_INTRO; 
      break;  
     }
}
