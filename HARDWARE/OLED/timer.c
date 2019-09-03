/******************** (C) COPYRIGHT  源地工作室 ********************************
 * 文件名  ：timer.c
 * 描述    ：初始化通用定时器TIM2，实现TIM2定时功能    
 * 作者    ：zhuoyingxingyu
 * 淘宝    ：源地工作室http://vcc-gnd.taobao.com/
 * 论坛地址：极客园地-嵌入式开发论坛http://vcc-gnd.com/
 * 版本更新: 2017-04-20
 * 硬件连接: 无
 * 调试方式：J-Link-OB
**********************************************************************************/	
//头文件
#include "timer.h"
#include "led.h"
#include "sim800c.h"
//变量定义
uint16_t TIM_Count;	//定时器计数


void TimerNvic_Config(void);

 /**
  * @file   TimerNvic_Config
  * @brief  系统中断配置
  * @param  无
  * @retval 无
  */
void TimerNvic_Config(void)
{ 	
	NVIC_InitTypeDef	NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0); 		//设置中断组为2
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn; 		//中断事件为TIM2
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; 	//抢占优先级0   
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;      //响应优先级1 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;         //允许中断 
	NVIC_Init(&NVIC_InitStructure);                         //根据指定参数初始化中断寄存器
}
 /**
  * @file   TIM2_Config
  * @brief  调用函数库，初始化定时器2的配置
  * @param  无
  * @retval 无
  */
void TIM2_Config(void)
{
	//定时器定时时间T计算公式：T=(TIM_Period+1)*(TIM_Prescaler+1)/TIMxCLK
	TIM_TimeBaseInitTypeDef	 TIM_BaseInitStructure;			//定义一个定时器结构体变量
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);   	//使能定时器2
	TIM_DeInit( TIM2);//将IM2定时器初始化位复位值
	TIM_InternalClockConfig(TIM2);//配置 TIM2 内部时钟
	TIM_BaseInitStructure.TIM_Period = 1000;//设置自动重载寄存器值为最大值	0~65535之间  1000000/1000=1000us=1ms													
	//TIM_Period（TIM1_ARR）=1000，计数器向上计数到1000后产生更新事件，计数值归零 也就是 1MS产生更新事件一次
	TIM_BaseInitStructure.TIM_Prescaler = 71;//自定义预分频系数，提供给定时器的时钟	0~65535之间
													//设置预分频器分频系数71，即CK_INT=72M, TIM1_CLK=72/72=1MHz 
	TIM_BaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1; //时钟分割为0
	TIM_BaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;   
													//TIM向上计数模式 从0开始向上计数，计数到1000后产生更新事件
	TIM_TimeBaseInit(TIM2, &TIM_BaseInitStructure); //根据指定参数初始化TIM时间基数寄存器
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);//清中断，以免一启用中断后立即产生中断 
	TIM_ARRPreloadConfig(TIM2, DISABLE);//禁止ARR 预装载缓冲器
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);//使能TIM2中断源 
	TIM_Cmd(TIM2, ENABLE);//TIM2总开关：开启
  TimerNvic_Config();	
}


 /**
  * @file   TIM2_IRQHandler
  * @brief  定时器2中断处理函数,1ms的中断
  * @param  无
  * @retval 无
  */
void TIM2_IRQHandler(void)
{	
	static uint16_t count=0;
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) 		//判断中断标志位是否为1；为1则中断响应啦，0则没有
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);       	//清楚中断挂起位
		count++;
		TIM_Count--;										//定时延时变量减1
//		if(count==500)										//计数到1000ms
//		{
//	    								//计数清零
//			GPIO_ResetBits(GPIOC,GPIO_Pin_6);
//			
//		}		

//				if(count==1000)										//计数到1000ms
//		{
//   								//计数清零
//			GPIO_ResetBits(GPIOC,GPIO_Pin_0);
//			
//		}	

//		
//		if(count==1500)										//计数到1000ms
//		{
//   		count=0;						//计数清零
//			GPIO_SetBits(GPIOC,GPIO_Pin_6);
//			
//		}	
		
		if(Timer0_start)
		Times++;
		if(Times > shijian)
		{
			Timer0_start = 0;
			Times = 0;
		}
		
	}
   
}

 /**
  * @file   TIM2_delay
  * @brief  定时器产生延时
  * @param  延时时间T = time*1(ms) 
  * @retval 无
  */
void TIM2_delay(u16 time)
{
 	TIM_Count = time;
	while(TIM_Count); 
}
