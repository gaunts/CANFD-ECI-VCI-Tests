/*
 * file        : adc_mgr.c
 * description : <insert here>
 * copyright   : (c) 2019 Kickmaker
 * author      : Luis CRUZ
 */

// ==================================================
//     INCLUDE
// ==================================================

#include "adc_mgr.h"
#include "tim.h"



// ==================================================
//     TYPE DEFINITIONS
// ==================================================

typedef struct {

    ADC_HandleTypeDef * dev_1;
    ADC_HandleTypeDef * dev_2;

    struct {

        uint16_t        raw [ ADC_MANAGER_CHANNEL_MAX_CHANNELS ];

    } data;

    volatile bool       new_sample;

} ADC_MGR_RUNTIME_t;



// ==================================================
//     GLOBAL VARIABLES
// ==================================================

static ADC_MGR_RUNTIME_t adc_mgr_runtime;

#ifdef ENABLE_DATA_ACQ_ADC_TEST
uint16_t data_acq_adc_buffer [ DATA_ACQ_ADC_BUFFER_SIZE ];
uint32_t data_acq_adc_index = 0;
volatile bool data_acq_adc_busy = false;
#endif



// ==================================================
//     PUBLIC FUNCTIONS
// ==================================================

HAL_StatusTypeDef
ADC_MGR_Init ( ADC_HandleTypeDef * dev_1 ,
               ADC_HandleTypeDef * dev_2 )
{
    HAL_StatusTypeDef status = HAL_ERROR;

    if ( ( dev_1 != NULL ) && ( dev_2 != NULL ) )
    {
        memset( ( void * )&adc_mgr_runtime , 0x00 , sizeof( ADC_MGR_RUNTIME_t ) );

        ///// ADC device
        adc_mgr_runtime.dev_1 = dev_1;
        adc_mgr_runtime.dev_2 = dev_2;









        status = HAL_TIM_Base_Start( &htim20 );

        if ( status != HAL_OK )
        {
            return status;
        }







        ///// ADC - DEV_1
        status = HAL_ADCEx_Calibration_Start( adc_mgr_runtime.dev_1 , ADC_SINGLE_ENDED );

        if ( status != HAL_OK )
        {
            return status;
        }

        ///// ADC - DEV_2
        status = HAL_ADCEx_Calibration_Start( adc_mgr_runtime.dev_2 , ADC_SINGLE_ENDED );

        if ( status != HAL_OK )
        {
            return status;
        }







        status = HAL_ADC_Start_IT( adc_mgr_runtime.dev_1 );

        if ( status != HAL_OK )
        {
            return status;
        }

        status = HAL_ADC_Start_IT( adc_mgr_runtime.dev_2 );

        if ( status != HAL_OK )
        {
            return status;
        }
    }

    return status;
}

bool
ADC_MGR_NewSampleAvailable ( void )
{
    bool new_sample;

    new_sample                 = adc_mgr_runtime.new_sample;
    adc_mgr_runtime.new_sample = false;

    return new_sample;
}

uint32_t
ADC_MGR_Read_Channel_Raw ( ADC_MANAGER_CHANNEL_t channel )
{
    return adc_mgr_runtime.data.raw[ channel ];
}

int32_t
ADC_MGR_Read_Channel_mV ( ADC_MANAGER_CHANNEL_t channel )
{
    return ADC_MGR_COUNT_TO_MILLI_VOLT( ADC_MGR_Read_Channel_Raw( channel ) );
}

HAL_StatusTypeDef
ADC_MGR_ADC_ConvCpltCallback ( ADC_HandleTypeDef * dev )
{
    HAL_StatusTypeDef status = HAL_ERROR;

    if ( dev != NULL )
    {
        if ( dev == adc_mgr_runtime.dev_1 )
        {
            const uint32_t channel_ref = ADC_MANAGER_CHANNEL_MOTOR_2_TEMPERATURE , max_samples = 3;

            static uint32_t sample_count = 0;

            if ( __HAL_ADC_GET_FLAG( dev , ADC_FLAG_EOC ) == true )
            {
                if ( sample_count < max_samples )
                {
                    adc_mgr_runtime.data.raw[ channel_ref + sample_count ] = HAL_ADC_GetValue( dev );

                    sample_count++;
                }
            }

            if ( __HAL_ADC_GET_FLAG( dev , ADC_FLAG_EOS ) )
            {
                adc_mgr_runtime.new_sample = true;
                sample_count               = 0;

#if defined( ENABLE_DATA_ACQ_ADC_TEST ) && defined( ENABLE_DATA_ACQ_ADC_TEST_TARGET_ADC1 )
                if ( ( data_acq_adc_busy == true ) && ( data_acq_adc_index < DATA_ACD_ADC_BUFFER_SIZE ) )
                {
                    // Options:
                    //     - ADC_MANAGER_CHANNEL_MOTOR_2_TEMPERATURE
                    //     - ADC_MANAGER_CHANNEL_MOTOR_1_TEMPERATURE
                    //     - ADC_MANAGER_CHANNEL_VREF_INT
                    uint16_t target = ADC_MANAGER_CHANNEL_VREF_INT;

                    data_acq_adc_buffer[ data_acq_adc_index ] = adc_mgr_runtime.data.raw[ target ];

                    if ( ++data_acq_adc_index >= DATA_ACD_ADC_BUFFER_SIZE )
                    {
                        data_acq_adc_busy = false;
                    }
                }
#endif
            }
        }
        else
        if ( dev == adc_mgr_runtime.dev_2 )
        {
            const uint32_t channel_ref = ADC_MANAGER_CHANNEL_MOTOR_1_CURRENT_AB , max_samples = 4;

            static uint32_t sample_count = 0;

            if ( __HAL_ADC_GET_FLAG( dev , ADC_FLAG_EOC ) == true )
            {
                if ( sample_count < max_samples )
                {
                    adc_mgr_runtime.data.raw[ channel_ref + sample_count ] = HAL_ADC_GetValue( dev );

                    sample_count++;
                }
            }

            if ( sample_count == max_samples )
            {
                adc_mgr_runtime.new_sample = true;
                sample_count               = 0;

#if defined( ENABLE_DATA_ACQ_ADC_TEST ) && defined( ENABLE_DATA_ACQ_ADC_TEST_TARGET_ADC2 )
                if ( ( data_acq_adc_busy == true ) && ( data_acq_adc_index < DATA_ACQ_ADC_BUFFER_SIZE ) )
                {
                    // Options:
                    //     - ADC_MANAGER_CHANNEL_MOTOR_1_CURRENT_AB
                    //     - ADC_MANAGER_CHANNEL_MOTOR_1_CURRENT_CD
                    //     - ADC_MANAGER_CHANNEL_MOTOR_2_CURRENT_AB
                    //     - ADC_MANAGER_CHANNEL_MOTOR_2_CURRENT_CD
                    uint16_t target = ADC_MANAGER_CHANNEL_MOTOR_2_CURRENT_CD;

                    data_acq_adc_buffer[ data_acq_adc_index ] = adc_mgr_runtime.data.raw[ target ];

                    if ( ++data_acq_adc_index >= DATA_ACQ_ADC_BUFFER_SIZE )
                    {
                        data_acq_adc_busy = false;
                    }
                }
#endif
            }
        }

        status = HAL_OK;
    }

    return status;
}



// End of file
