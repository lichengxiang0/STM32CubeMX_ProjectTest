/**
  ******************************************************************************
  * File Name          : stm32_bsp_uart.c
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
	
#include "stm32f1xx_hal.h"
#include "stm32_bsp_uart.h"


/**/
#if RxBufSize1 != 0
	uint8_t RxBuf1[RxBufSize1];
	User_Timer_Typedef    st_usart1_rx_com_timer = USER_TIMER_INIT_VALUE;    //��ʱ�����ṹ��
	USART_Buf_TypeDef 		RxBuf_Struct1={&st_usart1_rx_com_timer,RxBufSize1,0,0,0,0,RxBuf1,&huart1};
#endif

#if TxBufSize1 != 0
	uint8_t TxBuf1[TxBufSize1];
	User_Timer_Typedef    st_usart1_tx_com_timer = USER_TIMER_INIT_VALUE;    //��ʱ�����ṹ��
	USART_Buf_TypeDef 		TxBuf_Struct1={&st_usart1_tx_com_timer,TxBufSize1,0,0,0,0,TxBuf1,&huart1};
#endif

#if RxBufSize2 != 0
	uint8_t RxBuf2[RxBufSize2];
	User_Timer_Typedef    st_usart2_rx_com_timer = USER_TIMER_INIT_VALUE;    //��ʱ�����ṹ��
	USART_Buf_TypeDef 		RxBuf_Struct2={&st_usart2_rx_com_timer,RxBufSize2,0,0,0,0,RxBuf2,&huart2};
#endif

#if TxBufSize2 != 0
	uint8_t TxBuf2[TxBufSize2];
	User_Timer_Typedef    st_usart2_tx_com_timer = USER_TIMER_INIT_VALUE;    //��ʱ�����ṹ��
	USART_Buf_TypeDef 		TxBuf_Struct2={&st_usart2_tx_com_timer,TxBufSize2,0,0,0,0,TxBuf2,&huart2};
#endif


/*******************************************************************************
* ������  :  	  uart_GetChar
* ��������:       �ӽ��ջ�������һ�ֽڣ���������ֽڣ���ȷ���������������ݡ�
* �������:       - UART_BUF * ptRx: ���ջ������ṹ����׵�ַ
*
* �������:       ��
* �� �� ֵ:       �ӽ��ջ�������ȡ�Ǹ��ֽڵ�����
* ����˵���� 
* �޸�����    	  �汾��     �޸���	     �޸�����
* -----------------------------------------------
* 2010/06/11      V1.0	     00574zsm	       XXXX
*******************************************************************************/

uint8_t UART_GetChar(USART_Buf_TypeDef * ptRx)
{	
	uint8_t ucData;
	ucData = *((*ptRx).pcBufAddr + (*ptRx).ucBufRdInde);
	if ((*ptRx).ucBufCnt != 0)
	{	
		if (++(*ptRx).ucBufRdInde == (*ptRx).ucBufSize)
		{
		   (*ptRx).ucBufRdInde = 0;
		}
		__HAL_UART_DISABLE_IT(ptRx->ptUartHandel,UART_IT_RXNE);//�ؽ����ж�

		--(*ptRx).ucBufCnt;
		__HAL_UART_ENABLE_IT(ptRx->ptUartHandel,UART_IT_RXNE);//�������ж�
	}
	
	return ucData;
}
/*******************************************************************************
* ������  : 			 uart_ReadChar
* ��������:        �ӽ��ջ�������ȡָ��λ�õ�һ�ֽڣ�����������ֽڣ���ȷ���������������ݡ�
* �������:        - UART_BUF * stRx�� ���ջ������ṹ��ĵ�ַ
*									 - ucNum��ָ����������Ҫ��ȡ�ֽڵ�λ��
* �������:        ��
* �� �� ֵ: 			 �ӽ��ջ�������ȡ�Ǹ��ֽڵ�����
* ����˵���� 
* �޸�����    	  �汾��     �޸���	     �޸�����
* -----------------------------------------------
* 2010/06/11      V1.0	     00574zsm	       XXXX
*******************************************************************************/
uint8_t UART_ReadChar(USART_Buf_TypeDef * ptRx, uint8_t ucNum)	 
{	
	uint8_t ucData;
	uint16_t  i;
	i = ucNum;
	if ((*ptRx).ucBufCnt >= ucNum)
	 {
		i += (*ptRx).ucBufRdInde;
		if (i >= (*ptRx).ucBufSize)
		{	
			i -=((*ptRx).ucBufSize);
		}
	 }
	else
	 {
	  i=0;
	 }
	ucData = *((*ptRx).pcBufAddr + i);
	return ucData;
}

