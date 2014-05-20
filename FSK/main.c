
//******************************************************************************
#include "msp430.h"
//int i=0;
//
//unsigned int *Data;

//#define FRAM_START 0x1C60

int main(void)
{
   WDTCTL = WDTPW | WDTHOLD;                 // Stop Watchdog

  // Configure GPIO
  P2SEL1 |= BIT5 | BIT6;                    // USCI_A1 UART operation
  P2SEL0 &= ~(BIT6 | BIT6);
  PJSEL0 |= BIT4 | BIT5;                    // For XT1

  P1OUT &= ~BIT0;                           // Clear LED to start
  P1DIR |= BIT0;

  P1IE  |= BIT3;   							// P1.3 interrupt enable
  P1IES &= ~BIT3;							// interrupt flag is set with a high-to-low transition


//
  P1DIR |= BIT2 ;                     // P1.2  pmw output
  P1SEL0 |= BIT2;                    // P1.2 options select


  // CCR1 reset/set
  // Disable the GPIO power-on default high-impedance mode to activate
  // previously configured port settings
  PM5CTL0 &= ~LOCKLPM5;

//  // XT1 Setup
  CSCTL0_H = CSKEY >> 8;                    // Unlock CS registers
//  CSCTL1 = DCOFSEL_6;
  CSCTL1 = DCOFSEL_3;
  CSCTL2 = SELA__LFXTCLK | SELS__DCOCLK | SELM__DCOCLK;
  CSCTL3 = DIVA__1 | DIVS__1 | DIVM__1;     // set all dividers
  CSCTL4 &= ~LFXTOFF;
  do
  {
    CSCTL5 &= ~LFXTOFFG;                    // Clear XT1 fault flag
    SFRIFG1 &= ~OFIFG;
  }while (SFRIFG1&OFIFG);                   // Test oscillator fault flag
  CSCTL0_H = 0;                             // Lock CS registers


// Configure USCI_A0 for UART mode
  UCA1CTLW0 = UCSWRST;                      // Put eUSCI in reset
    UCA1CTLW0 |=  UCSSEL__SMCLK;
    // Baud Rate calculation
     // 4000000/(16*2400) = 104.1667
     // Fractional portion = 0.1667
     // User's Guide Table 21-4: UCBRSx = 0x01
     // UCBRFx = int ( (26.041-26)*16) = 2
    UCA1MCTLW = 0X0000 |UCBRF_0 | UCOS16;
    UCA1BR0 = 100;
    UCA0BR1 = 0;


  UCA1CTL1 &= ~UCSWRST;                     // Initialize eUSCI
  UCA1IE |= UCTXIE;                         // Enable USCI_A0 TX interrupt

//  Data= (unsigned int *)FRAM_START;

  TA1CTL = TASSEL__SMCLK | MC__UP ;  // SMCLK, up mode, clear TAR
  TA1CCTL1 = OUTMOD_3;
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
    	while(!(UCA0IFG&UCTXIFG));
//    	UCA1TXBUF = 0xAA;  							// P1.3 interrupt enable
    	UCA1TXBUF = 0x59;

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
//				   TA1CTL = MC__STOP ;
			if(TA1R>700)
										   		 			  {
										   		 			TA1R=0;
										   		 			P1OUT |= BIT0;
										   		 			  }

			TA1CCR1 = 200;
			TA1CCR0 = 400-1;                         // PWM Period




//			TA1R=0;
				   		 		   //TA1CCTL1 = OUTMOD_3;
//				   		 		  if(TA1R<100)
//				   		 			  {
//				   		 			TA1CTL = MC__STOP ;
//				   		 			TA1R=0;
//				   		 			TA1CCTL1 = OUTMOD_7;
//				   		 			TA1CTL = TASSEL__SMCLK | MC__UP;
//				   		 			P1OUT |= BIT0;
//				   		 			  }
					P1IES &= ~BIT3;
		}

		else

		{
//		           TA1CTL = MC__STOP ;
			TA1CCR1 = 400;
			TA1CCR0 = 800-1;                         // PWM Period
		   P1IES |= BIT3;
//		   TA1CTL = TASSEL__SMCLK | MC__UP;
		}

		P1IFG &= ~BIT3;
		break;/* P1IV P1IFG.3 */
	case 10: break;
	case 12: break;
	case 14: break;
	default : break;
	}
}
	// end P1_ISR
