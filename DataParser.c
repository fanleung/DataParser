#inlcude "DataParser.h"

#define RECEIVE_BUFFERSIZE 1024
static uint8_t mReceiveBuff[RECEIVE_BUFFERSIZE];
volatile uint8_t IsReceived = 0;
volatile uint16_t ReceiveLength;


static uint8_t mTransmitBuff[64];

enum ParseState
{
    ParseState_Header,
    ParseState_Handle,
    ParseState_Length_L,
    ParseState_Length_H,
    ParseState_Payload,
    ParseState_Sum_L,
    ParseState_Sum_H,
};


//Receive
static enum ParseState mParseState = ParseState_Header;
enum UartHandle mHandle;
static uint16_t mParseLength;
static uint16_t mParseCount;
static uint8_t mParsePayload[64];
static uint16_t mParseSum;
static uint16_t mCalcSum;


//actually Send Data by *onSend
void L2CAP_Send
(
        uint16_t length,             
        enum UartHandle handle,      
        const uint8_t* payload,
        void (*onSend)(uint8_t* packet, uint16_t length)
)
{
    mTransmitBuff[0] = 0xAA;
    mTransmitBuff[1] = handle;

    mTransmitBuff[2] = (length >> 0) & 0xFF;
    mTransmitBuff[3] = (length >> 8) & 0xFF;

    memcpy(mTransmitBuff + 4, payload, length);

    uint16_t sum = 0;
    for(uint16_t index = 0; index < length; index++){
        sum += payload[index];
    }

    mTransmitBuff[length + 4] = (sum >> 0) & 0xFF;
    mTransmitBuff[length + 5] = (sum >> 8) & 0xFF;

    onSend(mTransmitBuff, length + 6);
}


//1.在这函数中先判断漏不漏数据
//2.在onReceive中判断什么操作，再将payload作对应的处理
void L2CAP_OnPacketReceived
(
        const uint8_t* packet, uint16_t length,
        void (*onReceived)(enum UartHandle handle, uint8_t* payload, uint16_t length)
)
{
    if(IsReceived == 1)
    {
        printf("ReceiveLength: %d\r\n", ReceiveLength);

        //todo 解决长度为14， 21时怎么处理
        for(uint16_t index = 0; index < ReceiveLength; index ++)
        {
            uint8_t byte = packet[index];
            switch (mParseState) {
            case ParseState_Header:
                if(byte == 0xAA) {
                    mParseState = ParseState_Handle;
                }
                break;

            case ParseState_Handle:
                if(byte < MaxHandle){
                    mHandle = (enum UartHandle)byte;
                    mParseState = ParseState_Length_L;
                } else {
                    // TODO
                    printf("mHandle > MaxHandle ... \r\n");
                    mParseState = ParseState_Header;
                }
                break;

            case ParseState_Length_L:
                mParseLength = byte;
                mParseState = ParseState_Length_H;
                break;

            case ParseState_Length_H:
                mParseLength |= byte << 8;
                mParseCount = 0;
                mCalcSum = 0;
                mParseState = ParseState_Payload;

                // TODO mParseLength不超过20
                if(mParseLength > 20){
                    printf("mParseLength > 20 ... \r\n");
                    mParseState = ParseState_Header;
                }

                break;

            case ParseState_Payload:
                mCalcSum += byte;
                mParsePayload[mParseCount] = byte;
                mParseCount += 1;
                if(mParseCount == mParseLength){
                    mParseState = ParseState_Sum_L;
                }
                break;

            case ParseState_Sum_L:
                mParseSum = byte;
                mParseState = ParseState_Sum_H;
                break;


            case ParseState_Sum_H:
                mParseSum |= byte << 8;
                if(mParseSum == mCalcSum) {
                    onReceived(mHandle, mParsePayload, mParseLength);
                }else{
                    log("mParseSum: %04X != mCalcSum: %04X\r\n", mParseSum, mCalcSum);
                }
                mParseState = ParseState_Header;
                break;
            }
        }
        /*        接收数据长度清零        */
        IsReceived = 0;
        ReceiveLength = 0;
        /*        开启下一次接收        */
//        LinkLayer_Receive((uint8_t*)packet, length);
    }

}



void App_uart_Receive(void)
{
    L2CAP_OnPacketReceived(mReceiveBuff, RECEIVE_BUFFERSIZE, L2CAP_OnReceive);
}

void App_uart_Send(enum UartHandle handle, uint8_t* data, uint16_t length)
{
    L2CAP_Send(length, handle, data, L2CAP_OnSend);
}


static void L2CAP_OnSend(uint8_t* packet, uint16_t length)
{
    LinkLayer_Send(packet, length);
}

static void L2CAP_OnReceive(enum UartHandle handle, uint8_t* payload, uint16_t length)
{
      switch(handle){
      case ***:
        break;
        
        ...
        
      default:
        break;
    }
}
