/**
  ******************************************************************************
  * @file    $Project/USER/sim800c_init.c  
  * @author  
  * @version V1.0
  * @date    2017-11-29
  * @brief   sim800c初始化
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */

#include "sim800c_init.h"



#include "string.h"
#include "stdio.h"
/* Includes ------------------------------------------------------------------*/
//gsm 模块SIM800c 标志位
extern  bool Sim800_Receive_Read_Flag; //AT指令发送后，计时到达标志
extern bool Sim800_Receive_overtime_Flag;//AT指令发送开始标志
extern  bool Sim800_Receive_Read_Flag_1; //AT指令发送后，计时到达标志
extern bool Sim800_Receive_overtime_Flag_1;//AT指令发送开始标志
bool Sim800_send_flag=false;//一次发送标志
uint8_t  Sim800_send_count=0;//重复5次发送计数
bool  Sim800_Ack_Flag=false;//反馈后，判断内容是否正确进入下一步标志
Sim800_Send_State_TypeDef Gsm_Send_Step = GSM_SEND_AT ;//GSM 模块枚举
extern bool Sim800_server_closed; //GSM收到 CLOSED +PDP等服务器断开反馈标志
bool  sim800_sleep_wake_up=false;//GSM 唤醒标志
extern uint8_t Recv_State ; //串口接收进程
bool  sim800_tcp_success=false; //TCP 连接成功
int gsm_server_status=0;//TCP状态返回0 1 2 3

char gsm_start_code[]={"AT\r\n"}; 								   //检查模块串口工作2
char gsm_start_code_ack[]={"OK"};
char gsm_state_code[]={"AT+CPIN?\r\n"}; 					   //检查SIM卡状态
char gsm_state_code_ack[]={"READY"};
char gsm_strength_code[]={"AT+CSQ\r\n"}; 					   //查询信号强度
char gsm_strength_code_ack[]={"OK"};
char gsm_register_code[]={"AT+CREG?\r\n"}; 				   //检查GSM网络注册状态
char gsm_register_code_ack[]={"0,1\r\n\r\nOK"};
char gsm_gprs_register_code[]={"AT+CGREG?\r\n"}; 		 //检查GPRS网络注册状态
char gsm_gprs_register_code_ack[]={"0,1\r\n\r\nOK"};
char gsm_cgatt_code[]={"AT+CGATT=1\r\n"}; 				   //使模块附着GPRS网络						
char gsm_cgatt_code_ack[]={"OK"};
char gsm_cipmode_code[]={"AT+CIPMODE=1\r\n"}; 			 //设置GPRS的TCP/IP透传模式				
char gsm_cipmode_code_ack[]={"OK"}; 
char gsm_cstt_code[]={"AT+CSTT=CMNET\r\n"}; 		     //设置APN
char gsm_cstt_code_ack[]={"OK"};
char gsm_ciicr_code[]={"AT+CIICR\r\n"}; 					   //激活移动场景，建立无线链路		
char gsm_ciicr_code_ack[]={"AT+CIICR\r\r\nOK"};
char gsm_cifsr_code[]={"AT+CIFSR\r\n"}; 					   //获取模块IP地址
char gsm_cifsr_code_ack[]={"."};
char gsm_cipstart_code[]={"AT+CIPSTART=TCP,120.236.164.53,9567\r\n"}; 		//设置要连接的远程服务器类型，ip地址和端口(AT+CIPSTART="TCP","59.173.139.198","65005")
char gsm_cipstart_code_ack[]={"\r\nCONNECT\r\n"};


char gsm_server_status_code[]={"AT+CIPSTATUS\r\n"};						//检查服务器是否连接状态
char gsm_server_status_ack[]={"INITIAL"}; 										//未进行过sim800c配置
char gsm_server_status_ack2[]={"\r\nCONNECT\r\n"};									  //tcp已建立成功
char gsm_server_status_ack3[]={"CLOSED"};											//tcp已关闭连接

