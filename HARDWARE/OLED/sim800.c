#include "timer.h"
#include "usart.h"
#include "delay.h"
#include "string.h"

#define Buf2_Max 200 					  //����2���泤��
char *p1,*p2;   
u16 Times=0,shijian=0;
vu8 Timer0_start;	//��ʱ��0��ʱ����������
char First_Int;

const char *string = "AT+CIPSTART=\"TCP\",\"tcp.tlink.io\",8647";//IP��¼������,��Ҫ�����Լ���IP�Ͷ˿����޸�

char Uart2_Buf[Buf2_Max]; //����2���ջ���
char *p1,*p2;    

void Ram_Initialization(void)
{
	Times=0;
	shijian=0;
	Timer0_start=0;
	p1=Uart2_Buf;
	p2=p1;
}

/*******************************************************************************
* ������ : CLR_Buf2
* ����   : �������2��������
* ����   : 
* ���   : 
* ����   : 
* ע��   : 
*******************************************************************************/
void CLR_Buf2(void)
{
	u16 k;
	for(k=0;k<Buf2_Max;k++)      //��������������
	{
		Uart2_Buf[k] = 0x00;
	}
  p1=Uart2_Buf;               //�����ַ�������ʼ�洢λ��
}

/*******************************************************************************
* ������ : Find
* ����   : �жϻ������Ƿ���ָ�����ַ���
* ����   : 
* ���   : 
* ����   : unsigned char:1 �ҵ�ָ���ַ���0 δ�ҵ�ָ���ַ� 
* ע��   : 
*******************************************************************************/

u8 Find(char *a)
{ 
  if(strstr(Uart2_Buf,a)!=NULL)
	    return 1;
	else
			return 0;
}

/*******************************************************************************
* ������ : Second_AT_Command
* ����   : ����ATָ���
* ����   : �������ݵ�ָ�롢���͵ȴ�ʱ��(��λ��MS)
* ���   : 
* ����   : 
* ע��   : 
*******************************************************************************/

void Second_AT_Command(char *b,char *a,u16 wait_time)         
{
	u8 i;
	char *c;
	c = b;										//�����ַ�����ַ��c
	CLR_Buf2(); 
  i = 0;
	while(i == 0)                    
	{
		if(!Find(a)) 
		{
			if(Timer0_start == 0)
			{
				b = c;							//���ַ�����ַ��b
				for (; *b!='\0';b++)
				{
					while(USART_GetFlagStatus(USART2, USART_FLAG_TC)==RESET);
					USART_SendData(USART2,*b);//UART2_SendData(*b);
				}
				UART2_SendLR();	
				Times = 0;
				shijian = wait_time;
				Timer0_start = 1;
		   }
    }
 	  else
		{
			i = 1;
			Timer0_start = 0;
		}
	}
	CLR_Buf2(); 
}

/*******************************************************************************
* ������ : Wait_CREG
* ����   : �ȴ�ģ��ע��ɹ�
* ����   : 
* ���   : 
* ����   : 
* ע��   : 
*******************************************************************************/
void Wait_CREG(void)
{
	u8 i;
	u8 k;
	i = 0;
  while(i == 0)        			
	{
		CLR_Buf2();        
		UART2_SendString("AT+CREG?");   //��ѯ�ȴ�ģ��ע��ɹ�
		UART2_SendLR();
		delay_ms(5000);  						
	    for(k=0;k<Buf2_Max;k++)      			
    	{
			if(Uart2_Buf[k] == ':')
			{
				if((Uart2_Buf[k+4] == '1')||(Uart2_Buf[k+4] == '5'))   //˵��ע��ɹ�
				{
					i = 1;
					UART1_SendLR();
				  break;
				}
			}
		}
		UART1_SendString("ע����.....");
	}
}

