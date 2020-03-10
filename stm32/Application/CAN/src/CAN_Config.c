/**
 * @file CAN_Config.c
 * @author D. DE LIMA
 * @date Nov 21, 2019
 * @brief
 *
 */
/***********************************
 *
 * INCLUDES
 *
 *************************************/
#include "fdcan.h"
#include "CAN_Def.h"

/***********************************
 *
 * FUNCTIONS
 *
 *************************************/

/**
 * @brief This function configures the CAN Bus and initializes all of the defined variables
 */

void CAN_Bus_Config(uint32_t u32_node_id)
{
	// Init defined variables
	u8_Buffer_Fill_Index = 0;

	for (int index = 0; index < MSG_BUFFER_SIZE; index++)
	{
		u64_Msg_Buffer[index][MSG_BUFF_DATA] = 0xFFFFFFFF;
	}

	u8_Target_Board = 0;

	for (int index = 0; index < BUS_NB; index++)
	{
		u8_Bus_Status[index] = BUS_AVAIL;
	}

	for (int index = 0; index < SLAVE_NB; index++)
	{
		u8_Slave_Status[index] = OK;
	}

	u32_Timer_Wait_Tx = 0;
	u32_Timer_Wait_Ack = 0;
	u8_Bus_No_Ack_Counter = 0;

	u8_Last_Message_Id = 0;
	u8_Received_Msg_Type = 0;
	u16_Received_Msg_Id = 0;
	u16_Received_Msg_Addr = 0;

	/* Configure reception filter to Rx FIFO 0 on both FDCAN instances */
	sFilterConfig.IdType = FDCAN_STANDARD_ID;
	sFilterConfig.FilterIndex = 0;
	sFilterConfig.FilterType = FDCAN_FILTER_MASK;
	sFilterConfig.FilterConfig = FDCAN_FILTER_TO_RXFIFO0;
	sFilterConfig.FilterID1 = u32_node_id;
//	sFilterConfig.FilterID1 = 0x111;
	sFilterConfig.FilterID2 = 0x7FF;

	if (HAL_FDCAN_ConfigFilter(&hfdcan1, &sFilterConfig) != HAL_OK)
	{
		Error_Handler();
	}

	if (HAL_FDCAN_ConfigFilter(&hfdcan2, &sFilterConfig) != HAL_OK)
	{
		Error_Handler();
	}

	/* Configure global filter on both FDCAN instances:
	 Filter all remote frames with STD and EXT ID
	 Reject non matching frames with STD ID and EXT ID */
	if (HAL_FDCAN_ConfigGlobalFilter(&hfdcan1, FDCAN_REJECT, FDCAN_REJECT,
	FDCAN_FILTER_REMOTE, FDCAN_FILTER_REMOTE) != HAL_OK)
	{
		Error_Handler();
	}
	if (HAL_FDCAN_ConfigGlobalFilter(&hfdcan2, FDCAN_REJECT, FDCAN_REJECT,
	FDCAN_FILTER_REMOTE, FDCAN_FILTER_REMOTE) != HAL_OK)
	{
		Error_Handler();
	}

	/* Prepare Tx message Header */
	TxHeader.Identifier = 0x111;
	TxHeader.IdType = FDCAN_STANDARD_ID;
	TxHeader.TxFrameType = FDCAN_DATA_FRAME;
	TxHeader.DataLength = FDCAN_DLC_BYTES_8;
	TxHeader.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
	TxHeader.BitRateSwitch = FDCAN_BRS_OFF;
	TxHeader.FDFormat = FDCAN_FD_CAN;
	TxHeader.TxEventFifoControl = FDCAN_NO_TX_EVENTS;
	TxHeader.MessageMarker = 0;

	/* Configure and enable Tx Delay Compensation, required for BRS mode.
	 TdcOffset default recommended value: DataTimeSeg1 * DataPrescaler
	 TdcFilter default recommended value: 0 */
	if (HAL_FDCAN_ConfigTxDelayCompensation(&hfdcan1, 5, 0) != HAL_OK)
	{
		Error_Handler();
	}
	if (HAL_FDCAN_ConfigTxDelayCompensation(&hfdcan2, 5, 0) != HAL_OK)
	{
		Error_Handler();
	}
	if (HAL_FDCAN_EnableTxDelayCompensation(&hfdcan1) != HAL_OK)
	{
		Error_Handler();
	}
	if (HAL_FDCAN_EnableTxDelayCompensation(&hfdcan2) != HAL_OK)
	{
		Error_Handler();
	}

	/* Start the FDCAN module on both FDCAN instances */
	if (HAL_FDCAN_Start(&hfdcan1) != HAL_OK)
	{
		Error_Handler();
	}
	if (HAL_FDCAN_Start(&hfdcan2) != HAL_OK)
	{
		Error_Handler();
	}

	for (int index = 0; index < TX_PAYLOAD_SIZE; index++)
	{
		u8_FDCAN_Tx_Payload[index] = 0x00;
	}

	for (int index = 0; index < RX_PAYLOAD_SIZE; index++)
	{
		u8_FDCAN_Rx_Payload[index] = 0x00;
	}

}
