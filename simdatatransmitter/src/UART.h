/*
 * UART.h
 *
 *  Created on: 2014-5-14
 *      Author: lxy
 */

#ifndef UART_H_
#define UART_H_

void UART(unsigned int *start_address, unsigned int length);
void IO_config(void);
void clk_init(void);
void uart_init(void);


#endif /* UART_H_ */
