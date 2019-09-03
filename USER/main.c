/*----------------------------------------------------------------------------
资源配置：
串口1 ：PA9   PA10 调试使用
串口2 ：PA2   PA3  GPRS
串口3 ：PB10  PB11 预留
串口4 ：PC10  PC11 LORA
串口5 ：PD2   PC12 占用

OLED：
					GND    电源地             
					VCC  接5V或3.3v电源
					D0   接PA5
					D1   接PA7
					RES  接PA1
					DC   接PA0
					CS   接PA4    
RS485模块：  
					VCC接5V
					TXD接PA9
					RXD接PA10
					GND接电源地
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

int send_data_USART(const unsigned char *ptr_data,const unsigned char data_num,USART_TypeDef* USARTx);//向串口发送数据

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
	
		
	//ph 16->字符串
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
			USART_SendData(USARTx,*ptr_data); //向串口发送数据
	
	  	date_i=0;
			while((USART_GetFlagStatus(USARTx,USART_FLAG_TC)!=SET)&&(date_i<0xfff))
			{
				date_i++;
			}//等待发送结束
			ptr_data++;
	}
	return 1;
}


int main(void)
{		
		NVIC_Configuration(); 	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
		Ram_Initialization();
		LED_Init();	
		TimerNvic_Config();
		TIM2_Config();	 
		delay_init();	    	 //延时函数初始化	 
		OLED_Init();			//初始化OLED  

		uart1_init(9600);	 //串口初始化为9600
		uart2_init(9600);
		uart3_init(9600);	 //串口初始化为9600
		uart4_init(9600);
	 		
		delay_ms(1000); 
		Second_AT_Command("AT","OK",3000);	//退出透传模式，避免模块还处于透传模式中
		delay_ms(2000);
		Second_AT_Command("AT+CPIN?","READY",3000);	//退出透传模式，避免模块还处于透传模式中
		delay_ms(2000);
		UART2_SendString("AT+CSQ\r\n");
		delay_ms(1000);
		Wait_CREG();   //等待模块注册成功
		CLR_Buf2();  
		Connect_Server(); //设置GPRS参数
		CLR_Buf2(); 
		
		while(1) 
		{		
			send_data_USART(send_command_do_t,8,UART4);//向串口4发送命令//do
			delay_ms(600);
			
			send_data_USART(send_command_ph_t,8,USART3);//向串口3发送命令//ph
			delay_ms(600);
			
			send_data_USART(send_command_nh,8,USART1);//向串口1发送命令//nn
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
	
	UART2_SendString("AT+CIPSEND\r\n");	//关闭连接
  TIM2_delay(1000);
//	UART2_SendString(send_data_web(data_ph,data_nh,data_t));
	send_data_USART(send_data_web(data_ph,data_do,data_nh,data_t),23,USART2);
  TIM2_delay(2000); 
  USART_SendData(USART2,0X1A);   
  TIM2_delay(1000); 
//	while(!Find("SEND"))
  CLR_Buf2();   
			
			LED1=~LED1;
			Rec_Server_Data();//接收数据并原样返回	
		}
		
	
}








//------------------------------------------------------
//			if(flag_intr>8)
//			{
//				for(i=0;i<8;i++)
//				{
//					USART_SendData(USART3,sendate[i]); //串口3 查询传感器
//					delay_ms(2);		
//				}
//				
//				sendtopc[1]=ans_date[3];
//				sendtopc[2]=ans_date[4];
//				
//				for(i=0;i<6;i++)
//				{
//					USART_SendData(UART4,sendtopc[i]);	//串口4 Lora to PC
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
//			OLED_ShowNum(30,3,j,3,24);//显示ASCII字符的码值 
//			//delay_ms(200);
//			Rec_Server_Data();//接收数据并原样返回	
		  



