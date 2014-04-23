#include <msp430.h>

unsigned int ADC_Result[3];                // 12-bit ADC conversion result array

int main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // 关闭看门狗
  //端口配置

  // Configure GPIO
    P1OUT = 0;
    P1DIR = BIT0;                             // For LED
    P1SEL0 |= BIT4;
    P1SEL1 |= BIT4;

      PM5CTL0 &= ~LOCKLPM5;
      //配置ADC12
      REFCTL0 |= REFVSEL_3 | REFON;             // Select internal ref = 2.5V
      ADC12CTL0 = ADC12SHT0_2 | ADC12ON;
      ADC12CTL1 = ADC12SHP + ADC12CONSEQ_3; // 队列采样       11 = Repeat-sequence-of-channels
      ADC12CTL2 |= ADC12RES_2;                  // 12-bit conversion results
      ADC12MCTL0 |= ADC12INCH_4 | ADC12VRSEL_0; // A4 ADC input select; 0-3.3v
      //_delay_cycles(75);                       // reference settling ~75us

      //configure DMA CHANNEL 0
      DMACTL0 |= DMA0TSEL__ADC12IFG;    // DMA channel 0 transfer select 26: ADC12IFG
      __data16_write_addr((unsigned short) &DMA0SA,(unsigned long) &ADC12MEM0);

      __data16_write_addr((unsigned short) &DMA0DA,(unsigned long) 0XB000);

      DMA0SZ = 128;                            // 转换1次
      DMA0CTL = DMADT_4 | DMADSTINCR_3 | DMAEN | DMAIE | DMALEVEL;
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
}

#pragma vector=DMA_VECTOR
__interrupt void DMA0_ISR (void)
{
  switch(__even_in_range(DMAIV,16))
  {
    case  0: break;                          // No interrupt
    case  2:
      // sequence of conversions complete
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
