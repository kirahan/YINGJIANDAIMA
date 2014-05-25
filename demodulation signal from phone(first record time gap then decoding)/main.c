#include <msp430.h> 
#include <HandShake.h>
/*
 * main.c
 */

//#define  FIFO_ADDRESS  0X1D04
#define PASSWOARD 21846

//void Data_ass(unsigned int data_type, unsigned int *address, unsigned int step_num, unsigned char rotate_formula, float thinking_time_input,  float operating_time_input );

int main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
	  P1OUT &= ~BIT0;                           // Clear LED to start
	  P1DIR |= BIT0;
	
	HandShake();
	P1OUT |= BIT0; // P1.0 = 1
	__bis_SR_register(LPM3_bits+GIE);         // Enter LPM3, interrupts enabled
	__no_operation();                         // For debugger





	return 0;
}
