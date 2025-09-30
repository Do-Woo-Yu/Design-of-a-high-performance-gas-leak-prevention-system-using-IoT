/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "lcd_user.h"
#include "extern.h"
#include "main_image.h"
#include "main_image2.h"
#include "main_image3.h"

// WS2812 LED 부분
#ifndef __WS2812_SPI_H_
#define __WS2812_SPI_H_

#include "stm32f4xx_hal.h"


/* Exported types ------------------------------------------------------------*/

/* Private defines -----------------------------------------------------------*/
#define WS2812_NUM_LEDS 60
#define WS2812_RESET_PULSE 60
#define WS2812_BUFFER_SIZE (WS2812_NUM_LEDS * 24 + WS2812_RESET_PULSE)
/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/

/* Exported functions prototypes ---------------------------------------------*/
void WS2812_SPI_Init(SPI_HandleTypeDef *hspi);
void WS2812_Set(uint16_t led_no, uint8_t r, uint8_t g, uint8_t b);
void WS2812_SetAll(uint8_t r, uint8_t g, uint8_t b);
void WS2812_Refresh(void);
#endif

#define WS2812_FILL_BUFFER(COLOR) \
for( uint8_t mask = 0x80; mask; mask >>= 1 ) { \
	if( COLOR & mask ) { \
		*ptr++ = 0xF8; \
	} else { \
		*ptr++ = 0xC0; \
	} \
}
static SPI_HandleTypeDef *m_Hspi;
	  
uint8_t ws2812_buffer[WS2812_BUFFER_SIZE];

void WS2812_SPI_Init(SPI_HandleTypeDef *hspi)
{
 	m_Hspi = hspi;
	HAL_SPI_Transmit_DMA(m_Hspi, ws2812_buffer, WS2812_BUFFER_SIZE);
}

void WS2812_Set(uint16_t led_no, uint8_t r, uint8_t g, uint8_t b) {
	if(led_no > WS2812_NUM_LEDS-1) return;
    uint8_t * ptr = &ws2812_buffer[24 * led_no];
    WS2812_FILL_BUFFER(g);
    WS2812_FILL_BUFFER(r);
    WS2812_FILL_BUFFER(b);
}

void WS2812_SetAll(uint8_t r, uint8_t g, uint8_t b) {
    uint8_t * ptr = ws2812_buffer;
    for( uint16_t i = 0; i < WS2812_NUM_LEDS; i++) {
        WS2812_FILL_BUFFER(g);
        WS2812_FILL_BUFFER(r);
        WS2812_FILL_BUFFER(b);
    }
}

void WS2812_Refresh(void)
{
	HAL_SPI_Transmit_DMA(m_Hspi, ws2812_buffer, WS2812_BUFFER_SIZE);
}

//WS2812 LED 변수
unsigned int Change = 0;
unsigned int Change_Count = 0;
unsigned int Red_color = 0;
unsigned int Red_color_cnt=0,safe_discolor=0;
unsigned int Red_Flag = 0, Green_Flag = 0, Blue_Flag = 0;

//LCD 및 디지털 시계 변수
unsigned int cho_0 = 0, cho_1 = 0, min_0 = 0, min_1 = 0, hour_0 = 0, hour_1;
unsigned int switchi_si = 0, switchi_bun = 0, push_btn = 0, push_tick = 0, stable_time = 0 ;
unsigned int count = 0;
unsigned char Clock_Sec_0[],Clock_Sec_1[], Clock_Min_0[], Clock_Min_1[], Clock_Hour_0[], Clock_Hour_1[];
unsigned int min_cnt=0, hour_cnt=0;

//CO 센서 변수
unsigned int CO_final_adc = 0;
word CO_adc1, CO_gas;
float CO_adc2=0.0;
extern unsigned int CO_gas_100,CO_gas_10,CO_gas_1,T3min_cnt,warming_time,CO_warming;

//CO2 센서 변수
unsigned int adc_cnt = 0,adc_cnt1 = 0,adc_cnt2 = 0, adc_cnt3 = 0, adc_1000ms = 0,adc_2000ms = 0,middle_adc = 0, final_adc = 0,num = 0,number = 0;
unsigned char CO2_1000[],CO2_100[], CO2_10[], CO2_1[];
word adc1, adc2, CO2_gas,semi_gas;

