/*********************************************************************
*                     SEGGER Microcontroller GmbH                    *
*                        The Embedded Experts                        *
**********************************************************************
*                                                                    *
*       (c) 2003 - 2021  SEGGER Microcontroller GmbH                 *
*                                                                    *
*       www.segger.com     Support: support@segger.com               *
*                                                                    *
**********************************************************************
----------------------------------------------------------------------
File    : BSP.c
Purpose : BSP for the ST STM32F429_Nucleo board
--------  END-OF-HEADER  ---------------------------------------------
*/

#include "BSP.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define RCC_BASE_ADDR    (0x40023800u)
#define RCC_AHBENR       (*(volatile unsigned int*)(RCC_BASE_ADDR + 0x30u))
#define RCC_LEDPORT_BIT  (1)

#define GPIOB_BASE_ADDR  (0x40020400u)
#define GPIOB_MODER      (*(volatile unsigned int*)(GPIOB_BASE_ADDR + 0x00u))
#define GPIOB_ODR        (*(volatile unsigned int*)(GPIOB_BASE_ADDR + 0x14u))

#define LED0_BIT         ( 0)  // LED1 (green) - PB0
#define LED1_BIT         ( 7)  // LED2 (blue)  - PB7
#define LED2_BIT         (14)  // LED4 (red)   - PB14

/*********************************************************************
*
*       Global functions
*
**********************************************************************
*/

/*********************************************************************
*
*       BSP_Init()
*/
void BSP_Init(void) {
  //
  // Initialize port for LEDs (sample application)
  //
  RCC_AHBENR  |=  (1u << RCC_LEDPORT_BIT);

  GPIOB_MODER &= ~(3u << (LED0_BIT * 2));  // Reset mode; sets port to input
  GPIOB_MODER |=  (1u << (LED0_BIT * 2));  // Set to output mode
  GPIOB_ODR   &= ~(1u <<  LED0_BIT);       // Initially clear LEDs

  GPIOB_MODER &= ~(3u << (LED1_BIT * 2));  // Reset mode; sets port to input
  GPIOB_MODER |=  (1u << (LED1_BIT * 2));  // Set to output mode
  GPIOB_ODR   &= ~(1u <<  LED1_BIT);       // Initially clear LEDs

  GPIOB_MODER &= ~(3u << (LED2_BIT * 2));  // Reset mode; sets port to input
  GPIOB_MODER |=  (1u << (LED2_BIT * 2));  // Set to output mode
  GPIOB_ODR   &= ~(1u <<  LED2_BIT);       // Initially clear LEDs

  //
  // Initialize pins for MAC address
  //

  // Init clock on GPIO
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);


  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;

  // Initialize PA3 & PC0 as output
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  // Activate Output
  GPIO_WriteBit(GPIOA,GPIO_Pin_3,1);
  GPIO_WriteBit(GPIOC,GPIO_Pin_0,1);

  // Initialize PD6 & PD7 as input
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
}

/*********************************************************************
*
*       BSP_SetLED()
*/
void BSP_SetLED(int Index) {
  if (Index == 0) {
    GPIOB_ODR |= (1u << LED0_BIT);         // Switch on LED0
  } else if (Index == 1) {
    GPIOB_ODR |= (1u << LED1_BIT);         // Switch on LED1
  } else if (Index == 2) {
    GPIOB_ODR |= (1u << LED2_BIT);         // Switch on LED2
  }
}

/*********************************************************************
*
*       BSP_ClrLED()
*/
void BSP_ClrLED(int Index) {
  if (Index == 0) {
    GPIOB_ODR &= ~(1u << LED0_BIT);        // Switch off LED0
  } else if (Index == 1) {
    GPIOB_ODR &= ~(1u << LED1_BIT);        // Switch off LED1
  } else if (Index == 2) {
    GPIOB_ODR &= ~(1u << LED2_BIT);        // Switch off LED2
  }
}

/*********************************************************************
*
*       BSP_ToggleLED()
*/
void BSP_ToggleLED(int Index) {
  if (Index == 0) {
    GPIOB_ODR ^= (1u << LED0_BIT);         // Toggle LED0
  } else if (Index == 1) {
    GPIOB_ODR ^= (1u << LED1_BIT);         // Toggle LED1
  } else if (Index == 2) {
    GPIOB_ODR ^= (1u << LED2_BIT);         // Toggle LED2
  }
}

/****** End Of File *************************************************/

