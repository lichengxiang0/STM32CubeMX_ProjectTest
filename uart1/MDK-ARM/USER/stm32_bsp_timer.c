/**
  ******************************************************************************
  * File Name          : stm32_bsp_tiemr.c
  * Description        : TIMER��غ�����1->���벶�����������Ͳ������Ͳ�����
													2->PWM_TIMER��������PWM�Ŀ����رգ�3->�û�������ĺ���ʵ��
													4->���PWM�ĺ궨��
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
#include "stm32_bsp_timer.h"


/* Private define --------------------------------------------------------*/
/*ϵͳ΢�뼶��ʱ�����������㺯��ִ�����ڣ���ȷ��us*/
#define	MICROSECOND_TIMER		(htim2)

#define	ADC_TRIG_TIMER			(htim3)



/* External functions --------------------------------------------------------*/
/**
  * @brief 	void	Statr_Microsecond_Timer(void)
	* @note	 	����΢�뼶��ʱ��
  */
void	Statr_Microsecond_Timer(void) 
{
	HAL_TIM_Base_Start(&MICROSECOND_TIMER);
}


/*---------------PWM�������غ���-----------------*/
/**
  * @brief 	void	Start_Pwm_Tiemr(void)
	* @note	 	����PWM��TIMER��AD�ɼ�(AD��PWM_TIMER��ͨ��4����)
  */
void	Start_Pwm_Tiemr(void)
{
	HAL_TIM_Base_Start(&PWM_TIMER);								//ʹ��PWMtimer��updata�ж�
	
	HAL_TIM_Base_Start(&ADC_TRIG_TIMER);					//����AD��ʱ�������ɼ�
	
	HAL_TIM_PWM_Start(&PWM_TIMER,TIM_CHANNEL_4);  //ʹ��PWM4�ıȽϲ����ж�
}


/**
  * @brief 	void	Enable_Pwm_Output(void)
	* @note	 	ʹ��PWM�����
  */
void	Enable_Pwm_Output(void)
{
	HAL_TIM_PWM_Start(&PWM_TIMER,TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&PWM_TIMER,TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&PWM_TIMER,TIM_CHANNEL_3);
	
	
	HAL_TIMEx_PWMN_Start(&PWM_TIMER,TIM_CHANNEL_2);
	HAL_TIMEx_PWMN_Start(&PWM_TIMER,TIM_CHANNEL_3);	  //��ʱ����MOE���Ѿ�ʹ����
	HAL_TIMEx_PWMN_Start(&PWM_TIMER,TIM_CHANNEL_1);
}


/**
  * @brief 	void	Enable_Pwm_Output(void)
	* @note	 	�ر�PWM�����������PWM_TIEMR��CHANNEL4(����AD)
  */
void	Disable_Pwm_Output(void)
{
	HAL_TIM_PWM_Stop(&PWM_TIMER,TIM_CHANNEL_1);
	HAL_TIM_PWM_Stop(&PWM_TIMER,TIM_CHANNEL_2);
	HAL_TIM_PWM_Stop(&PWM_TIMER,TIM_CHANNEL_3);
	
	HAL_TIMEx_PWMN_Stop(&PWM_TIMER,TIM_CHANNEL_2);
	HAL_TIMEx_PWMN_Stop(&PWM_TIMER,TIM_CHANNEL_3);	  //��ʱ����MOE���Ѿ�ʹ����
	HAL_TIMEx_PWMN_Stop(&PWM_TIMER,TIM_CHANNEL_1);   
}



/*---------------����ʱ�亯��-----------------*/
/**
  * @brief 	float Get_Function_Period(uint32_t	*ul_last_sys_micros)
	* @note	 	���㺯��ִ������(us)
  */
float Get_Function_Period(uint32_t	*ul_last_sys_micros)
{
	uint32_t	ul_now_sys_micros;
	float			period;
	
	ul_now_sys_micros = __HAL_TIM_GET_COUNTER(&MICROSECOND_TIMER);
	
	if( ul_now_sys_micros < (*ul_last_sys_micros) )
	{ 
		period =  ((float)(ul_now_sys_micros + (0xffff - (*ul_last_sys_micros) ) ) / 1000000.0f);
	}
	else	
	{
		period =  ((float)(ul_now_sys_micros - (*ul_last_sys_micros) ) / 1000000.0f);
	}
	*ul_last_sys_micros = ul_now_sys_micros;	
	
	return(period);
}
/**
  * @brief 	void	Update_User_Timer_Cnt(User_Timer_Typedef *st_user_timer_p)
	* @note	 	�����û�TIMER(���)��CNT
  */