char gsm_change_cmd_mode[]={"+++"};														//数据切换到指令
char gsm_change_cmd_mode_ack[]={"OK"};
char gsm_change_data_mode[]={"ATO\r\n"};											//指令切换到数据
char gsm_change_data_mode_ack[]={"\r\nCONNECT\r\n"};

char gsm_error_ack[]={"ERROR"};

char gsm_buad_set_cmd[]={"AT+IPR=38400\r\n"};                 //设置波特率
char gsm_buad_set_cmd_ack[]={"OK"};
char gsm_cmd_write_cmd[]={"AT&W\r\n"};                        //保存设置

char gsm_tcp_close[]={"AT+CIPCLOSE\r\n"};
char gsm_tcp_close_ack[]={"CLOSE OK"};
char gsm_server_close[]={"AT+CIPSHUT\r\n"};
char gsm_server_close_ack[]={"SHUT OK"};
//网络状态返回
char gsm_server_return_CLOSED[]={"CLOSED"};
char gsm_server_return_DEACT[]={"+PDP: DEACT"};

//低功耗
char low_power_dtr[]={"AT+CSCLK=1"};              //设置休眠模式
char low_power_self[]={"AT+CSCLK=2"};             //设置休眠模式

char sleep_mode[]={"sleep"}; 
char closed_mode[]={"reveive closed"}; 

extern Uart_Recv_TypeDef Uart_Recv_Struct; 
extern UART_HandleTypeDef huart1;

