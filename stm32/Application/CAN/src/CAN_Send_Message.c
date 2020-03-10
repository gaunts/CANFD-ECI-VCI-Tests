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
#include "CAN_Def.h"
#include "CAN_Send_Message.h"

/***********************************
 *
 * FUNCTIONS
 *
 *************************************/
/**
 * @brief This function fills the message buffer with messages to be sent over the CAN Bus
 * @param pt_tx_message
 * @param u32_dest_address
 * @return
 */
uint8_t CAN_Send_Msg(uint64_t *pt_tx_message, uint32_t u32_dest_address)
{
	uint8_t u8_success = 0;
	while (u8_success != 1){

		if (((u64_Msg_Buffer[u8_Buffer_Fill_Index][MSG_BUFF_DATA] >> SHIFT_ID) & MASK_ID) >= ID_NONE)
		{
			u64_Msg_Buffer[u8_Buffer_Fill_Index][MSG_BUFF_ADDR] = u32_dest_address;
			u64_Msg_Buffer[u8_Buffer_Fill_Index][MSG_BUFF_DATA] = *pt_tx_message;

			u8_Buffer_Fill_Index++;
			u8_Buffer_Fill_Index %= MSG_BUFFER_SIZE;

			u8_success = 1;
		}
		else
		{
			u8_Buffer_Fill_Index++;
			u8_Buffer_Fill_Index %= MSG_BUFFER_SIZE;
		}
	}

	return u8_success;
}
