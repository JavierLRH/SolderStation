#include <SPI.h>
#include <Adafruit_ST7735.h>
#include <Adafruit_GFX.h>

#include "Arduino.h"
#include "configuration.h"
#include "LCD.h"


//===========================================================================
//============================= POSITIONS ===================================
//===========================================================================

#define X_AMB 120
#define Y_AMB 8
#define SIZE_AMBIENT 1

#define X_TEMP 10
#define Y_TEMP 55
#define SIZE_TEMP 3

#define X_LABEL_TEMP 20
#define Y_LABEL_TEMP 40
#define SIZE_LABEL 1

#define X_PWM 15
#define Y_PWM 100
#define SIZE_PWM 1

#define X_SEL 90
#define Y_SEL 55
#define SIZE_SEL_TEMP 3

#define X_LABEL_SEL 95
#define Y_LABEL_SEL 40
#define SIZE_LABEL_SEL_TEMP 1


#define X_POT_MODE 85
#define Y_POT_MODE 80
#define SIZE_POT_MODE 1

//===========================================================================
//============================= COLOURS =====================================
//===========================================================================
#define BLACK    0x0000
#define BLUE     0x001F
#define RED      0xF800
#define GREEN    0x07E0
#define CYAN     0x07FF
#define MAGENTA  0xF81F
#define YELLOW   0xFFE0 
#define WHITE    0xFFFF
#define GREY     0xC618
#define THEME    0x8410

Adafruit_ST7735 tft = Adafruit_ST7735(CS_DISP_PIN,DC_DISP_PIN,RST_DISP_PIN );
//Adafruit_ST7735 tft = Adafruit_ST7735(CS_DISP_PIN, DC_DISP_PIN, MOSI_DISP_PIN, CLK_DISP_PIN, RST_DISP_PIN);


uint16_t lcd_update_timer=0;

void LCD_set_ambient_temperature(int CJ_temp);
void LCD_set_handle_temp(unsigned int main_sensor_temp);
void LCD_set_PWM(unsigned int output);
void LCD_set_SEL_TEMP(unsigned int set_temp);
void LCD_set_pot_increment(unsigned int incremento_enc);
//===========================================================================
//============================= FUNCTIONS ===================================
//===========================================================================


void LCD_init(void)
{ 
  pinMode(BL_DISP_PIN, OUTPUT);
  analogWrite(BL_DISP_PIN, 255);//Display full bright

  tft.initR(INITR_BLACKTAB);
  tft.setRotation(1);
}

void LCD_clear(void)
{
  analogWrite(BL_DISP_PIN,255);
  tft.fillScreen(THEME);
  tft.fillRect(0,0, 160, 20, GREY); //Encabezado
}

void LCD_OFF_screen(void)
{
  analogWrite(BL_DISP_PIN,255);
  tft.fillScreen(THEME); 
  tft.setCursor(45, 50);
  tft.setTextColor(WHITE,THEME);
  tft.setTextSize(4);
  tft.println("OFF");
}

void LCD_update(unsigned int CJ_temp,unsigned int main_sensor_temp, 
                unsigned int output,unsigned int set_temp, unsigned int incremento_enc)
{
  if(lcd_update_timer>=LCD_UP)
  {
   LCD_set_ambient_temperature((unsigned int) CJ_temp);               
   LCD_set_handle_temp((unsigned int) main_sensor_temp);  
   LCD_set_PWM((unsigned int)output);
   LCD_set_SEL_TEMP((unsigned int) set_temp);
   LCD_set_pot_increment(incremento_enc);
  
   lcd_update_timer=0;
  } 
}

void LCD_refresh(void)
{
  lcd_update_timer=LCD_UP;
}

void LCD_timer_update(void)
{
  lcd_update_timer++;
}

void LCD_set_ambient_temperature(int CJ_temp)
{ 
  int a;
  tft.setCursor(X_AMB, Y_AMB);
  tft.setTextColor(BLUE, GREY);
  tft.setTextSize(SIZE_AMBIENT);
  tft.println("T:");
  tft.setCursor(X_AMB+15, Y_AMB);
  tft.println(((int)CJ_temp));
}

void LCD_set_handle_temp(unsigned int main_sensor_temp)
{
  if(main_sensor_temp<100)//Clean the centenar unit
  {
   tft.fillRect(X_TEMP+36, Y_TEMP, 20, 25, THEME); 
  }
  
  tft.setCursor(X_TEMP, Y_TEMP);
  tft.setTextColor(WHITE, THEME);
  tft.setTextSize(SIZE_TEMP);
  tft.println(main_sensor_temp);
  
  
  
  tft.setCursor(X_LABEL_TEMP, Y_LABEL_TEMP);
  tft.setTextSize(SIZE_LABEL);
  tft.println("Sensor");
  
  tft.setCursor(X_TEMP+55, Y_TEMP);
  tft.println("C");
}

void LCD_set_PWM(unsigned int output)
{
  unsigned int lcd_power_output;
  
  //Display the % of PWM used
   tft.setCursor(X_PWM, Y_PWM); 
   tft.setTextColor(WHITE,THEME);
   tft.setTextSize(SIZE_PWM);
   tft.println("Power:");
   tft.setCursor(X_PWM +40, Y_PWM); 
   lcd_power_output=map(output, 0, 255, 0, 100);
   
   tft.println(((double)lcd_power_output),0);
   tft.setCursor(X_PWM +65, Y_PWM); 
   //tft.println("%",0);

  
    
   tft.drawRect(X_PWM, Y_PWM+10,100 ,10,WHITE);
   tft.fillRect(X_PWM, Y_PWM+10,lcd_power_output ,10,WHITE);
   tft.fillRect(X_PWM+lcd_power_output+1, Y_PWM+11,100-lcd_power_output-2 ,8,THEME); 

}

void LCD_set_SEL_TEMP(unsigned int set_temp)
{
  //Display set temp      
   tft.setCursor(X_SEL, Y_SEL);
   tft.setTextColor(WHITE,THEME);
   tft.setTextSize(SIZE_SEL_TEMP);
   tft.println(set_temp);
   
   tft.setCursor(X_LABEL_SEL, Y_LABEL_SEL);
   tft.setTextSize(SIZE_LABEL_SEL_TEMP);
   tft.println("Selected");
   tft.setCursor(X_SEL+55, Y_SEL);
   tft.println("C"); 
}

void LCD_set_pot_increment(unsigned int incremento_enc)
{
  //Incremento giro potenciometro
   tft.setCursor(X_POT_MODE, Y_POT_MODE);
   tft.setTextSize(SIZE_POT_MODE);
   tft.println("Incr: ");
   tft.setCursor(X_POT_MODE+30, Y_POT_MODE);          
   tft.println(incremento_enc);    
   if(incremento_enc<10) 
     {
      tft.setCursor(X_POT_MODE+34, Y_POT_MODE);     
      tft.println("  "); 
   
     }
}
