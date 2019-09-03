#ifndef __SIM800C_H
#define __SIM800C_H			  	 
#include "sys.h"
#include "stdlib.h"	    	

extern char *p1,*p2;  
extern u16 Times,shijian;
extern vu8 Timer0_start;	//定时器0延时启动计数器
void CLR_Buf2(void);
u8 Find(char *a);
void Ram_Initialization(void);
void Second_AT_Command(char *b,char *a,u16 wait_time);
void Wait_CREG(void);
void Set_ATE0(void);
void Connect_Server(void);
void Rec_Server_Data(void);



#endif 

