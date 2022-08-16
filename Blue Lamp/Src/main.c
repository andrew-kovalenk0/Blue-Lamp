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
#include "colon_on.h"
#include "colon_off.h"
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
uint8_t power = 0;
uint8_t set_power = 0;
uint8_t predifined_parametrs = 0;


/*
	STATUS WORD:
	1 - Change of set minute enable
	2 - Change of set hour enable
	4 - Change of set power enable
	8 - Ligth is ON
	10 - Change of set hour ON
	20 - Change of set minute ON
	40 - Change of set power ON
	80 - Сolon main time ON
*/


void initialization()
{
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
	RCC->APB2ENR |= RCC_APB2ENR_LTDCEN;
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOEEN;
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOJEN;
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOGEN;
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOKEN;
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOIEN;
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
	RCC->APB1ENR |= RCC_APB1ENR_TIM5EN;
	RCC->APB1ENR |= RCC_APB1ENR_PWREN;

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

	// Timers
	TIM2->PSC = 1080;
	TIM2->ARR = 6000000;
	TIM2->DIER |= TIM_DIER_UIE;
	NVIC_SetPriority(TIM2_IRQn, 0);
	NVIC_EnableIRQ(TIM2_IRQn);
	TIM2->CR1 |= TIM_CR1_CEN;

	TIM5->PSC = 1080000;
	TIM5->ARR = 1;
	TIM5->DIER |= TIM_DIER_UIE;
	NVIC_SetPriority(TIM5_IRQn, 15);
	NVIC_EnableIRQ(TIM5_IRQn);

	// EXTI
	SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI0_PI | SYSCFG_EXTICR1_EXTI1_PI | SYSCFG_EXTICR1_EXTI3_PI;
	SYSCFG->EXTICR[1] |= SYSCFG_EXTICR2_EXTI4_PB | SYSCFG_EXTICR2_EXTI6_PG;
	SYSCFG->EXTICR[3] |= SYSCFG_EXTICR4_EXTI15_PB;
	EXTI->IMR |= EXTI_IMR_MR0 | EXTI_IMR_MR1 | EXTI_IMR_MR3 | EXTI_IMR_MR4 | EXTI_IMR_MR6 | EXTI_IMR_MR15;
	EXTI->FTSR |= EXTI_FTSR_TR0 | EXTI_FTSR_TR1 | EXTI_FTSR_TR3 | EXTI_FTSR_TR4 | EXTI_FTSR_TR6 | EXTI_FTSR_TR15;
	NVIC_SetPriority(EXTI0_IRQn, 15);
	NVIC_SetPriority(EXTI1_IRQn, 15);
	NVIC_SetPriority(EXTI3_IRQn, 15);
	NVIC_SetPriority(EXTI4_IRQn, 15);
	NVIC_SetPriority(EXTI9_5_IRQn, 15);
	NVIC_SetPriority(EXTI15_10_IRQn, 15);
	NVIC_EnableIRQ(EXTI0_IRQn);
	NVIC_EnableIRQ(EXTI1_IRQn);
	NVIC_EnableIRQ(EXTI3_IRQn);
	NVIC_EnableIRQ(EXTI4_IRQn);
	NVIC_EnableIRQ(EXTI9_5_IRQn);
	NVIC_EnableIRQ(EXTI15_10_IRQn);
	__enable_irq ();

	//Cooler
	GPIOC->MODER |= GPIO_MODER_MODER7_0;
	GPIOC->BSRR |= GPIO_BSRR_BR_7;

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


void change_digit_set_time(int pozition, int digit)
{
	int x = 0;
	int k = 0;

	if(pozition==1)
		x = 0;
	if(pozition==2)
		x = 38;
	if(pozition==3)
		x = 86;
	if(pozition==4)
		x = 124;

	if(digit==10)
		for(int i = 0; i <= 57; ++i)
			for(int j = 0; j <= 32; ++j)
				screen[x+5113+i*480+j] = none_picture_3[k++];
	if(digit==0)
		for(int i = 0; i <= 57; ++i)
			for(int j = 0; j <= 32; ++j)
				screen[x+5113+i*480+j] = zero_picture_1[k++];
	if(digit==1)
		for(int i = 0; i <= 57; ++i)
			for(int j = 0; j <= 32; ++j)
				screen[x+5113+i*480+j] = one_picture_1[k++];
	if(digit==2)
		for(int i = 0; i <= 57; ++i)
			for(int j = 0; j <= 32; ++j)
				screen[x+5113+i*480+j] = two_picture_1[k++];
	if(digit==3)
		for(int i = 0; i <= 57; ++i)
			for(int j = 0; j <= 32; ++j)
				screen[x+5113+i*480+j] = three_picture_1[k++];
	if(digit==4)
		for(int i = 0; i <= 57; ++i)
			for(int j = 0; j <= 32; ++j)
				screen[x+5113+i*480+j] = four_picture_1[k++];
	if(digit==5)
		for(int i = 0; i <= 57; ++i)
			for(int j = 0; j <= 32; ++j)
				screen[x+5113+i*480+j] = five_picture_1[k++];
	if(digit==6)
		for(int i = 0; i <= 57; ++i)
			for(int j = 0; j <= 32; ++j)
				screen[x+5113+i*480+j] = six_picture_1[k++];
	if(digit==7)
		for(int i = 0; i <= 57; ++i)
			for(int j = 0; j <= 32; ++j)
				screen[x+5113+i*480+j] = seven_picture_1[k++];
	if(digit==8)
		for(int i = 0; i <= 57; ++i)
			for(int j = 0; j <= 32; ++j)
				screen[x+5113+i*480+j] = eigth_picture_1[k++];
	if(digit==9)
		for(int i = 0; i <= 57; ++i)
			for(int j = 0; j <= 32; ++j)
				screen[x+5113+i*480+j] = nine_picture_1[k++];
	LTDC_Layer2->CFBAR = (uint32_t)screen;
	LTDC->SRCR |= LTDC_SRCR_VBR;
}


void change_digit_main_time(int pozition, int digit)
{
	int x = 0;
	int k = 0;

	if(pozition==1)
		x = 0;
	if(pozition==2)
		x = 110;
	if(pozition==3)
		x = 246;
	if(pozition==4)
		x = 356;

	if(digit==0)
		for(int i = 0; i <= 173; ++i)
			for(int j = 0; j <= 98; ++j)
				screen[x+42253+i*480+j] = zero_picture_2[k++];
	if(digit==1)
		for(int i = 0; i <= 173; ++i)
			for(int j = 0; j <= 98; ++j)
				screen[x+42253+i*480+j] = one_picture_2[k++];
	if(digit==2)
		for(int i = 0; i <= 173; ++i)
			for(int j = 0; j <= 98; ++j)
				screen[x+42253+i*480+j] = two_picture_2[k++];
	if(digit==3)
		for(int i = 0; i <= 173; ++i)
			for(int j = 0; j <= 98; ++j)
				screen[x+42253+i*480+j] = three_picture_2[k++];
	if(digit==4)
		for(int i = 0; i <= 173; ++i)
			for(int j = 0; j <= 98; ++j)
				screen[x+42253+i*480+j] = four_picture_2[k++];
	if(digit==5)
		for(int i = 0; i <= 173; ++i)
			for(int j = 0; j <= 98; ++j)
				screen[x+42253+i*480+j] = five_picture_2[k++];
	if(digit==6)
		for(int i = 0; i <= 173; ++i)
			for(int j = 0; j <= 98; ++j)
				screen[x+42253+i*480+j] = six_picture_2[k++];
	if(digit==7)
		for(int i = 0; i <= 173; ++i)
			for(int j = 0; j <= 98; ++j)
				screen[x+42253+i*480+j] = seven_picture_2[k++];
	if(digit==8)
		for(int i = 0; i <= 173; ++i)
			for(int j = 0; j <= 98; ++j)
				screen[x+42253+i*480+j] = eigth_picture_2[k++];
	if(digit==9)
		for(int i = 0; i <= 173; ++i)
			for(int j = 0; j <= 98; ++j)
				screen[x+42253+i*480+j] = nine_picture_2[k++];
	LTDC_Layer2->CFBAR = (uint32_t)screen;
	LTDC->SRCR |= LTDC_SRCR_VBR;
}


void change_digit_power(int power)
{
	int k = 0;

	if(power==200)
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

	if(power==0)
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

	if(power==10)
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

	if(power==20)
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

	if(power==30)
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

	if(power==40)
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

	if(power==50)
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

	if(power==60)
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

	if(power==70)
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

	if(power==80)
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

	if(power==90)
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

	if(power==100)
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


void change_colon_visibility(int position)
{
	int k = 0;
	if(position != 0)
		for(int i = 0; i <= 89; ++i)
			for(int j = 0; j <= 14; ++j)
				screen[233+62400+i*480+j] = colon_on[k++];
	else
		for(int i = 0; i <= 89; ++i)
			for(int j = 0; j <= 14; ++j)
				screen[233+62400+i*480+j] = colon_off[k++];

	LTDC_Layer2->CFBAR = (uint32_t)screen;
	LTDC->SRCR |= LTDC_SRCR_VBR;
}


void TIM2_IRQHandler(void)
{
	if((flags & 0x8) != 0)
	{
		if(minute == 0)
		{
			if(minute_2 == 0)
			{
				if(hour == 0)
				{
					hour = 9;
					minute_2 = 5;
					minute = 9;
					--hour_2;
					change_digit_main_time(1, hour_2);
					change_digit_main_time(2, hour);
					change_digit_main_time(3, minute_2);
					change_digit_main_time(4, minute);
				}
				else
				{
					minute_2 = 5;
					minute = 9;
					--hour;
					change_digit_main_time(2, hour);
					change_digit_main_time(3, minute_2);
					change_digit_main_time(4, minute);
				}
			}
			else
			{
				minute = 9;
				--minute_2;
				change_digit_main_time(3, minute_2);
				change_digit_main_time(4, minute);
			}
		}
		else
		{
			--minute;
			change_digit_main_time(4, minute);
		}
	}
	TIM2->SR &= ~TIM_SR_UIF;
}


void TIM5_IRQHandler(void)
{
	++cnt;

	if(cnt == 100000 && (flags & 0x1) != 0)
	{
		cnt = 0;
		if((flags & 0x20) == 0)
		{
			change_digit_set_time(3, 10);
			change_digit_set_time(4, 10);
			flags |= 0x20;
		}
		else
		{
			change_digit_set_time(3, set_minute_2);
			change_digit_set_time(4, set_minute);
			flags &= ~0x20;
		}
	}

	if(cnt == 100000 && (flags & 0x2) != 0)
	{
		cnt = 0;
		if((flags & 0x10) == 0)
		{
			change_digit_set_time(1, 10);
			change_digit_set_time(2, 10);
			flags |= 0x10;
		}
		else
		{
			change_digit_set_time(1, set_hour_2);
			change_digit_set_time(2, set_hour);
			flags &= ~0x10;
		}
	}

	if(cnt == 100000 && (flags & 0x4) != 0)
	{
		cnt = 0;
		if((flags & 0x40) == 0)
		{
			change_digit_power(200);
			flags |= 0x40;
		}
		else
		{
			change_digit_power(set_power);
			flags &= ~0x40;
		}
	}

	if(cnt == 150000 && (flags & 0x8) != 0)
	{
		if((flags & 0x80) != 0)
		{
			flags &= ~0x80;
			change_colon_visibility(0);
		}
		else
		{
			flags |= 0x80;
			change_colon_visibility(1);
		}

	}

	if((hour_2 == 0 && hour == 0 && minute_2 == 0 && minute == 0) && (flags & 0x8) != 0)
	{
		NVIC_EnableIRQ(EXTI0_IRQn);
		NVIC_EnableIRQ(EXTI1_IRQn);
		NVIC_EnableIRQ(EXTI9_5_IRQn);
		NVIC_EnableIRQ(EXTI15_10_IRQn);
		flags &= ~0x1;
		flags &= ~0x2;
		flags &= ~0x4;
		flags &= ~0x8;
		TIM3->CCR1 = 0;
		TIM2->CR1 &= ~TIM_CR1_CEN;
		TIM2->CNT = 0;
	}

	if((GPIOG->IDR & 0x40) == 0 && (GPIOB->IDR & 0x8000) == 0)
		GPIOC->BSRR |= GPIO_BSRR_BS_7;
	else
		GPIOC->BSRR |= GPIO_BSRR_BR_7;

	if(cnt == 150001)
		cnt = 0;
}


void EXTI0_IRQHandler()
{
	uint8_t press_cnt = 0;
	for(int i = 0; i < 10; ++i)
	{
		if((GPIOI->IDR & 0x1) != 0)
		{
			++press_cnt;
			for(int i = 0; i <= 360000; ++i);
		}
	}

	if(((flags & 0x8) == 0) & (press_cnt == 10))
	{
		switch (flags & 0b00000111)
		{
		case 0:
		{
			flags |= 0x1;
			change_digit_set_time(3, set_minute_2);
			change_digit_set_time(4, set_minute);
		}
			break;
		case 1:
		{
			flags |= 0x2;
			flags &= ~0x1;
			change_digit_set_time(3, set_minute_2);
			change_digit_set_time(4, set_minute);
			minute = set_minute;
			minute_2 = set_minute_2;
			change_digit_set_time(1, set_hour_2);
			change_digit_set_time(2, set_hour);
		}
			break;
		case 2:
		{
			flags |= 0x4;
			flags &= ~0x2;
			change_digit_set_time(1, set_hour_2);
			change_digit_set_time(2, set_hour);
			hour = set_hour;
			hour_2 = set_hour_2;
			change_digit_power(set_power);
		}
			break;
		default:
		{
			flags &= ~0x1;
			flags &= ~0x2;
			flags &= ~0x4;
			power = set_power;
			change_digit_power(set_power);
		}
			break;
		}
	}
	for(int i = 0; i <= 3600000; ++i);
	EXTI->PR |= EXTI_PR_PR0;
}


void EXTI1_IRQHandler()
{
	uint8_t press_cnt = 0;
	for(int i = 0; i < 10; ++i)
	{
		if((GPIOI->IDR & 0x2) != 0)
		{
			++press_cnt;
			for(int i = 0; i <= 360000; ++i);
		}
	}

	if((flags & 0x8) == 0 && (flags & 0x1) == 0 && (flags & 0x2) == 0  && (flags & 0x4) == 0 && press_cnt == 10)
	{	
		if(predifined_parametrs == 3)
			predifined_parametrs = 0;
		if(predifined_parametrs == 2)
		{
			predifined_parametrs = 3;
			set_hour_2 = 0;
			set_hour = 0;
			set_minute_2 = 0;
			set_minute = 0;
			set_power = 0;
			hour_2 = set_hour_2;
			hour = set_hour;
			minute_2 = set_minute_2;
			minute = set_minute;
			power = set_power;
			change_digit_set_time(1, set_hour_2);
			change_digit_set_time(2, set_hour);
			change_digit_set_time(3, set_minute_2);
			change_digit_set_time(4, set_minute);
			change_digit_power(set_power);
		}
		if(predifined_parametrs == 1)
		{
			predifined_parametrs = 2;
			set_hour_2 = 2;
			set_hour = 5;
			set_minute_2 = 3;
			set_minute = 0;
			set_power = 100;
			hour_2 = set_hour_2;
			hour = set_hour;
			minute_2 = set_minute_2;
			minute = set_minute;
			power = set_power;
			change_digit_set_time(1, set_hour_2);
			change_digit_set_time(2, set_hour);
			change_digit_set_time(3, set_minute_2);
			change_digit_set_time(4, set_minute);
			change_digit_power(set_power);
		}
		if(predifined_parametrs == 0)
		{
			predifined_parametrs = 1;
			set_hour_2 = 0;
			set_hour = 1;
			set_minute_2 = 0;
			set_minute = 0;
			set_power = 50;
			hour_2 = set_hour_2;
			hour = set_hour;
			minute_2 = set_minute_2;
			minute = set_minute;
			power = set_power;
			change_digit_set_time(1, set_hour_2);
			change_digit_set_time(2, set_hour);
			change_digit_set_time(3, set_minute_2);
			change_digit_set_time(4, set_minute);
			change_digit_power(set_power);
		}
	}

	for(int i = 0; i <= 3600000; ++i);
	EXTI->PR |= EXTI_PR_PR1;
}


void EXTI3_IRQHandler()
{
	uint8_t press_cnt = 0;
	for(int i = 0; i < 10; ++i)
	{
		if((GPIOI->IDR & 0x8) != 0)
		{
			++press_cnt;
			for(int i = 0; i <= 360000; ++i);
		}
	}

	if((flags & 0x8) == 0 && (flags & 0x1) == 0 && (flags & 0x2) == 0  && (flags & 0x4) == 0 && press_cnt == 10)
	{
		flags |= 0x8;
		flags |= 0x80;
		TIM3->CCR1 = power;
		TIM2->CNT = 0;
		TIM2->CR1 |= TIM_CR1_CEN;
		change_digit_main_time(1, hour_2);
		change_digit_main_time(2, hour);
		change_digit_main_time(3, minute_2);
		change_digit_main_time(4, minute);
		predifined_parametrs = 0;
		NVIC_DisableIRQ(EXTI0_IRQn);
		NVIC_DisableIRQ(EXTI1_IRQn);
		NVIC_DisableIRQ(EXTI9_5_IRQn);
		NVIC_DisableIRQ(EXTI15_10_IRQn);
	}
	else
	{
		TIM2->CR1 &= ~TIM_CR1_CEN;
		TIM2->CNT = 0;
		TIM3->CCR1 = 0;
		flags &= ~0x8;
		flags &= ~0x80;
		change_colon_visibility(1);
	}

	for(int i = 0; i <= 3600000; ++i);
	EXTI->PR |= EXTI_PR_PR3;
}


void EXTI4_IRQHandler()
{
	uint8_t press_cnt = 0;
	for(int i = 0; i < 10; ++i)
	{
		if((GPIOB->IDR & 0x10) != 0)
		{
			++press_cnt;
			for(int i = 0; i <= 360000; ++i);
		}
	}
	if(press_cnt == 10)
	{
		flags &= ~0x1;
		flags &= ~0x2;
		flags &= ~0x4;
		flags &= ~0x8;
		flags &= ~0x80;
		TIM3->CCR1 = 0;
		TIM2->CR1 &= ~TIM_CR1_CEN;
		TIM2->CNT = 0;
		power = 0;
		minute = 0;
		minute_2 = 0;
		hour = 0;
		hour_2 = 0;
		set_power = 0;
		set_minute = 0;
		set_minute_2 = 0;
		set_hour = 0;
		set_hour_2 = 0;
		set_power = 0;
		change_digit_main_time(1, 0);
		change_digit_main_time(2, 0);
		change_digit_main_time(3, 0);
		change_digit_main_time(4, 0);
		change_digit_set_time(1, 0);
		change_digit_set_time(2, 0);
		change_digit_set_time(3, 0);
		change_digit_set_time(4, 0);
		change_digit_power(0);
		change_colon_visibility(1);
		NVIC_EnableIRQ(EXTI0_IRQn);
		NVIC_EnableIRQ(EXTI1_IRQn);
		NVIC_EnableIRQ(EXTI9_5_IRQn);
		NVIC_EnableIRQ(EXTI15_10_IRQn);
	}

	for(int i = 0; i <= 3600000; ++i);
	EXTI->PR |= EXTI_PR_PR4;
}


void EXTI9_5_IRQHandler()
{
	uint8_t press_cnt = 0;
	for(int i = 0; i < 10; ++i)
	{
		if((GPIOG->IDR & 0x40) != 0)
		{
			++press_cnt;
			for(int i = 0; i <= 360000; ++i);
		}
	}
	if(press_cnt == 10)
	{
		if((flags & 0x2) != 0 && (set_hour_2 + set_hour) != 0)
		{
			if(set_hour == 0)
			{
				--set_hour_2;
				set_hour = 9;
			}
			else
			{
				if(set_hour == 0)
				{
					if(set_hour_2 != 0)
					{
						--set_hour_2;
						set_hour = 9;
					}
					else
					{
						set_hour = 0;
						set_hour_2 = 0;
					}
				}
				--set_hour;
			}
		}
		if((flags & 0x1) != 0 && (set_minute + set_minute_2) != 0)
		{
			if(set_minute == 0)
			{
				--set_minute_2;
				set_minute = 9;
			}
			else
			{
				if(set_minute == 0)
				{
					if(set_minute_2 != 0)
					{
						--set_minute_2;
						set_minute = 9;
					}
					else
					{
						set_minute = 0;
						set_minute_2 = 0;
					}
				}
				--set_minute;
			}
		}
		if((flags & 0x4) != 0 && set_power != 0)
			set_power -= 10;
	}

	for(int i = 0; i <= 3600000; ++i);
	EXTI->PR |= EXTI_PR_PR6;
}


void EXTI15_10_IRQHandler()
{
	uint8_t press_cnt = 0;
	for(int i = 0; i < 10; ++i)
	{
		if((GPIOB->IDR & 0x8000) != 0)
		{
			++press_cnt;
			for(int i = 0; i <= 360000; ++i);
		}
	}

	if(press_cnt == 10)
	{
		if((flags & 0x2) != 0 && (set_hour_2 + set_hour) != 18)
		{
			++set_hour;
			if(set_hour == 10)
			{
				++set_hour_2;
				set_hour = 0;
			}
		}
		if((flags & 0x1) != 0 && (set_minute + set_minute_2) != 18)
		{
			++set_minute;
			if(set_minute == 10)
			{
				++set_minute_2;
				set_minute = 0;
			}
		}
		if((flags & 0x4) != 0 && set_power != 100)
			set_power += 10;
	}

	for(int i = 0; i <= 3600000; ++i);
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
	change_digit_main_time(1, hour_2);
	change_digit_main_time(2, hour);
	change_digit_main_time(3, minute_2);
	change_digit_main_time(4, minute);

	// Time 2
	change_digit_set_time(1, hour_2);
	change_digit_set_time(2, hour);
	change_digit_set_time(3, minute_2);
	change_digit_set_time(4, minute);

	// Power
	change_digit_power(power);

	// Timers
	TIM5->CR1 |= TIM_CR1_CEN;
	TIM2->CR1 &= ~TIM_CR1_CEN;
	TIM2->CNT = 0;

	flags &= ~0x1;
	flags &= ~0x2;
	flags &= ~0x4;
	flags &= ~0x8;
	flags &= ~0x80;
	TIM3->CCR1 = 0;
	TIM2->CR1 &= ~TIM_CR1_CEN;
	TIM2->CNT = 0;
	power = 0;
	minute = 0;
	minute_2 = 0;
	hour = 0;
	hour_2 = 0;
	set_power = 0;
	set_minute = 0;
	set_minute_2 = 0;
	set_hour = 0;
	set_hour_2 = 0;
	set_power = 0;
	change_digit_main_time(1, 0);
	change_digit_main_time(2, 0);
	change_digit_main_time(3, 0);
	change_digit_main_time(4, 0);
	change_digit_set_time(1, 0);
	change_digit_set_time(2, 0);
	change_digit_set_time(3, 0);
	change_digit_set_time(4, 0);
	change_digit_power(0);
	change_colon_visibility(1);
	NVIC_EnableIRQ(EXTI0_IRQn);
	NVIC_EnableIRQ(EXTI1_IRQn);
	NVIC_EnableIRQ(EXTI9_5_IRQn);
	NVIC_EnableIRQ(EXTI15_10_IRQn);

	while(1);
}
