#include "timer.h"
#include "usart.h"
#include "delay.h"
#include "string.h"

#define Buf2_Max 200 					  //串口2缓存长度
char *p1,*p2;   
u16 Times=0,shijian=0;
vu8 Timer0_start;	//定时器0延时启动计数器
char First_Int;

const char *string = "AT+CIPSTART=\"TCP\",\"tcp.tlink.io\",8647";//IP登录服务器,需要根据自己的IP和端口做修改

char Uart2_Buf[Buf2_Max]; //串口2接收缓存
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
* 函数名 : CLR_Buf2
* 描述   : 清除串口2缓存数据
* 输入   : 
* 输出   : 
* 返回   : 
* 注意   : 
*******************************************************************************/
void CLR_Buf2(void)
{
	u16 k;
	for(k=0;k<Buf2_Max;k++)      //将缓存内容清零
	{
		Uart2_Buf[k] = 0x00;
	}
  p1=Uart2_Buf;               //接收字符串的起始存储位置
}

/*******************************************************************************
* 函数名 : Find
* 描述   : 判断缓存中是否含有指定的字符串
* 输入   : 
* 输出   : 
* 返回   : unsigned char:1 找到指定字符，0 未找到指定字符 
* 注意   : 
*******************************************************************************/

u8 Find(char *a)
{ 
  if(strstr(Uart2_Buf,a)!=NULL)
	    return 1;
	else
			return 0;
}

/*******************************************************************************
* 函数名 : Second_AT_Command
* 描述   : 发送AT指令函数
* 输入   : 发送数据的指针、发送等待时间(单位：MS)
* 输出   : 
* 返回   : 
* 注意   : 
*******************************************************************************/

void Second_AT_Command(char *b,char *a,u16 wait_time)         
{
	u8 i;
	char *c;
	c = b;										//保存字符串地址到c
	CLR_Buf2(); 
  i = 0;
	while(i == 0)                    
	{
		if(!Find(a)) 
		{
			if(Timer0_start == 0)
			{
				b = c;							//将字符串地址给b
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
* 函数名 : Wait_CREG
* 描述   : 等待模块注册成功
* 输入   : 
* 输出   : 
* 返回   : 
* 注意   : 
*******************************************************************************/
void Wait_CREG(void)
{
	u8 i;
	u8 k;
	i = 0;
  while(i == 0)        			
	{
		CLR_Buf2();        
		UART2_SendString("AT+CREG?");   //查询等待模块注册成功
		UART2_SendLR();
		delay_ms(5000);  						
	    for(k=0;k<Buf2_Max;k++)      			
    	{
			if(Uart2_Buf[k] == ':')
			{
				if((Uart2_Buf[k+4] == '1')||(Uart2_Buf[k+4] == '5'))   //说明注册成功
				{
					i = 1;
					UART1_SendLR();
				  break;
				}
			}
		}
		UART1_SendString("注册中.....");
	}
}

/*******************************************************************************
* 函数名 : Set_ATE0
* 描述   : 取消回显
* 输入   : 
* 输出   : 
* 返回   : 
* 注意   : 
*******************************************************************************/
void Set_ATE0(void)
{
	Second_AT_Command("ATE0","OK",3000);								//取消回显		
}
/*******************************************************************************
* 函数名 : Connect_Server
* 描述   : GPRS连接服务器函数
* 输入   : 
* 输出   : 
* 返回   : 
* 注意   : 
*******************************************************************************/
void Connect_Server(void)
{
	UART2_SendString("AT+CIPCLOSE\r\n");		
	TIM2_delay(2000);
//	UART1_SendString("关闭连接\r\n");	

	UART2_SendString("AT+CIPSHUT\r\n");		
	TIM2_delay(2000);
//	UART1_SendString("撤销GPRS PDP环境\r\n");	
	
	UART2_SendString("AT+CGCLASS=\"B\"\r\n");		
	TIM2_delay(2000);
//	UART1_SendString("设置GPRS移动台类别\r\n");	
	
	UART2_SendString("AT+CGDCONT=1,\"IP\",\"CMNET\"\r\n");		
	TIM2_delay(2000);
//	UART1_SendString("配置方案\r\n");	
	
	UART2_SendString("AT+CGATT=1\r\n");		
	TIM2_delay(2000);
//	UART1_SendString("附着GPRS网络\r\n");
	
	UART2_SendString("AT+CGACT=1,1\r\n");		
	TIM2_delay(2000);
//	UART1_SendString("附着方式\r\n");
	

  Second_AT_Command((char*)string,"CONNECT",5000);//建立连接
	TIM2_delay(8000);
	CLR_Buf2(); 
	
	UART2_SendString("AT+CIPSEND\r\n");	//关闭连接
  TIM2_delay(3000);
	UART2_SendString("0VW2J1GD89XU7036");//Q9684X7Z5KP39ITO
  TIM2_delay(8000); 
  USART_SendData(USART2,0X1A);   
  TIM2_delay(3000); 
//	while(!Find("SEND"))
  CLR_Buf2();                                    //清串口接收缓存为透传模式准备
}                      //清串口接收缓存为透传模式准备

/*******************************************************************************
* 函数名 : Rec_Server_Data
* 描述   : 接收服务器数据函数,并原样返回
* 输入   : 
* 输出   : 
* 返回   : 
* 注意   : 
*******************************************************************************/
void Rec_Server_Data(void)
{
	if(p2!=p1)   		//说明还有数据未发出
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
					Uart2_Buf[First_Int] = Res;  	  //将接收到的字符串存到缓存中
					First_Int++;                			//缓存指针向后移动
					if(First_Int>Buf2_Max)       		//如果缓存满,将缓存指针指向缓存的首地址
					{
						First_Int = 0;
					}   

					*p1 = Res;  	  //将接收到的字符串存到缓存中
					p1++;                			//缓存指针向后移动
					if(p1>&Uart2_Buf[Buf2_Max])       		//如果缓存满,将缓存指针指向缓存的首地址
					{
						p1=Uart2_Buf;			
					} 
					
					
				if(strstr(Uart2_Buf,"gd1k")!=NULL)    //若缓存字符串中含
				{				
					GPIO_ResetBits(GPIOB,GPIO_Pin_9);GPIO_SetBits(GPIOC,GPIO_Pin_0);
					CLR_Buf2(); 
				}
				
				else if(strstr(Uart2_Buf,"gd2k")!=NULL)  //
				{				
					GPIO_SetBits(GPIOB,GPIO_Pin_9);GPIO_ResetBits(GPIOC,GPIO_Pin_0);
						CLR_Buf2(); 
				}
			
				else if(strstr(Uart2_Buf,"gd3k")!=NULL)  //检测是否收到led1
				{				
					GPIO_SetBits(GPIOB,GPIO_Pin_9); GPIO_SetBits(GPIOC,GPIO_Pin_0);
						CLR_Buf2(); 
				}
				
				else  	
				{				
					
				}
			
			}			
}
