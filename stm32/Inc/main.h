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
#include "stm32g4xx_hal.h"
#include "stm32g4xx_ll_pwr.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define APB1_TIMER_CLOCK_FREQUENCY 160000000
#define CONSOLE_BAUDERATE 115200
#define SCHEDULER_TIM_PRESCALER APB1_TIMER_CLOCK_FREQUENCY/1000000
#define SCHEDULER_TIM_COUNTER_PERIOD 100
#define ADC_SAMPLING_TIM_PRESCALER 68
#define ADC_SAMPLING_TIM_COUNTER_PERIOD 100
#define MOTOR_PWM_TIM_PRESCALER APB1_TIMER_CLOCK_FREQUENCY/100000
#define MOTOR_PWM_TIM_COUNTER_PERIOD 100
#define MOTOR_ACCEL_TIM_PRESCALER APB1_TIMER_CLOCK_FREQUENCY/100000
#define MOTOR_ACCEL_TIM_COUNTER_PERIOD 10
#define MOTOR_ENCODER_TIM_PRESCALER 2
#define MOTOR_ENCODER_TIM_COUNTER_PERIOD 65534
#define TEC_TIM_PRESCALER 79
#define TEC_TIM_COUNTER_PERIOD 65534
#define FAIL 0
#define OK 1
#define FALSE 0
#define TRUE 1
#define SPI4_CS_M1_Pin GPIO_PIN_3
#define SPI4_CS_M1_GPIO_Port GPIOE
#define SPI4_CS_M2_Pin GPIO_PIN_4
#define SPI4_CS_M2_GPIO_Port GPIOE
#define STALLn_BEMFVn_M1_Pin GPIO_PIN_13
#define STALLn_BEMFVn_M1_GPIO_Port GPIOC
#define FAULTn_M1_Pin GPIO_PIN_14
#define FAULTn_M1_GPIO_Port GPIOC
#define STALLn_BEMFVn_M2_Pin GPIO_PIN_15
#define STALLn_BEMFVn_M2_GPIO_Port GPIOC
#define FAULTn_M2_Pin GPIO_PIN_9
#define FAULTn_M2_GPIO_Port GPIOF
#define RESET_M1_Pin GPIO_PIN_10
#define RESET_M1_GPIO_Port GPIOF
#define AIN1_STEP_M1_Pin GPIO_PIN_0
#define AIN1_STEP_M1_GPIO_Port GPIOC
#define AIN2_DIR_M1_Pin GPIO_PIN_1
#define AIN2_DIR_M1_GPIO_Port GPIOC
#define BIN1_M1_Pin GPIO_PIN_2
#define BIN1_M1_GPIO_Port GPIOC
#define BIN2_M1_Pin GPIO_PIN_3
#define BIN2_M1_GPIO_Port GPIOC
#define RESET_M2_Pin GPIO_PIN_2
#define RESET_M2_GPIO_Port GPIOF
#define AIN1_STEP_M2_Pin GPIO_PIN_0
#define AIN1_STEP_M2_GPIO_Port GPIOA
#define AIN2_DIR_M2_Pin GPIO_PIN_1
#define AIN2_DIR_M2_GPIO_Port GPIOA
#define BIN1_M2_Pin GPIO_PIN_2
#define BIN1_M2_GPIO_Port GPIOA
#define BIN2_M2_Pin GPIO_PIN_3
#define BIN2_M2_GPIO_Port GPIOA
#define SENS_24V_Pin GPIO_PIN_4
#define SENS_24V_GPIO_Port GPIOA
#define TEMP_M1_Pin GPIO_PIN_5
#define TEMP_M1_GPIO_Port GPIOA
#define CURRENT_A_M1_Pin GPIO_PIN_6
#define CURRENT_A_M1_GPIO_Port GPIOA
#define CURRENT_B_M1_Pin GPIO_PIN_7
#define CURRENT_B_M1_GPIO_Port GPIOA
#define TEMP_M2_Pin GPIO_PIN_4
#define TEMP_M2_GPIO_Port GPIOC
#define SENSOR_IR_1_Pin GPIO_PIN_5
#define SENSOR_IR_1_GPIO_Port GPIOC
#define SENSOR_IR_1_EXTI_IRQn EXTI9_5_IRQn
#define CURRENT_A_M2_Pin GPIO_PIN_0
#define CURRENT_A_M2_GPIO_Port GPIOB
#define CURRENT_B_M2_Pin GPIO_PIN_1
#define CURRENT_B_M2_GPIO_Port GPIOB
#define ENC_2_A_Pin GPIO_PIN_2
#define ENC_2_A_GPIO_Port GPIOB
#define SWITCH_1_Pin GPIO_PIN_7
#define SWITCH_1_GPIO_Port GPIOE
#define SWITCH_1_EXTI_IRQn EXTI9_5_IRQn
#define SWITCH_2_Pin GPIO_PIN_8
#define SWITCH_2_GPIO_Port GPIOE
#define SWITCH_2_EXTI_IRQn EXTI9_5_IRQn
#define SENSOR_IR_1_EN_Pin GPIO_PIN_9
#define SENSOR_IR_1_EN_GPIO_Port GPIOE
#define SENSOR_IR_2_Pin GPIO_PIN_10
#define SENSOR_IR_2_GPIO_Port GPIOE
#define SENSOR_IR_2_EXTI_IRQn EXTI15_10_IRQn
#define SWITCH_4_Pin GPIO_PIN_12
#define SWITCH_4_GPIO_Port GPIOE
#define SWITCH_4_EXTI_IRQn EXTI15_10_IRQn
#define SENSOR_IR_1_LED_Pin GPIO_PIN_13
#define SENSOR_IR_1_LED_GPIO_Port GPIOE
#define SENSOR_IR_2_LED_Pin GPIO_PIN_14
#define SENSOR_IR_2_LED_GPIO_Port GPIOE
#define SENSOR_IR_2_EN_Pin GPIO_PIN_15
#define SENSOR_IR_2_EN_GPIO_Port GPIOE
#define SWITCH_3_Pin GPIO_PIN_10
#define SWITCH_3_GPIO_Port GPIOB
#define SILENT_MODE_CAN2_Pin GPIO_PIN_11
#define SILENT_MODE_CAN2_GPIO_Port GPIOB
#define SW_ADDR_8_Pin GPIO_PIN_14
#define SW_ADDR_8_GPIO_Port GPIOB
#define SW_ADDR_7_Pin GPIO_PIN_15
#define SW_ADDR_7_GPIO_Port GPIOB
#define SW_ADDR_6_Pin GPIO_PIN_8
#define SW_ADDR_6_GPIO_Port GPIOD
#define SW_ADDR_5_Pin GPIO_PIN_9
#define SW_ADDR_5_GPIO_Port GPIOD
#define SW_ADDR_4_Pin GPIO_PIN_10
#define SW_ADDR_4_GPIO_Port GPIOD
#define END_STOP_M2_Pin GPIO_PIN_11
#define END_STOP_M2_GPIO_Port GPIOD
#define END_STOP_M2_EXTI_IRQn EXTI15_10_IRQn
#define SW_ADDR_3_Pin GPIO_PIN_12
#define SW_ADDR_3_GPIO_Port GPIOD
#define SW_ADDR_2_Pin GPIO_PIN_13
#define SW_ADDR_2_GPIO_Port GPIOD
#define SW_ADDR_1_Pin GPIO_PIN_14
#define SW_ADDR_1_GPIO_Port GPIOD
#define POWER_GOOD_3V3_Pin GPIO_PIN_15
#define POWER_GOOD_3V3_GPIO_Port GPIOA
#define ENC_2_B_Pin GPIO_PIN_12
#define ENC_2_B_GPIO_Port GPIOC
#define SILENT_MODE_CAN1_Pin GPIO_PIN_2
#define SILENT_MODE_CAN1_GPIO_Port GPIOD
#define LED_2_RED_Pin GPIO_PIN_3
#define LED_2_RED_GPIO_Port GPIOD
#define LED_2_GREEN_Pin GPIO_PIN_4
#define LED_2_GREEN_GPIO_Port GPIOD
#define LED_1_RED_Pin GPIO_PIN_5
#define LED_1_RED_GPIO_Port GPIOD
#define LED_1_GREEN_Pin GPIO_PIN_6
#define LED_1_GREEN_GPIO_Port GPIOD
#define POWER_GOOD_5V_Pin GPIO_PIN_7
#define POWER_GOOD_5V_GPIO_Port GPIOD
#define SILENT_MODE_CAN3_Pin GPIO_PIN_5
#define SILENT_MODE_CAN3_GPIO_Port GPIOB
#define ENC_1_A_Pin GPIO_PIN_6
#define ENC_1_A_GPIO_Port GPIOB
#define ENC_1_B_Pin GPIO_PIN_7
#define ENC_1_B_GPIO_Port GPIOB
#define END_STOP_M1_Pin GPIO_PIN_0
#define END_STOP_M1_GPIO_Port GPIOE
#define END_STOP_M1_EXTI_IRQn EXTI0_IRQn
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