// Gas_Valve_Lock 변수
unsigned int unLock_Flag = 0, Lock = 0;
unsigned int pulse_change = 0;
float Lock_Flag = 0.0;

// SD카드 변수
unsigned int gas_enable = 0,gas_alarm=0;

unsigned char gas_tr_1[15] = {0x7E, 0xFF, 0x06, 0x03, 0x00, 0x00, 0x01, 0xFE, 0xF7, 0xEF}; // 가스 경보음을 재생하는 데이터를 보냄 
unsigned char gas_re_1[250];

unsigned int gas_test = 0;
word check_sum = 0, final_check_sum = 0;

unsigned int Stop_Alarm = 0;

unsigned char stop_tr_1[15] = {0x7E, 0xFF, 0x06, 0x0E, 0x00, 0x00, 0x01, 0xFE, 0xF7, 0xEF}; // 가스 경보음을 정지하는 데이터를 보냄
unsigned char stop_re_1[250];

word stop_check_sum = 0, stop_final_check_sum = 0;

//bluetooth 모듈 변수
unsigned int ble_CO_tx_enable = 0,bluetooth_Flag=0,bluetooth_stop = 0,bluetooth_icon=0,reset_rx=0,CO_scan_cnt=0,re_reset_cnt=0;

volatile int bluetooth_tx_enable = 0,CO_TX = 0;

unsigned char RE_tr_1[11] = "AT+REBOOT\r\n"; // 블루투스 연결 명령어를 보내는 데이터 
unsigned char RE_re_1[250];

unsigned char CO_tr_1[18]; // 블루투스 CO 가스 농도를 알려주는 명령어를 보내는 데이터 
unsigned char CO_re_1[18]; // 블루투스 CO 가스 농도를 알려주는 명령어를 보내는 데이터 

unsigned char Re_cnt[2];

unsigned char Stop_tr_1[9] = "AT+DSCA\r\n"; // 블루투스 연결을 멈추는 명령어를 보내는 데이터 
unsigned char Stop_re_1[250] = "\0";

byte TIME_1[5] = {"00:00"};
byte TIME_2[3];
byte Date[11] = {"2000/00/00"};

byte RTC_EN = 0;
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
RTC_TimeTypeDef rtc_time;
RTC_DateTypeDef rtc_date;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;
DMA_HandleTypeDef hdma_adc1;

RTC_HandleTypeDef hrtc;

SPI_HandleTypeDef hspi1;
DMA_HandleTypeDef hdma_spi1_tx;

TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;

UART_HandleTypeDef huart3;
UART_HandleTypeDef huart6;

SRAM_HandleTypeDef hsram1;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_FMC_Init(void);
static void MX_TIM2_Init(void);
static void MX_USART3_UART_Init(void);
static void MX_RTC_Init(void);
static void MX_ADC1_Init(void);
static void MX_SPI1_Init(void);
static void MX_TIM3_Init(void);
static void MX_USART6_UART_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */


