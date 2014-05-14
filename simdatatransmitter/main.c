#include <msp430.h> 
#include "Data_ass.h"
/*
 * main.c
 */

#define  FIFO_ADDRESS  0X1D00

//void Data_ass(unsigned int data_type, unsigned int *address, unsigned int step_num, unsigned char rotate_formula, float thinking_time_input,  float operating_time_input );

int main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer



	Data_ass(1,(unsigned int*)FIFO_ADDRESS,1,"U",1.220,0.123);
	Data_ass(0,(unsigned int*)FIFO_ADDRESS+3,2,"R",18.150,7.123);
	Data_ass(0,(unsigned int*)FIFO_ADDRESS+6,3,"F",25.150,10.123);
	Data_ass(0,(unsigned int*)FIFO_ADDRESS+9,4,"D",30.150,12.123);
	Data_ass(0,(unsigned int*)FIFO_ADDRESS+12,5,"L",100.150,22.123);
	Data_ass(0,(unsigned int*)FIFO_ADDRESS+15,6,"B",200.150,10.123);
	Data_ass(0,(unsigned int*)FIFO_ADDRESS+18,7,"B'",15.150,3.123);
	Data_ass(0,(unsigned int*)FIFO_ADDRESS+21,8,"L'",12.150,2.123);
	Data_ass(0,(unsigned int*)FIFO_ADDRESS+24,9,"D'",13.150,1.123);
	Data_ass(0,(unsigned int*)FIFO_ADDRESS+27,10,"F'",1.020,3.123);
	Data_ass(0,(unsigned int*)FIFO_ADDRESS+30,11,"R'",1.50,2.123);
	Data_ass(2,(unsigned int*)FIFO_ADDRESS+33,12,"U'",1.10,0.123);
//	UART((unsigned int*)FIFO_ADDRESS,36);

	button_send(FIFO_ADDRESS);
	__bis_SR_register(LPM3_bits+GIE);         // Enter LPM3, interrupts enabled
	__no_operation();                         // For debugger





	return 0;
}
