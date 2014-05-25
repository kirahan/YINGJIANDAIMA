/*
 * HandShake.h
 *
 *  Created on: 2014-5-21
 *      Author: lxy
 */

#ifndef HANDSHAKE_H_
#define HANDSHAKE_H_


void HandShake();
void HANDS_IO_CONFIG();
void HANDS_CLK_CONFIG();
void HANDS_TA_CONFIG();
void HANDS_COMPARATOR_CONFIG();
void record16bits(unsigned int *value_16) ;
void HANDS_REGISTER_INIT();
void HANDS_DECODING();
void record0(unsigned int*address);
void record1(unsigned int*address);

#endif /* HANDSHAKE_H_ */