/* Start_wifi_msg function */
void Sim800_Init()
{	  
  /* Infinite loop */
			switch(Gsm_Send_Step)
			{
						case	GSM_SEND_AT:
										if(Gsm_Send_Cmd_Ack_2s(&Uart_Recv_Struct, gsm_start_code, gsm_start_code_ack)!=true)
										{
											if(Sim800_Ack_Flag==true)
											{
												Sim800_Ack_Flag=false;
											  Gsm_Send_Step=GSM_SEND_AT;											
											}				
										}
										else
										{
											if(Sim800_Ack_Flag==true)
											{
												Sim800_Ack_Flag=false;										  
												Gsm_Send_Step=GSM_SEND_AT_CPIN;												
											}
										}  			
										break;		

						case	GSM_SEND_AT_CPIN: 
							      if(Gsm_Send_Cmd_Ack_2s(&Uart_Recv_Struct, gsm_state_code, gsm_state_code_ack)!=true)
										{
											if(Sim800_Ack_Flag==true)
											{
												Sim800_Ack_Flag=false;
											  Gsm_Send_Step=GSM_SEND_AT_CPIN;										
											}			
										}
										else
										{
											if(Sim800_Ack_Flag==true)
											{
												Sim800_Ack_Flag=false;
											  Gsm_Send_Step=GSM_SEND_AT_CIPSTATUS;										
											}
										}  			
										break;
																
						case	GSM_SEND_AT_CIPSTATUS:		
                    if(Gsm_Send_Cmd_Ack_5s(&Uart_Recv_Struct, gsm_server_status_code, gsm_server_status_ack)!=true)
										{
											if(Sim800_Ack_Flag==true)
											{
												Sim800_Ack_Flag=false;
											  Gsm_Send_Step=GSM_SEND_AT_CIPSTATUS;										
											}			
										}
										else
										{
											if(Sim800_Ack_Flag==true)
											{
												Sim800_Ack_Flag=false;
											  Gsm_Send_Step=GSM_SEND_AT_CSQ;										
											}
										}			       
										break;
										
						case	GSM_SEND_AT_CSQ:
							      if(Gsm_Send_Cmd_Ack_2s(&Uart_Recv_Struct, gsm_strength_code, gsm_strength_code_ack)!=true)
										{
											if(Sim800_Ack_Flag==true)
											{
												Sim800_Ack_Flag=false;
											  Gsm_Send_Step=GSM_SEND_AT_CSQ;									
											}			
										}
										else
										{
											if(Sim800_Ack_Flag==true)
											{
												Sim800_Ack_Flag=false;
											  Gsm_Send_Step=GSM_SEND_AT_CREG;								
											}
										}  			
										break;								                                              

						case	GSM_SEND_AT_CREG:
										if(Gsm_Send_Cmd_Ack_2s(&Uart_Recv_Struct, gsm_register_code, gsm_register_code_ack)!=true)
										{
											if(Sim800_Ack_Flag==true)
											{
												Sim800_Ack_Flag=false;
											  Gsm_Send_Step=GSM_SEND_AT_CREG;									
											}			
										}
										else
										{
											if(Sim800_Ack_Flag==true)
											{
												Sim800_Ack_Flag=false;
											  Gsm_Send_Step=GSM_SEND_AT_CGREG;									
											}
										}  			
										break;					                                              

						case	GSM_SEND_AT_CGREG:
										if(Gsm_Send_Cmd_Ack_2s(&Uart_Recv_Struct, gsm_gprs_register_code, gsm_gprs_register_code_ack)!=true)
										{
											if(Sim800_Ack_Flag==true)
											{
												Sim800_Ack_Flag=false;
											  Gsm_Send_Step=GSM_SEND_AT_CGREG;									
											}			
										}
										else
										{
											if(Sim800_Ack_Flag==true)
											{
												Sim800_Ack_Flag=false;
											  Gsm_Send_Step=GSM_SEND_AT_CGATT;									
											}
										}  			
										break;				
					
						case	GSM_SEND_AT_CGATT:
										if(Gsm_Send_Cmd_Ack_2s(&Uart_Recv_Struct, gsm_cgatt_code, gsm_cgatt_code_ack)!=true)
										{
											if(Sim800_Ack_Flag==true)
											{
												Sim800_Ack_Flag=false;
											  Gsm_Send_Step=GSM_SEND_AT_CGATT;									
											}		
										}
										else
										{
											if(Sim800_Ack_Flag==true)
											{
												Sim800_Ack_Flag=false;
											  Gsm_Send_Step=GSM_SEND_AT_CIPMODE;								
											}
										}  			
										break;
									
						case	GSM_SEND_AT_CIPMODE:
										if(Gsm_Send_Cmd_Ack_2s(&Uart_Recv_Struct, gsm_cipmode_code, gsm_cipmode_code_ack)!=true)
										{
											if(Sim800_Ack_Flag==true)
											{
												Sim800_Ack_Flag=false;
											  Gsm_Send_Step=GSM_SEND_AT_CIPMODE;								
											}		
										}
										else
										{
											if(Sim800_Ack_Flag==true)
											{
												Sim800_Ack_Flag=false;
											  Gsm_Send_Step=GSM_SEND_AT_CSTT;								
											}
										}  			
										break;					                                              

						case	GSM_SEND_AT_CSTT:
										if(Gsm_Send_Cmd_Ack_2s(&Uart_Recv_Struct, gsm_cstt_code, gsm_cstt_code_ack)!=true)
										{
											if(Sim800_Ack_Flag==true)
											{
												Sim800_Ack_Flag=false;
											  Gsm_Send_Step=GSM_SEND_AT_CSTT;									
											}			
										}
										else
										{
											if(Sim800_Ack_Flag==true)
											{
												Sim800_Ack_Flag=false;
											  Gsm_Send_Step=GSM_SEND_AT_CIICR;								
											}
										}  			
										break;								                                              

						case	GSM_SEND_AT_CIICR: 
						       	if(Gsm_Send_Cmd_Ack_5s(&Uart_Recv_Struct, gsm_ciicr_code, gsm_ciicr_code_ack)!=true)
										{
											if(Sim800_Ack_Flag==true)
											{
												Sim800_Ack_Flag=false;												
											  Gsm_Send_Step=GSM_SEND_AT_CIICR;									
											}	
										}
										else
										{
											if(Sim800_Ack_Flag==true)
											{
												Sim800_Ack_Flag=false;
											  Gsm_Send_Step=GSM_SEND_AT_CIFSR;										
											}
										} 					
										break;

						case	GSM_SEND_AT_CIFSR:
										if(Gsm_Send_Cmd_Ack_2s(&Uart_Recv_Struct, gsm_cifsr_code, gsm_cifsr_code_ack)!=true)
										{
											if(Sim800_Ack_Flag==true)
											{
												Sim800_Ack_Flag=false;
											  Gsm_Send_Step=GSM_SEND_AT_CIFSR;									
											}			
										}
										else
										{
											if(Sim800_Ack_Flag==true)
											{
												Sim800_Ack_Flag=false;
											  Gsm_Send_Step=GSM_SEND_AT_CIPSTART;									
											}
										}  			
										break;
						case	GSM_SEND_AT_CIPSTART:
										if(Gsm_Send_Cmd_Ack_5s(&Uart_Recv_Struct, gsm_cipstart_code, gsm_cipstart_code_ack)!=true)
										{
											if(Sim800_Ack_Flag==true)
											{
												Sim800_Ack_Flag=false;
												sim800_tcp_success=true;
											  Gsm_Send_Step=GSM_SEND_AT;									
											}			
										}
										else
										{
											if(Sim800_Ack_Flag==true)
											{
												Sim800_Ack_Flag=false;
											  Gsm_Send_Step=GSM_SEND_AT_CIPSTART;									
											}
										} 	
										break;
				
							default :
									break;	
					}

	
  }

  

