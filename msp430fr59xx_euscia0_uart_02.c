/* --COPYRIGHT--,BSD_EX
 * Copyright (c) 2012, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *******************************************************************************
 * 
 *                       MSP430 CODE EXAMPLE DISCLAIMER
 *
 * MSP430 code examples are self-contained low-level programs that typically
 * demonstrate a single peripheral function or device feature in a highly
 * concise manner. For this the code may rely on the device's power-on default
 * register values and settings such as the clock configuration and care must
 * be taken when combining code from several examples to avoid potential side
 * effects. Also see www.ti.com/grace for a GUI- and www.ti.com/msp430ware
 * for an API functional library-approach to peripheral configuration.
 *
 * --/COPYRIGHT--*/
//******************************************************************************
//  MSP430FR59xx Demo - eUSCI_A0 UART echo at 9600 baud using BRCLK = 32768Hz
//
//  Description: This demo echoes back characters received via a PC serial port.
//  ACLK is used as a clock source and the device is put in LPM3
//  Note that level shifter hardware is needed to shift between RS232 and MSP
//  voltage levels.
//
//  The example code shows proper initialization of registers
//  and interrupts to receive and transmit data.
//  To test code in LPM3, disconnect the debugger and use an external supply
//  Otherwise use LPM0
//
//  ACLK = 32768Hz, MCLK =  SMCLK = default DCO
//
//                MSP430FR5969
//             -----------------
//            |                 |--LFXTIN (32768Hz reqd.)
//            |                 |--LFXTOUT
//            |                 |
//       RST -|     P2.0/UCA0TXD|----> PC (echo)
//            |                 |
//            |                 |
//            |     P2.1/UCA0RXD|<---- PC
//            |                 |
//
//   P. Thanigai
//   Texas Instruments Inc.
//   August 2012
//   Built with IAR Embedded Workbench V5.40 & Code Composer Studio V5.2
//******************************************************************************
#include "msp430.h"
 int i=0x11;

int main(void)
{
   WDTCTL = WDTPW | WDTHOLD;                 // Stop Watchdog

  // Configure GPIO
  P2SEL1 |= BIT5 | BIT6;                    // USCI_A1 UART operation
  P2SEL0 &= ~(BIT6 | BIT6);
  PJSEL0 |= BIT4 | BIT5;                    // For XT1

  P1IE  |= BIT3;   							// P1.3 interrupt enable
  P1IES |= BIT3;							// interrupt flag is set with a high-to-low transition
//
  P1DIR |= BIT2 ;                     // P1.2  pmw output
  P1SEL0 |= BIT2;                    // P1.2 options select


  // CCR1 reset/set
  // Disable the GPIO power-on default high-impedance mode to activate
  // previously configured port settings
  PM5CTL0 &= ~LOCKLPM5;

//  // XT1 Setup
  CSCTL0_H = CSKEY >> 8;                    // Unlock CS registers
  CSCTL2 = SELA__LFXTCLK | SELS__DCOCLK | SELM__DCOCLK;
  CSCTL3 = DIVA__1 | DIVS__1 | DIVM__1;     // set all dividers
  CSCTL4 &= ~LFXTOFF;
  do
  {
    CSCTL5 &= ~LFXTOFFG;                    // Clear XT1 fault flag
    SFRIFG1 &= ~OFIFG;
  }while (SFRIFG1&OFIFG);                   // Test oscillator fault flag
  CSCTL0_H = 0;                             // Lock CS registers



/*
  // clock for 1 Mhz
    CSCTL0_H = CSKEY >> 8;                    // Unlock CS registers
    CSCTL2 = SELA__LFXTCLK | SELS__DCOCLK | SELM__DCOCLK;
    CSCTL3 = DIVA__1 | DIVS__1 | DIVM__1;     // set all dividers


// Configure USCI_A0 for UART mode
//  UCA1CTLW0 = UCSWRST;                      // Put eUSCI in reset
//    UCA1CTLW0 |=  UCSSEL__SMCLK;
//    UCA1MCTLW = UCBRF_8 | UCOS16;
//    UCA1BR0 = 6;                              // 9600 baud
//
//    UCA1MCTLW |= 0x2000;                      // 1M/4800 - INT(32768/4800)=0.82
//                                              // UCBRSx value = 0xEE TO 0XBF (See U
*/


//////  UCA1CTLW0 |=  UCPEN | UCSSEL__ACLK;
//  UCA1CTLW0 |=  UCSSEL__ACLK;
//  UCA1BR0 = 6;                              // 4800 baud
//  UCA1MCTLW |= 0xA600;                      // 32768/4800 - INT(32768/4800)=0.82
//                                            // UCBRSx value = 0xEE TO 0XBF (See U

  ////  UCA1CTLW0 |=  UCPEN | UCSSEL__ACLK;
    UCA1CTLW0 |=  UCSSEL__ACLK;
    UCA1BR0 = 13;                              // 4800 baud
    UCA1MCTLW |= 0xB600;                      // 32768/4800 - INT(32768/4800)=0.82
                                              // UCBRSx value = 0xEE TO 0XBF (See U

  UCA1BR1 = 0;
  UCA1CTL1 &= ~UCSWRST;                     // Initialize eUSCI
  UCA1IE |= UCTXIE;                         // Enable USCI_A0 TX interrupt

  TA1CTL = TASSEL__SMCLK | MC__UP ;  // SMCLK, up mode, clear TAR

  __bis_SR_register(LPM3_bits+GIE);         // Enter LPM3, interrupts enabled
  __no_operation();                         // For debugger
}

