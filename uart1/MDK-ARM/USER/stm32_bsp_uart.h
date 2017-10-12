/**
  ******************************************************************************
  * File Name          : stm32_bsp_uart.h
  * Description        : ���ڻ�������غ����������������Ķ���Ͷ�ȡ
	******************************************************************************
  *
  * COPYRIGHT(c) 2016 aibird
  *
  ******************************************************************************
	*
	* Author:
	*
	******************************************************************************
	*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef	_STM32_BSP_UART_H
#define	_STM32_BSP_UART_H

#ifdef __cplusplus
 extern "C" {
#endif
	 
	 
/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"
//#include "stm32_bsp_timer.h"
#include "stm32_bsp_peripheral_init_export.h"



typedef struct								    //��������������������غ����Ĳ���
{	
		User_Timer_Typedef *st_timer;
		uint8_t  	ucBufSize;							//���������飺��������С
		volatile	uint8_t  ucBufWrInde;						//���������飺дָ��
		volatile	uint8_t  ucBufRdInde;						//���������飺��ָ��
		volatile	uint8_t  ucBufCnt;							//���������飺���ݼ���
		volatile	uint8_t  ucBufOvf;							//���������飺�����־��ע������
		uint8_t  * pcBufAddr;						//���������飺�׵�ַָ�����
		UART_HandleTypeDef	*ptUartHandel;
} USART_Buf_TypeDef;


typedef	struct {
	   uint16_t *pSendData;
		uint16_t DataLength;			 // ���ݳ���
}TransData;


#define RxBufSize1			0 	// USART1���ջ���������С200����ʹ����Ϊ0
#define TxBufSize1			0 	// USART1���ͻ���������С250����ʹ����Ϊ0

#define RxBufSize2			255 	// USART2���ջ���������С200����ʹ����Ϊ0
#define TxBufSize2			255 	// USART2���ͻ���������С250����ʹ����Ϊ0



#if RxBufSize1 != 0
	extern uint8_t RxBuf1[RxBufSize1];
	extern USART_Buf_TypeDef RxBuf_Struct1;
#endif

#if TxBufSize1 != 0
	extern uint8_t TxBuf1[TxBufSize1];
	extern USART_Buf_TypeDef TxBuf_Struct1;
#endif

#if RxBufSize2 != 0
	extern uint8_t RxBuf2[RxBufSize2];
	extern USART_Buf_TypeDef RxBuf_Struct2;
#endif

#if TxBufSize2 != 0
	extern uint8_t TxBuf2[TxBufSize2];
	extern USART_Buf_TypeDef TxBuf_Struct2;
#endif



//��������
uint8_t UART_GetChar(USART_Buf_TypeDef * ptRx);
uint8_t UART_ReadChar(USART_Buf_TypeDef * ptRx, uint8_t ucNum);
void UART_DelChar(USART_Buf_TypeDef * ptRx, uint8_t ucNum);
void UART_PutChar(USART_Buf_TypeDef * ptTx, uint8_t ucData);
void UART_PutChar_DMA(USART_Buf_TypeDef * ptTx, uint8_t *ucData ,uint16_t	uw_data_len);


void UART_Tx_ISR(USART_Buf_TypeDef * ptTx);
void UART_Rx_ISR(USART_Buf_TypeDef * ptRx);
void	UART_TxRx_ISR(UART_HandleTypeDef	*huart);


#ifdef __cplusplus
}
#endif


#endif
/*--------------------------------����------------------------------------------*/