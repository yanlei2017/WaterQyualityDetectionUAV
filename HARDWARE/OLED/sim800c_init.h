
/**
  ******************************************************************************
  * @file    $Project/USER/sim800c_init.h
  * @author  
  * @version V1.0
  * @date    2013-01-12
  * @brief   
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */ 

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SIM800C_INIT_H
#define __SIM800C_INIT_H

#ifdef __cplusplus
 extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
#include "sys.h"
/* Exported types ------------------------------------------------------------*/
typedef enum 
{ 
	 GSM_SEND_AT,
	 GSM_SEND_AT_CPIN,
	 GSM_SEND_AT_CIPSTATUS,
	 GSM_SEND_AT_CSQ,
	 GSM_SEND_AT_CREG,
	 GSM_SEND_AT_CGREG,
	 GSM_SEND_AT_CGATT,
	 GSM_SEND_AT_CIPMODE,
	 GSM_SEND_AT_CSTT,
	 GSM_SEND_AT_CIICR,
	 GSM_SEND_AT_CIFSR,
	 GSM_SEND_AT_CIPSTART,	 
	 GSM_SEND_AT_CIPCLOSE
} Sim800_Send_State_TypeDef;
/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
//bool sim800_Recv_Buf_Check(uint8_t* pData, uint16_t length);
void Sim800_Init();
//void sim800_Recv_Buf_Check(uint8_t* pData, uint16_t length);
//bool my_strstr(char *str, char *sub_str) ;
//void sim800_low_power_control();
static bool Gsm_Send_Cmd_Ack_2s(Uart_Recv_TypeDef* Recv_Struct,char* cmd_buf, char* ack_buf);
static bool Gsm_Send_Cmd_Ack_5s(Uart_Recv_TypeDef* Recv_Struct, char* cmd_buf, char* ack_buf);

bool Sim800_Send_Delay(void);
#ifdef __cplusplus
}
#endif

#endif /* __SYSTEM_INITIAL_H */
