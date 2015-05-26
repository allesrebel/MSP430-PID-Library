#ifndef MY_DRIVER_H
#define MY_DRIVER_H

#include "msp430.h"

//---------------------------------------------------------
//	Defines for Hardware Configuration For G2553
//---------------------------------------------------------
//	Defines for Internal Use
#define ID__8                                              (0x00c0)              /* /8 */
#define MC__CONT                                           (2*0x10u)             /* Continous Count Mode */
#define MC__UP                                             (0x0010)              /* Up mode: Timer counts up to TAxCCR0 */
#define TASSEL__SMCLK                                      (0x0200)              /* SMCLK */
#define ADC10SSEL__MCLK                                    ADC10SSEL_3           /* MCLK */
#define ADC10CONSEQ_0                                      CONSEQ_0              /* Single Channel */
#define ADC10ENC                                           (0x002)               /* ADC10 Enable Conversion */

#define uint32_t unsigned long
#define uint16_t unsigned int

#define ticks_1ms 16000	//	For a 16MHz Clock

//---------------------------------------------------------

//---------------------------------------------------------
//	Variables for Internal use
//---------------------------------------------------------
extern volatile uint32_t millisecond_count;
//---------------------------------------------------------

//---------------------------------------------------------
//	Internal Utility Functions
//---------------------------------------------------------
//	Error Function
void error();
//	Function to generate a tick count for another freq from a running freq
uint32_t pwmFreqTicksCalc(uint32_t target_freq, uint32_t operating_freq);
//---------------------------------------------------------

//---------------------------------------------------------
//	Library Init Functions
//---------------------------------------------------------
//	Init Driver Library to emulate Ardunio Library Functions
void init_driverLib();

//	Set up Clocks MCLK, SMCLK, and ACLK to 16MHz
void setup_clock();

//	Set up the Timer1 to be operating on 16MHz w/ 1ms tick
void setup_TimerA1();

//	Set up the ADC to do fastest poll on channel 1
void setup_ADC();
//---------------------------------------------------------

//---------------------------------------------------------
//	API Functions
//---------------------------------------------------------
uint32_t millis();
void start_ADC();	//	Sample/convert Channel 0
void set_TimerA1_CCR1(uint16_t ticks);
void set_TimerA1_PWM(double percent);
//---------------------------------------------------------

#endif
