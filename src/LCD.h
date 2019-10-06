/**
 * LCD.h
 *
 * Library to control the LCD Screen:
 *
 */
#ifndef LCD_H
#define LCD_H

//===========================================================================
//============================= OTHERS ======================================
//===========================================================================
#define LCD_UP 400000/TIME_TIMER  //LCD Refresh 400ms timner 200ms

//===========================================================================
//======================== FUNCTION DECLARATIONS ============================
//===========================================================================
void LCD_init(void);
void LCD_clear(void);
void LCD_OFF_screen(void);
void LCD_update(unsigned int CJ_temp,unsigned int main_sensor_temp, 
                unsigned int output,unsigned int set_temp, unsigned int incremento_enc);
void LCD_refresh(void);
void LCD_timer_update(void);

//===========================================================================
//============================= END MODULE ==================================
//===========================================================================
#endif //LCD_H
