#include "sys.h"

/*
�ж�A��ռ���ȼ���B�ߣ���ôA���жϿ�����B���津����������Ӧ���ȼ���
A��B��ռ���ȼ���ͬ����A��B����Ӧ���ȼ�����˭����Ӧ��

�����ȼ�����ռ���ȼ��ǿ��Դ�����ڽ��еĵ���ռ���ȼ��жϵġ� 
��ռ���ȼ���ͬ���жϣ�����Ӧ���ȼ������Դ�ϵ���Ӧ���ȼ����жϡ� 
��ռ���ȼ���ͬ���жϣ��������ж�ͬʱ����������£��ĸ���Ӧ���ȼ��ߣ��ĸ���ִ�С� 
��������жϵ���ռ���ȼ�����Ӧ���ȼ�����һ���Ļ������ĸ��ж��ȷ�������ִ��
*/
void NVIC_Configuration(void)
{
//	NVIC_InitTypeDef NVIC_InitStructure;			
//    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
//			//����NVIC��ʼ���ṹ��
//  /* Set the Vector Table base location at 0x08000000 */
//  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);				    //�����ж����ȼ���Ϊ2����ռ���ȼ��ʹ����ȼ�����λ(����0��3)
	
	/*��ʱ��2�ж���������*/
//	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;				    //�����ж�������
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;	//�����������ȼ�
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;			  //������Ӧ���ȼ�
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				    //ʹ��NVIC
//	NVIC_Init(&NVIC_InitStructure);
//	
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0); 		//�����ж���Ϊ2
//	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn; 		//�ж��¼�ΪTIM2
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3; 	//��ռ���ȼ�0   
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;      //��Ӧ���ȼ�1 
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;         //�����ж� 
//	NVIC_Init(&NVIC_InitStructure);        
//	
//	/*����1�ж���������*/
//	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;				  //�����ж�������
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;	//�����������ȼ�
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;			  //������Ӧ���ȼ�
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				    //ʹ��NVIC
//	NVIC_Init(&NVIC_InitStructure);
//  /*����2�ж���������*/
//  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;				  //�����ж�������
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;	//�����������ȼ�
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;			  //������Ӧ���ȼ�
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				    //ʹ��NVIC
//	NVIC_Init(&NVIC_InitStructure);
//	
//	/*����3�ж���������*/
//	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;				  //�����ж�������
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;	//�����������ȼ�
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;			  //������Ӧ���ȼ�
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				    //ʹ��NVIC
//	NVIC_Init(&NVIC_InitStructure);
  /*����2�ж���������*/
//  NVIC_InitStructure.NVIC_IRQChannel = USART4_IRQn;				  //�����ж�������
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;	//�����������ȼ�
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;			  //������Ӧ���ȼ�
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				    //ʹ��NVIC
//	NVIC_Init(&NVIC_InitStructure);
}
