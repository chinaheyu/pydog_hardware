#include "maestro.h"
#include "bsp_usart.h"
#include "cmsis_os.h"

// Use Compact protocol
// https://www.pololu.com/docs/0J40/5.e

#define MAESTRO_CHAMMEL_NUM 18
#define MAESTRO_MESSAGE_BUF_LENTH (3 + 2 * MAESTRO_CHAMMEL_NUM)
uint8_t* messageBuf;

void MAESTRO_Init(void)
{
    usart3_manage_init();
    messageBuf = pvPortMalloc(MAESTRO_MESSAGE_BUF_LENTH);
}

// units: quarter-microseconds
void MAESTRO_SetTarget(uint8_t channel, uint16_t target)
{
    messageBuf[0] = 0x84; // Command byte: Set Target.
    messageBuf[1] = channel; // First data byte holds channel number.
    messageBuf[2] = target & 0x7F; // Second byte holds the lower 7 bits of target.
    messageBuf[3] = (target >> 7) & 0x7F;   // Third data byte holds the bits 7-13 of target.
    
    usart3_transmit(messageBuf, 4);
}

// units: quarter-microseconds
void MAESTRO_SetMultipleTargets(uint8_t number_of_targets, uint8_t first_channel_number, uint16_t* targets)
{
    messageBuf[0] = 0x9F;
    messageBuf[1] = number_of_targets;
    messageBuf[2] = first_channel_number;
    
    for(int i = 0; i < number_of_targets; ++i)
    {
        messageBuf[3 + 2 * i] = targets[i] & 0x7F;
        messageBuf[4 + 2 * i] = (targets[i] >> 7) & 0x7F;
    }
    
     usart3_transmit(messageBuf, 3 + number_of_targets * 2);
}

// units: (0.25 ¦Ìs)/(10 ms), RDS3235 = 51.28
void MAESTRO_SetSpeed(uint8_t channel, uint16_t speed)
{
    messageBuf[0] = 0x87;
    messageBuf[1] = channel;
    messageBuf[2] = speed & 0x7F;
    messageBuf[3] = (speed >> 7) & 0x7F;
    
    usart3_transmit(messageBuf, 4);
}

// units: (0.25 ¦Ìs)/(10 ms)/(80 ms)
void MAESTRO_SetAcceleration(uint8_t channel, uint16_t acceleration)
{
    messageBuf[0] = 0x89;
    messageBuf[1] = channel;
    messageBuf[2] = acceleration & 0x7F;
    messageBuf[3] = (acceleration >> 7) & 0x7F;
    
    usart3_transmit(messageBuf, 4);
}

// units: 1/48 ¦Ìs
void MAESTRO_SetPWM(uint16_t on_time, uint16_t period)
{
    messageBuf[0] = 0x8A;
    messageBuf[1] = on_time & 0x7F;
    messageBuf[2] = (on_time >> 7) & 0x7F;
    messageBuf[3] = period & 0x7F;
    messageBuf[4] = (period >> 7) & 0x7F;
    
    usart3_transmit(messageBuf, 5);
}

