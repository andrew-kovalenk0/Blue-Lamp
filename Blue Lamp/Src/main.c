#include "stm32f746xx.h"
#include "five_picture_1.h"
#include "four_picture_1.h"
#include "nine_picture_1.h"
#include "one_picture_1.h"
#include "seven_picture_1.h"
#include "six_picture_1.h"
#include "three_picture_1.h"
#include "two_picture_1.h"
#include "zero_picture_1.h"
#include "eigth_picture_1.h"
#include "five_picture_2.h"
#include "four_picture_2.h"
#include "nine_picture_2.h"
#include "one_picture_2.h"
#include "seven_picture_2.h"
#include "six_picture_2.h"
#include "three_picture_2.h"
#include "two_picture_2.h"
#include "zero_picture_2.h"
#include "eigth_picture_2.h"
#include "five_picture_3.h"
#include "four_picture_3.h"
#include "nine_picture_3.h"
#include "none_picture_3.h"
#include "one_picture_3.h"
#include "seven_picture_3.h"
#include "six_picture_3.h"
#include "three_picture_3.h"
#include "two_picture_3.h"
#include "zero_picture_3.h"
#include "eigth_picture_3.h"
#include "init_picture.h"
#include "main_picture.h"


#define  DISPLAY_HSYNC            ((uint16_t)30)
#define  DISPLAY_HBP              ((uint16_t)13)
#define  DISPLAY_HFP              ((uint16_t)32)
#define  DISPLAY_VSYNC            ((uint16_t)10)
#define  DISPLAY_VBP              ((uint16_t)2)
#define  DISPLAY_VFP              ((uint16_t)2)
#define  DISPLAY_WIDTH 			  ((uint16_t)480)
#define  DISPLAY_HEIGHT			  ((uint16_t)272)
#define  PIXEL_SIZE				  ((uint16_t)4)
#define  REFRESH_RATE 			  (1665)


static uint16_t screen[130560];
uint8_t flags = 0;
uint32_t cnt = 0;
uint8_t minute = 0;
uint8_t minute_2 = 0;
uint8_t hour = 0;
uint8_t hour_2 = 0;
uint8_t set_minute = 0;
uint8_t set_minute_2 = 0;
uint8_t set_hour = 0;
uint8_t set_hour_2 = 0;
uint8_t power = 70;


/*
	STATUS WORD:
	1 - Change of set time enable
	2 - Change of set power enable
	3 - Ligth is on
*/


