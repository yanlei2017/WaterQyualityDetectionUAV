/**
  ******************************************************************************
  * @file    $Project/USER/sim800c_init.c  
  * @author  
  * @version V1.0
  * @date    2017-11-29
  * @brief   sim800c��ʼ��
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */

#include "sim800c_init.h"



#include "string.h"
#include "stdio.h"
/* Includes ------------------------------------------------------------------*/
//gsm ģ��SIM800c ��־λ
extern  bool Sim800_Receive_Read_Flag; //ATָ��ͺ󣬼�ʱ�����־
extern bool Sim800_Receive_overtime_Flag;//ATָ��Ϳ�ʼ��־
extern  bool Sim800_Receive_Read_Flag_1; //ATָ��ͺ󣬼�ʱ�����־
extern bool Sim800_Receive_overtime_Flag_1;//ATָ��Ϳ�ʼ��־
bool Sim800_send_flag=false;//һ�η��ͱ�־
uint8_t  Sim800_send_count=0;//�ظ�5�η��ͼ���
bool  Sim800_Ack_Flag=false;//�������ж������Ƿ���ȷ������һ����־
Sim800_Send_State_TypeDef Gsm_Send_Step = GSM_SEND_AT ;//GSM ģ��ö��
extern bool Sim800_server_closed; //GSM�յ� CLOSED +PDP�ȷ������Ͽ�������־
bool  sim800_sleep_wake_up=false;//GSM ���ѱ�־
extern uint8_t Recv_State ; //���ڽ��ս���
bool  sim800_tcp_success=false; //TCP ���ӳɹ�
int gsm_server_status=0;//TCP״̬����0 1 2 3

char gsm_start_code[]={"AT\r\n"}; 								   //���ģ�鴮�ڹ���2
char gsm_start_code_ack[]={"OK"};
char gsm_state_code[]={"AT+CPIN?\r\n"}; 					   //���SIM��״̬
char gsm_state_code_ack[]={"READY"};
char gsm_strength_code[]={"AT+CSQ\r\n"}; 					   //��ѯ�ź�ǿ��
char gsm_strength_code_ack[]={"OK"};
char gsm_register_code[]={"AT+CREG?\r\n"}; 				   //���GSM����ע��״̬
char gsm_register_code_ack[]={"0,1\r\n\r\nOK"};
char gsm_gprs_register_code[]={"AT+CGREG?\r\n"}; 		 //���GPRS����ע��״̬
char gsm_gprs_register_code_ack[]={"0,1\r\n\r\nOK"};
char gsm_cgatt_code[]={"AT+CGATT=1\r\n"}; 				   //ʹģ�鸽��GPRS����						
char gsm_cgatt_code_ack[]={"OK"};
char gsm_cipmode_code[]={"AT+CIPMODE=1\r\n"}; 			 //����GPRS��TCP/IP͸��ģʽ				
char gsm_cipmode_code_ack[]={"OK"}; 
char gsm_cstt_code[]={"AT+CSTT=CMNET\r\n"}; 		     //����APN
char gsm_cstt_code_ack[]={"OK"};
char gsm_ciicr_code[]={"AT+CIICR\r\n"}; 					   //�����ƶ�����������������·		
char gsm_ciicr_code_ack[]={"AT+CIICR\r\r\nOK"};
char gsm_cifsr_code[]={"AT+CIFSR\r\n"}; 					   //��ȡģ��IP��ַ
char gsm_cifsr_code_ack[]={"."};
char gsm_cipstart_code[]={"AT+CIPSTART=TCP,120.236.164.53,9567\r\n"}; 		//����Ҫ���ӵ�Զ�̷��������ͣ�ip��ַ�Ͷ˿�(AT+CIPSTART="TCP","59.173.139.198","65005")
char gsm_cipstart_code_ack[]={"\r\nCONNECT\r\n"};


char gsm_server_status_code[]={"AT+CIPSTATUS\r\n"};						//���������Ƿ�����״̬
char gsm_server_status_ack[]={"INITIAL"}; 										//δ���й�sim800c����
char gsm_server_status_ack2[]={"\r\nCONNECT\r\n"};									  //tcp�ѽ����ɹ�
char gsm_server_status_ack3[]={"CLOSED"};											//tcp�ѹر�����

char gsm_change_cmd_mode[]={"+++"};														//�����л���ָ��
char gsm_change_cmd_mode_ack[]={"OK"};
char gsm_change_data_mode[]={"ATO\r\n"};											//ָ���л�������
char gsm_change_data_mode_ack[]={"\r\nCONNECT\r\n"};

char gsm_error_ack[]={"ERROR"};

char gsm_buad_set_cmd[]={"AT+IPR=38400\r\n"};                 //���ò�����
char gsm_buad_set_cmd_ack[]={"OK"};
char gsm_cmd_write_cmd[]={"AT&W\r\n"};                        //��������

char gsm_tcp_close[]={"AT+CIPCLOSE\r\n"};
char gsm_tcp_close_ack[]={"CLOSE OK"};
char gsm_server_close[]={"AT+CIPSHUT\r\n"};
char gsm_server_close_ack[]={"SHUT OK"};
//����״̬����
char gsm_server_return_CLOSED[]={"CLOSED"};
char gsm_server_return_DEACT[]={"+PDP: DEACT"};

//�͹���
char low_power_dtr[]={"AT+CSCLK=1"};              //��������ģʽ
char low_power_self[]={"AT+CSCLK=2"};             //��������ģʽ

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

  

static bool Gsm_Send_Cmd_Ack_2s(Uart_Recv_TypeDef* Recv_Struct, char* cmd_buf, char* ack_buf)  //һ��ָ�� 2s
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

static bool Gsm_Send_Cmd_Ack_5s(Uart_Recv_TypeDef* Recv_Struct, char* cmd_buf, char* ack_buf)  //һ��ָ�� 5s
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
