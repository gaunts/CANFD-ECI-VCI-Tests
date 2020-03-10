/**
 * @file CAN_Send_Message.h
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

/***********************************
 *
 * PUBLIC FUNCTION PROTOTYPES
 *
 *************************************/
/**
 *
 * @brief This function fills the message buffer with messages to be sent over the CAN Bus
 * @param message_type
 * @param message_id
 * @param rx_adresse
 * @return
 */
uint8_t CAN_Send_Msg(uint64_t *pt_tx_message, uint32_t u32_dest_address);
