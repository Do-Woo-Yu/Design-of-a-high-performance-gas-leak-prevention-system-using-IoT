// user_define.h
// Revision : 2022. 05. 26

#define ENQ		0x05
#define ACK		0x06
#define STX		0x02
#define ETX		0x03
#define EOT		0x04
//#define CR		0x0d(Uncomment, error)
#define LF		0x0a

#define fontNo1   0x41       
#define fontNo2   0x03
//RGB565  
#define black	0x0000  //色
#define white	0xffff  //色
#define red		0xf800  //色
#define green	0x07e0  //色
#define blue	0x001f  //色
#define gray    0x8410  //色 //0x18c3
#define yellow  0xffe0  //色 color_red|color_green
#define violet  0xf81f  //色 color_red|color_blue
#define orange  0xFD00  //色
#define cyan    0x07ff  //色 color_green|color_blue
#define Transparency 0x0ff0 // 捧疙祸

//4.3" TFT LCD : 8Bit Bus肺 备己
#define	LCD_DATA	*(byte *)0x60000000	//A16:0(RS:0 ---> Data Selection)
#define	LCD_CMD 	*(byte *)0x60010000	//A16:1(RS:1 ---> Command Selection)

///////////////////////////////////define///////////////////////////////////////

// Nucleo Board狼 RED_LED(LD3)啊 High老锭 LED啊 ON
#define RED_LD3_OFF		GPIOB->BSRR  = (uint32_t)GPIO_PIN_14<<16	//PB14 <= 0
#define RED_LD3_ON		GPIOB->BSRR  = (uint32_t)GPIO_PIN_14 		//PB14 <= 1
#define RED_LD3_TOG		GPIOB->ODR  ^= (uint32_t)GPIO_PIN_14    	//^PB14  

// Nucleo Board狼 BLUE_LED(LD2)啊 High老锭 LED啊 ON
#define BLUE_LD2_OFF	GPIOB->BSRR  = (uint32_t)GPIO_PIN_7<<16		//PB7 <= 0
#define BLUE_LD2_ON		GPIOB->BSRR  = (uint32_t)GPIO_PIN_7 		//PB7 <= 1
#define BLUE_LD2_TOG	GPIOB->ODR  ^= (uint32_t)GPIO_PIN_7			//^PB7  

// LCD's Reset(PC8) : Active Low
#define LCD_RST_LOW		GPIOC->BSRR  = (uint32_t)GPIO_PIN_8<<16		//PC8 <= 0
#define LCD_RST_HIGH	GPIOC->BSRR  = (uint32_t)GPIO_PIN_8 		//PC8 <= 1

//#define RACK_SEL1_Low		GPIOA->BSRR = (uint32_t)GPIO_PIN_3 << 16	//PA3 <= 0
//#define RACK_SEL1_High	GPIOA->BSRR = (uint32_t)GPIO_PIN_3 			//PA3 <= 1


// Nucleo Board狼 User Switch(B1)甫 Push且 锭 High啊 涝仿
#define User_SW1	(((GPIOC->IDR)>>13) & 0x1)	//PC13's IDR : Input Data Register\


#define board_unit1   1
#define board_unit2   0



// end of user_define.h