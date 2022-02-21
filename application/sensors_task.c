#include "cmsis_os.h"
#include "bsp_usart.h"
#include "pstwo.h"


#define GAS_SENSOR_FIFO_BUFSIZE 128
fifo_s_t gas_sensor_fifo;

extern void publish_gas_sensor(float raw);

uint32_t usart2_rx_callback(uint8_t *buff, uint16_t len)
{
    fifo_s_puts_noprotect(&gas_sensor_fifo, (char *)buff, len);
    return 0;
}

int char2int(char* data, int len)
{
    int sum = 0;
    for(int i = 0; i < len; ++i)
    {
        sum *= 10;
        sum += data[i] - 0x30;
    }
    return sum;
}

static void send_cmd_to_sensor(const char* cmd)
{
    usart2_transmit((uint8_t*)cmd, strlen(cmd));
}

void sensors_task(void *argument)
{
    PS2_Init();
    PS2_SetInit();
    
    fifo_s_init(&gas_sensor_fifo, pvPortMalloc(GAS_SENSOR_FIFO_BUFSIZE), GAS_SENSOR_FIFO_BUFSIZE);
    
    usart2_manage_init();
    usart2_rx_callback_register(usart2_rx_callback);
    
    uint32_t concentration_after_filter = 0;
    uint32_t concentration_raw = 0;
    
    // 设置传感器的0点
    send_cmd_to_sensor("X 00400\r\n");
    
    // 初始化传感器为Streaming Mode
    send_cmd_to_sensor("K 1\r\n");
    
    // Z ##### z #####\r\n
    char frame[17];
    int data_idx = 0;
    
    // 开始循环读传感器
    while(1)
    {
        int num = fifo_s_used(&gas_sensor_fifo);
        while(num-- > 0)
        {
            char ch = fifo_s_get(&gas_sensor_fifo);
            if(ch == '\r')
            {
                if(data_idx >= 13)
                {
                    concentration_after_filter = char2int(frame + data_idx - 13, 5);
                }
                
                if(data_idx >= 5)
                {
                    concentration_raw = char2int(frame + data_idx - 5, 5);
                }
                
                publish_gas_sensor(concentration_raw);

                data_idx = 0;
            }
            else
            {
                frame[data_idx++] = ch;
            }
        }
        
        PS2_Receive();
        
        osDelay(20);
    }
}
