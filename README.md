## CANFD-ECI-VCI

This repository illustrates a problem I am having with the Linux version of the Ixxat USB-To-CAN FD driver (ECI).
It contains sources to reproduce the issue as well as some images representing my setup.

### History
I am working on a machine composed of several STM32 microcontrollers (STM32G474-VET6) communicating using CAN.

During development, we used windows software with the last version of the VCI Driver (v4.0.436) without any issue.
For production, we are using a Linux machine under Debian 10.3 with the ECI Driver (v1.11.3162.0) to receive data and send commands to this machine, using an Ixxat USB-To-CAN FD compact adapter.
The setup looks as follows:

![Production-setup-image](/Images/Ixxat-support-Production-setup.png)

After some testing, we realized that some messages were being lost. This is not due to the FIFO queue being filled faster than it can be read because the writing is rather slow.  
No errors are returned, the red led does not turn on on the adapter.


### Reproducing the issue
I tried to reproduce it with some really simple code and was only able to reproduce after slowing down the sending speed.

Here is my testing setup:  

![Test-setup-image](/Images/Ixxat-support-STM32-Windows-Linux-tests.png)


Here is the message sending loop, found in stm32/Src/canTests_main.c:
````c
u8_FDCAN_Tx_Payload[0] = 0x05;
u8_FDCAN_Tx_Payload[1] = 0x00;

TxHeader.Identifier = 1;
TxHeader.IdType = FDCAN_STANDARD_ID;
TxHeader.TxFrameType = FDCAN_DATA_FRAME;
TxHeader.DataLength = FDCAN_DLC_BYTES_2;
TxHeader.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
TxHeader.BitRateSwitch = FDCAN_BRS_OFF;
TxHeader.FDFormat = FDCAN_FD_CAN;
TxHeader.TxEventFifoControl = FDCAN_NO_TX_EVENTS;
TxHeader.MessageMarker = 0;

int i = 0;
while (i < 100)
{
  HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan1, &TxHeader, u8_FDCAN_Tx_Payload);
  HAL_Delay(100); // no problem with smaller or higher delays.
  i++;
}
````

And the receiving loop on the linux side:
````c
int recCount = 0;
while (true)
{
    dwCount = 1;
    hresult = ECI116_CtrlReceive(dwCtrlHandle, &dwCount, &rec, 1);

    if (hresult != ECI_OK)
    {
        OS_Sleep(1);
        continue;
    }
    if (rec.u.sCanMessage.u.V1.abData[0] == 0x05) // stm32 message
    {
        printf("Received %d\n", recCount);
        recCount++;
    }
}
````

On the linux side (ECI), we received less than 100 messages, when everything is received on the windows side (VCI).  
It seems like a delay of around 100 ms causes problems, which is aproximately the delay we had between messages in our production machine.  

__A delay of 10 ms causes no loss, as well as a delay of 150ms.__

### Configuration
Both drivers are setup in a similar way, according the the STM32 CAN pins setup:

![CAN-config-image](/Images/CAN-config.png)

Linux (ECI):
````c
ECI_HW_PARA astcHwPara = {0};
ECI_CTRL_CONFIG stcCtrlConfig = {0};
astcHwPara.wHardwareClass = ECI_HW_USB;

if (ECI116_Initialize(1, &astcHwPara) != ECI_OK)
    return false;

ECI_CANBTP stcBtpSdr     = {ECI_CAN_BTMODE_NATIVE, 2, 63, 16, 16, 0};
ECI_CANBTP stcBtpFdr     = {ECI_CAN_BTMODE_NATIVE, 2, 7, 2, 2, 0};

stcCtrlConfig.wCtrlClass = ECI_CTRL_CAN;
stcCtrlConfig.u.sCanConfig.dwVer = ECI_STRUCT_VERSION_V1;
stcCtrlConfig.u.sCanConfig.u.V1.bOpMode = ECI_CAN_OPMODE_STANDARD |  ECI_CAN_OPMODE_ERRFRAME;
stcCtrlConfig.u.sCanConfig.u.V1.bExMode = ECI_CAN_EXMODE_EXTDATA | ECI_CAN_EXMODE_FASTDATA | ECI_CAN_EXMODE_ISOFD;
stcCtrlConfig.u.sCanConfig.u.V1.sBtpSdr = stcBtpSdr;
stcCtrlConfig.u.sCanConfig.u.V1.sBtpFdr = stcBtpFdr;

if (ECI116_CtrlOpen(&dwCtrlHandle, 0, 0, &stcCtrlConfig) == ECI_OK)
    return (ECI116_CtrlStart(dwCtrlHandle) == ECI_OK);
````

Windows (VCI):
````C#
_control = (ICanControl2)_balObject.OpenSocket(0, typeof(ICanControl2));
var bitRate = new CanBitrate2(CanBitrateMode.Raw, 2, 63, 16, 16, 0);
var extendedBitRate = new CanBitrate2(CanBitrateMode.Raw, 2, 7, 2, 2, 0);

_control.InitLine(CanOperatingModes.Standard | CanOperatingModes.ErrFrame,
    CanExtendedOperatingModes.FastDataRate, CanFilterModes.Pass, 0, CanFilterModes.Pass, 0, bitRate, extendedBitRate);
_control.StartLine();
````

### How you can reproduce yourself

The source code for each module is present in the corresponding directory. Simply build and launch the executables.
If you do not have an STM32 microcontroller, I made a windows executable that demonstrates the problem, although it is not as reliable as testing with an STM32.  
The Windows-Linux setup is as follows:

![Windows-Linux-image](/Images/Ixxat-support-Windows-Linux-Test.png)

On the linux computer, build and launch the program available in the Linux-ECI folder.
On the windows computer, build and launch the program available in the Windows-VCI folder.

If both machines have an adapter connected and the drivers are installed, the programs should start without any issue.
On the windows side, you can select the delay you want to use, as well as increment it automatically. 
I made this to be sure you could test multiple delays, if the 100ms one does not cause any issue for you.

Here are the results on my computer:
![Windows-Linux-image](/Images/TestResults.png)

I hope you can find where the problem comes from, contact me for any further questions.


