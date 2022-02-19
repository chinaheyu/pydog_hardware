#include "usb_device.h"
#include "dwt_stm32_delay.h"
#include "w25qxx.h"
#include "bsp_usart.h"
#include "log.h"


// ��main�����У��ں�����ǰ�ĳ�ʼ������
void init_task_before_kernel_start(void)
{
    kernel_log("Initializing usart1.\r\n");
    usart1_manage_init();
    kernel_log("Initialize usart1 success.\r\n");
    
    kernel_log("Initializing USB device.\r\n");
    MX_USB_DEVICE_Init();
    kernel_log("Initialize USB device success.\r\n");
    
    kernel_log("Initialize DWT.\r\n");
    DWT_Delay_Init();
    kernel_log("Initialize DWT success.\r\n");
    
    kernel_log("Initializing SPI Flash.\r\n");
    while(!W25qxx_Init())
    {
        kernel_log("SPI Flash initialize failure, retrying...\r\n");
    }
    kernel_log("Initialize SPI Flash success.\r\n");
    
    kernel_log("Total initialization duration: %.2fms\r\n", get_time_ms_us());
}

// ����DAEMON_TASK_STARTUP_HOOK�������ػ���������ʱִ��һ��
// ���ڴ�Ų������ڽ�������ǰ��ʼ���Ĵ���
void init_task(void)
{
    
}
