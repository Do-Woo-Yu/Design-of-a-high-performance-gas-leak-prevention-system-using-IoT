/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32f4xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f4xx_it.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "lcd_user.h"
#include "extern.h"
#include "main_image.h"
#include "main_image2.h"
#include "main_image3.h"

//LCD및 타이머 변수
unsigned int T1000ms_cnt = 0, T10ms_cnt = 0,T3ms_cnt = 0,T3ms_cnt_2=0,T500ms_cnt=0,T100ms_cnt = 0,T200ms_cnt = 0,Key_Value =0, BF_key = 0, BFBF_key = 0,i = 0,i_2 = 0;
unsigned int Key_Value2 =0, BF_key2 = 0, BFBF_key2 = 0;
unsigned int clock_switch_cnt = 0,clock_switch_Flag=0;
unsigned int T30ms_1_cnt = 0, T30ms_cnt = 0,Time_cnt = 0,T3s_cnt=0;
extern unsigned int switchi_bun,switchi_si;
extern unsigned int cho_0, cho_1, min_0, min_1, hour_0, hour_1;
extern unsigned char cnt,cnt_10;
extern unsigned char Clock_Sec_0[],Clock_Sec_1[], Clock_Min_0[], Clock_Min_1[], Clock_Hour_0[], Clock_Hour_1[], Clock_Sec_1[], Clock_Sec_0[];
extern unsigned int min_cnt, hour_cnt;
extern byte RTC_EN;

// CO 센서 변수
extern unsigned int CO_final_adc;
extern unsigned short CO_gas,CO_adc1;
extern float CO_adc2;
float CO_AD_slop = 0.0, CO_AD_offset = 0.0, CO_AD_actual = 0.0;
unsigned int CO_gas_100= 0,CO_gas_10= 0,CO_gas_1= 0,T3min_cnt = 0,warming_time=0,CO_warming=0;
unsigned char CO_100[], CO_10[], CO_1[];

// CO2 센서 변수
unsigned int T1000ms_cnt_CO2 = 0,T1ms_cnt = 0, CO2 = 0;
unsigned int CO2_gas_1000 = 0,CO2_gas_100= 0,CO2_gas_10= 0,CO2_gas_1= 0;
float CO2_AD_slop = 0.0, CO2_AD_offset = 0.0, CO2_AD_actual = 0.0;
extern unsigned char CO2_1000[],CO2_100[], CO2_10[], CO2_1[];
extern unsigned int tr_count, adc_cnt,adc_cnt2,cnt_1000, number, number_2,adc_1000ms,adc_2000ms, num, final_adc;
extern unsigned short CO2_gas,adc1,adc2;
extern ADC_HandleTypeDef hadc1;

// WS2812 LED 변수
unsigned int LED_T1000ms_cnt=0,T5min = 0,LED_T10ms_cnt = 0, LED_Key_Value =0, LED_BF_key = 0, LED_BFBF_key = 0,Reset_White_LED_Flag = 0,Reset_Green_LED_Flag = 0,Reset_Blue_LED_Flag = 0,Reset_Purple_LED_Flag = 0,Reset_Yellow_LED_Flag = 0, Reset_Sky_Blue_LED_Flag = 0;
unsigned int LED_Mode = 0,Mode_Change = 0,Red_Flag_Mode = 0, Green_Flag_Mode = 0, Blue_Flag_Mode = 0,Mode_T3ms_cnt = 0,Mode_Time_cnt = 0,Mode_Change_cnt= 0,Static_Mode_Change_cnt=0;
extern unsigned int Change, Change_Count,Red_color,Red_color_cnt,safe_discolor;

// Gas_Valve_Lock 변수
unsigned int T750ms_cnt = 0;
extern unsigned int unLock_Flag,Lock;
extern float Lock_Flag;

// SD카드 변수
extern unsigned int gas_enable,gas_alarm,Stop_Alarm;
unsigned int Stop_Flag = 0,Stop_BF_key = 0,Stop_BFBF_key = 0, Stop_Key_Value = 0;

// bluetooth 모듈 변수
extern unsigned int bluetooth_tx_enable,bluetooth_Flag,bluetooth_stop,bluetooth_icon,CO_TX,CO_scan_cnt,re_reset_cnt;

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
byte T2_200ms_flag = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern DMA_HandleTypeDef hdma_adc1;
extern DMA_HandleTypeDef hdma_spi1_tx;
extern SPI_HandleTypeDef hspi1;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
extern UART_HandleTypeDef huart3;
extern UART_HandleTypeDef huart6;
/* USER CODE BEGIN EV */

/* USER CODE END EV */

