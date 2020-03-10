/**
 * @file CAN_Manager.h
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
 * @brief This function manages send and receive messages over the CAN bus
 * @param bus_id
 * @param rx_message
 * @return A new message indicator
 */
uint8_t CAN_Bus_Manager(uint8_t u8_bus_id, uint64_t *pt_rx_message);

