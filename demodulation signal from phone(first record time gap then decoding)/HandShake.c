/*
 * HandShake.c
 *
 *  Created on: 2014-5-21
 *      Author: lxy
 */
#include <msp430.h>
#include "HandShake.h"

//for record phone data
unsigned int start_flag=0;
unsigned int data_counter=0;
unsigned int timerregister_buffer;
unsigned int *TimerA_value;
#define timerA_value_address 0x1E00

/***************************************/
//FOR data decoding
unsigned int *data_high_16;
unsigned int *data_low_16;
#define low_message_address  0X1D00
#define high_message_address  0X1D02
//for state machine of decoding
unsigned int State;
unsigned int counter_1k=0;
unsigned int counter_2k=0;
unsigned int bit_count=0;
#define FRE_1K    0
#define FRE_2K    1
#define Space_1k  0x7C
#define Space_2k  0x3E
#define Space_reverse  0x5C
#define Allow_space_err     3


/******************************************************************/
/*****************main function     *******************************/
/******************************************************************/
void HandShake()
{
	HANDS_IO_CONFIG();
	PM5CTL0 &= ~LOCKLPM5;

	HANDS_CLK_CONFIG();
	HANDS_COMPARATOR_CONFIG();
	HANDS_TA_CONFIG();
	HANDS_REGISTER_INIT();

	__bis_SR_register(LPM0_bits + GIE);		  // LPM0, ADC10_ISR will force exit
//	__no_operation();                         // For debug
	HANDS_DECODING();
	__no_operation();

}
/******************************************************************/
/*ISR function: recording time_gap of each rising edge,
 * source signal come from smartphone*******************************/
/******************************************************************/

#pragma vector = TIMER1_A1_VECTOR
__interrupt void HAND_Timer_A (void)
{
if(TA1CCTL1 & BIT0)                 // interrupt CCIFG
	{
	if(start_flag==0)
	{
		start_flag=1;
		TA1CTL |= TACLR;
		timerregister_buffer=0;
	}

	else if ( TA1R< 40000)            // for 320ms
	    {
		*TimerA_value++=TA1CCR1-timerregister_buffer;
		timerregister_buffer=TA1CCR1;
		data_counter++;
		TA1IV = 0X0;
		}
		else
		{
			TA1CCTL1 &= ~CCIE;
			TA1IV = 0X0;
			TA1CTL |= TACLR;
		__bic_SR_register_on_exit(LPM0_bits); // Exit active CPU
		}
}
}
/******************************************************************/
/*****************all configuration and initialization*******************************/
/******************************************************************/
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
	TA1CTL = TASSEL__SMCLK | MC_2 | ID_3;  // SMCLK, continue mode
}


void HANDS_REGISTER_INIT()
{
	data_low_16=(unsigned int *)low_message_address;
	data_high_16=(unsigned int *)high_message_address;
	TimerA_value=(unsigned int *)timerA_value_address;
	*data_low_16=0x0;
	*data_high_16=0x0;
}



/******************************************************************/
/*****************decoding function *******************************/
/******************************************************************/

//unsigned int State=0;
//unsigned int counter_1k=0;
//unsigned int counter_2k=0;
//#define FRE_1K    0
//#define FRE_2K    1
//#define Space_1k  0x7C
//#define Space_2k  0x3E
//#define Space_reverse  0x5C
//#define Allow_space_err     3
//unsigned int bit_count=0;


void HANDS_DECODING()
{
	TimerA_value=(unsigned int *)timerA_value_address;                      //back to first data
	for(;data_counter>0;data_counter--,TimerA_value++)											    // start decoding
	{
		switch(State)
		{
		case FRE_1K:
		////////////next space value(7D) is also 1k
	if(*TimerA_value>Space_1k-Allow_space_err & *TimerA_value<Space_1k+Allow_space_err)
			{
				if(bit_count<16 & counter_1k==10)
				{
					record0(data_low_16);
						bit_count++;
						counter_1k=0;
				}
				else
					if(bit_count>=16 & counter_1k==10)
					{
						record0(data_high_16);
						bit_count++;
						counter_1k=0;
					}

			counter_1k++;
			}


		////////////next space value(3E) is 2k
    if(*TimerA_value>Space_2k-Allow_space_err & *TimerA_value<Space_2k+Allow_space_err)
			{
				State=FRE_2K;
				if(bit_count<16 & counter_1k>=9 & counter_1k<=11)
				{
					record0(data_low_16);
						bit_count++;
						counter_1k=0;
				}

				else if(bit_count>=16 & counter_1k>=9 & counter_1k<=11)
				{
					record0(data_high_16);
						bit_count++;
						counter_1k=0;
				}

				counter_1k=0;
				counter_2k++;
			}



       ////////////next space value(5D) change from 1k to 2k
	if(*TimerA_value>Space_reverse-Allow_space_err & *TimerA_value<Space_reverse+Allow_space_err)
		    {
				State=FRE_2K;
				counter_1k++;
				if(bit_count<16 & counter_1k>=9 & counter_1k<=11)
				{
					record0(data_low_16);
						bit_count++;
						counter_1k=0;
				}

				else if(bit_count>=16 & counter_1k>=9 & counter_1k<=11)
				{
					record0(data_high_16);
						bit_count++;
						counter_1k=0;
				}
				counter_1k=0;
		    }

			break;






		case FRE_2K:
			////////////next space value(3E) is also 2k
	 if(*TimerA_value>Space_2k-Allow_space_err & *TimerA_value<Space_2k+Allow_space_err)
				{

					if(bit_count<16 & counter_2k==20)
					{
						record1(data_low_16);
							bit_count++;
							counter_2k=0;
					}

					else if(bit_count>=16 &  counter_2k==20)
					{
						record1(data_high_16);
							bit_count++;
							counter_2k=0;
					}
					counter_2k++;
				}


			////////////next space value(7D) is 1k
	 if(*TimerA_value>Space_1k-Allow_space_err & *TimerA_value<Space_1k+Allow_space_err)
				{
					State=FRE_1K;
					if(bit_count<16 & counter_2k>=19 & counter_2k<=21)
					{
						record1(data_low_16);
							bit_count++;
							counter_2k=0;
					}
					else
						if(bit_count>=16 & counter_2k>=19 & counter_2k<=21)
						{
							record1(data_high_16);
							bit_count++;
							counter_2k=0;
						}
						counter_2k=0;
						counter_1k++;
				}



	       ////////////next space value(5E) change from 2k to 1k
	 if(*TimerA_value>Space_reverse-Allow_space_err & *TimerA_value<Space_reverse+Allow_space_err)
			    {
					State=FRE_1K;
					counter_2k++;
				if (bit_count < 16 & counter_2k >= 19 & counter_2k <= 21) {
					record1(data_low_16);
					bit_count++;
					counter_2k = 0;
				} else if (bit_count >= 16 & counter_2k >= 19
						& counter_2k <= 21)
						{
					record1(data_high_16);
					bit_count++;
					counter_2k = 0;
						}
					counter_2k=0;
			    }

			break;
		}



	}



}



void record0(unsigned int*address)
{
	*address=*address>>1;
}

void record1(unsigned int*address)
{
	*address=*address>>1;
	*address =*address | BITF;
}

