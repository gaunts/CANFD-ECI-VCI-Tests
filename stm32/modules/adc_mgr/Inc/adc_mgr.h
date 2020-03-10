/*
 * file        : adc_mgr.h
 * description : <insert here>
 * copyright   : (c) 2019 Kickmaker
 * author      : Luis CRUZ
 */

#ifndef __ADC_MGR_H__
#define __ADC_MGR_H__



// ==================================================
//     CPP GUARD [INIT]
// ==================================================

#ifdef __cplusplus
extern "C" {
#endif



// ==================================================
//     INCLUDE
// ==================================================

#include "common.h"



// ==================================================
//     DEFINITIONS
// ==================================================

#define ADC_MGR_VREF_MV             3300
#define ADC_MGR_VREF_INT_MV         1224  /* See MCU datasheet */
#define ADC_MGR_ADC_RESOLUTION      4096  /* 12 bits */



// ==================================================
//     MACRO
// ==================================================

#define ADC_MGR_COUNT_TO_MILLI_VOLT( x )        ( ( x * ADC_MGR_VREF_INT_MV ) / ADC_MGR_Read_Channel_Raw( ADC_MANAGER_CHANNEL_VREF_INT ) )



// ==================================================
//     TYPE DEFINITIONS
// ==================================================

typedef enum {

    ADC_MANAGER_CHANNEL_MOTOR_2_TEMPERATURE = 0 ,
    ADC_MANAGER_CHANNEL_MOTOR_1_TEMPERATURE     ,
    ADC_MANAGER_CHANNEL_VREF_INT                ,
    ADC_MANAGER_CHANNEL_MOTOR_1_CURRENT_AB      ,
    ADC_MANAGER_CHANNEL_MOTOR_1_CURRENT_CD      ,
    ADC_MANAGER_CHANNEL_MOTOR_2_CURRENT_AB      ,
    ADC_MANAGER_CHANNEL_MOTOR_2_CURRENT_CD      ,
    ADC_MANAGER_CHANNEL_MAX_CHANNELS            ,

} ADC_MANAGER_CHANNEL_t;



// ==================================================
//     TMP TEST
// ==================================================

//#define ENABLE_DATA_ACQ_ADC_TEST

#ifdef ENABLE_DATA_ACQ_ADC_TEST
#define DATA_ACQ_ADC_BUFFER_SIZE        5000

#if 0
#define ENABLE_DATA_ACQ_ADC_TEST_TARGET_ADC1
#else
#define ENABLE_DATA_ACQ_ADC_TEST_TARGET_ADC2
#endif

extern uint16_t data_acq_adc_buffer [ DATA_ACQ_ADC_BUFFER_SIZE ];
extern uint32_t data_acq_adc_index;
extern volatile bool data_acq_adc_busy;
#endif



// ==================================================
//     PUBLIC PROTOTYPES
// ==================================================

HAL_StatusTypeDef
ADC_MGR_Init ( ADC_HandleTypeDef * dev_1 ,
               ADC_HandleTypeDef * dev_2 );

bool
ADC_MGR_NewSampleAvailable ( void );

uint32_t
ADC_MGR_Read_Channel_Raw ( ADC_MANAGER_CHANNEL_t channel );

int32_t
ADC_MGR_Read_Channel_mV ( ADC_MANAGER_CHANNEL_t channel );

HAL_StatusTypeDef
ADC_MGR_ADC_ConvCpltCallback ( ADC_HandleTypeDef * dev );



// ==================================================
//     CPP GUARD [END]
// ==================================================

#ifdef __cplusplus
}
#endif



#endif /* __ADC_MGR_H__ */
