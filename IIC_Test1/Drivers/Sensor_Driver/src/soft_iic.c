#include "stm32f1xx_hal.h"
#include "soft_iic.h"


#define I2C_SDA 				GPIO_PIN_7
#define I2C_SCL 				GPIO_PIN_6	
#define I2C_PORT   			GPIOB

//#define	I2C_SDA_PORT		GPIOA

//
#define I2C_SCL_0 				HAL_GPIO_WritePin(I2C_PORT, I2C_SCL,GPIO_PIN_RESET)
#define I2C_SCL_1 				HAL_GPIO_WritePin(I2C_PORT, I2C_SCL,GPIO_PIN_SET)
#define I2C_SDA_0 				HAL_GPIO_WritePin(I2C_PORT, I2C_SDA,GPIO_PIN_RESET)
#define I2C_SDA_1   			HAL_GPIO_WritePin(I2C_PORT, I2C_SDA,GPIO_PIN_SET)
//
#define I2C_SDA_STATE   	HAL_GPIO_ReadPin(I2C_PORT, I2C_SDA)
#define I2C_DELAY 				I2C_Delay(100)
#define I2C_NOP						I2C_Delay(1) 		     //���Ĵ�NOP����ʱʱ�䣬���Ը���I2C���ݶ�ȡ�Ŀ���������Ҫ���Կɿ���

//
#define I2C_READY					0x00
#define I2C_BUS_BUSY			0x01	
#define I2C_BUS_ERROR			0x02
//
#define I2C_NACK	  			0x00 
#define I2C_ACK						0x01


void I2C_Delay(unsigned int dly) ;
/*************************************************************************************
����ԭ�ͣ�void I2C_Configuration(void)
�������ܣ�����I2C��IO�ںͳ�ʼ��I2C
�����������
�����������
�汾��A1.0
**************************************************************************************/
void I2C_Configuration(void)			
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.Pin = I2C_SCL|I2C_SDA;
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_OD;
	GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
  HAL_GPIO_Init(I2C_PORT, &GPIO_InitStructure); 
	
//	GPIO_InitStructure.Pin = I2C_SDA;
//	HAL_GPIO_Init(I2C_SDA_PORT, &GPIO_InitStructure); 
	
		
	I2C_SCL_1; 												    //��ʼ��I2C��ƽ
	I2C_SDA_1;
	I2C_DELAY;
}

/*************************************************************************************
����ԭ�ͣ�void I2C_Delay(unsigned int dly)
�������ܣ���ʱ����
���������unsigned int dly����ʾ��ʱ���ٸ�ʱ������
�����������
�汾��A1.0
**************************************************************************************/
void I2C_Delay(unsigned int dly)               
{


	while(--dly);	//dly=100: 8.75us; dly=100: 85.58 us (SYSCLK=72MHz)
}

/*************************************************************************************
����ԭ�ͣ�unsigned char I2C_START(void)
�������ܣ�ģ��I2C����ʱ��
�����������
�����������
�汾��A1.0
**************************************************************************************/
unsigned char I2C_START(void)
{ 
	I2C_SDA_1; 
 	I2C_NOP;
  // 
 	I2C_SCL_1; 
 	I2C_NOP;    
	//
 	if(!I2C_SDA_STATE) return I2C_BUS_BUSY;
	//
 	I2C_SDA_0;
 	I2C_NOP;
  //
 	I2C_SCL_0;  
 	I2C_NOP; 
	//
 	if(I2C_SDA_STATE) return I2C_BUS_ERROR;
	//
 	return I2C_READY;
}

/*************************************************************************************
����ԭ�ͣ�void I2C_STOP(void)
�������ܣ�ģ��I2C����ʱ��
�����������
�����������
�汾��A1.0
**************************************************************************************/
void I2C_STOP(void)
{
 	I2C_SDA_0; 
 	I2C_NOP;
  // 
 	I2C_SCL_1; 
 	I2C_NOP;    
	//
 	I2C_SDA_1;
 	I2C_NOP;
}

/*************************************************************************************
����ԭ�ͣ�void I2C_SendACK(void)
�������ܣ�ģ��I2C����Ӧ��(��������)
�����������
�����������
�汾��A1.0
**************************************************************************************/
void I2C_SendACK(void)
{
 	I2C_SDA_0;
 	I2C_NOP;
 	I2C_SCL_1;
 	I2C_NOP;
 	I2C_SCL_0; 
 	I2C_NOP;  
}

