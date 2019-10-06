/**
 * LCD.h
 *
 * Library to control the LCD Screen:
 *
 */
#ifndef ENCODER_H
#define ENCODER_H

//===========================================================================
//============================= OTHERS ======================================
//===========================================================================


//===========================================================================
//======================== FUNCTION DECLARATIONS ============================
//===========================================================================

void Read_EncA(void);
void Read_EncB(void);
unsigned int enc_button_read(void);
unsigned int get_incremento_encoder(void);
//===========================================================================
//============================= END MODULE ==================================
//===========================================================================
#endif //ENCODER
