#ifndef CAN_DEF_H_
#define CAN_DEF_H_
/**
 * @file CAN_Def.h
 * @author D. DE LIMA
 * @date Nov 21, 2019
 * @brief define all the rs232 bus constants
 *
 */
/***********************************
 *
 * INCLUDES
 *
 *************************************/

#include "inttypes.h"
#include "fdcan.h"

/***********************************
 *
 * BUS MANAGEMENT
 *
 *************************************/

//#define MSG_BUFF_ACK 	0
//#define MSG_BUFF_ID 	1
#define MSG_BUFF_ADDR				0
#define MSG_BUFF_DATA				1
#define MSG_BUFFER_SIZE   			100
#define TX_PAYLOAD_SIZE   			8U
#define RX_PAYLOAD_SIZE   			16U	// Do not change

#define TX_ATTEMPT_NB				5	// Number of re-transmission attempts

#define SLAVE_NB					7		// Number of Slaves

/* BUS STATUS */
#define BUS_ID_1					1
#define BUS_ID_2					2
#define BUS_ID_3					3
#define BUS_NB						3
#define BUS_AVAIL					0
#define BUS_WAITING					1

/* PERIODES */
#define TRANSMIT_PERIODE			100
#define RECEIVE_PERIODE 			10
#define TIMEOUT_ACK_PERIODE			50	// Period of each re-transmission attempt
#define TIMEOUT_REJECTED			600
#define TIMEOUT_FAILED				3000

typedef struct {
	int8_t i8_Msg_Tracking_Identifier;
	uint8_t u8_Msg_Type;
	uint16_t u16_Msg_Id;
	uint32_t u32_Msg_Src_Address;
	uint32_t u32_Msg_Dest_Address;
	uint64_t u64_Msg_Data;
} t_s_StagoCAN_Msg;

/***********************************
 *
 * CAN MESSAGE BUFFERS
 *
 *************************************/

#define MSG_BUFFER_SIZE   100

#ifdef CAN_DEF_ALLOC_DEF
	uint8_t u8_Buffer_Fill_Index;
	uint64_t u64_Msg_Buffer[MSG_BUFFER_SIZE][2];

	uint8_t u8_Target_Board;

	uint8_t u8_Bus_Status[BUS_NB];
	uint8_t u8_Slave_Status[SLAVE_NB];

	uint32_t u32_Timer_Wait_Tx;
	uint32_t u32_Timer_Wait_Ack;
	uint8_t u8_Bus_No_Ack_Counter;

	uint8_t u8_Last_Message_Id;
	uint8_t u8_Received_Msg_Type;
	uint16_t u16_Received_Msg_Id;
	uint16_t u16_Received_Msg_Addr;

	FDCAN_FilterTypeDef sFilterConfig;
	FDCAN_TxHeaderTypeDef TxHeader;
	FDCAN_RxHeaderTypeDef RxHeader;
	uint8_t u8_FDCAN_Tx_Payload[TX_PAYLOAD_SIZE];
	uint8_t u8_FDCAN_Rx_Payload[RX_PAYLOAD_SIZE];

#else
	extern uint8_t u8_Buffer_Fill_Index;
	extern uint64_t u64_Msg_Buffer[MSG_BUFFER_SIZE][2];

	extern uint8_t u8_Bus_Status[BUS_NB];		// Status of the given bus
	extern uint8_t u8_Slave_Status[SLAVE_NB];	// Status of the given slave

	extern uint8_t u8_Target_Board;			// Selected board for conf

	extern uint32_t u32_Timer_Wait_Tx;			// Transmit timer counter
	extern uint32_t u32_Timer_Wait_Ack;			// Ack timer counter
	extern uint8_t u8_Bus_No_Ack_Counter;		// Failed Ack counter

	extern uint8_t u8_Last_Message_Id;
	extern uint8_t u8_Received_Msg_Type;
	extern uint16_t u16_Received_Msg_Id;
	extern uint16_t u16_Received_Msg_Addr;

	extern FDCAN_FilterTypeDef sFilterConfig;
	extern FDCAN_TxHeaderTypeDef TxHeader;
	extern FDCAN_RxHeaderTypeDef RxHeader;
	extern uint8_t u8_FDCAN_Tx_Payload[TX_PAYLOAD_SIZE];
	extern uint8_t u8_FDCAN_Rx_Payload[RX_PAYLOAD_SIZE];

#endif /* CAN_DEF_ALLOC_DEF */

/***********************************
 *
 * CAN MESSAGES
 *
 *************************************/

#define ID_NONE	511

/* COMMON MESSAGE FIELDS */
#define TYPE_COMMAND		1
#define TYPE_STATUS			2
#define TYPE_ACK			3
#define TYPE_NB				4
#define TYPE_NONE			0xFF

#define SIZE_TYPE			2
#define SIZE_ID				9
#define SIZE_SRC_ADDRESS	8
#define SIZE_MAIN_STATE		3
#define SIZE_ORDER_STATE	2

#define SHIFT_TYPE			0
#define SHIFT_ID			SIZE_TYPE
#define SHIFT_SRC_ADDRESS	(SIZE_TYPE + SIZE_ID)
#define SHIFT_DATA			(SIZE_TYPE + SIZE_ID + SIZE_SRC_ADDRESS)
#define SHIFT_MAIN_STATE	(SIZE_TYPE + SIZE_ID_STATUS)
//#define SHIFT_ORDER_STATE	(SIZE_TYPE + SIZE_ID_STATUS + SIZE_MAIN_STATE)


#define MASK_TYPE			((1 << SIZE_TYPE) - 1)
#define MASK_ID				((1 << SIZE_ID) - 1)
#define MASK_SRC_ADDRESS	((1 << SIZE_SRC_ADDRESS) - 1)
#define MASK_MAIN_STATE		((1 << SIZE_MAIN_STATE) - 1)
//#define MASK_ORDER_STATE	((1 << SIZE_ORDER_STATE) - 1)

/***********************************
 *
 * COMMON
 *
 *************************************/
#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

/* Exported functions prototypes ---------------------------------------------*/
/**
 * @brief This function configures the CAN Bus
 */
void CAN_Bus_Config(uint32_t u32_node_id);

#endif /* CAN_DEF_H_ */
