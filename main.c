#include <msp430.h>

unsigned int ADC_Result[128];

int main(void)
{

  WDTCTL = WDTPW + WDTHOLD;                 // 关闭看门狗
  // Configure GPIO
    P1OUT = 0;
    P1DIR = BIT0;                             // For LED
    P1SEL0 |= BIT3;
    P1SEL1 |= BIT3;

    //配置ADC12
      REFCTL0 |= REFVSEL_3 | REFON;             // Select internal ref = 2.5V   tttt
      ADC12CTL0 = ADC12SHT0_2 | ADC12ON | ADC12MSC;
      ADC12CTL1 = ADC12SHP + ADC12CONSEQ_2; // 队列采样       11 = Repeat-sequence-of-channels
      ADC12CTL2 |= ADC12RES_2;                  // 12-bit conversion results
      ADC12MCTL0 |= ADC12INCH_3 | ADC12VRSEL_0; // A3 ADC input select; 0-3.3v
      //_delay_cycles(75);                       // reference settling ~75us
      // Disable the GPIO power-on default high-impedance mode to activate
       // previously configured port settings
       //PM5CTL0 &= ~LOCKLPM5;
     //configure DMA CHANNEL 0
      DMACTL0 |= DMA0TSEL__ADC12IFG;    // DMA channel 0 transfer select 26: ADC12IFG
      __data16_write_addr((unsigned short) &DMA0SA,(unsigned long) &ADC12MEM0);

      __data16_write_addr((unsigned short) &DMA0DA,(unsigned long) &ADC_Result[0]);

      DMA0SZ = 100;                            // 转换1次
      DMA0CTL = DMADT_4 + DMADSTINCR_3 + DMAEN + DMAIE;
      //配置 Repeated single transfer
      //Destination address is increased
      //destination as a byte
      //the source as a byte
      //level-sensitive triggers
      //DMA enable
      //DMA interrupt enable
      //目标地址递增,完成后中断.
  while(1)
  {
//	   __delay_cycles(5000);                    // Delay between conversions
	  ADC12CTL0 |= ADC12ENC + ADC12SC;
    __bis_SR_register(CPUOFF + GIE);
    __no_operation();                        // For debug only
  }
  return 1;
}

#pragma vector=DMA_VECTOR
__interrupt void DMA0_ISR (void)
{
  switch(__even_in_range(DMAIV,16))
  {
    case  0:
    	 break;                          // No interrupt
    case  2:
      // sequence of conversions complete
    	 ADC12CTL0 &= ~ADC12ENC;
    	 P1OUT |= ~P1OUT;
      __bic_SR_register_on_exit(CPUOFF);     // exit LPM
      break;                                 // DMA0IFG
    case  4: break;                          // DMA1IFG
    case  6: break;                          // DMA2IFG
    case  8: break;                          // Reserved
    case 10: break;                          // Reserved
    case 12: break;                          // Reserved
    case 14: break;                          // Reserved
    case 16: break;                          // Reserved
    default: break;
  }
}
