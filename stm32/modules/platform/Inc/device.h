/*
 * file        : device.h
 * description : <insert here>
 * copyright   : (c) 2019 Kickmaker
 * author      : Luis CRUZ
 */

#ifndef __DEVICE_H__
#define __DEVICE_H__



// ==================================================
//     CPP GUARD [INIT]
// ==================================================

#ifdef __cplusplus
extern "C" {
#endif



// ==================================================
//     INCLUDE
// ==================================================

#include <stm32g4xx_hal.h>
#include <stm32g4xx_ll_pwr.h>



// ==================================================
//     VERSION
// ==================================================

#define MAJOR           0
#define MINOR           1
#define REVISION        1



// ==================================================
//     PLATFORM
// ==================================================

///// Peripheral handlers
#define HANDLER__TIM__STEPPER_MOTOR_1_PWM           htim2
//#define HANDLER__TIM__STEPPER_MOTOR_1_ENCODER       htim5  /* htim4 */
#define HANDLER__TIM__STEPPER_MOTOR_1_CONTROL       htim7
#define HANDLER__TIM__STEPPER_MOTOR_2_PWM           htim1
//#define HANDLER__TIM__STEPPER_MOTOR_2_ENCODER       htim4  /* htim5 */
#define HANDLER__TIM__STEPPER_MOTOR_2_CONTROL       htim6
#define HANDLER__TIM__SCHEDULER                     htim15
#define HANDLER__ADC__TEMPERATURE                   hadc1
//#define HANDLER__ADC__CURRENT                       hadc2
#define HANDLER__UART__CONSOLE                      huart3



// ==================================================
//     FUSES
// ==================================================

///// Console
#define FUSE__CONSOLE               CONFIG__OPTION__ENABLED
#define FUSE__CONSOLE__ERROR        CONFIG__OPTION__ENABLED
#define FUSE__CONSOLE__WARNING      CONFIG__OPTION__ENABLED
#define FUSE__CONSOLE__RUNTIME      CONFIG__OPTION__ENABLED
#define FUSE__CONSOLE__DEBUG        CONFIG__OPTION__ENABLED



// ==================================================
//     CPP GUARD [END]
// ==================================================

#ifdef __cplusplus
}
#endif



#endif /* __DEVICE_H__ */