void initialization()
{
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
	RCC->APB2ENR |= RCC_APB2ENR_LTDCEN;
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOEEN;
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOJEN;
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOGEN;
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOKEN;
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOIEN;
	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;

	// Freq
	RCC->CR |= RCC_CR_HSEON;
	while (!(RCC->CR & RCC_CR_HSERDY));
	FLASH->ACR |= FLASH_ACR_LATENCY_5WS;
	RCC->PLLCFGR |= RCC_PLLCFGR_PLLM_0 | RCC_PLLCFGR_PLLM_3 | RCC_PLLCFGR_PLLM_4;
	RCC->PLLCFGR |= RCC_PLLCFGR_PLLN_4 | RCC_PLLCFGR_PLLN_5 | RCC_PLLCFGR_PLLN_7 | RCC_PLLCFGR_PLLN_8;
	RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLN_6;
	RCC->PLLCFGR |= RCC_PLLCFGR_PLLP_0;
	RCC->CFGR |= RCC_CFGR_PPRE1_2 | RCC_CFGR_PPRE2_2;
	RCC->PLLCFGR |= RCC_PLLCFGR_PLLSRC;
	RCC->CR |= RCC_CR_PLLON;
	while((RCC->CR & RCC_CR_PLLRDY) == 0);
	RCC->CFGR |= RCC_CFGR_SW_PLL;
	while((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_1);
	RCC->PLLSAICFGR |= RCC_PLLSAICFGR_PLLSAIN_6 | RCC_PLLSAICFGR_PLLSAIN_7;
	RCC->PLLSAICFGR |= RCC_PLLSAICFGR_PLLSAIR_0 | RCC_PLLSAICFGR_PLLSAIR_2;
	RCC->DCKCFGR1 	|= RCC_DCKCFGR1_PLLSAIDIVR_0;
	RCC->DCKCFGR1 	&= ~RCC_DCKCFGR1_PLLSAIDIVR_1;
	RCC->CR |= RCC_CR_PLLSAION;
	while ((RCC->CR & RCC_CR_PLLSAIRDY) == 0);

	// PMW
	GPIOC->MODER |= GPIO_MODER_MODER6_1;
	GPIOC->AFR[0] |= GPIO_AFRL_AFRL6_1;
	TIM3->PSC = 1080;
	TIM3->ARR = 100;
	TIM3->CCR1 = 0;
	TIM3->CCMR1 |= TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2;
	TIM3->CCER |= TIM_CCER_CC1E;
	TIM3->CR1 |= TIM_CR1_CEN;

	// EXTI
	// PI0 - D5
	// PI1 - D13
	// PI3 - D7
	// PB4 - D3
	// PG6 - D2
	SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI0_PI | SYSCFG_EXTICR1_EXTI3_PI;
	SYSCFG->EXTICR[1] |= SYSCFG_EXTICR2_EXTI4_PB | SYSCFG_EXTICR2_EXTI6_PG;
	SYSCFG->EXTICR[3] |= SYSCFG_EXTICR4_EXTI15_PB;
	EXTI->IMR |= EXTI_IMR_MR0 | EXTI_IMR_MR3 | EXTI_IMR_MR4 | EXTI_IMR_MR6 | EXTI_IMR_MR15;
	EXTI->RTSR |= EXTI_RTSR_TR0 | EXTI_RTSR_TR3 | EXTI_RTSR_TR4 | EXTI_RTSR_TR6 | EXTI_RTSR_TR15;
	NVIC_EnableIRQ(EXTI0_IRQn);
	NVIC_EnableIRQ(EXTI3_IRQn);
	NVIC_EnableIRQ(EXTI4_IRQn);
	NVIC_EnableIRQ(EXTI9_5_IRQn);
	NVIC_EnableIRQ(EXTI15_10_IRQn);
	__enable_irq ();

	//B0 PE4
	GPIOE->MODER   &= ~GPIO_MODER_MODER4;
	GPIOE->MODER   |= GPIO_MODER_MODER4_1;
	GPIOE->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR4_1;
	GPIOE->AFR[0] &= ~GPIO_AFRL_AFRL4_0;
	GPIOE->AFR[0] |= GPIO_AFRL_AFRL4_1 | GPIO_AFRL_AFRL4_2 | GPIO_AFRL_AFRL4_3;

	//B1 PJ13
	GPIOJ->MODER   &= ~GPIO_MODER_MODER13;
	GPIOJ->MODER   |= GPIO_MODER_MODER13_1;
	GPIOJ->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR13_1;
	GPIOJ->AFR[1] &= ~GPIO_AFRL_AFRL5_0;
	GPIOJ->AFR[1] |= GPIO_AFRL_AFRL5_1 | GPIO_AFRL_AFRL5_2 | GPIO_AFRL_AFRL5_3;

	//B2 PJ14
	GPIOJ->MODER   &= ~GPIO_MODER_MODER14;
	GPIOJ->MODER   |= GPIO_MODER_MODER14_1;
	GPIOJ->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR14_1;
	GPIOJ->AFR[1] &= ~GPIO_AFRL_AFRL6_0;
	GPIOJ->AFR[1] |= GPIO_AFRL_AFRL6_1 | GPIO_AFRL_AFRL6_2 | GPIO_AFRL_AFRL6_3;

	//B3 PJ15
	GPIOJ->MODER   &= ~GPIO_MODER_MODER15;
	GPIOJ->MODER   |= GPIO_MODER_MODER15_1;
	GPIOJ->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR15_1;
	GPIOJ->AFR[1] &= ~GPIO_AFRL_AFRL7_0;
	GPIOJ->AFR[1] |= GPIO_AFRL_AFRL7_1 | GPIO_AFRL_AFRL7_2 | GPIO_AFRL_AFRL7_3;

	//B4 PJ12
	GPIOJ->MODER   &= ~GPIO_MODER_MODER12;
	GPIOJ->MODER   |= GPIO_MODER_MODER12_1;
	GPIOJ->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR12_1;
	GPIOJ->AFR[1] &= ~GPIO_AFRL_AFRL4_0;
	GPIOJ->AFR[1] |= GPIO_AFRL_AFRL4_1 | GPIO_AFRL_AFRL4_2 | GPIO_AFRL_AFRL4_3;

	//B5 PK4
	GPIOK->MODER   &= ~GPIO_MODER_MODER4;
	GPIOK->MODER   |= GPIO_MODER_MODER4_1;
	GPIOK->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR4_1;
	GPIOK->AFR[0] &= ~GPIO_AFRL_AFRL4_0;
	GPIOK->AFR[0] |= GPIO_AFRL_AFRL4_1 | GPIO_AFRL_AFRL4_2 | GPIO_AFRL_AFRL4_3;

	//B6 PK5
	GPIOK->MODER   &= ~GPIO_MODER_MODER5;
	GPIOK->MODER   |= GPIO_MODER_MODER5_1;
	GPIOK->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR5_1;
	GPIOK->AFR[0] &= ~GPIO_AFRL_AFRL5_0;
	GPIOK->AFR[0] |= GPIO_AFRL_AFRL5_1 | GPIO_AFRL_AFRL5_2 | GPIO_AFRL_AFRL5_3;

	//B7 PK6
	GPIOK->MODER   &= ~GPIO_MODER_MODER6;
	GPIOK->MODER   |= GPIO_MODER_MODER6_1;
	GPIOK->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR6_1;
	GPIOK->AFR[0] &= ~GPIO_AFRL_AFRL6_0;
	GPIOK->AFR[0] |= GPIO_AFRL_AFRL6_1 | GPIO_AFRL_AFRL6_2 | GPIO_AFRL_AFRL6_3;

	//R0 PI15
	GPIOI->MODER   &= ~GPIO_MODER_MODER15;
	GPIOI->MODER   |= GPIO_MODER_MODER15_1;
	GPIOI->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR15_1;
	GPIOI->AFR[1] &= ~GPIO_AFRL_AFRL7_0;
	GPIOI->AFR[1] |= GPIO_AFRL_AFRL7_1 | GPIO_AFRL_AFRL7_2 | GPIO_AFRL_AFRL7_3;

	//R1 PJ0
	GPIOJ->MODER   &= ~GPIO_MODER_MODER0;
	GPIOJ->MODER   |= GPIO_MODER_MODER0_1;
	GPIOJ->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR0_1;
	GPIOJ->AFR[0] &= ~GPIO_AFRL_AFRL0_0;
	GPIOJ->AFR[0] |= GPIO_AFRL_AFRL0_1 | GPIO_AFRL_AFRL0_2 | GPIO_AFRL_AFRL0_3;

	//R2 PJ1
	GPIOJ->MODER   &= ~GPIO_MODER_MODER1;
	GPIOJ->MODER   |= GPIO_MODER_MODER1_1;
	GPIOJ->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR1_1;
	GPIOJ->AFR[0] &= ~GPIO_AFRL_AFRL1_0;
	GPIOJ->AFR[0] |= GPIO_AFRL_AFRL1_1 | GPIO_AFRL_AFRL1_2 | GPIO_AFRL_AFRL1_3;

	//R3 PJ2
	GPIOJ->MODER   &= ~GPIO_MODER_MODER2;
	GPIOJ->MODER   |= GPIO_MODER_MODER2_1;
	GPIOJ->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR2_1;
	GPIOJ->AFR[0] &= ~GPIO_AFRL_AFRL2_0;
	GPIOJ->AFR[0] |= GPIO_AFRL_AFRL2_1 | GPIO_AFRL_AFRL2_2 | GPIO_AFRL_AFRL2_3;

	//R4 PJ3

	GPIOJ->MODER   &= ~GPIO_MODER_MODER3;
	GPIOJ->MODER   |= GPIO_MODER_MODER3_1;
	GPIOJ->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR3_1;
	GPIOJ->AFR[0] &= ~GPIO_AFRL_AFRL3_0;
	GPIOJ->AFR[0] |= GPIO_AFRL_AFRL3_1 | GPIO_AFRL_AFRL3_2 | GPIO_AFRL_AFRL3_3;

	//R5 PJ4
	GPIOJ->MODER   &= ~GPIO_MODER_MODER4;
	GPIOJ->MODER   |= GPIO_MODER_MODER4_1;
	GPIOJ->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR4_1;
	GPIOJ->AFR[0] &= ~GPIO_AFRL_AFRL4_0;
	GPIOJ->AFR[0] |= GPIO_AFRL_AFRL4_1 | GPIO_AFRL_AFRL4_2 | GPIO_AFRL_AFRL4_3;

	//R6 PJ5
	GPIOJ->MODER   &= ~GPIO_MODER_MODER5;
	GPIOJ->MODER   |= GPIO_MODER_MODER5_1;
	GPIOJ->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR5_1;
	GPIOJ->AFR[0] &= ~GPIO_AFRL_AFRL5_0;
	GPIOJ->AFR[0] |= GPIO_AFRL_AFRL5_1 | GPIO_AFRL_AFRL5_2 | GPIO_AFRL_AFRL5_3;

	//R7 PJ6
	GPIOJ->MODER   &= ~GPIO_MODER_MODER6;
	GPIOJ->MODER   |= GPIO_MODER_MODER6_1;
	GPIOJ->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR6_1;
	GPIOJ->AFR[0] &= ~GPIO_AFRL_AFRL6_0;
	GPIOJ->AFR[0] |= GPIO_AFRL_AFRL6_1 | GPIO_AFRL_AFRL6_2 | GPIO_AFRL_AFRL6_3;

	//G0 PJ7
	GPIOJ->MODER   &= ~GPIO_MODER_MODER7;
	GPIOJ->MODER   |= GPIO_MODER_MODER7_1;
	GPIOJ->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR7_1;
	GPIOJ->AFR[0] &= ~GPIO_AFRL_AFRL7_0;
	GPIOJ->AFR[0] |= GPIO_AFRL_AFRL7_1 | GPIO_AFRL_AFRL7_2 | GPIO_AFRL_AFRL7_3;

	//G1 PJ8
	GPIOJ->MODER   &= ~GPIO_MODER_MODER8;
	GPIOJ->MODER   |= GPIO_MODER_MODER8_1;
	GPIOJ->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR8_1;
	GPIOJ->AFR[1] &= ~GPIO_AFRL_AFRL0_0;
	GPIOJ->AFR[1] |= GPIO_AFRL_AFRL0_1 | GPIO_AFRL_AFRL0_2 | GPIO_AFRL_AFRL0_3;

	//G2 PJ9
	GPIOJ->MODER   &= ~GPIO_MODER_MODER9;
	GPIOJ->MODER   |= GPIO_MODER_MODER9_1;
	GPIOJ->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR9_1;
	GPIOJ->AFR[1] &= ~GPIO_AFRL_AFRL1_0;
	GPIOJ->AFR[1] |= GPIO_AFRL_AFRL1_1 | GPIO_AFRL_AFRL1_2 | GPIO_AFRL_AFRL1_3;

	//G3 PJ10
	GPIOJ->MODER   &= ~GPIO_MODER_MODER10;
	GPIOJ->MODER   |= GPIO_MODER_MODER10_1;
	GPIOJ->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR10_1;
	GPIOJ->AFR[1] &= ~GPIO_AFRL_AFRL2_0;
	GPIOJ->AFR[1] |= GPIO_AFRL_AFRL2_1 | GPIO_AFRL_AFRL2_2 | GPIO_AFRL_AFRL2_3;

	//G4 PJ11
	GPIOJ->MODER   &= ~GPIO_MODER_MODER11;
	GPIOJ->MODER   |= GPIO_MODER_MODER11_1;
	GPIOJ->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR11_1;
	GPIOJ->AFR[1] &= ~GPIO_AFRL_AFRL3_0;
	GPIOJ->AFR[1] |= GPIO_AFRL_AFRL3_1 | GPIO_AFRL_AFRL3_2 | GPIO_AFRL_AFRL3_3;

	//G5 PK0
	GPIOK->MODER   &= ~GPIO_MODER_MODER0;
	GPIOK->MODER   |= GPIO_MODER_MODER0_1;
	GPIOK->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR0_1;
	GPIOK->AFR[0] &= ~GPIO_AFRL_AFRL0_0;
	GPIOK->AFR[0] |= GPIO_AFRL_AFRL0_1 | GPIO_AFRL_AFRL0_2 | GPIO_AFRL_AFRL0_3;

	//G6 PK1
	GPIOK->MODER   &= ~GPIO_MODER_MODER1;
	GPIOK->MODER   |= GPIO_MODER_MODER1_1;
	GPIOK->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR1_1;
	GPIOK->AFR[0] &= ~GPIO_AFRL_AFRL1_0;
	GPIOK->AFR[0] |= GPIO_AFRL_AFRL1_1 | GPIO_AFRL_AFRL1_2 | GPIO_AFRL_AFRL1_3;

	//G7 PK2
	GPIOK->MODER   &= ~GPIO_MODER_MODER2;
	GPIOK->MODER   |= GPIO_MODER_MODER2_1;
	GPIOK->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR2_1;
	GPIOK->AFR[0] &= ~GPIO_AFRL_AFRL2_0;
	GPIOK->AFR[0] |= GPIO_AFRL_AFRL2_1 | GPIO_AFRL_AFRL2_2 | GPIO_AFRL_AFRL2_3;

	//VSYNC
	GPIOI->MODER   &= ~GPIO_MODER_MODER9;
	GPIOI->MODER   |= GPIO_MODER_MODER9_1;
	GPIOI->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR9_1;
	GPIOI->AFR[1] &= ~GPIO_AFRL_AFRL1_0;
	GPIOI->AFR[1] |= GPIO_AFRL_AFRL1_1 | GPIO_AFRL_AFRL1_2 | GPIO_AFRL_AFRL1_3;

	//HSYNC
	GPIOI->MODER   &= ~GPIO_MODER_MODER10;
	GPIOI->MODER   |= GPIO_MODER_MODER10_1;
	GPIOI->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR10_1;
	GPIOI->AFR[1] &= ~GPIO_AFRL_AFRL2_0;
	GPIOI->AFR[1] |= GPIO_AFRL_AFRL2_1 | GPIO_AFRL_AFRL2_2 | GPIO_AFRL_AFRL2_3;

	//CLK
	GPIOI->MODER   &= ~GPIO_MODER_MODER14;
	GPIOI->MODER   |= GPIO_MODER_MODER14_1;
	GPIOI->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR14_1;
	GPIOI->AFR[1] &= ~GPIO_AFRL_AFRL6_0;
	GPIOI->AFR[1] |= GPIO_AFRL_AFRL6_1 | GPIO_AFRL_AFRL6_2 | GPIO_AFRL_AFRL6_3;

	//DE
	GPIOK->MODER   &= ~GPIO_MODER_MODER7;
	GPIOK->MODER   |= GPIO_MODER_MODER7_1;
	GPIOK->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR7_1;
	GPIOK->AFR[0] &= ~GPIO_AFRL_AFRL7_0;
	GPIOK->AFR[0] |= GPIO_AFRL_AFRL7_1 | GPIO_AFRL_AFRL7_2 | GPIO_AFRL_AFRL7_3;

	//LED
	GPIOK->MODER &= ~GPIO_MODER_MODER3;
	GPIOK->MODER |= GPIO_MODER_MODER3_0;
	GPIOK->BSRR |= GPIO_BSRR_BS_3;

	//ON
	GPIOI->MODER &= ~GPIO_MODER_MODER12;
	GPIOI->MODER |= GPIO_MODER_MODER12_0;
	GPIOI->BSRR |= GPIO_BSRR_BS_12;

	GPIOC->MODER &= ~GPIO_MODER_MODER7;
	GPIOC->MODER |= GPIO_MODER_MODER7_0;
	GPIOC->BSRR |= GPIO_BSRR_BS_7;

	LTDC->SSCR |= ((DISPLAY_HSYNC - 1) << 16 | (DISPLAY_VSYNC - 1));
	LTDC->BPCR |= ((DISPLAY_HSYNC+DISPLAY_HBP-1) << 16 | (DISPLAY_VSYNC+DISPLAY_VBP-1));
	LTDC->AWCR |= ((DISPLAY_WIDTH + DISPLAY_HSYNC + DISPLAY_HBP - 1) << 16 | (DISPLAY_HEIGHT + DISPLAY_VSYNC + DISPLAY_VBP - 1));
	LTDC->TWCR |= ((DISPLAY_WIDTH + DISPLAY_HSYNC + DISPLAY_HBP + DISPLAY_HFP -1)<< 16 |(DISPLAY_HEIGHT + DISPLAY_VSYNC + DISPLAY_VBP + DISPLAY_VFP - 1));
	LTDC_Layer2->WHPCR |= (((DISPLAY_WIDTH + DISPLAY_HBP + DISPLAY_HSYNC - 1) << 16) | (DISPLAY_HBP + DISPLAY_HSYNC));
	LTDC_Layer2->WVPCR |= (((DISPLAY_HEIGHT + DISPLAY_VSYNC + DISPLAY_VBP - 1) << 16) |(DISPLAY_VSYNC + DISPLAY_VBP));
	LTDC_Layer2->PFCR = 2;
	LTDC_Layer2->BFCR |= ((4 << 8) | 5);
	LTDC_Layer2->CACR = 0xff;
	LTDC_Layer2->CFBLR |= (((PIXEL_SIZE * DISPLAY_WIDTH) << 16) | (PIXEL_SIZE * DISPLAY_WIDTH + 3));
	LTDC_Layer2->CFBLNR |= DISPLAY_HEIGHT;
	LTDC_Layer2->CR |= LTDC_LxCR_LEN;
	LTDC->SRCR |= LTDC_SRCR_VBR;
	LTDC->GCR |= LTDC_GCR_LTDCEN;

	for(int i = 0; i <= 130559; ++i)
		screen[i] = main_picture[i];

	GPIOK->BSRR |= GPIO_BSRR_BS_3;
}


void change_digit_main_time(int poz, int number)
{
	int x = 0;
	int k = 0;

	if(poz==1)
		x = 0;
	if(poz==2)
		x = 38;
	if(poz==3)
		x = 86;
	if(poz==4)
		x = 124;

	if(number==10)
		for(int i = 0; i <= 57; ++i)
			for(int j = 0; j <= 32; ++j)
				screen[x+5113+i*480+j] = none_picture_3[k++];
	if(number==0)
		for(int i = 0; i <= 57; ++i)
			for(int j = 0; j <= 32; ++j)
				screen[x+5113+i*480+j] = zero_picture_1[k++];
	if(number==1)
		for(int i = 0; i <= 57; ++i)
			for(int j = 0; j <= 32; ++j)
				screen[x+5113+i*480+j] = one_picture_1[k++];
	if(number==2)
		for(int i = 0; i <= 57; ++i)
			for(int j = 0; j <= 32; ++j)
				screen[x+5113+i*480+j] = two_picture_1[k++];
	if(number==3)
		for(int i = 0; i <= 57; ++i)
			for(int j = 0; j <= 32; ++j)
				screen[x+5113+i*480+j] = three_picture_1[k++];
	if(number==4)
		for(int i = 0; i <= 57; ++i)
			for(int j = 0; j <= 32; ++j)
				screen[x+5113+i*480+j] = four_picture_1[k++];
	if(number==5)
		for(int i = 0; i <= 57; ++i)
			for(int j = 0; j <= 32; ++j)
				screen[x+5113+i*480+j] = five_picture_1[k++];
	if(number==6)
		for(int i = 0; i <= 57; ++i)
			for(int j = 0; j <= 32; ++j)
				screen[x+5113+i*480+j] = six_picture_1[k++];
	if(number==7)
		for(int i = 0; i <= 57; ++i)
			for(int j = 0; j <= 32; ++j)
				screen[x+5113+i*480+j] = seven_picture_1[k++];
	if(number==8)
		for(int i = 0; i <= 57; ++i)
			for(int j = 0; j <= 32; ++j)
				screen[x+5113+i*480+j] = eigth_picture_1[k++];
	if(number==9)
		for(int i = 0; i <= 57; ++i)
			for(int j = 0; j <= 32; ++j)
				screen[x+5113+i*480+j] = nine_picture_1[k++];
	LTDC_Layer2->CFBAR = (uint32_t)screen;
	LTDC->SRCR |= LTDC_SRCR_VBR;
}


void change_digit_set_time(int poz, int number)
{
	int x = 0;
	int k = 0;

	if(poz==1)
		x = 0;
	if(poz==2)
		x = 110;
	if(poz==3)
		x = 246;
	if(poz==4)
		x = 356;

	if(number==0)
		for(int i = 0; i <= 173; ++i)
			for(int j = 0; j <= 98; ++j)
				screen[x+42253+i*480+j] = zero_picture_2[k++];
	if(number==1)
		for(int i = 0; i <= 173; ++i)
			for(int j = 0; j <= 98; ++j)
				screen[x+42253+i*480+j] = one_picture_2[k++];
	if(number==2)
		for(int i = 0; i <= 173; ++i)
			for(int j = 0; j <= 98; ++j)
				screen[x+42253+i*480+j] = two_picture_2[k++];
	if(number==3)
		for(int i = 0; i <= 173; ++i)
			for(int j = 0; j <= 98; ++j)
				screen[x+42253+i*480+j] = three_picture_2[k++];
	if(number==4)
		for(int i = 0; i <= 173; ++i)
			for(int j = 0; j <= 98; ++j)
				screen[x+42253+i*480+j] = four_picture_2[k++];
	if(number==5)
		for(int i = 0; i <= 173; ++i)
			for(int j = 0; j <= 98; ++j)
				screen[x+42253+i*480+j] = five_picture_2[k++];
	if(number==6)
		for(int i = 0; i <= 173; ++i)
			for(int j = 0; j <= 98; ++j)
				screen[x+42253+i*480+j] = six_picture_2[k++];
	if(number==7)
		for(int i = 0; i <= 173; ++i)
			for(int j = 0; j <= 98; ++j)
				screen[x+42253+i*480+j] = seven_picture_2[k++];
	if(number==8)
		for(int i = 0; i <= 173; ++i)
			for(int j = 0; j <= 98; ++j)
				screen[x+42253+i*480+j] = eigth_picture_2[k++];
	if(number==9)
		for(int i = 0; i <= 173; ++i)
			for(int j = 0; j <= 98; ++j)
				screen[x+42253+i*480+j] = nine_picture_2[k++];
	LTDC_Layer2->CFBAR = (uint32_t)screen;
	LTDC->SRCR |= LTDC_SRCR_VBR;
}


void change_digit_power(int number)
{
	int k = 0;

	if(number==200)
	{
		for(int i = 0; i <= 57; ++i)
			for(int j = 0; j <= 32; ++j)
				screen[76+4861+i*480+j] = none_picture_3[k++];
		k = 0;
		for(int i = 0; i <= 57; ++i)
			for(int j = 0; j <= 32; ++j)
				screen[38+4861+i*480+j] = none_picture_3[k++];
		k = 0;
		for(int i = 0; i <= 57; ++i)
			for(int j = 0; j <= 32; ++j)
				screen[4861+i*480+j] = none_picture_3[k++];
	}

	if(number==0)
	{
		for(int i = 0; i <= 57; ++i)
			for(int j = 0; j <= 32; ++j)
				screen[76+4861+i*480+j] = zero_picture_3[k++];
		k = 0;
		for(int i = 0; i <= 57; ++i)
			for(int j = 0; j <= 32; ++j)
				screen[38+4861+i*480+j] = none_picture_3[k++];
		k = 0;
		for(int i = 0; i <= 57; ++i)
			for(int j = 0; j <= 32; ++j)
				screen[4861+i*480+j] = none_picture_3[k++];
	}

	if(number==10)
	{
		for(int i = 0; i <= 57; ++i)
			for(int j = 0; j <= 32; ++j)
				screen[76+4861+i*480+j] = zero_picture_3[k++];
		k = 0;
		for(int i = 0; i <= 57; ++i)
			for(int j = 0; j <= 32; ++j)
				screen[38+4861+i*480+j] = one_picture_3[k++];
		k = 0;
		for(int i = 0; i <= 57; ++i)
			for(int j = 0; j <= 32; ++j)
				screen[4861+i*480+j] = none_picture_3[k++];
	}

	if(number==20)
	{
		for(int i = 0; i <= 57; ++i)
			for(int j = 0; j <= 32; ++j)
				screen[76+4861+i*480+j] = zero_picture_3[k++];
		k = 0;
		for(int i = 0; i <= 57; ++i)
			for(int j = 0; j <= 32; ++j)
				screen[38+4861+i*480+j] = two_picture_3[k++];
		k = 0;
		for(int i = 0; i <= 57; ++i)
			for(int j = 0; j <= 32; ++j)
				screen[4861+i*480+j] = none_picture_3[k++];
	}

	if(number==30)
	{
		for(int i = 0; i <= 57; ++i)
			for(int j = 0; j <= 32; ++j)
				screen[76+4861+i*480+j] = zero_picture_3[k++];
		k = 0;
		for(int i = 0; i <= 57; ++i)
			for(int j = 0; j <= 32; ++j)
				screen[38+4861+i*480+j] = three_picture_3[k++];
		k = 0;
		for(int i = 0; i <= 57; ++i)
			for(int j = 0; j <= 32; ++j)
				screen[4861+i*480+j] = none_picture_3[k++];
	}

	if(number==40)
	{
		for(int i = 0; i <= 57; ++i)
			for(int j = 0; j <= 32; ++j)
				screen[76+4861+i*480+j] = zero_picture_3[k++];
		k = 0;
		for(int i = 0; i <= 57; ++i)
			for(int j = 0; j <= 32; ++j)
				screen[38+4861+i*480+j] = four_picture_3[k++];
		k = 0;
		for(int i = 0; i <= 57; ++i)
			for(int j = 0; j <= 32; ++j)
				screen[4861+i*480+j] = none_picture_3[k++];
	}

	if(number==50)
	{
		for(int i = 0; i <= 57; ++i)
			for(int j = 0; j <= 32; ++j)
				screen[76+4861+i*480+j] = zero_picture_3[k++];
		k = 0;
		for(int i = 0; i <= 57; ++i)
			for(int j = 0; j <= 32; ++j)
				screen[38+4861+i*480+j] = five_picture_3[k++];
		k = 0;
		for(int i = 0; i <= 57; ++i)
			for(int j = 0; j <= 32; ++j)
				screen[4861+i*480+j] = none_picture_3[k++];
	}

	if(number==60)
	{
		for(int i = 0; i <= 57; ++i)
			for(int j = 0; j <= 32; ++j)
				screen[76+4861+i*480+j] = zero_picture_3[k++];
		k = 0;
		for(int i = 0; i <= 57; ++i)
			for(int j = 0; j <= 32; ++j)
				screen[38+4861+i*480+j] = six_picture_3[k++];
		k = 0;
		for(int i = 0; i <= 57; ++i)
			for(int j = 0; j <= 32; ++j)
				screen[4861+i*480+j] = none_picture_3[k++];
	}

	if(number==70)
	{
		for(int i = 0; i <= 57; ++i)
			for(int j = 0; j <= 32; ++j)
				screen[76+4861+i*480+j] = zero_picture_3[k++];
		k = 0;
		for(int i = 0; i <= 57; ++i)
			for(int j = 0; j <= 32; ++j)
				screen[38+4861+i*480+j] = seven_picture_3[k++];
		k = 0;
		for(int i = 0; i <= 57; ++i)
			for(int j = 0; j <= 32; ++j)
				screen[4861+i*480+j] = none_picture_3[k++];
	}

	if(number==80)
	{
		for(int i = 0; i <= 57; ++i)
			for(int j = 0; j <= 32; ++j)
				screen[76+4861+i*480+j] = zero_picture_3[k++];
		k = 0;
		for(int i = 0; i <= 57; ++i)
			for(int j = 0; j <= 32; ++j)
				screen[38+4861+i*480+j] = eigth_picture_3[k++];
		k = 0;
		for(int i = 0; i <= 57; ++i)
			for(int j = 0; j <= 32; ++j)
				screen[4861+i*480+j] = none_picture_3[k++];
	}

	if(number==90)
	{
		for(int i = 0; i <= 57; ++i)
			for(int j = 0; j <= 32; ++j)
				screen[76+4861+i*480+j] = zero_picture_3[k++];
		k = 0;
		for(int i = 0; i <= 57; ++i)
			for(int j = 0; j <= 32; ++j)
				screen[38+4861+i*480+j] = nine_picture_3[k++];
		k = 0;
		for(int i = 0; i <= 57; ++i)
			for(int j = 0; j <= 32; ++j)
				screen[4861+i*480+j] = none_picture_3[k++];
	}

	if(number==100)
	{
		for(int i = 0; i <= 57; ++i)
			for(int j = 0; j <= 32; ++j)
				screen[76+4861+i*480+j] = zero_picture_3[k++];
		k = 0;
		for(int i = 0; i <= 57; ++i)
			for(int j = 0; j <= 32; ++j)
				screen[38+4861+i*480+j] = zero_picture_3[k++];
		k = 0;
		for(int i = 0; i <= 57; ++i)
			for(int j = 0; j <= 32; ++j)
				screen[4861+i*480+j] = one_picture_3[k++];
	}

	LTDC_Layer2->CFBAR = (uint32_t)screen;
	LTDC->SRCR |= LTDC_SRCR_VBR;
}


void SysTick_Handler(void)
{
	++cnt;

	if(cnt == 475000 && (flags & 0x4) != 0)
	{
		if(minute == 0)
		{
			if(minute_2 == 0)
			{
				if(hour == 0)
				{
					cnt = 819;
					hour = 9;
					minute_2 = 5;
					minute = 9;
					--hour_2;
					change_digit_set_time(1,hour_2);
					change_digit_set_time(2,9);
					change_digit_set_time(3,5);
					change_digit_set_time(4,9);
				}
				else
				{
					cnt = 683;
					minute_2 = 5;
					minute = 9;
					--hour;
					change_digit_2(2,hour);
					change_digit_2(3,5);
					change_digit_2(4,9);
				}
			}
			else
			{
				cnt = 414;
				minute = 9;
				--minute_2;
				change_digit_set_time(3,minute_2);
				change_digit_set_time(4,9);
			}
		}
		else
		{
			cnt = 136;
			--minute;
			change_digit_set_time(4,minute);
		}
	}

	if(cnt == 150000 && (flags & 0x1) != 0)
	{
		cnt = 0;
		if((flags & 0x80) == 0)
		{
			change_digit_main_time(1,10);
			change_digit_main_time(2,10);
			change_digit_main_time(3,10);
			change_digit_main_time(4,10);
			flags |= 0x80;
		}
		else
		{
			change_digit_main_time(1,hour_2);
			change_digit_main_time(2,hour);
			change_digit_main_time(3,minute_2);
			change_digit_main_time(4,minute);
			flags &= ~0x80;
		}
	}
	if(cnt == 150000 && (flags & 0x2) != 0)
	{
		cnt = 0;
		if((flags & 0x40) == 0)
		{
			change_digit_power(200);
			flags |= 0x40;
		}
		else
		{
			change_digit_power(power);
			flags &= ~0x40;
		}
	}
	if((hour_2 == 0 && hour == 0 && minute_2 == 0 && minute == 0))
	{
		flags &= ~0x4;
		TIM3->CCR1 = 0;
	}
	if(cnt == 600000)
		cnt = 0;
}


void EXTI0_IRQHandler()
{
	if((flags & 0x4) == 0)
	{
		if((flags & 0x2) != 0)
		{
			flags &= ~0x2;
			change_digit_power(power);
		}
		else
		{
			if((flags & 0x1) != 0)
			{
				flags |= 0x2;
				flags &= ~0x1;
				set_minute = minute;
				set_minute_2 = minute_2;
				set_hour = hour;
				set_hour_2 = hour_2;
				change_digit_main_time(1,set_hour_2);
				change_digit_main_time(2,set_hour);
				change_digit_main_time(3,set_minute_2);
				change_digit_main_time(4,set_minute);
			}
			else
				flags |= 0x1;
		}
	}
	for(int i = 0; i <= 1500000; ++i);
	EXTI->PR |= EXTI_PR_PR0;
}


void EXTI3_IRQHandler()
{
	if((flags & 0x4) == 0 && (flags & 0x1) == 0 && (flags & 0x2) == 0)
	{
		flags |= 0x4;
		TIM3->CCR1 = power;
		change_digit_main_time(1,set_hour_2);
		change_digit_main_time(2,set_hour);
		change_digit_main_time(3,set_minute_2);
		change_digit_main_time(4,set_minute);
		change_digit_set_time(1,hour_2);
		change_digit_set_time(2,hour);
		change_digit_set_time(3,minute_2);
		change_digit_set_time(4,minute);
	}
	else
	{
		if((flags & 0x1) == 0 && (flags & 0x2) == 0)
		{
			TIM3->CCR1 = 0;
			flags &= ~0x4;
		}
	}
	for(int i = 0; i <= 1500000; ++i);
	EXTI->PR |= EXTI_PR_PR3;
}


void EXTI4_IRQHandler()
{
	flags &= ~0x4;
	TIM3->CCR1 = 0;
	power = 0;
	minute = 0;
	minute_2 = 0;
	hour = 0;
	hour_2 = 0;
	change_digit_main_time(1,0);
	change_digit_main_time(2,0);
	change_digit_main_time(3,0);
	change_digit_main_time(4,0);
	change_digit_set_time(1,0);
	change_digit_set_time(2,0);
	change_digit_set_time(3,0);
	change_digit_set_time(4,0);
	change_digit_power(0);

	for(int i = 0; i <= 1500000; ++i);
	EXTI->PR |= EXTI_PR_PR4;
}


void EXTI9_5_IRQHandler()
{
	if((flags & 0x2) != 0 && power != 0)
		power -= 10;
	if((flags & 0x1) != 0)
	{
		if((hour_2 == 0 && hour == 0 && minute_2 == 0 && minute == 0))
			goto end_minus;
		else
		{
			if(minute_2 == 0)
			{
				if(hour == 0)
				{
					--hour_2;
					hour = 9;
				}
				else
				{
					--hour;
					minute_2 = 3;
				}
			}
			else
				minute_2 -= 3;
		}
	}
	end_minus:
	for(int i = 0; i <= 1500000; ++i);
	EXTI->PR |= EXTI_PR_PR6;
}


void EXTI15_10_IRQHandler()
{
	if((flags & 0x2) != 0 && power != 100)
		power += 10;
	if((flags & 0x1) != 0)
	{
		if(hour_2 >= 9 && hour >= 9 && minute_2 >= 3 && minute >= 0)
			goto end_plus;
		else
		{
			minute_2 += 3;
			if(minute_2 == 6)
			{
				++hour;
				minute_2 = 0;
				if(hour == 9)
				{
					++hour_2;
					hour = 0;
				}
			}
		}
	}
	end_plus:
	for(int i = 0; i <= 1500000; ++i);
	EXTI->PR |= EXTI_PR_PR15;
}


int main(void)
{
	initialization();

	// Initialization picture
	LTDC_Layer2->CFBAR = (uint32_t)init_picture;
	LTDC->SRCR |= LTDC_SRCR_VBR;
	for(int i = 0; i <= 5000000; ++i);

	// Main picture
	LTDC_Layer2->CFBAR = (uint32_t)main_picture;
	LTDC->SRCR |= LTDC_SRCR_VBR;

	// Time
	change_digit_main_time(1,hour_2);
	change_digit_main_time(2,hour);
	change_digit_main_time(3,minute_2);
	change_digit_main_time(4,minute);

	// Time 2
	change_digit_set_time(1,hour_2);
	change_digit_set_time(2,hour);
	change_digit_set_time(3,minute_2);
	change_digit_set_time(4,minute);

	// Power
	change_digit_power(power);

	// Timer
	SysTick_Config(180);

	while(1);
}
