#include<msp430.h>
#include<string.h>
void store_singlechanel(int *register_address,int *flag, int flag_turn );
unsigned int *ADC_Result;
unsigned int count=0;

int flag_red=1;
int flag_yellow=1;
int flag_green=1;
int flag_white=1;
int flag_orange=1;
int flag_blue=1;

#define FRAM_TEST_START 0xD000
#define turn_red 0x2000
#define turn_yellow 0x3000
#define turn_green 0x4000
#define turn_white 0x5000
#define turn_orange 0x6000
#define turn_blue 0x7000
int main(void)
{
	  WDTCTL = WDTPW | WDTHOLD;                 // Stop WDT

	  // Configure GPIO
	      P1OUT = 0;
	      P1DIR = BIT0;                             // For LED


	    //set ADC input ports
	    //P1.2 for  'red'     	A2
		//P1.3 for 	'yellow'	A3
		//P1.4 for 	'green'		A4
		//P1.5 for 	'white'		A5
		//P2.4 for 	'orange'	A7
		//P4.3 for 	'blue'		A11
//	        P1SEL0 |= BIT3 ;
//	        P1SEL1 |= BIT3 ;


		  P1SEL0 |= BIT2 | BIT3 | BIT4 | BIT5 ;
		  P1SEL1 |= BIT2 | BIT3 | BIT4 | BIT5 ;
	      P2SEL0 |=	BIT3 ;
		  P2SEL1 |= BIT3 ;

		  P4SEL0 |=	BIT3 ;
		  P4SEL1 |= BIT3 ;
	      // Disable the GPIO power-on default high-impedance mode to activate
	      // previously configured port settings
	      PM5CTL0 &= ~LOCKLPM5;
	      while(REFCTL0 & REFGENBUSY);              // If ref generator busy, WAIT
	      REFCTL0 |= REFVSEL_3 | REFON;             // Select internal ref = 2.5V
	      //   _delay_cycles(75);                       // reference settling ~75us

	      // Configure ADC12
	      ADC12CTL0 = ADC12SHT0_2 | ADC12ON;         			// 16 click cycle for sample-and-hold time.
	      ADC12CTL1 = ADC12SHP | ADC12PDIV_3 |ADC12DIV_7 | ADC12SSEL_2  | ADC12CONSEQ_1; 		// SAMPON form the sampling timer, ADC predivider by 64, clock divider by 8, CLOCK SOURCE SELECT MCLK= 1MHZ
	      ADC12CTL2 |= ADC12RES_2;                  // 12-bit conversion results
	      ADC12CTL3 |= ADC12CSTARTADD_0;

	      ADC12IER0 |= ADC12IE0 | ADC12IE1 | ADC12IE2 | ADC12IE3 | ADC12IE4 | ADC12IE5;                    // Enable ADC conv complete interrupts
	      	      ADC12MCTL0 |= ADC12INCH_2 | ADC12VRSEL_0; // A2 ADC input select; 0-3.3v
	      	      ADC12MCTL1 |= ADC12INCH_3 | ADC12VRSEL_0; // A3 ADC input select; 0-3.3v
	      	      ADC12MCTL2 |= ADC12INCH_4 | ADC12VRSEL_0; // A12 ADC input select; 0-3.3v
	      	      ADC12MCTL3 |= ADC12INCH_5 | ADC12VRSEL_0; // A5 ADC input select; 0-3.3v
	      	      ADC12MCTL4 |= ADC12INCH_7 | ADC12VRSEL_0 ; // A7 ADC input select; 0-3.3v
	      	       ADC12MCTL5 |= ADC12INCH_11 | ADC12VRSEL_0 | ADC12EOS; // A11 ADC input select; 0-3.3v

	      	      //	      	      ADC12IER0 |= ADC12IE1;
	      	      //ADC12MCTL2 |= ADC12INCH_3 | ADC12VRSEL_0 | ADC12EOS; // A3 ADC input select; 0-3.3v

	      ADC_Result= (unsigned int *)FRAM_TEST_START;



	      while(1)
	      {

	    	//   __delay_cycles(5000);                    // Delay between conversions
	    	  ADC12CTL0 |= ADC12ENC + ADC12SC;
	    	  __bis_SR_register(LPM0_bits + GIE);
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
    case 12:								//red facet
    	store_singlechanel((int *)&ADC12MEM0,(int *)&flag_red, turn_red );
    	__bic_SR_register_on_exit(LPM0_bits); // Exit active CPU
      break;                                // Clear CPUOFF bit from 0(SR)
    case 14:								//yellow facet
    	store_singlechanel((int *)&ADC12MEM1,(int *)&flag_yellow, turn_yellow );
    	__bic_SR_register_on_exit(LPM0_bits); // Exit active CPU
    	break;                         // Vector 14:  ADC12MEM1
    case 16:							//green facet
    	store_singlechanel((int *)&ADC12MEM2,(int *)&flag_green, turn_green );
    	__bic_SR_register_on_exit(LPM0_bits); // Exit active CPU
    	break;                          // Vector 16:  ADC12MEM2
    case 18:							//white facet
    	store_singlechanel((int *)&ADC12MEM3,(int *)&flag_white, turn_white );
    	__bic_SR_register_on_exit(LPM0_bits); // Exit active CPU
    	break;                         // Vector 18:  ADC12MEM3
    case 20:							//orange facet
    	store_singlechanel((int *)&ADC12MEM4,(int *)&flag_orange, turn_orange );
    	__bic_SR_register_on_exit(LPM0_bits); // Exit active CPU
    	break;                         // Vector 20:  ADC12MEM4
    case 22:							//blue facet
    	store_singlechanel((int *)&ADC12MEM5,(int *)&flag_blue, turn_blue );
    	__bic_SR_register_on_exit(LPM0_bits); // Exit active CPU
    	break;                         // Vector 22:  ADC12MEM5
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

void store_singlechanel(int *register_address,int *flag, int flag_turn ){
	if (*register_address <0x400 & *flag==0)               // start
	    	    	 {
	    	    	 	 P1OUT &= ~BIT0; // P1.0 = 0
	    	    	 	 *ADC_Result++=flag_turn | *register_address;
	    	    	 	 count=count+1;
	    	    	 	*flag=1;
	    	    	 }
	    	    	 else if(*register_address >0x4B0 & *register_address <0x580 & *flag==1) // low voltage
	    	    	 {
	    	    		 *ADC_Result++=flag_turn | *register_address;
	    	    		 P1OUT |= BIT0; // P1.0 = 1
	    	    		 count=count+1;
	    	    		 *flag=0;
	    	    	 }
	    	    	 else if(*register_address >0x950 & *register_address <0xB40 & *flag==1)
	    	    	 {
	    	    		 *ADC_Result++=flag_turn | *register_address;
	    	    		 P1OUT |= BIT0; // P1.0 = 1
	    	    	     count=count+1;
	    	    	     *flag=0;
	    	    	 }

}