void	Update_User_Timer_Cnt(User_Timer_Typedef *st_user_timer_p)
{
	st_user_timer_p->ul_timer_once_cnt = 0;
	
	if(USER_TIMER_RUN == st_user_timer_p->en_timer_state)
	{
		st_user_timer_p->ul_timer_now_cnt = HAL_GetTick();
		if(st_user_timer_p->ul_timer_now_cnt == st_user_timer_p->ul_timer_start_cnt)
		{
			st_user_timer_p->ul_timer_once_cnt = 0;
		}
		else if(st_user_timer_p->ul_timer_now_cnt > st_user_timer_p->ul_timer_start_cnt)
		{
			st_user_timer_p->ul_timer_once_cnt = st_user_timer_p->ul_timer_now_cnt - st_user_timer_p->ul_timer_start_cnt;
		}
		else 
		{
			st_user_timer_p->ul_timer_once_cnt = st_user_timer_p->ul_timer_now_cnt + 0xFFFFFFFF - st_user_timer_p->ul_timer_start_cnt;
		}
		
		st_user_timer_p->ul_timer_cnt = st_user_timer_p->ul_timer_pause_cnt + st_user_timer_p->ul_timer_once_cnt;
	}
	else if(USER_TIMER_PAUSE == st_user_timer_p->en_timer_state)
	{
		st_user_timer_p->ul_timer_cnt = st_user_timer_p->ul_timer_pause_cnt;
	}
	else
	{
		st_user_timer_p->ul_timer_cnt = 0;
	}
}


/**
  * @brief 	void	Update_User_Timer_Cnt(User_Timer_Typedef *st_user_timer_p)
	* @note	 	�����û�TIMER(���)
  */
void	Start_User_Timer(User_Timer_Typedef *st_user_timer_p)
{
	if(USER_TIMER_STOP == st_user_timer_p->en_timer_state)
	{
		st_user_timer_p->en_timer_state = USER_TIMER_RUN;
		
		st_user_timer_p->ul_timer_start_cnt = HAL_GetTick();
		st_user_timer_p->ul_timer_now_cnt = HAL_GetTick();
		st_user_timer_p->ul_timer_pause_cnt = 0;
		Update_User_Timer_Cnt(st_user_timer_p);
	}
}


/**
  * @brief 	void	Stop_User_Timer(User_Timer_Typedef *st_user_timer_p)
	* @note	 	ֹͣ�û�TIMER(���)
  */
void	Stop_User_Timer(User_Timer_Typedef *st_user_timer_p)
{
	if(USER_TIMER_STOP != st_user_timer_p->en_timer_state)
	{
		st_user_timer_p->en_timer_state = USER_TIMER_STOP;
		st_user_timer_p->ul_timer_start_cnt = HAL_GetTick();
		st_user_timer_p->ul_timer_now_cnt = HAL_GetTick();
		st_user_timer_p->ul_timer_pause_cnt = 0;
		Update_User_Timer_Cnt(st_user_timer_p);
	}
}

/**
  * @brief 	void	Reset_User_Timer(User_Timer_Typedef *st_user_timer_p)
	* @note	 	��λ�û�TIMER(���)
  */
void	Reset_User_Timer(User_Timer_Typedef *st_user_timer_p)
{
	/*ֻ����timerʹ��ʱ���и�λ*/
	if(USER_TIMER_STOP != st_user_timer_p->en_timer_state)
	{
		st_user_timer_p->en_timer_state = USER_TIMER_RUN;
		st_user_timer_p->ul_timer_start_cnt = HAL_GetTick();
		st_user_timer_p->ul_timer_now_cnt = HAL_GetTick();
		st_user_timer_p->ul_timer_pause_cnt = 0;
		Update_User_Timer_Cnt(st_user_timer_p);
	}
}



/**
  * @brief 	void	Pause_User_Timer(User_Timer_Typedef *st_user_timer_p)
	* @note	 	��ͣ�û�TIMER(���)
  */
void	Pause_User_Timer(User_Timer_Typedef *st_user_timer_p)
{
	if(USER_TIMER_RUN == st_user_timer_p->en_timer_state)
	{
		st_user_timer_p->en_timer_state = USER_TIMER_PAUSE;
		st_user_timer_p->ul_timer_pause_cnt += st_user_timer_p->ul_timer_once_cnt;
		Update_User_Timer_Cnt(st_user_timer_p);
	}
}


/**
  * @brief 	void	Resume_User_Timer(User_Timer_Typedef *st_user_timer_p)
	* @note	 	�ָ��û�TIMER(���)
  */
void	Resume_User_Timer(User_Timer_Typedef *st_user_timer_p)
{
	if(USER_TIMER_PAUSE == st_user_timer_p->en_timer_state)
	{
		st_user_timer_p->en_timer_state = USER_TIMER_RUN;
		st_user_timer_p->ul_timer_start_cnt = HAL_GetTick();
		st_user_timer_p->ul_timer_now_cnt = HAL_GetTick();
		Update_User_Timer_Cnt(st_user_timer_p);
	}
}