/******************************************************************************/
/*           Cortex-M4 Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */
  while (1)
  {
  }
  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */

  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler(void)
{
  /* USER CODE BEGIN MemoryManagement_IRQn 0 */

  /* USER CODE END MemoryManagement_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_MemoryManagement_IRQn 0 */
    /* USER CODE END W1_MemoryManagement_IRQn 0 */
  }
}

/**
  * @brief This function handles Pre-fetch fault, memory access fault.
  */
void BusFault_Handler(void)
{
  /* USER CODE BEGIN BusFault_IRQn 0 */

  /* USER CODE END BusFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_BusFault_IRQn 0 */
    /* USER CODE END W1_BusFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Undefined instruction or illegal state.
  */
void UsageFault_Handler(void)
{
  /* USER CODE BEGIN UsageFault_IRQn 0 */

  /* USER CODE END UsageFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_UsageFault_IRQn 0 */
    /* USER CODE END W1_UsageFault_IRQn 0 */
  }
}

/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler(void)
{
  /* USER CODE BEGIN SVCall_IRQn 0 */

  /* USER CODE END SVCall_IRQn 0 */
  /* USER CODE BEGIN SVCall_IRQn 1 */

  /* USER CODE END SVCall_IRQn 1 */
}

/**
  * @brief This function handles Debug monitor.
  */
void DebugMon_Handler(void)
{
  /* USER CODE BEGIN DebugMonitor_IRQn 0 */

  /* USER CODE END DebugMonitor_IRQn 0 */
  /* USER CODE BEGIN DebugMonitor_IRQn 1 */

  /* USER CODE END DebugMonitor_IRQn 1 */
}

/**
  * @brief This function handles Pendable request for system service.
  */
void PendSV_Handler(void)
{
  /* USER CODE BEGIN PendSV_IRQn 0 */

  /* USER CODE END PendSV_IRQn 0 */
  /* USER CODE BEGIN PendSV_IRQn 1 */

  /* USER CODE END PendSV_IRQn 1 */
}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */

  /* USER CODE END SysTick_IRQn 0 */
  HAL_IncTick();
  /* USER CODE BEGIN SysTick_IRQn 1 */

  /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32F4xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f4xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles TIM2 global interrupt.
  */
