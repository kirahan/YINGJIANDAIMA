/****************************************************
 * Name: Data_ass.c
 * Function: generator simulated rubik's cube single rotate data into an FRAM memory
 * Parameter input: data_type; step_num; rotate_formula; thinking_time; operating_time;
 *
 *
 *
 *
 * Parameter output: N/A
 *
 * Explain:
 * 			Data Form
 *___________________________________________________________________________________________________
 *|				|				|				|				 |                     | 			|
 *|	start_flag	|	step_value	| rotate_data 	|  thinking_time | operating_time      | end_flag	|
 *|		2bits	|		8bits	|	4bits		|		16bits   |    16bits           |  2bits	    |
 *|             |               |               |                |                     |            |
 *|             |               |               |                |                     |            |
 *|_____________|_______________|_______________|________________|_____________________|____________|
 *
 *
 * Case example:
 *
 *
 *
 *
 * Created on: 2014-5-13
 * Author: Kira
 *******************************************/
#include "msp430.h"
#include "Data_ass.h"
#include "string.h"

unsigned int start_flag;
unsigned int end_flag;
unsigned int step_value;
unsigned int rotate_data;
unsigned int thinking_time;
unsigned int operating_time;
unsigned int shift_bit_buffer1;
unsigned int shift_bit_buffer2;

void Data_ass(unsigned int data_type, unsigned int *address, unsigned int step_num, char *rotate_formula, float thinking_time_input,  float operating_time_input )
{




		cal_flag(data_type);
		step_value=step_num<<6;
		cal_rotate(rotate_formula);
		thinking_time=cal_time(thinking_time_input);
		operating_time=cal_time(operating_time_input);

		shift_bit_buffer1=thinking_time>>14;
		*address++=start_flag+step_value+rotate_data+shift_bit_buffer1;			//2bits start_flag +8bits step_value +4bits rotate_data +2 bits highest thingking_time

		shift_bit_buffer1=thinking_time<<2;
		shift_bit_buffer2=operating_time>>14;
		*(address++)=shift_bit_buffer1+shift_bit_buffer2;			//14bits thingking_time+2 bits highest operating_time

		shift_bit_buffer1=operating_time<<2;
		*(address++)=shift_bit_buffer1+end_flag;					//14bits operating_time +2 bits end_flag

}


void cal_flag(unsigned int data_type)
{
	if(data_type==0)                       // middle data
	{
			start_flag=0x0;
			end_flag=0x0;
	}
	else if(data_type==1)				   // start flag
	{
			start_flag=high_2_bits;
			end_flag=0x0;
	}
	else if(data_type==2)				 // end flag
		{
				start_flag=0x00;
				end_flag=low_2_bits;
		}
//	else
//	return(0);
}


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
void cal_rotate(char *rotate_formula)
{
		if (strcmp(rotate_formula, "R") == 0)
		rotate_data=R_clockwise;

	else
		if (strcmp(rotate_formula, "R'") == 0)
				rotate_data=R_anticlockwise;
    else
    	if (strcmp(rotate_formula, "L") == 0)
       			rotate_data=L_clockwise;
	else
		if (strcmp(rotate_formula, "L'") == 0)
				rotate_data=L_anticlockwise;
	else
		if (strcmp(rotate_formula, "U") == 0)
				rotate_data=U_clockwise;
	else
		if (strcmp(rotate_formula, "U'") == 0)
				rotate_data=U_anticlockwise;
	else
		if (strcmp(rotate_formula, "D") == 0)
				rotate_data=D_clockwise;
	else
		if (strcmp(rotate_formula, "D'") == 0)
				rotate_data=D_anticlockwise;
	else
		if (strcmp(rotate_formula, "F") == 0)
				rotate_data=F_clockwise;
	else
		if (strcmp(rotate_formula, "F'") == 0)
				rotate_data=F_anticlockwise;
	else
		if (strcmp(rotate_formula, "B") == 0)
				rotate_data=B_clockwise;
	else
		if (strcmp(rotate_formula, "B'") == 0)
				rotate_data=B_anticlockwise;
//	else
//		return(0);
}


int cal_time(float time)
{
	unsigned int time_out;
	unsigned int time_unit;
	unsigned int buffer;

if(time>16)
{
	time_out=(unsigned int)time;
	time_unit=1;
	time_out=time_out<<1;
	time_out=time_out+time_unit;
}
else
{
	time_out=(unsigned int)time;
	buffer=(int)(time*1000-time_out*1000);
	time_out=time_out<<10;
	time_out=time_out+buffer;
	time_unit=0;
	time_out=time_out<<1;
	time_out=time_out+time_unit;
}

return(time_out);
}


