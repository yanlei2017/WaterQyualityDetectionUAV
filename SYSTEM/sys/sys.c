#include "sys.h"

/*
中断A抢占优先级比B高，那么A的中断可以在B里面触发，忽略响应优先级；
A和B抢占优先级相同，则A、B的响应优先级决定谁先响应；

高优先级的抢占优先级是可以打断正在进行的低抢占优先级中断的。 
抢占优先级相同的中断，高响应优先级不可以打断低响应优先级的中断。 
抢占优先级相同的中断，当两个中断同时发生的情况下，哪个响应优先级高，哪个先执行。 
如果两个中断的抢占优先级和响应优先级都是一样的话，则看哪个中断先发生就先执行
*/
void NVIC_Configuration(void)
{
//	NVIC_InitTypeDef NVIC_InitStructure;			
//    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//设置NVIC中断分组2:2位抢占优先级，2位响应优先级
//			//定义NVIC初始化结构体
//  /* Set the Vector Table base location at 0x08000000 */
//  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);				    //设置中断优先级组为2，先占优先级和从优先级各两位(可设0～3)
	
	/*定时器2中断向量配置*/
//	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;				    //设置中断向量号
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;	//设置抢先优先级
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;			  //设置响应优先级
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				    //使能NVIC
//	NVIC_Init(&NVIC_InitStructure);
//	
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0); 		//设置中断组为2
//	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn; 		//中断事件为TIM2
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3; 	//抢占优先级0   
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;      //响应优先级1 
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;         //允许中断 
//	NVIC_Init(&NVIC_InitStructure);        
//	
//	/*串口1中断向量配置*/
//	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;				  //设置中断向量号
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;	//设置抢先优先级
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;			  //设置响应优先级
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				    //使能NVIC
//	NVIC_Init(&NVIC_InitStructure);
//  /*串口2中断向量配置*/
//  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;				  //设置中断向量号
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;	//设置抢先优先级
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;			  //设置响应优先级
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				    //使能NVIC
//	NVIC_Init(&NVIC_InitStructure);
//	
//	/*串口3中断向量配置*/
//	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;				  //设置中断向量号
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;	//设置抢先优先级
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;			  //设置响应优先级
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				    //使能NVIC
//	NVIC_Init(&NVIC_InitStructure);
  /*串口2中断向量配置*/
//  NVIC_InitStructure.NVIC_IRQChannel = USART4_IRQn;				  //设置中断向量号
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;	//设置抢先优先级
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;			  //设置响应优先级
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				    //使能NVIC
//	NVIC_Init(&NVIC_InitStructure);
}