/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_FMC_Init();
  MX_TIM2_Init();
  MX_USART3_UART_Init();
  MX_RTC_Init();
  MX_ADC1_Init();
  MX_SPI1_Init();
  MX_TIM3_Init();
  MX_USART6_UART_Init();
  /* USER CODE BEGIN 2 */
  
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  LCD_RST_LOW;
  HAL_Delay(100);	//100ms soft Delay
  LCD_RST_HIGH;
  HAL_Delay(100);	//100ms soft Delay
  LCD_Init();
  clear_all(black);
  
  //dis_picture(&main_image[0], 0, 0, 480, 272); // 480 x 272 pixel LCD 배경화면 표시(가스누출방지)
  //HAL_Delay(5000); // 5초 동안 기다림.
  dis_picture(&main_image2[0], 0, 0, 480, 272); // 480 x 272 pixel LCD 배경화면 표시(밤하늘) // 주석 풀어야해!!!!!!!!!!!!!!!!!!!!
  
  HAL_TIM_Base_Start_IT(&htim2);
  
  RTC_TimeTypeDef sTime = {0};
  RTC_DateTypeDef sDate = {0};
  
  //HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_1);
  //HAL_TIM_PWM_Stop(&htim3,TIM_CHANNEL_1);
  
  bluetooth_tx_enable = 1;// bluetooth 연결 시작
  
  ble_CO_tx_enable = 1; // CO 농도를 스마트폰으로 보내기 시작
  
  while (1)
  {	
	static unsigned int test_cnt = 0;
	
	test_cnt++;
	
	if(bluetooth_tx_enable)
	{
		bluetooth_tx_enable = 0;
		HAL_UART_Transmit_IT(&huart6, RE_tr_1, 11); // RE_tr_1 = "AT+REBOOT\r\n"; // 블루투스 연결 명령어를 보내는 데이터 
		//sprintf(CO_tr_1, "AT+SPPSEND=3,%03d\r\n", CO_gas);
		//HAL_UART_Transmit_IT(&huart6, "AT\r\n", 4);
		bluetooth_icon = 1;
	}
	
	HAL_UART_Receive_IT(&huart6,RE_re_1,250);
	
	if(CO_scan_cnt == 10)
	{
		for(int ble_rx = 0; ble_rx <=249; ble_rx++)
		{
			RE_re_1[ble_rx] = '\0';
		}
	}
	
	if(Stop_Alarm) // SD 카드 모듈 재생 정지 명령어
	 {
		stop_check_sum = stop_tr_1[1]+stop_tr_1[2]+stop_tr_1[3]+stop_tr_1[4]+stop_tr_1[5]+stop_tr_1[6];
		stop_final_check_sum = ~stop_check_sum + 1; // check_sum의 2의 보수 값
		stop_tr_1[7] = (byte)(stop_final_check_sum >> 8); // final_check_sum은 int이므로 byte로 바꾸어주어야함.
		stop_tr_1[8] = (byte)(stop_final_check_sum);
		HAL_UART_Transmit_IT(&huart3,stop_tr_1,10);
		
		HAL_UART_Receive_IT(&huart3,stop_re_1,10);
		Stop_Alarm = 0;
	 }
	
	// 어플을 통해서 블루투스 모듈로 데이터를 보낸 값들을 처리하는 부분 
	for(int R = 0; R <= 249; R++)
	{
	  	if(RE_re_1[R] == 'D' && RE_re_1[R+1] == 'A' && RE_re_1[R+2] == 'T' && RE_re_1[R+3] == 'A')
		{
		  if((RE_re_1[R+9] == 'a' && RE_re_1[R+10] == 'a') || (RE_re_1[R+9] == 'b' && RE_re_1[R+10] == 'b') || (RE_re_1[R+9] == 'c' && RE_re_1[R+10] == 'c') || (RE_re_1[R+9] == 'd' && RE_re_1[R+10] == 'd') || (RE_re_1[R+9] == 'e' && RE_re_1[R+10] == 'e') || (RE_re_1[R+9] == 'f' && RE_re_1[R+10] == 'f') || (RE_re_1[R+9] == 'g' && RE_re_1[R+10] == 'g')|| (RE_re_1[R+9] == 'h' && RE_re_1[R+10] == 'h')|| (RE_re_1[R+9] == 'i' && RE_re_1[R+10] == 'i')|| (RE_re_1[R+9] == 'j' && RE_re_1[R+10] == 'j') )
		  {
			Re_cnt[0] = RE_re_1[R+9];
			Re_cnt[1] = RE_re_1[R+10];
		  }
		  /*else if(RE_re_1[R+9] == '%' && RE_re_1[R+10] == '%')
		  {
			 Stop_Alarm = 1; 
			Re_cnt[0] = RE_re_1[R+9];
			Re_cnt[1] = RE_re_1[R+10];
		  }*/
	   }
		
	   
	   if(RE_re_1[R] == '@' || RE_re_1[R+1] == '@')
	   {
			if (CO_TX >= 1)
			{
				CO_TX = 0;
				if(CO_gas >= 100)
				{
				  sprintf(CO_tr_1, "AT+SPPSEND=3,%03d\r\n", CO_gas);
				  HAL_UART_Transmit_IT(&huart6, CO_tr_1, 18);
				  for(int reset_RE_re_1 = 0; reset_RE_re_1 <=249; reset_RE_re_1++)
				  {
					RE_re_1[reset_RE_re_1] = '\0';
				  }
				}
				else if(CO_gas >= 10 && CO_gas < 100)
				{
				  sprintf(CO_tr_1, "AT+SPPSEND=3,%02d\r\n", CO_gas);
				  HAL_UART_Transmit_IT(&huart6, CO_tr_1, 17);
				  for(int reset_RE_re_1 = 0; reset_RE_re_1 <=249; reset_RE_re_1++)
				  {
					RE_re_1[reset_RE_re_1] = '\0';
				  }
				}
				else if(CO_gas < 10)
				{
				  sprintf(CO_tr_1, "AT+SPPSEND=3,%01d\r\n", CO_gas);
				  HAL_UART_Transmit_IT(&huart6, CO_tr_1, 16);
				  for(int reset_RE_re_1 = 0; reset_RE_re_1 <=249; reset_RE_re_1++)
				  {
					RE_re_1[reset_RE_re_1] = '\0';
				  }
				}
			}
	   }
	} 
	
	
	//LCD 디지털 시계 시간 맞추는 부분
	static byte set_rtc_hours = 0, set_rtc_minutes = 0, set_rtc_seconds = 0;
	
	if(RTC_EN)
	{
		RTC_EN = 0;
		
		sTime.Hours = set_rtc_hours;
		sTime.Minutes = set_rtc_minutes;
		sTime.Seconds = set_rtc_seconds;
		sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
		sTime.StoreOperation = RTC_STOREOPERATION_RESET;
		if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN) != HAL_OK)
		{
			Error_Handler();
		}
		sDate.WeekDay = RTC_WEEKDAY_MONDAY;
		sDate.Month = RTC_MONTH_MARCH;
		sDate.Date = 21;
		sDate.Year = 23;

		if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN) != HAL_OK)
		{
			Error_Handler();
		}
   }
   
   	if(min_cnt)
	{
	  RTC_EN = 1;
	  min_cnt = 0;
	  
	  if(++set_rtc_minutes > 59)
	  {
		set_rtc_minutes = 0;
	  }
	}
	
	if(hour_cnt)
	{
	  RTC_EN = 1;
	  hour_cnt = 0;

	  if(++set_rtc_hours > 23)
	  {
		set_rtc_hours = 0;
	  }
	}
	
	if(T2_200ms_flag)	//200ms마다 RTC값 읽어옴
	{
		T2_200ms_flag = 0;
		HAL_RTC_GetTime(&hrtc, &rtc_time, FORMAT_BIN); //RTC로부터 시간을 받아옴(시,분,초)
		HAL_RTC_GetDate(&hrtc, &rtc_date, FORMAT_BIN); //RTC로부터 날짜를 받아옴(년,월,일,요일)
	}
	
	TIME_1[0] = (rtc_time.Hours) / 10 + 0;
	TIME_1[1] = (rtc_time.Hours) % 10 + 0;
	TIME_1[3] = (rtc_time.Minutes) / 10 + 0;
	TIME_1[4] = (rtc_time.Minutes) % 10 + 0;
	
	TIME_2[0] = (rtc_time.Seconds)/10 + 0;
	TIME_2[1] = (rtc_time.Seconds)%10 + 0;

	static dword scan = 0;
	scan++;
	
	//WS2812 LED 색깔 설정
	static unsigned int r = 100 , g = 100, b = 100;
	
	//red = 255,0,0
	//green = 0,255,0
	//blue = 0,0,255
	//Purple = 255,0,255
	//Yellow = 255,255,0
	//Blue = 0,255,255
	//white = 255,255,255
	/*WS2812_SPI_Init(&hspi1);
	  
	WS2812_SetAll(r,g,b);

    HAL_SPI_Transmit_DMA(&hspi1, ws2812_buffer, WS2812_BUFFER_SIZE);*/
	if(safe_discolor)
	{
	  	static unsigned int reset_rx = 0;
		
	    // 블루투스 어플로 버튼을 눌렀을 떄 RX에서 받은 값에 따라 LED 색상이 변경 하는 부분
		for(int Ble_rx_cnt = 0; Ble_rx_cnt <= 1; Ble_rx_cnt++) // default
		{
		  if(Re_cnt[Ble_rx_cnt] == 'i') // LED static mode
		  {
			LED_Mode = 0;
			Re_cnt[0] = '\0';
			Re_cnt[1] = '\0';
			for(reset_rx = 0; reset_rx <= 249; reset_rx++) 
			{
			  RE_re_1[reset_rx] = '0';
			}
		  }
		  else if(Re_cnt[Ble_rx_cnt] == 'j') // LED gradation mode
		  {
			LED_Mode = 1;
			Re_cnt[0] = '\0';
			Re_cnt[1] = '\0';
			for(reset_rx = 0; reset_rx <= 249; reset_rx++) 
			{
			  RE_re_1[reset_rx] = '0';
			}
		  }
		  
		  if(LED_Mode == 0)
		  {
			  if(Re_cnt[Ble_rx_cnt] == 'a')
			  {
				Change_Count = 0;
				Re_cnt[0] = '\0';
				Re_cnt[1] = '\0';
				for(reset_rx = 0; reset_rx <= 249; reset_rx++) 
				{
				  RE_re_1[reset_rx] = '0';
				}
			  }
			  
			  else if(Re_cnt[Ble_rx_cnt] == 'b') // white
			  {
				Change_Count = 1;
				
				Re_cnt[0] = '\0';
				Re_cnt[1] = '\0';
				for(reset_rx = 0; reset_rx <= 249; reset_rx++)
				{
				  RE_re_1[reset_rx] = '0';
				}
			  }
			  
			  else if(Re_cnt[Ble_rx_cnt] == 'c') // green
			  {
				Change_Count = 2;
				Re_cnt[0] = '\0';
				Re_cnt[1] = '\0';
				for(reset_rx = 0; reset_rx <= 249; reset_rx++)
				{
				  RE_re_1[reset_rx] = '0';
				}
			  }
			  
			  else if(Re_cnt[Ble_rx_cnt] == 'd') // blue
			  {
				Change_Count = 3;
				Re_cnt[0] = '\0';
				Re_cnt[1] = '\0';
				for(reset_rx = 0; reset_rx <= 249; reset_rx++)
				{
				  RE_re_1[reset_rx] = '0';
				}
			  }
			  
			  else if(Re_cnt[Ble_rx_cnt] == 'e') // purple
			  {
				Change_Count = 4;
				Re_cnt[0] = '\0';
				Re_cnt[1] = '\0';
				for(reset_rx = 0; reset_rx <= 249; reset_rx++)
				{
				  RE_re_1[reset_rx] = '0';
				}
			  }
			  
			  else if(Re_cnt[Ble_rx_cnt] == 'f') // yellow
			  {
				Change_Count = 5;
				Re_cnt[0] = '\0';
				Re_cnt[1] = '\0';
				for(reset_rx = 0; reset_rx <= 249; reset_rx++)
				{
				  RE_re_1[reset_rx] = '0';
				}
			  }
			  
			  else if(Re_cnt[Ble_rx_cnt] == 'g') // sky blue
			  {
				Change_Count = 6;
				Re_cnt[0] = '\0';
				Re_cnt[1] = '\0';
				for(reset_rx = 0; reset_rx <= 249; reset_rx++)
				{
				  RE_re_1[reset_rx] = '0';
				}
			  }
		  }
		  else if(LED_Mode == 1)
		  {
			  if(Re_cnt[Ble_rx_cnt] == 'a')
			  {
				Change_Count = 0;
				Re_cnt[0] = '\0';
				Re_cnt[1] = '\0';
				for(reset_rx = 0; reset_rx <= 249; reset_rx++) 
				{
				  RE_re_1[reset_rx] = '0';
				}
			  }
			  
			  else if(Re_cnt[Ble_rx_cnt] == 'b') // white
			  {
				Change_Count = 1;
				
				Re_cnt[0] = '\0';
				Re_cnt[1] = '\0';
				for(reset_rx = 0; reset_rx <= 249; reset_rx++)
				{
				  RE_re_1[reset_rx] = '0';
				}
			  }
			  
			  else if(Re_cnt[Ble_rx_cnt] == 'c') // green
			  {
				Change_Count = 2;
				Re_cnt[0] = '\0';
				Re_cnt[1] = '\0';
				for(reset_rx = 0; reset_rx <= 249; reset_rx++)
				{
				  RE_re_1[reset_rx] = '0';
				}
			  }
			  
			  else if(Re_cnt[Ble_rx_cnt] == 'd') // blue
			  {
				Change_Count = 3;
				Re_cnt[0] = '\0';
				Re_cnt[1] = '\0';
				for(reset_rx = 0; reset_rx <= 249; reset_rx++)
				{
				  RE_re_1[reset_rx] = '0';
				}
			  }
			  
			  else if(Re_cnt[Ble_rx_cnt] == 'e') // purple
			  {
				Change_Count = 4;
				Re_cnt[0] = '\0';
				Re_cnt[1] = '\0';
				for(reset_rx = 0; reset_rx <= 249; reset_rx++)
				{
				  RE_re_1[reset_rx] = '0';
				}
			  }
			  
			  else if(Re_cnt[Ble_rx_cnt] == 'f') // yellow
			  {
				Change_Count = 5;
				Re_cnt[0] = '\0';
				Re_cnt[1] = '\0';
				for(reset_rx = 0; reset_rx <= 249; reset_rx++)
				{
				  RE_re_1[reset_rx] = '0';
				}
			  }
			  
			  else if(Re_cnt[Ble_rx_cnt] == 'g') // sky blue
			  {
				Change_Count = 6;
				Re_cnt[0] = '\0';
				Re_cnt[1] = '\0';
				for(reset_rx = 0; reset_rx <= 249; reset_rx++)
				{
				  RE_re_1[reset_rx] = '0';
				}
			  }
		  }
		}
		
		switch(Change_Count)
		{
			 case 0 :

			  WS2812_SetAll(Red_Flag,Green_Flag,Blue_Flag);// 꺼짐 상태

			  HAL_SPI_Transmit_DMA(&hspi1, ws2812_buffer, WS2812_BUFFER_SIZE);
			  
			  break;
			  
			 case 1 :
			  //WS2812_SetAll(33,33,33);// White
			 
			  WS2812_SetAll(Red_Flag,Green_Flag,Blue_Flag);

			  HAL_SPI_Transmit_DMA(&hspi1, ws2812_buffer, WS2812_BUFFER_SIZE);
			  
			  break;
				  
			 case 2 :
			  
			  //WS2812_SetAll(0,100,0); // Green
			 
			  WS2812_SetAll(Red_Flag,Green_Flag,Blue_Flag);

			  HAL_SPI_Transmit_DMA(&hspi1, ws2812_buffer, WS2812_BUFFER_SIZE);
			  
			  break;
			  
			 case 3 :
			  
			  //WS2812_SetAll(0,0,100); // Blue
			 
			  WS2812_SetAll(Red_Flag,Green_Flag,Blue_Flag);

			  HAL_SPI_Transmit_DMA(&hspi1, ws2812_buffer, WS2812_BUFFER_SIZE);
			  
			  break;
			  
			 case 4 :
			  
			  //WS2812_SetAll(50,0,50); // Purple
			 
			  WS2812_SetAll(Red_Flag,Green_Flag,Blue_Flag);

			  HAL_SPI_Transmit_DMA(&hspi1, ws2812_buffer, WS2812_BUFFER_SIZE);
			  
			  break;	
			  
			case 5 :
			  
			  //WS2812_SetAll(50,50,0); // Yellow
			
			 WS2812_SetAll(Red_Flag,Green_Flag,Blue_Flag);

			  HAL_SPI_Transmit_DMA(&hspi1, ws2812_buffer, WS2812_BUFFER_SIZE);
			  
			  break;
		  
			 case 6 :
			  
			 //WS2812_SetAll(0,50,50); // Sky Blue 
			  WS2812_SetAll(Red_Flag,Green_Flag,Blue_Flag);

			  HAL_SPI_Transmit_DMA(&hspi1, ws2812_buffer, WS2812_BUFFER_SIZE);
			  
			  break;
			  
			/*case 7 :
			 WS2812_SetAll(100,0,0);// Red

			 HAL_SPI_Transmit_DMA(&hspi1, ws2812_buffer, WS2812_BUFFER_SIZE);
			 
			 break;*/
		  
		}
  }
	  static unsigned int CO_gas_test = 0;
	  if(CO_gas >= 30)
	  {
		 Lock = 1;
		 
		 safe_discolor = 0;
		 
		 for(int Ble_rx_cnt_flag = 0; Ble_rx_cnt_flag <= 1; Ble_rx_cnt_flag++)
		 {
			 if(Re_cnt[Ble_rx_cnt_flag] == 'h')
			 {
				stop_check_sum = stop_tr_1[1]+stop_tr_1[2]+stop_tr_1[3]+stop_tr_1[4]+stop_tr_1[5]+stop_tr_1[6];
				stop_final_check_sum = ~stop_check_sum + 1; // check_sum의 2의 보수 값
				stop_tr_1[7] = (byte)(stop_final_check_sum >> 8); // final_check_sum은 int이므로 byte로 바꾸어주어야함.
				stop_tr_1[8] = (byte)(stop_final_check_sum);
				HAL_UART_Transmit_IT(&huart3,stop_tr_1,10);
				
				HAL_UART_Receive_IT(&huart3,stop_re_1,10);
				
				HAL_TIM_PWM_Stop(&htim3,TIM_CHANNEL_1);
				
				Re_cnt[0] = '\0';
				Re_cnt[1] = '\0';
				for(int reset_rx_flag = 0; reset_rx_flag <= 249; reset_rx_flag++)
				{
				  RE_re_1[reset_rx_flag] = '0';
				}
			 }
	     }
		   
		 if(gas_enable == 2)
	     {
			check_sum = gas_tr_1[1]+gas_tr_1[2]+gas_tr_1[3]+gas_tr_1[4]+gas_tr_1[5]+gas_tr_1[6];
			final_check_sum = ~check_sum + 1; // check_sum의 2의 보수 값
			gas_tr_1[7] = (byte)(final_check_sum >> 8); // final_check_sum은 int이므로 byte로 바꾸어주어야함.
			gas_tr_1[8] = (byte)(final_check_sum);
			HAL_UART_Transmit_IT(&huart3,gas_tr_1,10);
			
			HAL_UART_Receive_IT(&huart3,gas_re_1,10);
	     }
		 
		 if(Lock_Flag == 5.0)
		 {
		   HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_1);
		 }
		 else if(Lock_Flag == 5.2)
		 {
			pulse_change = 500;	
			HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_1);
		 }
		 else if(Lock_Flag == 5.4)
		 {
			pulse_change = 1000;
			HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_1);
		 }
		 else if(Lock_Flag == 5.6)
		 {
			pulse_change = 1500;
			HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_1);
		 }
		 else if(Lock_Flag == 5.8)
		 {
			pulse_change = 2000;
			HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_1);
		 }
		 else if(Lock_Flag == 6.0)
		 {
			pulse_change = 2500;
			HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_1);
		 }
		 else if(Lock_Flag > 6.0)
		 {
		   HAL_TIM_PWM_Stop(&htim3,TIM_CHANNEL_1);
		 }
		 
		 WS2812_SetAll(100,0,0);// Red

	     HAL_SPI_Transmit_DMA(&hspi1, ws2812_buffer, WS2812_BUFFER_SIZE);
		 
	  }
	  else if(CO_gas < 30)
	 {
	 	Lock = 0;
	  	Lock_Flag = 0.0;
		gas_enable = 0;
		safe_discolor = 1;
	  }
	 
  /*if(unLock_Flag >= 1000)
  {
	Lock = 1;
	if(Lock_Flag == 1)
	{
		HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_1);
	}
	else if(Lock_Flag == 2)
	{
		HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_1);
	}
	else
	{
		HAL_TIM_PWM_Stop(&htim3,TIM_CHANNEL_1);
	}
  }
  else
  {
	Lock = 0;
	Lock_Flag = 0;
  }*/
  
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 180;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Activate the Over-Drive mode
  */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */
  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.ScanConvMode = ENABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.DiscontinuousConvMode = ENABLE;
  hadc1.Init.NbrOfDiscConversion = 1;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 2;
  hadc1.Init.DMAContinuousRequests = DISABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_0;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Rank = 2;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief RTC Initialization Function
  * @param None
  * @retval None
  */
