/**
 * @file CAN_Manager.c
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

#include "CAN_Manager.h"
#include "CAN_Transmit.h"
#include "CAN_Send_Message.h"

/***********************************
 *
 * PRIVATE FUNCTION PROTOTYPES
 *
 *************************************/
void CAN_Decode_Msg(uint64_t *pt_rx_message);

/***********************************
 *
 * FUNCTIONS
 *
 *************************************/
/**
 *
 * @brief This function manages send and receive messages over the CAN bus
 * @param bus_id
 */
/**
 *
 * @brief This function manages sent and received messages over a CAN bus.
 * 		  If the bus is available it allows transmission or message over the bus
 * 		  otherwise it awaits the reception of an ack message.S
 * 		  Received messages are stored in the pointed variable rx_message
 * 		  and it returns a boolean value indicating if a new message is received.
 * @param bus_id
 * @param rx_message
 * @return new_message
 */
uint8_t CAN_Bus_Manager(uint8_t u8_bus_id, uint64_t *pt_rx_message)
{
	uint8_t u8_new_message = FALSE;
	if (HAL_FDCAN_GetRxFifoFillLevel(&hfdcan1, FDCAN_RX_FIFO0) >= 1)
	{
		/* Retrieve Rx message from RX FIFO0 */
		if (HAL_FDCAN_GetRxMessage(&hfdcan1, FDCAN_RX_FIFO0, &RxHeader,
				u8_FDCAN_Rx_Payload) != HAL_OK)
		{
			Error_Handler();
		}
		else
		{
			CAN_Decode_Msg(pt_rx_message);
			u8_new_message = TRUE;
		}
	}

	if (u8_Bus_Status[u8_bus_id] == BUS_WAITING) //full speed
	{
		if (u16_Received_Msg_Id == u8_Last_Message_Id && u8_Received_Msg_Type == TYPE_ACK)
		{
			// Sent message received
			u8_Bus_Status[u8_bus_id] = BUS_AVAIL;
			u8_Bus_No_Ack_Counter = 0;
			u8_Slave_Status[u8_Target_Board] = OK;

//			LED_MGR_SetPower(LED_IND_2_GREEN, POWER_ON);
////			HAL_Delay(50);
//			LED_MGR_SetPower(LED_IND_2_GREEN, POWER_OFF);

		}
		else
		{
			// No Ack received after T_MAX_ACK_WAIT ms

			// Re-send the last message
//			if (HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan1, &TxHeader,
//					u8_FDCAN_Tx_Payload) != HAL_OK)
//			{
//				Error_Handler();
//			}

			u32_Timer_Wait_Ack = HAL_GetTick();			// reset timer
			u8_Bus_No_Ack_Counter++;					// Count failure

			if (1)
			{
				// Sent message not received
				u8_Bus_Status[u8_bus_id] = BUS_AVAIL;
				u8_Bus_No_Ack_Counter = 0;
				u8_Slave_Status[u8_Target_Board] = FAIL;	// Set slave as failed
//
//				LED_MGR_SetPower(LED_IND_2_RED, POWER_ON);
////				HAL_Delay(50);
//				LED_MGR_SetPower(LED_IND_2_RED, POWER_OFF);
			}
		}
	}
	else if (u8_Bus_Status[u8_bus_id] == BUS_AVAIL)
	{
		if (u32_Timer_Wait_Tx + TRANSMIT_PERIODE < HAL_GetTick())
		{
			CAN_Bus_Transmit(u8_bus_id);

			u32_Timer_Wait_Tx = HAL_GetTick();			// reset timer
		}
	}

	return u8_new_message;
}

/**
 * @brief This function decodes a received message.
 * 		  If it's a command or status message, it adds an ack message
 * 		  to the message buffer.
 * @param rx_message
 */
void CAN_Decode_Msg(uint64_t *pt_rx_message)
{

	uint64_t u64_tx_ack_message = 0;
	uint32_t u32_src_address = 0;
	*pt_rx_message = 0;
	uint8_t u8_index = 0;

	/* Convert byte array to Single variable */
	while (u8_index < TX_PAYLOAD_SIZE)
	{
		*pt_rx_message += (((uint64_t)u8_FDCAN_Rx_Payload[u8_index]) << (8 * u8_index));
		u8_index++;
	}

	// Reset rx payload
	for (u8_index = 0; u8_index < RX_PAYLOAD_SIZE; u8_index++)
	{
		u8_FDCAN_Rx_Payload[u8_index] = 0x00;
	}

	u8_Received_Msg_Type = (*pt_rx_message >> SHIFT_TYPE) & MASK_TYPE;
	u16_Received_Msg_Id = ID_NONE;

	// Ack Message
	if (u8_Received_Msg_Type == TYPE_ACK)
	{
		u16_Received_Msg_Id = (*pt_rx_message >> SHIFT_ID) & MASK_ID;
	}
	// Command Message
	else if (u8_Received_Msg_Type == TYPE_COMMAND)
	{

		u16_Received_Msg_Id = (*pt_rx_message >> SHIFT_ID) & MASK_ID;
		u32_src_address = (*pt_rx_message >> SHIFT_SRC_ADDRESS) & MASK_SRC_ADDRESS;
		// Build Ack message
		u64_tx_ack_message += (TYPE_ACK & MASK_TYPE) << SHIFT_TYPE;
		u64_tx_ack_message += (u16_Received_Msg_Id & MASK_ID) << SHIFT_ID;
		u64_tx_ack_message += (RxHeader.Identifier & MASK_SRC_ADDRESS) << SHIFT_SRC_ADDRESS;
		u64_tx_ack_message += (u8_Received_Msg_Type & MASK_TYPE) << SHIFT_DATA; // Data corresponds to type cmd or type status
		// Send message
//		CONSOLE( "Sending Ack message" EOL );
		CAN_Send_Msg(&u64_tx_ack_message, u32_src_address);
		u32_Timer_Wait_Tx = 0;

	}
	// Status Message
	else if (u8_Received_Msg_Type == TYPE_STATUS)
	{

		u16_Received_Msg_Id = (*pt_rx_message >> SHIFT_ID) & MASK_ID;
		u32_src_address = (*pt_rx_message >> SHIFT_SRC_ADDRESS) & MASK_SRC_ADDRESS;
		// Build Ack message
		u64_tx_ack_message += (TYPE_ACK & MASK_TYPE) << SHIFT_TYPE;
		u64_tx_ack_message += (u16_Received_Msg_Id & MASK_ID) << SHIFT_ID;
		u64_tx_ack_message += (RxHeader.Identifier & MASK_SRC_ADDRESS) << SHIFT_SRC_ADDRESS;
		u64_tx_ack_message += (u8_Received_Msg_Type & MASK_TYPE) << SHIFT_DATA;
		// Send message
//		CONSOLE( "Sending Ack message" EOL );
		CAN_Send_Msg(&u64_tx_ack_message, u32_src_address);
		u32_Timer_Wait_Tx = 0;

	}
	else
	{
		u16_Received_Msg_Id = ID_NONE;
	}
}
