/*
 * button_send.c
 *
 *  Created on: 2014-5-14
 *      Author: lxy
 */
#include <msp430.h>
#include "Data_ass.h"
unsigned int *send_address;

void button_send(unsigned int *address)


{
	  P1OUT &= ~BIT0;                           // Clear LED to start
	  P1DIR |= BIT0;

	  P1SEL0 |= BIT1 ;
	  P1SEL1 |= BIT1 ;

	  // Configure ADC12
	   ADC12CTL0 = ADC12SHT0_2 | ADC12ON;         // 16 click cycle
	   ADC12CTL1 = ADC12SHP;                     // ADCCLK = MODOSC; sampling timer
	   ADC12CTL2 |= ADC12RES_2;                  // 12-bit conversion results
	   ADC12IER0 |= ADC12IE0;                    // Enable ADC conv complete interrupt
	   ADC12MCTL0 |= ADC12INCH_1; // A3 ADC input select; 0-3.3v

	   _delay_cycles(75);                       // reference settling ~75us




	   send_address=address;


	   while(1)
	     {
	    //  __delay_cycles(5000);                    // Delay between conversions
	       ADC12CTL0 |= ADC12ENC | ADC12SC;         // Sampling and conversion start
	       __bis_SR_register(LPM0_bits + GIE);      // LPM0, ADC10_ISR will force exit
	       __no_operation();                        // For debug only
	     }
	   }



#pragma vector = ADC12_VECTOR
__interrupt void ADC12_ISR(void)
{
  switch(__even_in_range(ADC12IV, ADC12IV_ADC12RDYIFG))
  {
    case  0: break;                         // Vector  0:  No interrupt
    case  2: break;                         // Vector  2:  ADC12MEMx Overflow
    case  4: break;                         // Vector  4:  Conversion time overflow
    case  6: break;                         // Vector  6:  ADC12HI
    case  8: break;                         // Vector  8:  ADC12LO
    case 10: break;                         // Vector 10:  ADC12IN
    case 12:
    	 if (ADC12MEM0 <= 0x020)  // ADC12MEM = A1 > 0.5V?
    	 {
    		 P1OUT |= BIT0; // P1.0 = 1
    	 	 UART((unsigned int*)send_address,36);
    	 }
    	 else
    	      P1OUT &= ~BIT0;                     // P1.0 = 0
    	 __bic_SR_register_on_exit(LPM0_bits); // Exit active CPU// Vector 12:  ADC12MEM0 Interrupt
      break;                                // Clear CPUOFF bit from 0(SR)
    case 14:break;                         // Vector 14:  ADC12MEM1
    case 16:break;                          // Vector 16:  ADC12MEM2
    case 18: break;                         // Vector 18:  ADC12MEM3
    case 20: break;                         // Vector 20:  ADC12MEM4
    case 22: break;                         // Vector 22:  ADC12MEM5
    case 24: break;                         // Vector 24:  ADC12MEM6
    case 26: break;                         // Vector 26:  ADC12MEM7
    case 28: break;                         // Vector 28:  ADC12MEM8
    case 30: break;                         // Vector 30:  ADC12MEM9
    case 32: break;                         // Vector 32:  ADC12MEM10
    case 34: break;                         // Vector 34:  ADC12MEM11
    case 36: break;                         // Vector 36:  ADC12MEM12
    case 38: break;                         // Vector 38:  ADC12MEM13
    case 40: break;                         // Vector 40:  ADC12MEM14
    case 42: break;                         // Vector 42:  ADC12MEM15
    case 44: break;                         // Vector 44:  ADC12MEM16
    case 46: break;                         // Vector 46:  ADC12MEM17
    case 48: break;                         // Vector 48:  ADC12MEM18
    case 50: break;                         // Vector 50:  ADC12MEM19
    case 52: break;                         // Vector 52:  ADC12MEM20
    case 54: break;                         // Vector 54:  ADC12MEM21
    case 56: break;                         // Vector 56:  ADC12MEM22
    case 58: break;                         // Vector 58:  ADC12MEM23
    case 60: break;                         // Vector 60:  ADC12MEM24
    case 62: break;                         // Vector 62:  ADC12MEM25
    case 64: break;                         // Vector 64:  ADC12MEM26
    case 66: break;                         // Vector 66:  ADC12MEM27
    case 68: break;                         // Vector 68:  ADC12MEM28
    case 70: break;                         // Vector 70:  ADC12MEM29
    case 72: break;                         // Vector 72:  ADC12MEM30
    case 74: break;                         // Vector 74:  ADC12MEM31
    case 76: break;                         // Vector 76:  ADC12RDY
    default: break;
  }
}