void TIM2_IRQHandler(void)
{
  /* USER CODE BEGIN TIM2_IRQn 0 */

  /* USER CODE END TIM2_IRQn 0 */
  HAL_TIM_IRQHandler(&htim2);
  /* USER CODE BEGIN TIM2_IRQn 1 */
  
	//1ms timer
  	
	static word t2_200ms_cnt = 0;
	
	if(CO_gas < 30)
	{
					
			if(++T200ms_cnt >= 20)
			{
			T200ms_cnt = 0;

			// LED_Mode가 0이면 일반모드, LED_Mode가 1이면 그라데이션 모드
			if(LED_Mode == 0)
			{
				if(Change_Count == 0)   // 꺼짐
				{
								Red_Flag = 0;
								Green_Flag = 0;
								Blue_Flag = 0;
				}
				else if(Change_Count == 1)  // White
				{
								Red_Flag = 33;
								Green_Flag = 33;
								Blue_Flag = 33;
				}
				else if(Change_Count == 2)  // Green
				{
								Red_Flag = 0;
								Green_Flag = 100;
								Blue_Flag = 0;
				}
				else if(Change_Count == 3)  // Blue
				{
								Red_Flag = 0;
								Green_Flag = 0;
								Blue_Flag = 100;
				}
				else if(Change_Count == 4)  // Purple
				{
								Red_Flag = 50;
								Green_Flag = 0;
								Blue_Flag = 50;
				}
				else if(Change_Count == 5)  // Yellow
				{
								Red_Flag = 50;
								Green_Flag = 50;
								Blue_Flag = 0;
				}
				else if(Change_Count == 6)  // Sky_Blue
				{
								Red_Flag = 0;
								Green_Flag = 50;
								Blue_Flag = 50;
				}
			}

			// 변수 하나 지정해서 모드 변경( 초록색 버튼을 3초간 누르면 모드 변경 )					 
			// 밝아졌다가 다시 어두워지는 모드 그라데이션 모드(2가지)

			else if(LED_Mode == 1)  // 그라데이션 모드
			{
			if(Change_Count == 0)    // White 그라이데이션( 0 ~ 33 증가하다가 0 )
			{						
					Red_Flag = 0;
					Green_Flag = 0;
					Blue_Flag = 0;
					
					Reset_White_LED_Flag = 1;
					Reset_Green_LED_Flag = 1;
					Reset_Blue_LED_Flag = 1;
					Reset_Purple_LED_Flag = 1;
					Reset_Yellow_LED_Flag = 1;
					Reset_Sky_Blue_LED_Flag = 1;
			}
			
			if(Change_Count == 1)    // White 그라이데이션( 0 ~ 150 증가하다가 0 )
			{
							
					if(Reset_White_LED_Flag)
					{
									Reset_White_LED_Flag = 0;
									Red_Flag_Mode = 0;
									Green_Flag_Mode = 0;
									Blue_Flag_Mode = 0;
									Red_Flag = 0;
									Green_Flag = 0;
									Blue_Flag = 0;
					}
					 if(Red_Flag_Mode == 0)
					 {      
									if(++Red_Flag >= 150)
									{
											Red_Flag_Mode = 1;
									} 
					 }
					 else if(Red_Flag_Mode == 1)
					 {
									if(--Red_Flag <= 0)
									{
											Red_Flag_Mode = 0;
									}							
					 }
					if(Green_Flag_Mode == 0)
					 {     
									if(++Green_Flag >= 150)
									{
											Green_Flag_Mode = 1;
									}
					 }
					 else if(Green_Flag_Mode == 1)
					 {

									if(--Green_Flag <= 0)
									{
											Green_Flag_Mode = 0;		
									}							
					 }
					if(Blue_Flag_Mode == 0)
					 {      
									if(++Blue_Flag >= 150)
									{
											Blue_Flag_Mode = 1;
									}
					 }
					 else if(Blue_Flag_Mode == 1)
					 {

									if(--Blue_Flag <= 0)
									{
											Blue_Flag_Mode = 0;		
									}
					 }
					Reset_Green_LED_Flag = 1;
					Reset_Blue_LED_Flag = 1;
					Reset_Purple_LED_Flag = 1;
					Reset_Yellow_LED_Flag = 1;
					Reset_Sky_Blue_LED_Flag = 1;
			}
			else if(Change_Count == 2)   // Green 그라이데이션( 0 ~ 150 증가하다가 0 )
			{      
					if(Reset_Green_LED_Flag)
					{
									Reset_Green_LED_Flag = 0;
									Red_Flag_Mode = 0;
									Green_Flag_Mode = 0;
									Blue_Flag_Mode = 0;
									Red_Flag = 0;
									Green_Flag = 0;
									Blue_Flag = 0;
					}
					if(Green_Flag_Mode == 0)
					{
								if(++Green_Flag >= 150)
								{
										Green_Flag_Mode = 1;
								}
					}
					else if(Green_Flag_Mode == 1)
					{

								if(--Green_Flag <= 0)
								{
										Green_Flag_Mode = 0;		
								}							
					}
					Reset_White_LED_Flag = 1;
					Reset_Blue_LED_Flag = 1;
					Reset_Purple_LED_Flag = 1;
					Reset_Yellow_LED_Flag = 1;
					Reset_Sky_Blue_LED_Flag = 1;
			}
			else if(Change_Count == 3)   // Blue 그라이데이션( 0 ~ 150 증가하다가 0 )
			{      
					if(Reset_Blue_LED_Flag)
					{
									Reset_Blue_LED_Flag = 0;
									Red_Flag_Mode = 0;
									Green_Flag_Mode = 0;
									Blue_Flag_Mode = 0;
									Red_Flag = 0;
									Green_Flag = 0;
									Blue_Flag = 0;
					}
					if(Blue_Flag_Mode == 0)
					{
								if(++Blue_Flag >= 150)
								{
										Blue_Flag_Mode = 1;
								}
					}
					else if(Blue_Flag_Mode == 1)
					{

								if(--Blue_Flag <= 0)
								{
										Blue_Flag_Mode = 0;		
								}							
					}
					Reset_White_LED_Flag = 1;
					Reset_Green_LED_Flag = 1;
					Reset_Purple_LED_Flag = 1;
					Reset_Yellow_LED_Flag = 1;
					Reset_Sky_Blue_LED_Flag = 1;
			}
			else if(Change_Count == 4)   // Purple 그라이데이션( 0 ~ 150 증가하다가 0 )
			{      
					if(Reset_Purple_LED_Flag)
					{
									Reset_Purple_LED_Flag = 0;
									Red_Flag_Mode = 0;
									Green_Flag_Mode = 0;
									Blue_Flag_Mode = 0;
									Red_Flag = 0;
									Green_Flag = 0;
									Blue_Flag = 0;
					}
					if(Red_Flag_Mode == 0)
					 {      
									if(++Red_Flag >= 150)
									{
											Red_Flag_Mode = 1;
									} 
					 }
					 else if(Red_Flag_Mode == 1)
					 {
									if(--Red_Flag <= 0)
									{
											Red_Flag_Mode = 0;
									}							
					 }
					if(Blue_Flag_Mode == 0)
					{
								if(++Blue_Flag >= 150)
								{
										Blue_Flag_Mode = 1;
								}
					}
					else if(Blue_Flag_Mode == 1)
					{

								if(--Blue_Flag <= 0)
								{
										Blue_Flag_Mode = 0;		
								}							
					}
					Reset_White_LED_Flag = 1;
					Reset_Green_LED_Flag = 1;
					Reset_Blue_LED_Flag = 1;
					Reset_Yellow_LED_Flag = 1;
					Reset_Sky_Blue_LED_Flag = 1;
			}
			else if(Change_Count == 5)   // Yellow 그라이데이션( 0 ~ 150 증가하다가 0 )
			{      
					if(Reset_Yellow_LED_Flag)
					{
									Reset_Yellow_LED_Flag = 0;
									Red_Flag_Mode = 0;
									Green_Flag_Mode = 0;
									Blue_Flag_Mode = 0;
									Red_Flag = 0;
									Green_Flag = 0;
									Blue_Flag = 0;
					}
					if(Red_Flag_Mode == 0)
					 {      
									if(++Red_Flag >= 150)
									{
											Red_Flag_Mode = 1;
									} 
					 }
					 else if(Red_Flag_Mode == 1)
					 {
									if(--Red_Flag <= 0)
									{
											Red_Flag_Mode = 0;
									}							
					 }
					if(Green_Flag_Mode == 0)
					{
								if(++Green_Flag >= 150)
								{
										Green_Flag_Mode = 1;
								}
					}
					else if(Green_Flag_Mode == 1)
					{

								if(--Green_Flag <= 0)
								{
										Green_Flag_Mode = 0;		
								}							
					}
					Reset_White_LED_Flag = 1;
					Reset_Green_LED_Flag = 1;
					Reset_Blue_LED_Flag = 1;
					Reset_Purple_LED_Flag = 1;
					Reset_Sky_Blue_LED_Flag = 1;
			}
			
			else if(Change_Count == 6)   // Sky Blue 그라이데이션( 0 ~ 150 증가하다가 0 )
			{      
					if(Reset_Sky_Blue_LED_Flag)
					{
									Reset_Sky_Blue_LED_Flag = 0;
									Red_Flag_Mode = 0;
									Green_Flag_Mode = 0;
									Blue_Flag_Mode = 0;
									Red_Flag = 0;
									Green_Flag = 0;
									Blue_Flag = 0;
					}
					if(Green_Flag_Mode == 0)
					{
								if(++Green_Flag >= 150)
								{
										Green_Flag_Mode = 1;
								}
					}
					else if(Green_Flag_Mode == 1)
					{

								if(--Green_Flag <= 0)
								{
										Green_Flag_Mode = 0;		
								}							
					}					
					if(Blue_Flag_Mode == 0)
					{
								if(++Blue_Flag >= 150)
								{
										Blue_Flag_Mode = 1;
								}
					}
					else if(Blue_Flag_Mode == 1)
					{

								if(--Blue_Flag <= 0)
								{
										Blue_Flag_Mode = 0;		
								}							
					}							
					Reset_White_LED_Flag = 1;
					Reset_Green_LED_Flag = 1;
					Reset_Blue_LED_Flag = 1;
					Reset_Purple_LED_Flag = 1;
					Reset_Yellow_LED_Flag = 1;
			}
		}
	  }
					
	  if(++t2_200ms_cnt >= 500)
		{
			t2_200ms_cnt = 0;
			T2_200ms_flag = 1;
			if(++i > 1)
			{
			  i = 0;
			}
			if(i == 0)
			{
				 LCD_word_big(195,5,black, white, human_word_100ptBitmaps_88x118, ":", 88, 119);
			}	
			if(i == 1)
			{
				LCD_word_big(195,5, black, white, human_word_100ptBitmaps_null_88x118, " ", 88, 119);
			}
		}
	}
	
	HAL_ADC_Start(&hadc1); // ADC 변환 시작

	CO_adc1 = HAL_ADC_GetValue(&hadc1);

	CO_final_adc += CO_adc1;
	
	if(++T1000ms_cnt >= 1000) // 1s timer
	{ 
	  	T1000ms_cnt = 0;
		
		CO_TX++;
		
		warming_time++;
		
		CO_warming++;
		
		CO_scan_cnt++;
		
		re_reset_cnt++;
		
	  if(Lock)
	  {
		  if(++Lock_Flag >= 300) // 가스 밸브 잠금 장치 변수 증가 2초 동안 동작 하고 5분 뒤에 다시 0으로 됨
		  {
		  	Lock_Flag = 0.0;
		  }
		  if(++gas_enable >= 300) // 가스 밸브 잠금 장치 변수 증가 2초 동안 동작 하고 5분 뒤에 다시 0으로 됨
		  {
		  	gas_enable = 0;
		  }
	  }

		if(bluetooth_icon == 1)
		{
			LCD_word(450,230, black, cyan, Consolas_24x44, "B", 24, 44);
			bluetooth_icon = 0;
		}
		else if(bluetooth_icon == 0)
		{
		  LCD_word(450,230, black, cyan, Consolas_24x44, " ", 24, 44);
		  bluetooth_icon = 2;
		}
		
		CO_adc2 = CO_final_adc / 1000.0;
		  
		//CO_AD_slop  = (3300.0 - 720.0) / (4095.0 - 893.4545455);
		
		//CO_AD_offset = 720.0 - (CO_AD_slop * 893.4545455);
		
		//CO_AD_actual = (CO_AD_slop * CO_adc2) + CO_AD_offset;
		
		//CO_gas = ((word)(CO_AD_actual) / 10) - 55; // CO 농도 값
		
		//CO_gas = 7.448 - 19.757*(CO_adc2 / 910) + 16.388*(CO_adc2 / 910)*(CO_adc2 / 910) - 60;
	  
	  	static float CO_gas_test_num = 0;
		
		CO_gas_test_num = CO_adc2/871;
	  
	    CO_gas = 7.448 - (19.757*(CO_adc2 / 871)) + (16.388*(CO_adc2 / 871)*(CO_adc2 / 871));
		
		if(CO_gas >= 60000)
		{
		  CO_gas = 0;
		}
		
		CO_final_adc = 0;
		
		if(warming_time == 181)
		{
		  Change_Count = 0;
		}
		
		if(warming_time <= 180)
		{
		  CO_gas = 0;
		}
		
		if(CO_gas < 30)
		{
		  T30ms_cnt = 0;
		  if(++T30ms_1_cnt == 1)
		  {
			clear_all(black);
			dis_picture(&main_image2[0], 0, 0, 480, 272); // 480 x 272 pixel LCD 배경화면 표시(밤하늘)
		  }
		  if(++T3s_cnt >= 1)
		  {
			  T3s_cnt = 0;
			  if(Change == 1)
			  {
				Time_cnt = 0;
				T3ms_cnt++;
				if(T3ms_cnt >= 3)
				{
				  T3ms_cnt = 0;
				  clock_switch_cnt = 1;
				  if(clock_switch_cnt == 1)
				  {
					 LCD_word(50,210, black, green, Consolas_24x44, "Time Change Mode", 24, 44);
				  }
				}
			  }
			  else if(Change == 0)
			  {
				T3ms_cnt = 0;
			  }
			  if(switchi_bun == 1)
			  {
				T3ms_cnt_2++;
				if(T3ms_cnt_2 >= 3)
				{
				  T3ms_cnt_2 = 0;
				  bluetooth_tx_enable = 1;
				}
			  }
			  else if(switchi_bun == 0)
			  {
				T3ms_cnt_2 = 0;
			  }
				
			  if(Mode_Change == 1)
			  {
				Mode_Time_cnt = 0;
				Mode_T3ms_cnt++;
				if(Mode_T3ms_cnt >= 3)
				{
				  Mode_T3ms_cnt = 0;
				  Mode_Change_cnt = 1;
				  LED_Mode++;
				  if(LED_Mode >= 2)
				  {
					  	LED_Mode = 0;
						Mode_Change_cnt = 2;
						Static_Mode_Change_cnt = 1;
				  }
				}
			  }

			  if(Mode_Change_cnt == 1)
			  {
				 Mode_Change_cnt = 0;
				 LCD_word(30,210, black, red, Consolas_24x44, "G", 24, 44);
				 LCD_word(60,210, black, orange, Consolas_24x44, "r", 24, 44);
				 LCD_word(90,210, black, yellow, Consolas_24x44, "a", 24, 44);
				 LCD_word(120,210, black, green, Consolas_24x44, "d", 24, 44);
				 LCD_word(150,210, black, cyan, Consolas_24x44, "a", 24, 44);
				 LCD_word(180,210, black, blue, Consolas_24x44, "t", 24, 44);
				 LCD_word(210,210, black, violet, Consolas_24x44, "i", 24, 44);
				 LCD_word(240,210, black, red, Consolas_24x44, "o", 24, 44);
				 LCD_word(270,210, black, orange, Consolas_24x44, "n", 24, 44);
				 LCD_word(300,210, black, yellow, Consolas_24x44, " ", 24, 44);
				 LCD_word(330,210, black, yellow, Consolas_24x44, "M", 24, 44);
				 LCD_word(360,210, black, green, Consolas_24x44, "o", 24, 44);
				 LCD_word(390,210, black, cyan, Consolas_24x44, "d", 24, 44);
				 LCD_word(420,210, black, blue, Consolas_24x44, "e", 24, 44);
			  }
			  else if(Mode_Change_cnt == 0)
			  {
				LCD_word(30, 210, black, green, Consolas_24x44, "                    ", 24, 44);
				Mode_Change_cnt = 2;
			  }
			  
			  if(Static_Mode_Change_cnt == 1)
			  {
				 Static_Mode_Change_cnt = 0;
				 LCD_word(80,210, black, gray, Consolas_24x44, "S", 24, 44);
				 LCD_word(110,210, black, gray, Consolas_24x44, "t", 24, 44);
				 LCD_word(140,210, black, gray, Consolas_24x44, "a", 24, 44);
				 LCD_word(170,210, black, gray, Consolas_24x44, "t", 24, 44);
				 LCD_word(200,210, black, gray, Consolas_24x44, "i", 24, 44);
				 LCD_word(230,210, black, gray, Consolas_24x44, "c", 24, 44);
				 LCD_word(260,210, black, gray, Consolas_24x44, " ", 24, 44);
				 LCD_word(290,210, black, gray, Consolas_24x44, "M", 24, 44);
				 LCD_word(320,210, black, gray, Consolas_24x44, "o", 24, 44);
				 LCD_word(350,210, black, gray, Consolas_24x44, "d", 24, 44);
				 LCD_word(380,210, black, gray, Consolas_24x44, "e", 24, 44);
			  }
			  else if(Static_Mode_Change_cnt == 0)
			  {
				LCD_word(30, 210, black, green, Consolas_24x44, "                    ", 24, 44);
				Static_Mode_Change_cnt = 2;
			  }
	  }
			
			 
			//시계 시의 십의 자리 숫자  
			sprintf(Clock_Hour_1, "%d", TIME_1[0]);
			LCD_word_big(18, 5, black,white, human_word_100ptBitmaps_88x118, Clock_Hour_1, 88, 119); 
			  
			//시계 시의 일의 자리 숫자  
			sprintf(Clock_Hour_0, "%d", TIME_1[1]);
			LCD_word_big(105, 5, black, white, human_word_100ptBitmaps_88x118, Clock_Hour_0, 88, 119);  
			  
			//시계 : 표시
			/*if(i == 0)
			{
				 LCD_word_big(195,5,black, white, human_word_100ptBitmaps_88x118, ":", 88, 119);
			}	
			if(i == 1)
			{
				LCD_word_big(195,5, black, white, human_word_100ptBitmaps_null_88x118, " ", 88, 119);
			}*/
			
			//시계 분의 십의 자리 숫자
			sprintf(Clock_Min_1, "%d", TIME_1[3]);
			LCD_word_big(285, 5, black, white, human_word_100ptBitmaps_88x118, Clock_Min_1, 88, 119); 
			  
			//시계 분의 일의 자리 숫자
			sprintf(Clock_Min_0, "%d", TIME_1[4]);
			LCD_word_big(375, 5, black, white, human_word_100ptBitmaps_88x118, Clock_Min_0, 88, 119); 
			
			
			//시계 초의 십의 자리 숫자
			//sprintf(Clock_Sec_1, "%d", TIME_2[0]);
			//LCD_word_big(18, 125, black, white, human_word_100ptBitmaps_88x118, Clock_Sec_1, 88, 119); 
			
			//시계 초의 일의 자리 숫자
			//sprintf(Clock_Sec_0, "%d", TIME_2[1]);
			//LCD_word_big(108, 125, black, white, human_word_100ptBitmaps_88x118, Clock_Sec_0, 88, 119); 
			
			//TIME_1[0] = (rtc_time.Hours) / 10 + 0; // 시 십의 자리
			//TIME_1[1] = (rtc_time.Hours) % 10 + 0;// 시 일의 자리
			//TIME_1[3] = (rtc_time.Minutes) / 10 + 0;// 분 십의 자리
			//TIME_1[4] = (rtc_time.Minutes) % 10 + 0;// 분 일의 자리
			//TIME_2[0] = (rtc_time.Seconds)/10 + 0; // 초 십의 자리
			//TIME_2[1] = (rtc_time.Seconds)%10 + 0; // 초 일의 자리
			
			if(++TIME_2[1] > 9)
			{
			  TIME_2[1] = 0;
			  if(++TIME_2[0] > 5)
			  {
				TIME_2[0] = 0;
				TIME_1[4]++;
			  }
			}
			
			if(TIME_1[4] > 9)
			{
			  TIME_1[4] = 0;
			  if(++TIME_1[3] > 5)
			  {
				TIME_1[3] = 0;
				TIME_1[1]++;
			  }
			}
			
			if(TIME_1[1] > 9)
			{
			  TIME_1[1] = 0;
			  if(++TIME_1[0] > 2)
			  {
				TIME_1[0] = 0;
			  }
			}
			
			if(TIME_1[0] == 2)
			{
				if(TIME_1[1] > 3)
				{
				  TIME_1[1] = 0;
				  TIME_1[0]= 0;
				}
			}
			
			CO_gas_100 = CO_gas / 100;
			
			CO_gas_10 = (CO_gas % 100)/10;
			
			CO_gas_1 = (CO_gas%100)%10;
			
			LCD_word_middle(70, 140, black, red, consolas_50pt_36x61, "AM", 40, 61); 
			
			LCD_word_middle(165, 137, black, red, consolas_50pt_36x61, ":", 40, 61); 
			
			if(CO_warming <= 180)
			{
			  LCD_word_middle(250, 145, black, yellow, Consolas_24x44, "Warm Up", 24, 44); 
			}
			else if(CO_warming == 181)
			{
			  LCD_word_middle(250, 145, black, yellow, Consolas_24x44, "       ", 24, 44); 
			}
			else
			{
				LCD_word_middle(360, 145, black, red, Consolas_24x44, "ppm", 24, 44); 
				
				if(CO_gas_100 >= 1)
				{
					sprintf(CO_100, "%d", CO_gas_100);
					LCD_word_middle(220, 140, black, yellow, consolas_50pt_36x61, CO_100, 40, 61); 
					if(CO_gas_10 >= 0)
					{
					  sprintf(CO_10, "%d", CO_gas_10);
				      LCD_word_middle(260, 140, black, yellow, consolas_50pt_36x61, CO_10, 40, 61); 
					}
				}
				else
				{
				   LCD_word_middle(220, 140, black, yellow, consolas_50pt_null_36x61, " ", 40, 61); 
				}
				if(CO_gas_10 >= 1)
				{
				   sprintf(CO_10, "%d", CO_gas_10);
				   LCD_word_middle(260, 140, black, yellow, consolas_50pt_36x61, CO_10, 40, 61); 
				}
				else
				{
				   LCD_word_middle(260, 140, black, yellow, consolas_50pt_null_36x61, " ", 40, 61); 
				}
				sprintf(CO_1, "%d", CO_gas_1);
				LCD_word_middle(300, 140, black, yellow, consolas_50pt_36x61, CO_1, 40, 61); 
			}
		}
		else
		{
			T30ms_1_cnt = 0;
			if(++T30ms_cnt == 1)
			{
				clear_all(black);
				dis_picture(&main_image[0], 0, 0, 480, 272); // 480 x 272 pixel LCD 배경화면 표시(가스누출방지)
			}
			
			LCD_word(50, 210, red, green, Consolas_24x44, "                ", 24, 44);
			
			LCD_word_middle(70, 160, red, yellow, consolas_50pt_36x61, "AM", 40, 61); 
			
			LCD_word_middle(165, 157, red, yellow, consolas_50pt_36x61, ":", 40, 61); 
			
			LCD_word_middle(360, 165, red, yellow, Consolas_24x44, "ppm", 24, 44); 
			
			CO_gas_100 = CO_gas / 100;
			
			CO_gas_10 = (CO_gas % 100)/10;
			
			CO_gas_1 = (CO_gas%100)%10;
			
			if(CO_gas_100 >= 1)
			{
				sprintf(CO_100, "%d", CO_gas_100);
				LCD_word_middle(220, 160, red, white, consolas_50pt_36x61, CO_100, 40, 61); 
			}
			else
			{
			   LCD_word_middle(220, 160, red, white, consolas_50pt_null_36x61, " ", 40, 61); 
			}
			if(CO_gas_10 >= 0)
			{
			   sprintf(CO_10, "%d", CO_gas_10);
			   LCD_word_middle(260, 160, red, white, consolas_50pt_36x61, CO_10, 40, 61); 
			}
			else
			{
			   LCD_word_middle(260, 160, red, white, consolas_50pt_null_36x61, " ", 40, 61); 
			}
			sprintf(CO_1, "%d", CO_gas_1);
			LCD_word_middle(300, 160, red, white, consolas_50pt_36x61, CO_1, 40, 61); 
		}
	}
	
  if(++LED_T10ms_cnt >= 10)
  {
		LED_T10ms_cnt = 0;
		
		Change = HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_3);
		
		Mode_Change = HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_13);
		
		if(Change == LED_BF_key)
		{
		  if(Change == 1)
		  {
			if(LED_BF_key != LED_BFBF_key)
			{
			  LED_Key_Value++;
			  Change_Count++;
			  Stop_Alarm = 1;
			  HAL_TIM_PWM_Stop(&htim3,TIM_CHANNEL_1);
			  clock_switch_cnt = 0;
			  if(Change_Count > 6)
			  {
				Change_Count = 0;
			  }
			}
		  }
		  LED_BFBF_key = LED_BF_key;
		}
		LED_BF_key = Change;
		
		Stop_Flag = HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_3);
		
		if(Stop_Flag == Stop_BF_key)
		{
		  if(Stop_Flag == 1)
		  {
			if(Stop_BF_key != Stop_BFBF_key)
			{
			  Stop_Key_Value++;
			  Stop_Alarm++;
			}
		  }
		  Stop_BFBF_key = Stop_BF_key;
		}
		Stop_BF_key = Stop_Flag;
		
		switchi_si = HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_13);
		
		switchi_bun = HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_12);
		
		if(clock_switch_cnt)
		{
			// 시계의 분 조절
			if(switchi_bun == BF_key)
			{
				if(switchi_bun == 1)
				{
					if(BF_key != BFBF_key)
					{   
						//키를 눌렀을때 원하는 동작 이부분에 넣기
						Key_Value++;
						min_cnt++;
					}
				}
				BFBF_key = BF_key;
			}
			BF_key = switchi_bun;
			
			// 시계의 시 조절
			if(switchi_si == BF_key2)
			{
				if(switchi_si == 1)
				{
					if(BF_key2 != BFBF_key2)
					{   
						//키를 눌렀을때 원하는 동작 이부분에 넣기
						Key_Value2++;	  
						hour_cnt++;
					}
				}
				BFBF_key2 = BF_key2;
			}
			BF_key2 = switchi_si;
 	 }
	 else
	 {
	   if(++Time_cnt == 1)
	   {
		 LCD_word(50, 210, black, green, Consolas_24x44, "                ", 24, 44);
	   }
	 }
  }

  /* USER CODE END TIM2_IRQn 1 */
}