/*******************************************************************************
* ������  :			   uart_DelChar
* ��������:        �ӽ��ջ��������ָ�����ȵ����ݣ�
* �������:        - UART_BUF * ptRx: ���ջ������ṹ��ĵ�ַ
*									 - ucNum��ָ����������Ҫɾ���ֽڵĳ���									
* �������:        ��
* �� �� ֵ:        ��
* ����˵���� 
* �޸�����    	  �汾��     �޸���	     �޸�����
* -----------------------------------------------
* 2010/06/11      V1.0	     00574zsm	       XXXX
*******************************************************************************/
void UART_DelChar(USART_Buf_TypeDef * ptRx, uint8_t ucNum)	 
{	
	uint16_t i;
	if ((*ptRx).ucBufCnt >= ucNum)	
	{	
		__HAL_UART_DISABLE_IT(ptRx->ptUartHandel,UART_IT_RXNE);//�ؽ����ж�
		(*ptRx).ucBufCnt -= ucNum;	
		__HAL_UART_ENABLE_IT(ptRx->ptUartHandel,UART_IT_RXNE);//�������ж�
		i = ucNum;
		i += (*ptRx).ucBufRdInde;
		if (i >= (*ptRx).ucBufSize)
		{
		  i -= (*ptRx).ucBufSize;
		}
		(*ptRx).ucBufRdInde = i;
	}
	else
	{  
		__HAL_UART_DISABLE_IT(ptRx->ptUartHandel,UART_IT_RXNE);//�ؽ����ж�
   	    i = (*ptRx).ucBufCnt;
        (*ptRx).ucBufCnt = 0;
		__HAL_UART_ENABLE_IT(ptRx->ptUartHandel,UART_IT_RXNE);//�������ж�
        i += (*ptRx).ucBufRdInde;
		if (i >= (*ptRx).ucBufSize)
		{
		  i -= (*ptRx).ucBufSize;
		}
		(*ptRx).ucBufRdInde = i;

	 }
}




/*******************************************************************************
* ������  :			   uart_PutChar
* ��������:        ���ͻ�����д��һ���ֽ�����
* �������:        - UART_BUF * ptTx: ���ͻ������ṹ����׵�ַ
*									 - cData��д�뷢�ͻ������е�����									
* �������:        ��
* �� �� ֵ:        ��
* ����˵���� 
* �޸�����    	  �汾��     �޸���	     �޸�����
* -----------------------------------------------
* 2010/06/11      V1.0	     00574zsm	       XXXX
*******************************************************************************/

void UART_PutChar(USART_Buf_TypeDef * ptTx, uint8_t ucData)
{	
	while ((*ptTx).ucBufCnt == (*ptTx).ucBufSize);			 //���ͻ��������ȴ������д�����������󻺳���
	__HAL_UART_DISABLE_IT(ptTx->ptUartHandel,UART_IT_TXE);//�ؽ����ж�
	if((*ptTx).ucBufCnt == 0)
	{
		if(__HAL_UART_GET_FLAG(ptTx->ptUartHandel, UART_FLAG_TXE)== SET) //check if transmit data register full or not
		{
			ptTx->ptUartHandel->Instance->DR = ucData;
			__HAL_UART_ENABLE_IT(ptTx->ptUartHandel,UART_IT_TXE);//�������ж�
			return;
		}
	}	
	*((*ptTx).pcBufAddr + (*ptTx).ucBufWrInde) = ucData;
	if(++(*ptTx).ucBufWrInde == (*ptTx).ucBufSize)
	{
	  (*ptTx).ucBufWrInde = 0;
	}
	++(*ptTx).ucBufCnt;
	__HAL_UART_ENABLE_IT(ptTx->ptUartHandel,UART_IT_TXE);//�������ж�
}




/*******************************************************************************
* ������  :        USARTx_Tx_ISR
* ��������:        �˺���Ϊ���ڷ�������жϵķ�����
* ����    :        - UART_BUF * ptTx: ���ͻ������ṹ����׵�ַ
* ���    :        ��
* �� �� ֵ:        ��
* ����˵���� 
* �޸�����    	  �汾��     �޸���	     �޸�����
* -----------------------------------------------
* 2010/06/11      V1.0	     00574zsm	       XXXX
*******************************************************************************/
void UART_Tx_ISR(USART_Buf_TypeDef * ptTx)
{
	 if((__HAL_UART_GET_FLAG(ptTx->ptUartHandel, UART_FLAG_TXE) != RESET) && (__HAL_UART_GET_IT_SOURCE(ptTx->ptUartHandel, UART_IT_TXE) != RESET))
   {    
		 __HAL_UART_CLEAR_FLAG(ptTx->ptUartHandel,UART_FLAG_TXE);
	  	if((* ptTx).ucBufCnt)
	  	{
		 		--(* ptTx).ucBufCnt;
				ptTx->ptUartHandel->Instance->DR = *(ptTx->pcBufAddr + ptTx->ucBufRdInde);		
		 		if(++(* ptTx).ucBufRdInde >= (* ptTx).ucBufSize)
				{
		   		  (* ptTx).ucBufRdInde = 0;
				}
	  	}
			else
			{
				/* Disable the UART Transmit Data Register Empty Interrupt */
				__HAL_UART_DISABLE_IT(ptTx->ptUartHandel, UART_IT_TXE);
			}
   } 
}

