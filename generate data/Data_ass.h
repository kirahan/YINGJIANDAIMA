/*
 * Data_ass.h
 *
 *  Created on: 2014-5-13
 *      Author: lxy
 */




#ifndef DATA_ASS_H_
#define DATA_ASS_H_

#define DATA_SIZE  48
#define  high_2_bits  BITF+BITE
#define  low_2_bits  BIT0+BIT1
#define  low_4_bits  BIT0+BIT1
#define  low_8	_bits 0x00ff
/*********************************************************
*       ROTATE DATA FROM bit5-bit2     in a 16 bits memory
*					**********0000**
*			R=0010    R'=0011
*			L=1100    L'=1101
*			U=0100    U'=0101
*			D=1010    D'=1011
*			F=1000    F'=1001
*			B=0110    B'=0111
*********************************************************/
#define R_clockwise BIT3
#define R_anticlockwise BIT3+BIT2
#define L_clockwise BIT5+BIT4
#define L_anticlockwise BIT5+BIT4+BIT2

#define U_clockwise BIT4
#define U_anticlockwise BIT4+BIT2
#define D_clockwise BIT5+BIT3
#define D_anticlockwise BIT5+BIT3+BIT2

#define F_clockwise BIT5
#define F_anticlockwise BIT5+BIT2
#define B_clockwise BIT4+BIT3
#define B_anticlockwise BIT4+BIT3+BIT2
/********************************************************/


//function declare
void Data_ass(unsigned int data_type, unsigned int *address, unsigned int step_num, char *rotate_formula, float thinking_time_input,  float operating_time_input );
void cal_flag(unsigned int data_type);
void cal_rotate(char *rotate_formula);
int cal_time(float time);

#endif /* DATA_ASS_H_ */