/*************************************************************************************
����ԭ�ͣ�void I2C_SendNACK(void)
�������ܣ�ģ��I2C������Ӧ��(��������)
�����������
�����������
�汾��A1.0
**************************************************************************************/
void I2C_SendNACK(void)
{
	I2C_SDA_1;
	I2C_NOP;
	I2C_SCL_1;
	I2C_NOP;
	I2C_SCL_0; 
	I2C_NOP;  
}


/*************************************************************************************
����ԭ�ͣ�unsigned char I2C_SendByte(uint8_t i2c_data)
�������ܣ�ģ��I2C����1byte����
���������i2c_data��Ҫ���͵�����
���������unsigned char���ӻ�����Ӧ��
�汾��A1.0
**************************************************************************************/
unsigned char I2C_SendByte(uint8_t i2c_data)
{
 	unsigned char i;
 	//
	I2C_SCL_0;
 	for(i=0;i<8;i++)
 	{  
  		if(i2c_data&0x80) I2C_SDA_1;
   		else I2C_SDA_0;
			//
  		i2c_data<<=1;
  		I2C_NOP;
			//
  		I2C_SCL_1;
  		I2C_NOP;
  		I2C_SCL_0;
  		I2C_NOP; 
 	}
	//
 	I2C_SDA_1; 
 	I2C_NOP;
 	I2C_SCL_1;
 	I2C_NOP;   
 	if(I2C_SDA_STATE)
 	{
  		I2C_SCL_0;
  		return I2C_NACK;
 	}
 	else
 	{
  		I2C_SCL_0;
  		return I2C_ACK;  
 	}    
}


/*************************************************************************************
����ԭ�ͣ�unsigned char I2C_ReceiveByte_NoACK(void)
�������ܣ�ģ��I2C����1byte���ݣ����Ҳ�����Ӧ���ź�
�����������
���������unsigned char�����յ�������
�汾��A1.0
**************************************************************************************/
unsigned char I2C_ReceiveByte_NoACK(void)
{
	unsigned char i,i2c_data;
	//
 	I2C_SDA_1;
 	I2C_SCL_0; 
 	i2c_data=0;
	//
 	for(i=0;i<8;i++)
 	{
  		I2C_SCL_1;
  		I2C_NOP; 
  		i2c_data<<=1;
			//
  		if(I2C_SDA_STATE)	i2c_data|=0x01; 
  
  		I2C_SCL_0;  
  		I2C_NOP;         
 	}
	I2C_SendNACK();
 	return i2c_data;
}


/*************************************************************************************
����ԭ�ͣ�unsigned char I2C_ReceiveByte_WithACK(void)
�������ܣ�ģ��I2C����1byte���ݣ����ҷ���Ӧ���ź�
�����������
���������unsigned char�����յ�������
�汾��A1.0
**************************************************************************************/
unsigned char I2C_ReceiveByte_WithACK(void)
{
	unsigned char i,i2c_data;
	//
 	I2C_SDA_1;
 	I2C_SCL_0; 
 	i2c_data=0;
	//
 	for(i=0;i<8;i++)
 	{
  		I2C_SCL_1;
  		I2C_NOP; 
  		i2c_data<<=1;
			
  		if(I2C_SDA_STATE)	i2c_data|=0x01; 
  
  		I2C_SCL_0;  
  		I2C_NOP;         
 	}
	I2C_SendACK();
 	return i2c_data;
}


//-------------------------------------------------Ӧ�ò����------------------------------------------------//
/*************************************************************************************
����ԭ�ͣ�void I2C_Write_Single_Reg(uint8_t device_addr,uint8_t reg_addr,uint8_t reg_value)
�������ܣ������豸�ĵ����Ĵ�����д������
���������uint8_t device_addr�����豸��ַ
		  uint8_t reg_addr��   ���豸�мĴ�����ַ
		  uint8_t reg_value    Ҫд��Ĵ���������		   
�����������
�汾��A1.0
**************************************************************************************/
void I2C_Write_Single_Reg(uint8_t device_addr,uint8_t reg_addr,uint8_t reg_value)
{
	I2C_START();
	I2C_SendByte(device_addr);				//Բ�㲩ʿ:����������д��ַ
	I2C_SendByte(reg_addr);  					//Բ�㲩ʿ:����������PWM��ַ
	I2C_SendByte(reg_value); 					//Բ�㲩ʿ:����������PWMֵ
	I2C_STOP();	
}