/*******************************************************************************
* ������  :        USARTx_Rx_ISR
* ��������:        �˺���Ϊ���ڽ����жϵķ�����
* �������:        - UART_BUF * ptRx: ���ջ������ṹ��ĵ�ַ
* �������:				 ��
* �� �� ֵ:        ��
* ����˵���� 
* �޸�����    	   �汾��     �޸���	     �޸�����
* -----------------------------------------------

*******************************************************************************/

void UART_Rx_ISR(USART_Buf_TypeDef * ptRx)
{
 	uint8_t ucData;
	
//	 __HAL_UART_CLEAR_OREFLAG(ptRx->ptUartHandel);	
	if((__HAL_UART_GET_FLAG(ptRx->ptUartHandel, UART_FLAG_RXNE) != RESET) && (__HAL_UART_GET_IT_SOURCE(ptRx->ptUartHandel, UART_IT_RXNE) != RESET))
  {
			ucData = (uint8_t)(ptRx->ptUartHandel->Instance->DR&0x01FF);
			
	  	if(((ptRx->ptUartHandel->Instance->SR)& 0x0000) == 0)//�޴���
	  	{
				/*buf���������������*/
				if(++(* ptRx).ucBufCnt > (* ptRx).ucBufSize)  
				{
//		   			(* ptRx).ucBufCnt = (* ptRx).ucBufSize;   //test
						(* ptRx).ucBufCnt = 0;  //test
		   			(* ptRx).ucBufOvf = 1;
				}
				else
				{
					*((* ptRx).pcBufAddr + (* ptRx).ucBufWrInde) = ucData;         
					if(++(* ptRx).ucBufWrInde >= (* ptRx).ucBufSize)
					{
							(* ptRx).ucBufWrInde = 0; 
					}	
				}
	   }
  }
} 


void	UART_TxRx_ISR(UART_HandleTypeDef	*huart)
{
	uint32_t tmp_flag = 0, tmp_it_source = 0;

  tmp_flag = __HAL_UART_GET_FLAG(huart, UART_FLAG_PE);
  tmp_it_source = __HAL_UART_GET_IT_SOURCE(huart, UART_IT_PE);  
  /* UART parity error interrupt occurred ------------------------------------*/
  if((tmp_flag != RESET) && (tmp_it_source != RESET))
  { 
    __HAL_UART_CLEAR_PEFLAG(huart);
    
    huart->ErrorCode |= HAL_UART_ERROR_PE;
  }
  
	
  tmp_flag = __HAL_UART_GET_FLAG(huart, UART_FLAG_FE);
  /* UART frame error interrupt occurred -------------------------------------*/
  if((tmp_flag != RESET))
  { 
    __HAL_UART_CLEAR_FEFLAG(huart);
    
    huart->ErrorCode |= HAL_UART_ERROR_FE;
  }
  
  tmp_flag = __HAL_UART_GET_FLAG(huart, UART_FLAG_NE);
  /* UART noise error interrupt occurred -------------------------------------*/
  if((tmp_flag != RESET))
  { 
    __HAL_UART_CLEAR_NEFLAG(huart);
    
    huart->ErrorCode |= HAL_UART_ERROR_NE;
  }
  
  tmp_flag = __HAL_UART_GET_FLAG(huart, UART_FLAG_ORE);
  /* UART Over-Run interrupt occurred ----------------------------------------*/
  if((tmp_flag != RESET))
  { 
    __HAL_UART_CLEAR_OREFLAG(huart);
    
    huart->ErrorCode |= HAL_UART_ERROR_ORE;
  }
  


  if(huart->ErrorCode != HAL_UART_ERROR_NONE)
  {
    /* Set the UART state ready to be able to start again the process */
    huart->State = HAL_UART_STATE_READY;
    
    HAL_UART_ErrorCallback(huart);
  }  
}
/*--------------------------------����------------------------------------------*/