/**
  * @brief This function handles TIM3 global interrupt.
  */
void TIM3_IRQHandler(void)
{
  /* USER CODE BEGIN TIM3_IRQn 0 */

  /* USER CODE END TIM3_IRQn 0 */
  HAL_TIM_IRQHandler(&htim3);
  /* USER CODE BEGIN TIM3_IRQn 1 */

  /* USER CODE END TIM3_IRQn 1 */
}

/**
  * @brief This function handles SPI1 global interrupt.
  */
void SPI1_IRQHandler(void)
{
  /* USER CODE BEGIN SPI1_IRQn 0 */

  /* USER CODE END SPI1_IRQn 0 */
  HAL_SPI_IRQHandler(&hspi1);
  /* USER CODE BEGIN SPI1_IRQn 1 */

  /* USER CODE END SPI1_IRQn 1 */
}

/**
  * @brief This function handles USART3 global interrupt.
  */
void USART3_IRQHandler(void)
{
  /* USER CODE BEGIN USART3_IRQn 0 */

  /* USER CODE END USART3_IRQn 0 */
  HAL_UART_IRQHandler(&huart3);
  /* USER CODE BEGIN USART3_IRQn 1 */

  /* USER CODE END USART3_IRQn 1 */
}

/**
  * @brief This function handles DMA2 stream0 global interrupt.
  */
void DMA2_Stream0_IRQHandler(void)
{
  /* USER CODE BEGIN DMA2_Stream0_IRQn 0 */

  /* USER CODE END DMA2_Stream0_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_adc1);
  /* USER CODE BEGIN DMA2_Stream0_IRQn 1 */

  /* USER CODE END DMA2_Stream0_IRQn 1 */
}

/**
  * @brief This function handles DMA2 stream3 global interrupt.
  */
void DMA2_Stream3_IRQHandler(void)
{
  /* USER CODE BEGIN DMA2_Stream3_IRQn 0 */

  /* USER CODE END DMA2_Stream3_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_spi1_tx);
  /* USER CODE BEGIN DMA2_Stream3_IRQn 1 */

  /* USER CODE END DMA2_Stream3_IRQn 1 */
}

/**
  * @brief This function handles USART6 global interrupt.
  */
void USART6_IRQHandler(void)
{
  /* USER CODE BEGIN USART6_IRQn 0 */

  /* USER CODE END USART6_IRQn 0 */
  HAL_UART_IRQHandler(&huart6);
  /* USER CODE BEGIN USART6_IRQn 1 */

  /* USER CODE END USART6_IRQn 1 */
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
