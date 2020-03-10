/**
 * @file CAN_Send_Message.c
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
#include "CAN_Transmit.h"

/***********************************
 *
 * PRIVATE FUNCTION PROTOTYPES
 *
 *************************************/

void CAN_Prepare_Msg(uint64_t tx_message);

/***********************************
 *
 * FUNCTIONS
 *
 *************************************/

/**
 *
 * @brief This function transmits a message over the CAN bus
 * 		  It checks first if there's an acknowledge message to be transmitted
 * 		  Then it checks if there's a command to be transmitted
 * 		  And finally it checks if there's a status to be transmitted
 * @param bus_id
 */
void CAN_Bus_Transmit(uint8_t u8_bus_id)
{
	uint8_t u8_new_payload = FALSE;
	uint8_t u8_index = 0;
	uint8_t u8_count = 0;

	u8_index = u8_Buffer_Fill_Index + 1; // Start from the oldest message
	u8_index %= MSG_BUFFER_SIZE;

	while (u8_new_payload == FALSE && u8_count <= MSG_BUFFER_SIZE)
	{
		//look for ACK
		if (((u64_Msg_Buffer[u8_index][MSG_BUFF_DATA] >> SHIFT_TYPE) & MASK_TYPE) == TYPE_ACK
			&& ((u64_Msg_Buffer[u8_index][MSG_BUFF_DATA] >> SHIFT_ID) & MASK_ID) != ID_NONE)
		{
			// ACK found: prepare message
			CAN_Prepare_Msg(u64_Msg_Buffer[u8_index][MSG_BUFF_DATA]);
			u8_new_payload = TRUE;
		}
		else
		{
			u8_count++;
			u8_index++;
			u8_index %= MSG_BUFFER_SIZE;
		}
	}

	u8_count = 0;
	while (u8_new_payload == FALSE && u8_count <= MSG_BUFFER_SIZE)
	{
		//look for CMD
		if (((u64_Msg_Buffer[u8_index][MSG_BUFF_DATA] >> SHIFT_TYPE) & MASK_TYPE) == TYPE_COMMAND
			&& ((u64_Msg_Buffer[u8_index][MSG_BUFF_DATA] >> SHIFT_ID) & MASK_ID) != ID_NONE)
		{
			// CMD found: prepare message
			CAN_Prepare_Msg(u64_Msg_Buffer[u8_index][MSG_BUFF_DATA]);
			u8_new_payload = TRUE;

			u8_Bus_Status[u8_bus_id] = BUS_WAITING;
			u32_Timer_Wait_Ack = HAL_GetTick();

		}
		else
		{
			u8_count++;
			u8_index++;
			u8_index %= MSG_BUFFER_SIZE;
		}
	}

	u8_count = 0;
	while (u8_new_payload == FALSE && u8_count <= MSG_BUFFER_SIZE)
	{
		//look for STS
		if (((u64_Msg_Buffer[u8_index][MSG_BUFF_DATA] >> SHIFT_TYPE) & MASK_TYPE) == TYPE_STATUS
			&& ((u64_Msg_Buffer[u8_index][MSG_BUFF_DATA] >> SHIFT_ID) & MASK_ID) != ID_NONE)
		{
			// STS found: prepare message
			CAN_Prepare_Msg(u64_Msg_Buffer[u8_index][MSG_BUFF_DATA]);
			u8_new_payload = TRUE;

			u8_Bus_Status[u8_bus_id] = BUS_WAITING;
			u32_Timer_Wait_Ack = HAL_GetTick();
		}
		else
		{
			u8_count++;
			u8_index++;
			u8_index %= MSG_BUFFER_SIZE;
		}
	}

	if (u8_new_payload == TRUE)
	{
		TxHeader.Identifier = u64_Msg_Buffer[u8_index][MSG_BUFF_ADDR];	// Get address
		u8_Last_Message_Id = ((u64_Msg_Buffer[u8_index][MSG_BUFF_DATA] >> SHIFT_ID) & MASK_ID);	// Record message id
		u64_Msg_Buffer[u8_index][MSG_BUFF_DATA] = (ID_NONE & MASK_ID) << SHIFT_ID;	// free buffer cell

		/* Add message to TX FIFO of FDCAN instance 1 */
//		HAL_Delay(50);
		if (HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan1, &TxHeader, u8_FDCAN_Tx_Payload) != HAL_OK)
		{
			Error_Handler();
//			LED_MGR_SetPower(LED_IND_1_RED, POWER_ON);
//			HAL_Delay(50);
//			LED_MGR_SetPower(LED_IND_1_RED, POWER_OFF);
		}
		else
		{
//			LED_MGR_SetPower(LED_IND_1_GREEN, POWER_ON);
//			HAL_Delay(50);
//			LED_MGR_SetPower(LED_IND_1_GREEN, POWER_OFF);
		}
	}
}

/**
 *
 * @brief This function fills the transmit payload array with the data of the message to be transmitted.
 * @param message_id
 */
void CAN_Prepare_Msg(uint64_t u64_tx_message)
{
	uint8_t u8_index = 0;

	/* Covert Single variable to byte array */
	while (u8_index < TX_PAYLOAD_SIZE)
	{
		u8_FDCAN_Tx_Payload[u8_index] = ((u64_tx_message >> (8 * u8_index)) & 255);
		u8_index++;
	}

	u8_FDCAN_Tx_Payload[TX_PAYLOAD_SIZE] = 0;
}