static void MX_RTC_Init(void)
{

  /* USER CODE BEGIN RTC_Init 0 */

  /* USER CODE END RTC_Init 0 */

  /* USER CODE BEGIN RTC_Init 1 */

  /* USER CODE END RTC_Init 1 */
  /** Initialize RTC Only
  */
  hrtc.Instance = RTC;
  hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
  hrtc.Init.AsynchPrediv = 127;
  hrtc.Init.SynchPrediv = 255;
  hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
  hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN RTC_Init 2 */

  /* USER CODE END RTC_Init 2 */

}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 0;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 90e6/1e3-1;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 300-1;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 5000-1;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 2500;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */
  HAL_TIM_MspPostInit(&htim3);

}

/**
  * @brief USART3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 9600;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */

}

/**
  * @brief USART6 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART6_UART_Init(void)
{

  /* USER CODE BEGIN USART6_Init 0 */

  /* USER CODE END USART6_Init 0 */

  /* USER CODE BEGIN USART6_Init 1 */

  /* USER CODE END USART6_Init 1 */
  huart6.Instance = USART6;
  huart6.Init.BaudRate = 115200;
  huart6.Init.WordLength = UART_WORDLENGTH_8B;
  huart6.Init.StopBits = UART_STOPBITS_1;
  huart6.Init.Parity = UART_PARITY_NONE;
  huart6.Init.Mode = UART_MODE_TX_RX;
  huart6.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart6.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart6) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART6_Init 2 */

  /* USER CODE END USART6_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA2_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA2_Stream0_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream0_IRQn);
  /* DMA2_Stream3_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream3_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream3_IRQn);

}

/* FMC initialization function */
static void MX_FMC_Init(void)
{

  /* USER CODE BEGIN FMC_Init 0 */

  /* USER CODE END FMC_Init 0 */

  FMC_NORSRAM_TimingTypeDef Timing = {0};

  /* USER CODE BEGIN FMC_Init 1 */

  /* USER CODE END FMC_Init 1 */

  /** Perform the SRAM1 memory initialization sequence
  */
  hsram1.Instance = FMC_NORSRAM_DEVICE;
  hsram1.Extended = FMC_NORSRAM_EXTENDED_DEVICE;
  /* hsram1.Init */
  hsram1.Init.NSBank = FMC_NORSRAM_BANK1;
  hsram1.Init.DataAddressMux = FMC_DATA_ADDRESS_MUX_DISABLE;
  hsram1.Init.MemoryType = FMC_MEMORY_TYPE_SRAM;
  hsram1.Init.MemoryDataWidth = FMC_NORSRAM_MEM_BUS_WIDTH_8;
  hsram1.Init.BurstAccessMode = FMC_BURST_ACCESS_MODE_DISABLE;
  hsram1.Init.WaitSignalPolarity = FMC_WAIT_SIGNAL_POLARITY_LOW;
  hsram1.Init.WrapMode = FMC_WRAP_MODE_DISABLE;
  hsram1.Init.WaitSignalActive = FMC_WAIT_TIMING_BEFORE_WS;
  hsram1.Init.WriteOperation = FMC_WRITE_OPERATION_ENABLE;
  hsram1.Init.WaitSignal = FMC_WAIT_SIGNAL_DISABLE;
  hsram1.Init.ExtendedMode = FMC_EXTENDED_MODE_DISABLE;
  hsram1.Init.AsynchronousWait = FMC_ASYNCHRONOUS_WAIT_ENABLE;
  hsram1.Init.WriteBurst = FMC_WRITE_BURST_DISABLE;
  hsram1.Init.ContinuousClock = FMC_CONTINUOUS_CLOCK_SYNC_ONLY;
  hsram1.Init.PageSize = FMC_PAGE_SIZE_NONE;
  /* Timing */
  Timing.AddressSetupTime = 2;
  Timing.AddressHoldTime = 15;
  Timing.DataSetupTime = 7;
  Timing.BusTurnAroundDuration = 6;
  Timing.CLKDivision = 16;
  Timing.DataLatency = 17;
  Timing.AccessMode = FMC_ACCESS_MODE_A;
  /* ExtTiming */

  if (HAL_SRAM_Init(&hsram1, &Timing, NULL) != HAL_OK)
  {
    Error_Handler( );
  }

  /* USER CODE BEGIN FMC_Init 2 */

  /* USER CODE END FMC_Init 2 */
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_RESET);

  /*Configure GPIO pin : Color_Change_Pin */
  GPIO_InitStruct.Pin = Color_Change_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(Color_Change_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : PC3 */
  GPIO_InitStruct.Pin = GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : PA3 */
  GPIO_InitStruct.Pin = GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : PB2 */
  GPIO_InitStruct.Pin = GPIO_PIN_2;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : Bun_Change_Pin Si_Change_Pin */
  GPIO_InitStruct.Pin = Bun_Change_Pin|Si_Change_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
