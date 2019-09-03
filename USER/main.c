/*----------------------------------------------------------------------------
��Դ���ã�
����1 ��PA9   PA10 ����ʹ��
����2 ��PA2   PA3  GPRS
����3 ��PB10  PB11 Ԥ��
����4 ��PC10  PC11 LORA
����5 ��PD2   PC12 ռ��

OLED��
					GND    ��Դ��             
					VCC  ��5V��3.3v��Դ
					D0   ��PA5
					D1   ��PA7
					RES  ��PA1
					DC   ��PA0
					CS   ��PA4    
RS485ģ�飺  
					VCC��5V
					TXD��PA9
					RXD��PA10
					GND�ӵ�Դ��
//----------------------------------------------------------------------------*/

#include "delay.h"
#include "sys.h"
#include "SysTick.h"
#include "oled.h"
#include "bmp.h"
#include "usart.h"	
#include "string.h"
#include "led.h"
#include "timer.h"
#include "sim800c.h"

unsigned char data_to_web[]={'#','A', 'A',0x00,0x00,'.',0x00,0x00,  0x00,0x00,0x00,'.',0x00, 0x00,0x00,'.',0x00,0x00,0x00,0x00,'.',0x00,0x00};
unsigned char * send_data_web(unsigned char *pdata_ph,unsigned char *pdata_do,unsigned char *pdata_nh,unsigned char *pdata_t);
const unsigned char send_command_ph_t[8]={0x02,0x03,0x00,0x00,0x00,0x02,0xc4,0x38};
const unsigned char send_command_do_t[8]={0x01,0x03,0x00,0x00,0x00,0x02,0xc4,0x0b};
const unsigned char send_command_nh[8]={0x02,0x03,0x00,0x00,0x00,0x01,0x84,0x39};

int send_data_USART(const unsigned char *ptr_data,const unsigned char data_num,USART_TypeDef* USARTx);//�򴮿ڷ�������

unsigned char data_do[2]={0x00,0x00};
unsigned char data_ph[2]={0x00,0x00};
unsigned char data_nh[2]={0x00,0x00};
unsigned char data_t[2]={0x00,0x00};

unsigned char * send_data_web(unsigned char *pdata_ph,unsigned char *pdata_do,unsigned char *pdata_nh,unsigned char *pdata_t)
{
	int data_ph=((pdata_ph[0]<<8)&0xff00)+(pdata_ph[1]&0x00ff);
	int data_do=((pdata_do[0]<<8)&0xff00)+(pdata_do[1]&0x00ff)-15;
	int data_nh=((pdata_nh[0]<<8)&0xff00)+(pdata_nh[1]&0x00ff);
	int data_t=((pdata_t[0]<<8)&0xff00)+(pdata_t[1]&0x00ff)-10;
	
		
	//ph 16->�ַ���
	data_to_web[8]=(data_ph%10000/1000+'0');	
	data_to_web[9]=(data_ph%1000/100+'0');
	data_to_web[10]='.';
	data_to_web[11]=(data_ph%100/10+'0');
	data_to_web[12]=(data_ph%10+'0');
	
	data_to_web[18]=(data_t%10000/1000+'0');	
	data_to_web[19]=(data_t%1000/100+'0');
	data_to_web[20]=(data_t%100/10+'0');
	data_to_web[21]='.';
	data_to_web[22]=(data_t%10+'0');
	
	

	data_to_web[3]=(data_do%10000/1000+'0');	
	data_to_web[4]=(data_do%1000/100+'0');
	data_to_web[5]='.';	
	data_to_web[6]=(data_do%100/10+'0');
	data_to_web[7]=(data_do%10+'0');
	
	data_to_web[13]=(data_nh%10000/1000+'0');	
	data_to_web[14]=(data_nh%1000/100+'0');
	data_to_web[15]='.';	
	data_to_web[16]=(data_nh%100/10+'0');
	data_to_web[17]=(data_nh%10+'0');
	
	return data_to_web;
}


