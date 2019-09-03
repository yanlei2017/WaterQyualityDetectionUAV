#ifndef __Timer_H
#define	__Timer_H

#include "stm32f10x.h"
#include "stm32f10x_tim.h"
#include "usart.h"	
void TimerNvic_Config(void);
void TIM2_Config(void);
void TIM2_delay(u16 time);
#endif
