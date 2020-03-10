#include <stdio.h>
#include "canTests_main.h"

#include "CAN_Manager.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdarg.h>
#include <math.h>

#include "main.h"

#define DEF_NODE_ID 2


uint64_t u64_Rx_Raw_Msg = 0;
uint64_t u64_Tx_Raw_Msg = 0;
uint8_t u8_New_Message_Received = FALSE;

void sending_loop(void)
{
	u8_FDCAN_Tx_Payload[0] = 0x3A;
	u8_FDCAN_Tx_Payload[1] = 0x12;
	u8_FDCAN_Tx_Payload[2] = 0x00;
	u8_FDCAN_Tx_Payload[3] = 0x00;
	u8_FDCAN_Tx_Payload[4] = 0x00;
	u8_FDCAN_Tx_Payload[5] = 0x00;
	u8_FDCAN_Tx_Payload[6] = 0x00;
	u8_FDCAN_Tx_Payload[7] = 0x00;

	u8_FDCAN_Tx_Payload[8] = 0x00;

	TxHeader.Identifier = 1;
	TxHeader.IdType = FDCAN_STANDARD_ID;
	TxHeader.TxFrameType = FDCAN_DATA_FRAME;
	TxHeader.DataLength = FDCAN_DLC_BYTES_8;
	TxHeader.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
	TxHeader.BitRateSwitch = FDCAN_BRS_OFF;
	TxHeader.FDFormat = FDCAN_FD_CAN;
	TxHeader.TxEventFifoControl = FDCAN_NO_TX_EVENTS;
	TxHeader.MessageMarker = 0;

	int i = 0;
	while (i < 100)
	{
		HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan1, &TxHeader, u8_FDCAN_Tx_Payload);
		HAL_Delay(100);
		i++;
	}
}

void canTests_main(void)
{
	CAN_Bus_Config(DEF_NODE_ID);

	while (true)
	{
		HAL_Delay(1);
		if (HAL_FDCAN_GetRxFifoFillLevel(&hfdcan1, FDCAN_RX_FIFO0) == 0)
			continue;

		if (HAL_FDCAN_GetRxMessage(&hfdcan1, FDCAN_RX_FIFO0, &RxHeader,
						u8_FDCAN_Rx_Payload) != HAL_OK)
			continue;

		if (u8_FDCAN_Rx_Payload[0] == 0x69)
			sending_loop();
	}
}
