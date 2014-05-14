
//******************************************************************************
#include "msp430.h"
#include "UART.h"
unsigned int i=0;
unsigned int max_length=0;
unsigned int *Data;

//#define FRAM_START 0x1C60

void UART(unsigned int *start_address, unsigned int length)
{
  // Configure GPIO
   IO_config();

  // CCR1 reset/set
  // Disable the GPIO power-on default high-impedance mode to activate
  // previously configured port settings
  PM5CTL0 &= ~LOCKLPM5;


//  // XT1 Setup
  clk_init();
  max_length=length;

// Configure USCI_A0 for UART mode
  uart_init();

  Data= (unsigned int *)start_address;

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
//    	UCA1TXBUF = 0xAA;  							// P1.3 interrupt enable
    	if(i<max_length)
    	{
    	UCA1TXBUF = *Data++;
    	i++;
    	}
        else
        {
         i=0;
        __bic_SR_register_on_exit(LPM3_bits);
        }
        break;
    case USCI_UART_UCSTTIFG: break;
    case USCI_UART_UCTXCPTIFG: break;
  }
}


void IO_config(void)								//config USCI_A1 UART    P2.5 as TX P2.6 as RX
{	  P2SEL1 |= BIT5 | BIT6;                    // USCI_A1 UART operation
	  P2SEL0 &= ~(BIT6 | BIT6);
	  PJSEL0 |= BIT4 | BIT5;                    // For XT1
}


void clk_init(void)
{
CSCTL0_H = CSKEY >> 8;                    // Unlock CS registers
CSCTL2 = SELA__LFXTCLK | SELS__DCOCLK | SELM__DCOCLK;
CSCTL3 = DIVA__1 | DIVS__1 | DIVM__1;     // set all dividers
CSCTL4 &= ~LFXTOFF;
do
{
  CSCTL5 &= ~LFXTOFFG;                    // Clear XT1 fault flag
  SFRIFG1 &= ~OFIFG;
}while (SFRIFG1&OFIFG);                   // Test oscillator fault flag
CSCTL0_H = 0;								// Lock CS registers
}

void uart_init(void)								//baud rate 1200
{
	  	UCA1CTLW0 = UCSWRST;                      // Put eUSCI in reset
	    UCA1CTLW0 |=  UCSSEL__SMCLK;
	    // Baud Rate calculation
	     // 1000000/(16*2400) = 26.041
	     // Fractional portion = 0.041
	     // User's Guide Table 21-4: UCBRSx = 0x01
	     // UCBRFx = int ( (26.041-26)*16) = 1
	    UCA1MCTLW = 0X4900 |UCBRF_4 | UCOS16;
	    UCA1BR0 = 45;
	    UCA0BR1 = 0;

	    UCA1CTL1 &= ~UCSWRST;                     // Initialize eUSCI
	    UCA1IE |= UCTXIE;                         // Enable USCI_A0 TX interrupt
}