static bool Gsm_Send_Cmd_Ack_2s(Uart_Recv_TypeDef* Recv_Struct, char* cmd_buf, char* ack_buf)  //一般指令 2s
{	
	bool ack_result = false;
	if(Sim800_send_flag == false)
	{		
		HAL_UART_Transmit(&huart1, (uint8_t*)cmd_buf, strlen(cmd_buf), 0xFFFF);
		Sim800_send_flag =true;
		Sim800_Receive_overtime_Flag=true;
		Sim800_send_count++;
		HAL_GPIO_TogglePin(GPIOC, gsm_test_Pin);
	}
	if(Sim800_Receive_Read_Flag== true)
	{
		Sim800_Receive_Read_Flag= false;
		Sim800_send_flag=false;

    Recv_Struct->Rx_Count=0;
    Recv_State=0;		
		if(strstr((char*)Uart_Recv_Struct.Uart_Data,ack_buf)!=NULL)
		{
			Sim800_send_count=0;
			ack_result = true; 
		}	
    memset(&Uart_Recv_Struct,0,sizeof(Uart_Recv_TypeDef));
		Sim800_Ack_Flag=true;
    return ack_result;		
	}
}

static bool Gsm_Send_Cmd_Ack_5s(Uart_Recv_TypeDef* Recv_Struct, char* cmd_buf, char* ack_buf)  //一般指令 5s
{	
	bool ack_result = false;
	if(Sim800_send_flag == false)
	{	
		HAL_UART_Transmit(&huart1, (uint8_t*)cmd_buf, strlen(cmd_buf), 0xFFFF);		
		Sim800_send_flag =true;
		Sim800_Receive_overtime_Flag_1=true;
		Sim800_send_count++;
		HAL_GPIO_TogglePin(GPIOC, gsm_test_Pin);
	}
	if(Sim800_Receive_Read_Flag_1== true)
	{
		Sim800_Receive_Read_Flag_1= false;
		Sim800_send_flag=false;
    Recv_Struct->Rx_Count=0;
    Recv_State=0;		
		if(strstr((char*)Uart_Recv_Struct.Uart_Data,ack_buf)!=NULL)
		{
			Sim800_send_count=0;
			ack_result = true; 
		}	
    memset(&Uart_Recv_Struct,0,sizeof(Uart_Recv_TypeDef));
		Sim800_Ack_Flag=true;
    return ack_result;		
	}
}




bool Sim800_Send_Delay(void)
{
	static uint8_t state_step=0;
	bool Delay_Flag = false;
	return Delay_Flag;
}




/*************************END OF FILE**************************/
