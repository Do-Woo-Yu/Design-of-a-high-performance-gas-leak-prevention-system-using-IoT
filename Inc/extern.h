//extern.h
// Revision : 2022. 05. 26

typedef unsigned char	byte;                //1byte 
typedef unsigned short	word;                //2byte                       
typedef unsigned long	dword;               //

extern byte T2_Sec_Flag;
extern void	LCD_Init(void);
extern void XY_Coordinate(word X, word Y);
extern void Background_Color(word color);
extern void Active_Window(word XL, word XR, word YT, word YB);
extern void Draw_Line(word XS, word YS, word XE, word YE, word color);
extern void clear_all(word b_color);
extern byte zheng[];
extern void dis_field(void);



extern const unsigned char wind_image[];
extern const unsigned char white_image[];

extern byte abc;
extern byte time2_1mcnt_flag;
extern void dis_picture(const byte *p, unsigned int x, unsigned int y, word width, word height);
extern byte Push_SW;
extern byte Go_next;
extern byte rx_buf[255];
#define NO_BACKGROUND   1
#define BACKGROUND      0
extern byte back_button;
/////////////////////////////////////
extern byte Timer_flag_start_white;                 // 타이머 플래그 스타트 화이트를 1로 만들어 줘서 올화이트를 한 번만 실행하게 만드는 변수
/////////////////////////////////////
extern word time2_1mcnt;
/////////////////////////////////////예약시간
extern byte share1;
extern byte remainder1;
extern byte share2;
extern byte remainder2;
extern byte share3;
extern byte remainder3;
extern word reserve_sec1;//300
extern word reserve_sec2;//600
extern word reserve_sec3;//900초
extern word Time_start;
extern byte Timer_flag;
extern char updown_button;
extern byte T2_sec;
/////////////////////////////////////예약시간
//온습도센서
extern byte err_flag;
extern byte two_sec_flag;
extern word interrupt_cnt;
extern float Real_humi_value, Real_temp_value;
extern byte sec;
extern byte INT_flag;
extern byte noback_flag;
extern byte reserv_white;
extern byte reserv_count2;
extern byte reserv_count3;
extern byte reserv_count4;
#define		START_H				GPIOC->ODR |= GPIO_PIN_0;  //PC0--->1
#define		START_L				GPIOC->ODR &= ~GPIO_PIN_0; //PC0--->0

#define		INT_TIM2_two_sec	1
#define 	NO_INTERRUPT		0


//스팀
#define		STEAM_H				GPIOG->ODR |= GPIO_PIN_6;  //PG6--->1    or 
#define		STEAM_L			 	GPIOG->ODR &= ~GPIO_PIN_6; //PG6--->0    and bit반전

extern byte time3000_flag;

extern unsigned char Consolas_60pt[];
extern unsigned char Consolas_24x44[];
extern unsigned char Consolas_16x30[];
extern byte human_word_100ptBitmaps_88x118[];
extern byte human_word_100ptBitmaps_null_88x118[];

extern RTC_TimeTypeDef rtc_time;
extern RTC_DateTypeDef rtc_date;

extern byte TIME_1[5];
extern byte TIME_2[3];

extern byte T2_200ms_flag;

extern byte consolas_50pt_36x61[];
extern byte consolas_50pt_null_36x61[];

extern unsigned int Red_Flag, Green_Flag, Blue_Flag, LED_Mode;

// end of extern.h