#pragma vector=USCI_A1_VECTOR
__interrupt void USCI_A1_ISR(void)
{
  switch(__even_in_range(UCA1IV, USCI_UART_UCTXCPTIFG))
  {
    case USCI_NONE: break;
    case USCI_UART_UCRXIFG:
         break;
    case USCI_UART_UCTXIFG:
//    	while(!(UCA0IFG&UCTXIFG));
    	UCA1TXBUF = 0xAA;
    	__no_operation();
    	break;
    case USCI_UART_UCSTTIFG: break;
    case USCI_UART_UCTXCPTIFG: break;
  }
}

#pragma vector=PORT1_VECTOR
__interrupt void P1_ISR(void) {
	switch(__even_in_range(P1IV,P1IV_P1IFG7))
	{
	case 0: break;
	case 2: break;
	case 4: break;
	case 6: break;
	case 8:
		/*
		 *Port x interrupt edge select
		0b = PxIFG flag is set with a low-to-high transition
		1b = PxIFG flag is set with a high-to-low transition
		 * */
		if( P1IES & BIT3 )
		{
				//	TA1CTL |= MC__UP;
			while(1)
						{
						//while(TA1R>40 & TA1R<390)
						 //   {
						//	if(TA1R> 400)
							//	break;
						//}

						while(TA1R>410 & TA1R<780)
									    {
										if(TA1R> 0 &  TA1R<400)
											break;
									}

						break;
						}


			//	   TA1CTL |= MC__STOP | TASSEL__SMCLK ;
				   TA1CCR0 = 400;                         // PWM Period
				   TA1CCR1 = 200;
				   TA1R=0;
				   TA1CCTL1 = OUTMOD_3;                      // CCR1 reset/set
					P1IES &= ~BIT3;
			//		TA1CTL |= MC__UP;

		}

		else
		{

			while(1)
			{
			while(TA1R>10 & TA1R<190)
			    {
				if(TA1R> 200)
					break;
			}

			while(TA1R>190 & TA1R<390)
						    {
							if(TA1R> 0 &  TA1R<200)
								break;
						}

			break;
			}
		//   TA1CTL |= MC__STOP | TASSEL__SMCLK ;
		   TA1CCR0 = 800;                         // PWM Period
		   TA1CCR1 = 400;
		   P1IES |= BIT3;
			TA1CCTL1 = OUTMOD_3;
		//	TA1CTL |= MC__UP;
		}

		P1IFG &= ~BIT3;
		break;/* P1IV P1IFG.3 */
	case 10: break;
	case 12: break;
	case 14: break;
	default : break;
	}
}// end P1_ISR

