/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdint.h>
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
extern uint8_t CDC_Transmit_FS(uint8_t* Buf, uint16_t Len);
void pwm1_setvalue(uint16_t value);
void pwm2_setvalue(uint16_t value);
void pwm3_setvalue(uint16_t value);

void SetMyDay(uint8_t date);
void SetMyMonth(uint8_t month);
void SetMyYear(uint8_t year);
void SetMyHours(uint8_t val);
void SetMyMinutes(uint8_t val);
void SetMySeconds(uint8_t val);
void Interrupt_EncoderChange();
void Interrupt_ButtonPressed();
void Interrupt_USART3(uint8_t *pData, uint16_t size);

void Menu_Red_Callback(int param);
void Menu_Green_Callback(int param);
void Menu_Blue_Callback(int param);
void Menu_SetHours_Callback(int param);
void Menu_SetMinutes_Callback(int param);
void Menu_SetSeconds_Callback(int param);
void Menu_SetDay_Callback(int param);
void Menu_SetMonth_Callback(int param);
void Menu_SetYear_Callback(int param);
void Menu_StoreSettings_Callback(int param);
void Menu_LoadSettings_Callback(int param);

void LCD_DrawSpectrum(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t brightness);

uint8_t logaritmize(uint16_t* inputValue, uint16_t inputMax, uint16_t outputMax);


/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define MY_BTN_Pin GPIO_PIN_1
#define MY_BTN_GPIO_Port GPIOA
#define MY_BTN_EXTI_IRQn EXTI1_IRQn
#define MY_LED_Pin GPIO_PIN_2
#define MY_LED_GPIO_Port GPIOA
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
