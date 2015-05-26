#include "mydriverlib.h"

/*
 * Variables Used
 */
volatile uint32_t millisecond_count = 0;

/*
 *	Init the driver library
 *	Allows use of Ardunio Like Functions
 */
void init_driverLib(){
	setup_clock();
	setup_TimerA1();
	setup_ADC();
}

/*
 * Sets up the TimerA1 Module to Run on 16MHz
 * Assigns -
 * 	CCR0 to fire an interrupt every millisecond for milis()
 */
void setup_TimerA1(){
	// Configure Timer0A to operate in Up count mode with 16MHz Speed
	TA1CTL = TASSEL__SMCLK | MC__CONT ;	// Source select, divider, count mode
	TA1CTL |= TACLR;	// To reset TA1R and apply dividers (Auto resets)

	// Configure CCR0 to fire an Interrupt every 1ms for millis()
	TA1CCTL0 = CCIE;		// Interrupt enable
	// CCR0 Configuration
	TA1CCR0 = ticks_1ms;	// 1ms of time per fire

	// CCR1 Configuration for PWM with Output Port (G2553 TA1.1 is P2.1)
	TA1CCTL1 = OUTMOD_6;
	P2DIR |= BIT1;	//
	P2SEL |= BIT1;	//	Enable Output On P2.1
	P2SEL2 &= ~BIT1;//
}

/*
 * Configure CCR1's Value
 */
void set_TimerA1_CCR1(uint16_t ticks){
	TA1CCR1 = ticks;
}

/*
 * Configure CCR1's Value with a Specific Duty Cycle
 * Uses's the 1ms timer for duty cycle frequency
 */

/*
 * Sets up the DCO with 16MHz
 * Assigns -
 * 	MCLK = DCO 16MHz
 *	SMCLK = DCO 16MHz
 */
void setup_clock() {
	if (CALBC1_16MHZ == 0xFF)	// if calibration constant erased
			{
		while (1)
			;			// Do not load trap cpu
	}
	DCOCTL = 0;	//Clear the Settings
	BCSCTL1 = CALBC1_16MHZ;	// Set range
	DCOCTL = CALDCO_16MHZ;	// Set DCO step + modulation
}

//	Set up the ADC for 10 Bit operation
//	With Continous Samples on the same channel
void setup_ADC(){
	// Allow configuration fo the ADC14 Module
	ADC10CTL0 &= ~(ADC10ENC);

	//			Use mclk , single ch conv,  signal from sample timer
	ADC10CTL0 = ADC10SHT_2 | ADC10ON | ADC10IE;
	ADC10CTL1 = INCH_0 | ADC10SSEL__MCLK | ADC10CONSEQ_0;

	// Set up A0 Pin on Board (P1.0)
	P1SEL |= BIT0;
	P1SEL2 |= BIT0;
	ADC10AE0 = BIT0;
}

/*
 * Calculates the number ticks required to generate a desired
 * number of ticks needed to achieve frequency
 * Note assumes that Operating Frequency is greater than Target
 */
uint32_t pwmFreqTicksCalc(uint32_t target_freq, uint32_t operating_freq){
	double period_desired = (double)1/target_freq;
	double period_operating = (double)1/operating_freq;
	return period_desired/period_operating;
}

//	Start the Sampling and Conversion from P1.0
void start_ADC(){
	//Start Conversions from the selected channel
	ADC10CTL0 |= ADC10ENC | ADC10SC;        // Start conversion -software trigger
}

// API Function Millis() - Returns milliseconds elapsed
uint32_t millis(){
	return millisecond_count;
}

// ISR Millisecond incrementor for millis();
#pragma vector=TIMER1_A0_VECTOR
__interrupt void timerA1_CCR0_ISR(void){
	TA1CCTL0 &= ~CCIFG;	// Clear the Interrupt
	millisecond_count++;
	TA1CCR0 += ticks_1ms;
}

/*
 * Error Function that Halts System Operation
 * Debugging should reveal which function was called from
 * stack image.
 */
void error(void)
{
    volatile uint32_t i;
    P1DIR |= BIT0;
    while (1)
    {
        P1OUT ^= BIT0;
        __delay_cycles(3100000);
    }
}

