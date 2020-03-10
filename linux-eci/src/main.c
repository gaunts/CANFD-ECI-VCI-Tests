#include <ECI116.h>
#include <stdlib.h>
#include <stdbool.h>

ECI_CTRL_HDL dwCtrlHandle    = ECI_INVALID_HANDLE;
bool connect()
{
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
    
    ECI_HW_INFO info = {0};
    ECI116_GetInfo(0, &info);
    OS_Sleep(1000);

    return false;
}

ECI_CTRL_MESSAGE getStartMsg()
{
    ECI_CTRL_MESSAGE stcCtrlMsg = {0};

    stcCtrlMsg.wCtrlClass                            = ECI_CTRL_CAN;
    stcCtrlMsg.u.sCanMessage.dwVer                   = ECI_STRUCT_VERSION_V1;
    stcCtrlMsg.u.sCanMessage.u.V1.dwMsgId            = 1;
    stcCtrlMsg.u.sCanMessage.u.V1.uMsgInfo.Bits.dlc  = 2;
    stcCtrlMsg.u.sCanMessage.u.V1.uMsgInfo.Bits.type = ECI_CAN_MSGTYPE_DATA;
    stcCtrlMsg.u.sCanMessage.u.V1.uMsgInfo.Bits.srr  = 0;
    stcCtrlMsg.u.sCanMessage.u.V1.uMsgInfo.Bits.ext  = 0;

    stcCtrlMsg.u.sCanMessage.u.V1.abData[0] = 0x02;
    stcCtrlMsg.u.sCanMessage.u.V1.abData[1] = 0x00;

    return stcCtrlMsg;
}

int main()
{
    DWORD dwCount = 0;
    int recCount = 0;
    int expected = 0;
    int pinged = 0;
    ECI_RESULT hresult;
    ECI_CTRL_MESSAGE rec = {0};
    ECI_CTRL_MESSAGE stcCtrlMsg = getStartMsg();

    if (!connect())
    {
        printf("Error while connecting to the adapter\n");
        return 0;
    }

    while (true)
    {
        dwCount = 1;
        hresult = ECI116_CtrlReceive(dwCtrlHandle, &dwCount, &rec, 1);

        if (hresult != ECI_OK)
        {
            OS_Sleep(1);
            continue;
        }

        if (rec.u.sCanMessage.u.V1.abData[0] == 0x01)
        {
            stcCtrlMsg.u.sCanMessage.u.V1.abData[0] = 0x02;
            ECI116_CtrlSend(dwCtrlHandle, &stcCtrlMsg, 500);
            stcCtrlMsg.u.sCanMessage.u.V1.abData[0] = 0x04;
            pinged = 1;
            recCount = 0;
            expected = 0;
        }
        else if (pinged == 1 && rec.u.sCanMessage.u.V1.abData[0] == 0x03)
        {
            int recv = rec.u.sCanMessage.u.V1.abData[1];
            if (expected != recv)
            {
                ECI116_CtrlSend(dwCtrlHandle, &stcCtrlMsg, 500);
                printf("Missed one\n");
            }
            else
            {
                printf("OK for %d\n", recCount);
            }
            
            recCount++;
            expected = (recv + 1) % 0xFF;
        }
    }
}