int send_data_USART(const unsigned char * ptr_data,const unsigned char data_num,USART_TypeDef* USARTx)
{
	u8 data_count=0;
	u16 date_i=0;
	for(data_count=0;data_count<data_num;data_count++)
	{  
			USART_SendData(USARTx,*ptr_data); //�򴮿ڷ�������
	
	  	date_i=0;
			while((USART_GetFlagStatus(USARTx,USART_FLAG_TC)!=SET)&&(date_i<0xfff))
			{
				date_i++;
			}//�ȴ����ͽ���
			ptr_data++;
	}
	return 1;
}


int main(void)
{		
		NVIC_Configuration(); 	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
		Ram_Initialization();
		LED_Init();	
		TimerNvic_Config();
		TIM2_Config();	 
		delay_init();	    	 //��ʱ������ʼ��	 
		OLED_Init();			//��ʼ��OLED  

		uart1_init(9600);	 //���ڳ�ʼ��Ϊ9600
		uart2_init(9600);
		uart3_init(9600);	 //���ڳ�ʼ��Ϊ9600
		uart4_init(9600);
	 		
		delay_ms(1000); 
		Second_AT_Command("AT","OK",3000);	//�˳�͸��ģʽ������ģ�黹����͸��ģʽ��
		delay_ms(2000);
		Second_AT_Command("AT+CPIN?","READY",3000);	//�˳�͸��ģʽ������ģ�黹����͸��ģʽ��
		delay_ms(2000);
		UART2_SendString("AT+CSQ\r\n");
		delay_ms(1000);
		Wait_CREG();   //�ȴ�ģ��ע��ɹ�
		CLR_Buf2();  
		Connect_Server(); //����GPRS����
		CLR_Buf2(); 
		
		while(1) 
		{		
			send_data_USART(send_command_do_t,8,UART4);//�򴮿�4��������//do
			delay_ms(600);
			
			send_data_USART(send_command_ph_t,8,USART3);//�򴮿�3��������//ph
			delay_ms(600);
			
			send_data_USART(send_command_nh,8,USART1);//�򴮿�1��������//nn
			delay_ms(600);
		
		
			if(flag_nh_finsh==1||flag_do_finsh==1||flag_ph_finsh==1)
			{
				data_t[0]=receive_data_do_t[5];
				data_t[1]=receive_data_do_t[6];
			
				data_ph[0]=receive_data_ph_t[3];
				data_ph[1]=receive_data_ph_t[4];
			
				data_do[0]=receive_data_do_t[3];
				data_do[1]=receive_data_do_t[4];	
			
				data_nh[0]=receive_data_nh[3];
				data_nh[1]=receive_data_nh[4];

				delay_ms(500);

				flag_ph_finsh=0;
				flag_do_finsh=0;
				flag_nh_finsh=0;
			}
	
	UART2_SendString("AT+CIPSEND\r\n");	//�ر�����
  TIM2_delay(1000);
//	UART2_SendString(send_data_web(data_ph,data_nh,data_t));
	send_data_USART(send_data_web(data_ph,data_do,data_nh,data_t),23,USART2);
  TIM2_delay(2000); 
  USART_SendData(USART2,0X1A);   
  TIM2_delay(1000); 
//	while(!Find("SEND"))
  CLR_Buf2();   
			
			LED1=~LED1;
			Rec_Server_Data();//�������ݲ�ԭ������	
		}
		
	
}








//------------------------------------------------------
//			if(flag_intr>8)
//			{
//				for(i=0;i<8;i++)
//				{
//					USART_SendData(USART3,sendate[i]); //����3 ��ѯ������
//					delay_ms(2);		
//				}
//				
//				sendtopc[1]=ans_date[3];
//				sendtopc[2]=ans_date[4];
//				
//				for(i=0;i<6;i++)
//				{
//					USART_SendData(UART4,sendtopc[i]);	//����4 Lora to PC
//					delay_ms(3);
//				}
//				
//				
//				delay_ms(500);
//			}
//		//USART_SendData(USART2,0X66);
////------------------------------------------------------			
//			j=(ans_date[3]<<8) +(ans_date[4]&0x00ff);		
//			OLED_ShowString(3,3,"PH:");  
//			OLED_ShowNum(30,3,j,3,24);//��ʾASCII�ַ�����ֵ 
//			//delay_ms(200);
//			Rec_Server_Data();//�������ݲ�ԭ������	
		  



