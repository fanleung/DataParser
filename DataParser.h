#ifndef SRC_PROTOCOL_L2CAP_H_
#define SRC_PROTOCOL_L2CAP_H_

#include <stdint.h>

enum UartHandle
{
  ***
  ***
  ***
};

void L2CAP_Send
(
        uint16_t length,                //payload的长度
        enum UartHandle handle,      //
        const uint8_t* payload,
        void (*onSend)(uint8_t* packet, uint16_t length)
);


void L2CAP_OnPacketReceived
(
        const uint8_t* packet, uint16_t length,
        void (*onReceived)(enum UartHandle handle, uint8_t* payload, uint16_t length)
);

#endif /* SRC_PROTOCOL_L2CAP_H_ */
