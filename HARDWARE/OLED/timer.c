/******************** (C) COPYRIGHT  Դ�ع����� ********************************
 * �ļ���  ��timer.c
 * ����    ����ʼ��ͨ�ö�ʱ��TIM2��ʵ��TIM2��ʱ����    
 * ����    ��zhuoyingxingyu
 * �Ա�    ��Դ�ع�����http://vcc-gnd.taobao.com/
 * ��̳��ַ������԰��-Ƕ��ʽ������̳http://vcc-gnd.com/
 * �汾����: 2017-04-20
 * Ӳ������: ��
 * ���Է�ʽ��J-Link-OB
**********************************************************************************/	
//ͷ�ļ�
#include "timer.h"
#include "led.h"
#include "sim800c.h"
//��������
uint16_t TIM_Count;	//��ʱ������


void TimerNvic_Config(void);

 /**
  * @file   TimerNvic_Config
  * @brief  ϵͳ�ж�����
  * @param  ��
  * @retval ��
  */
void TimerNvic_Config(void)
{ 	
	NVIC_InitTypeDef	NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0); 		//�����ж���Ϊ2
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn; 		//�ж��¼�ΪTIM2
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; 	//��ռ���ȼ�0   
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;      //��Ӧ���ȼ�1 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;         //�����ж� 
	NVIC_Init(&NVIC_InitStructure);                         //����ָ��������ʼ���жϼĴ���
}
 /**
  * @file   TIM2_Config
  * @brief  ���ú����⣬��ʼ����ʱ��2������
  * @param  ��
  * @retval ��
  */
void TIM2_Config(void)
{
	//��ʱ����ʱʱ��T���㹫ʽ��T=(TIM_Period+1)*(TIM_Prescaler+1)/TIMxCLK
	TIM_TimeBaseInitTypeDef	 TIM_BaseInitStructure;			//����һ����ʱ���ṹ�����
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);   	//ʹ�ܶ�ʱ��2
	TIM_DeInit( TIM2);//��IM2��ʱ����ʼ��λ��λֵ
	TIM_InternalClockConfig(TIM2);//���� TIM2 �ڲ�ʱ��
	TIM_BaseInitStructure.TIM_Period = 1000;//�����Զ����ؼĴ���ֵΪ���ֵ	0~65535֮��  1000000/1000=1000us=1ms													
	//TIM_Period��TIM1_ARR��=1000�����������ϼ�����1000����������¼�������ֵ���� Ҳ���� 1MS���������¼�һ��
	TIM_BaseInitStructure.TIM_Prescaler = 71;//�Զ���Ԥ��Ƶϵ�����ṩ����ʱ����ʱ��	0~65535֮��
													//����Ԥ��Ƶ����Ƶϵ��71����CK_INT=72M, TIM1_CLK=72/72=1MHz 
	TIM_BaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1; //ʱ�ӷָ�Ϊ0
	TIM_BaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;   
													//TIM���ϼ���ģʽ ��0��ʼ���ϼ�����������1000����������¼�
	TIM_TimeBaseInit(TIM2, &TIM_BaseInitStructure); //����ָ��������ʼ��TIMʱ������Ĵ���
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);//���жϣ�����һ�����жϺ����������ж� 
	TIM_ARRPreloadConfig(TIM2, DISABLE);//��ֹARR Ԥװ�ػ�����
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);//ʹ��TIM2�ж�Դ 
	TIM_Cmd(TIM2, ENABLE);//TIM2�ܿ��أ�����
  TimerNvic_Config();	
}


 /**
  * @file   TIM2_IRQHandler
  * @brief  ��ʱ��2�жϴ�����,1ms���ж�
  * @param  ��
  * @retval ��
  */
void TIM2_IRQHandler(void)
{	
	static uint16_t count=0;
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) 		//�ж��жϱ�־λ�Ƿ�Ϊ1��Ϊ1���ж���Ӧ����0��û��
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);       	//����жϹ���λ
		count++;
		TIM_Count--;										//��ʱ��ʱ������1
//		if(count==500)										//������1000ms
//		{
//	    								//��������
//			GPIO_ResetBits(GPIOC,GPIO_Pin_6);
//			
//		}		

//				if(count==1000)										//������1000ms
//		{
//   								//��������
//			GPIO_ResetBits(GPIOC,GPIO_Pin_0);
//			
//		}	

//		
//		if(count==1500)										//������1000ms
//		{
//   		count=0;						//��������
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
  * @brief  ��ʱ��������ʱ
  * @param  ��ʱʱ��T = time*1(ms) 
  * @retval ��
  */
void TIM2_delay(u16 time)
{
 	TIM_Count = time;
	while(TIM_Count); 
}
