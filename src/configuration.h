/**
 * Configuration.h
 *
 * Basic settings such as:
 *
 * - PinOut
 * - Heater Constant
 * - PID
 */
#ifndef CONFIGURATION_H
#define CONFIGURATION_H

//===========================================================================
//============================= Getting Started =============================
//===========================================================================

//===========================================================================
//============================= PINOUT ======================================
//===========================================================================
//Analog input
#define TC A5
#define PTC_Weller A4
#define PTC_Hakko A3
#define CJC A2


//PWM out
#define HEATER_PIN 12
//#define HEATER_PIN 6 Complemento PWM del pin 12

//Encoder
#define SW_ENC_PIN 0
#define CLK_ENC_PIN 3
#define DT_ENC_PIN 2

//LCD
#define BL_DISP_PIN 9
#define CS_DISP_PIN 5
#define DC_DISP_PIN 10
#define MOSI_DISP_PIN ICSP-4
#define CLK_DISP_PIN ICSP-3
#define RST_DISP_PIN 13
 
#define BASE_PIN A0

//===========================================================================
//============================= STATION MODE ================================
//===========================================================================
#define Hakko
//#define Weller

//===========================================================================
//============================= TEMPERATURE MEASURE =========================
//===========================================================================
#if defined(Hakko)
//Ganancia del operacional 33
//Fn transferencia PTC 66+0.203(T-100)
//Fuente de corriente 1mA
//Fn transferencia tensión 2.178 - 0.67 + 0.0067T = 1.508 + 0.0067T (V/T)
//Fn transferencia temperatura -225.075 + 149.253V
//Fn transferencia temperatura -225.075 + 0.72878 [D]
#define EQUATION_M 0.72878
#define EQUATION_N -225.075

#elif defined(Weller)
#else
/*Error*/
#endif

//Ecuacion CJC
#define EQUATION_CJC_M 0.08566
#define EQUATION_CJC_N -50
//===========================================================================
//============================= PID =========================================
//===========================================================================
#define KP_VAL    2
#define KI_VAL    5
#define KD_VAL    1

#define MIN_TEMP  100 // Minimum setpoint temperature
#define MAX_TEMP  550 // Maximum setpoint temperature
#define PWM_MAX   230 // PWM limit
#define PWM_MIN   0

//===========================================================================
//============================= TIMER =======================================
//===========================================================================
#define TIME_TIMER 200000

//===========================================================================
//============================= OTHERS=======================================
//===========================================================================
// Auto turn-off time (in milliseconds)
#define TURN_OFF_TIME 3600000 //1h

//===========================================================================
//============================= END MODULE ==================================
//===========================================================================
#endif //CONFIGURATION_H