/*************************************************************************************
����ԭ�ͣ�void I2C_Write_Multi_Reg(uint8_t device_addr,uint8_t reg_addr,uint8_t *value_buf_addr, uint8_t reg_number)
�������ܣ������豸����������Ĵ�����д�����ݣ����豸���Զ������Ĵ�����ַ
���������uint8_t device_addr��		���豸��ַ
		  uint8_t reg_addr��   		���豸�мĴ�����ַ
		  uint8_t *value_buf_addr	Ҫд��Ĵ�����������ɵ�����
		  uint8_t reg_number    		Ҫд��ļĴ����ĸ���		   
�����������
�汾��A1.0
**************************************************************************************/
void I2C_Write_Multi_Reg(uint8_t device_addr,uint8_t reg_addr,uint8_t *value_buf_addr, uint8_t reg_number)
{
	uint8_t i = 0;
	I2C_START();
	I2C_SendByte(device_addr);				//Բ�㲩ʿ:����������д��ַ
	I2C_SendByte(reg_addr);  //Բ�㲩ʿ:����������PWM��ַ
	for(i=0;i<reg_number;i++)
	{
		I2C_SendByte( (*(value_buf_addr++)) ); //Բ�㲩ʿ:����������PWMֵ
	}	
	I2C_STOP();	
}


/*************************************************************************************
����ԭ�ͣ�uint8_t I2C_Read_Single_Reg(uint8_t device_addr,uint8_t reg_addr) 
�������ܣ���ȡ���豸�ĵ����Ĵ���ֵ
���������uint8_t device_addr��		���豸��ַ
		  uint8_t reg_addr��   		���豸�мĴ�����ַ		   
���������uint8_t�����豸�Ĵ���ֵ
�汾��A1.0
**************************************************************************************/
uint8_t I2C_Read_Single_Reg(uint8_t device_addr,uint8_t reg_addr)   
{
	uint8_t reg_value;
	
	I2C_START();
	I2C_SendByte(device_addr);					//Բ�㲩ʿ:����������д��ַ
	I2C_SendByte(reg_addr);    				//Բ�㲩ʿ:���������ǼĴ�����ַ
	I2C_START();
	I2C_SendByte(device_addr+1);      			//Բ�㲩ʿ:���������Ƕ���ַ
	reg_value=I2C_ReceiveByte_NoACK();			//Բ�㲩ʿ:���������ǼĴ���ֵ
	I2C_STOP();
	//
	return reg_value;
}


/*************************************************************************************
����ԭ�ͣ�void I2C_Read_Multi_Reg(uint8_t device_addr,uint8_t reg_addr, uint8_t *value_buf_addr, uint8_t reg_number) 
�������ܣ���ȡ���豸����������Ĵ���ֵ
���������uint8_t device_addr��		���豸��ַ
					uint8_t reg_addr��   		���豸�мĴ�����ʼ��ַ
					uint8_t *value_buf_addr    �����洢�Ĵ���ֵ�������׵�ַ
					uint8_t reg_number         Ҫ��ȡ�ļĴ����ĸ���		   
���������uint8_t�����豸�Ĵ���ֵ
�汾��A1.0
**************************************************************************************/

uint16_t	uw_i2c_bus_err_cnt = 0;  		//test
void I2C_Read_Multi_Reg(uint8_t device_addr,uint8_t reg_addr, uint8_t *value_buf_addr, uint8_t reg_number)   
{
	uint8_t i = 0;
	uint8_t		uc_device_ack_data = 0xFF,	uc_reg_ack_data = 0xFF;    //test
	
	
//	do
//	{
		I2C_START();
		uc_device_ack_data = I2C_SendByte(device_addr);					//Բ�㲩ʿ:����������д��ַ
		uc_reg_ack_data = I2C_SendByte(reg_addr);    						//Բ�㲩ʿ:���������ǼĴ�����ַ
//	}while( (!uc_device_ack_data)||(!uc_reg_ack_data) );    //test
	
//	I2C_START();
//	do
//	{
		I2C_START();
		uc_reg_ack_data = I2C_SendByte(device_addr+1);      			//Բ�㲩ʿ:���������Ƕ���ַ
//	}while(!uc_reg_ack_data);
	
	
	for(i=0;i<reg_number-1;i++)
	{
		value_buf_addr[i] = I2C_ReceiveByte_WithACK();	
	}
	value_buf_addr[reg_number-1] = I2C_ReceiveByte_NoACK();
	I2C_STOP();
}


