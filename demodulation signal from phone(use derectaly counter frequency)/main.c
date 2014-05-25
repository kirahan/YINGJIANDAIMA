/*
 * HandShake.c
 *
 *  Created on: 2014-5-21
 *      Author: lxy
 */
#include <msp430.h>

void HANDS_IO_CONFIG();
void HANDS_CLK_CONFIG();
void HANDS_TA_CONFIG();
void HANDS_COMPARATOR_CONFIG();
void record16bits(unsigned int *value_16) ;



unsigned int fre_counter=0;

unsigned int *data_high_16;
unsigned int *data_low_16;
unsigned int data_counter=0;
unsigned int *ErrorData;
unsigned int m;

#define error_message  0X1E00
#define low_message  0X1D00
#define high_message  0X1D02
void main()
{

	WDTCTL = WDTPW | WDTHOLD;                 // Stop WDT
	HANDS_IO_CONFIG();
	PM5CTL0 &= ~LOCKLPM5;

	HANDS_CLK_CONFIG();
	HANDS_COMPARATOR_CONFIG();
	HANDS_TA_CONFIG();
	ErrorData=(unsigned int *)error_message;
	for(m=0;m<40;m++)
	{
		*ErrorData++=0;
	}
	ErrorData=(unsigned int *)error_message;
	data_low_16=(unsigned int *)low_message;
	data_high_16=(unsigned int *)high_message;
	*data_low_16=0x1111;

		       __bis_SR_register(LPM0_bits + GIE);      // LPM0, ADC10_ISR will force exit
		       __no_operation();                         // For debug
//		       while(data_low_16==password);
//		    	   break;

}


#pragma vector = TIMER1_A1_VECTOR
__interrupt void HAND_Timer_A (void)
{
if(TA1CCTL1 & BIT0)                 // interrupt CCIFG
	{
	if (TA1CCR1 < 10000)
	    {
			fre_counter++;
			TA1IV = 0X0;
		}

		else if (data_counter < 16)
			record16bits(data_low_16);
		else if (16 <= data_counter & data_counter < 32)
			record16bits(data_high_16);
		else
		{
			TA1IV = 0X0;
			fre_counter = 0;
			TA1CTL |= TACLR;
			data_counter=0;
		__bic_SR_register_on_exit(LPM0_bits); // Exit active CPU
		}
}
}




/*****************************************/
void record16bits(unsigned int *value_16) {
	if (fre_counter > 7 & fre_counter < 13) {
		*value_16=*value_16>>1;
		data_counter++;
		fre_counter = 0;
		TA1CTL |= TACLR;
	} else if (fre_counter > 15 & fre_counter < 23) {
		*value_16 = *value_16>>1;
		*value_16 =*value_16 | BITF;
		data_counter++;
		fre_counter = 0;
		TA1CTL |= TACLR;
	} else if(fre_counter > 0)
	{
		*ErrorData++ = fre_counter + 1;
		fre_counter = 0;
		TA1CTL |= TACLR;
	}
}



void HANDS_IO_CONFIG()   // P1.4 ADC INPUT
{

	  // compare input
	      P1SEL0 |= BIT4;               // P1.4   compare input +
	      P1SEL1 |= BIT4;
	  // Configure GPIO
	  P3DIR |= BIT5;                            // P3.5 output direction
	  P3SEL1 |= BIT5;                           // Select CEOUT function on P3.5/CEOUT
}


void HANDS_CLK_CONFIG()
{
	CSCTL0_H = CSKEY >> 8;                    // Unlock CS registers
	//  CSCTL1 = DCOFSEL_6;
	CSCTL1 = DCOFSEL_0;
	CSCTL2 = SELA__LFXTCLK | SELS__DCOCLK | SELM__DCOCLK;
	CSCTL3 = DIVA__1 | DIVS__1 | DIVM__1;     // set all dividers
	CSCTL4 &= ~LFXTOFF;
}

void HANDS_COMPARATOR_CONFIG()
{
	  // Setup Comparator_E

	CECTL0 = CEIPEN | CEIPSEL_4;              // Enable V+, input channel CE1
	CECTL1 = CEPWRMD_1 | CEFDLY_3 | CEF;      // normal power mode; rising edge for CEIFG   ;  filter 900ns delay
	CECTL2 = CEREFL_1 | CERS_2 | CERSEL | CEREF1_0 |CEREF0_0;      // VREF is applied to -terminal
	                                            // R-ladder on;
	                                            // supplied to ref amplifier to get Vcref=1.2V    high_tag=0 low_tag=0
//	  CEINT = CEIE;
	  CECTL3 = BIT4;                            // Input Buffer Disable @P1.1/CE1
	  CECTL1 |= CEON;                           // Turn On Comparator_E
}


void HANDS_TA_CONFIG()
{
	TA1CCTL1 = CM_1 | CCIS_1 | SCS | CAP | OUTMOD_0 |CCIE;
	TA1CTL = TASSEL__SMCLK | MC_2;  // SMCLK, continue mode
}