/*******************************************************************************
* ������ : Set_ATE0
* ����   : ȡ������
* ����   : 
* ���   : 
* ����   : 
* ע��   : 
*******************************************************************************/
void Set_ATE0(void)
{
	Second_AT_Command("ATE0","OK",3000);								//ȡ������		
}
/*******************************************************************************
* ������ : Connect_Server
* ����   : GPRS���ӷ���������
* ����   : 
* ���   : 
* ����   : 
* ע��   : 
*******************************************************************************/
void Connect_Server(void)
{
	UART2_SendString("AT+CIPCLOSE\r\n");		
	TIM2_delay(2000);
//	UART1_SendString("�ر�����\r\n");	

	UART2_SendString("AT+CIPSHUT\r\n");		
	TIM2_delay(2000);
//	UART1_SendString("����GPRS PDP����\r\n");	
	
	UART2_SendString("AT+CGCLASS=\"B\"\r\n");		
	TIM2_delay(2000);
//	UART1_SendString("����GPRS�ƶ�̨���\r\n");	
	
	UART2_SendString("AT+CGDCONT=1,\"IP\",\"CMNET\"\r\n");		
	TIM2_delay(2000);
//	UART1_SendString("���÷���\r\n");	
	
	UART2_SendString("AT+CGATT=1\r\n");		
	TIM2_delay(2000);
//	UART1_SendString("����GPRS����\r\n");
	
	UART2_SendString("AT+CGACT=1,1\r\n");		
	TIM2_delay(2000);
//	UART1_SendString("���ŷ�ʽ\r\n");
	

  Second_AT_Command((char*)string,"CONNECT",5000);//��������
	TIM2_delay(8000);
	CLR_Buf2(); 
	
	UART2_SendString("AT+CIPSEND\r\n");	//�ر�����
  TIM2_delay(3000);
	UART2_SendString("0VW2J1GD89XU7036");//Q9684X7Z5KP39ITO
  TIM2_delay(8000); 
  USART_SendData(USART2,0X1A);   
  TIM2_delay(3000); 
//	while(!Find("SEND"))
  CLR_Buf2();                                    //�崮�ڽ��ջ���Ϊ͸��ģʽ׼��
}                      //�崮�ڽ��ջ���Ϊ͸��ģʽ׼��

/*******************************************************************************
* ������ : Rec_Server_Data
* ����   : ���շ��������ݺ���,��ԭ������
* ����   : 
* ���   : 
* ����   : 
* ע��   : 
*******************************************************************************/
void Rec_Server_Data(void)
{
	if(p2!=p1)   		//˵����������δ����
	{	
		while(USART_GetFlagStatus(USART2, USART_FLAG_TC)==RESET);
		USART_SendData(USART2,*p2);
		p2++;
    if(p2>&Uart2_Buf[Buf2_Max])
			p2=Uart2_Buf;
	}
}

void USART2_IRQHandler(void)                	
{
			u8 Res=0;
      if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
			{
					Res=USART_ReceiveData(USART2);
					Uart2_Buf[First_Int] = Res;  	  //�����յ����ַ����浽������
					First_Int++;                			//����ָ������ƶ�
					if(First_Int>Buf2_Max)       		//���������,������ָ��ָ�򻺴���׵�ַ
					{
						First_Int = 0;
					}   

					*p1 = Res;  	  //�����յ����ַ����浽������
					p1++;                			//����ָ������ƶ�
					if(p1>&Uart2_Buf[Buf2_Max])       		//���������,������ָ��ָ�򻺴���׵�ַ
					{
						p1=Uart2_Buf;			
					} 
					
					
				if(strstr(Uart2_Buf,"gd1k")!=NULL)    //�������ַ����к�
				{				
					GPIO_ResetBits(GPIOB,GPIO_Pin_9);GPIO_SetBits(GPIOC,GPIO_Pin_0);
					CLR_Buf2(); 
				}
				
				else if(strstr(Uart2_Buf,"gd2k")!=NULL)  //
				{				
					GPIO_SetBits(GPIOB,GPIO_Pin_9);GPIO_ResetBits(GPIOC,GPIO_Pin_0);
						CLR_Buf2(); 
				}
			
				else if(strstr(Uart2_Buf,"gd3k")!=NULL)  //����Ƿ��յ�led1
				{				
					GPIO_SetBits(GPIOB,GPIO_Pin_9); GPIO_SetBits(GPIOC,GPIO_Pin_0);
						CLR_Buf2(); 
				}
				
				else  	
				{				
					
				}
			
			}			
}
