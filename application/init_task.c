#include "usb_device.h"
#include "dwt_stm32_delay.h"
#include "w25qxx.h"
#include "bsp_usart.h"
#include "log.h"


// 在main函数中，内核启动前的初始化代码
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

// 基于DAEMON_TASK_STARTUP_HOOK，会在守护进程启动时执行一次
// 用于存放不急着在进程启动前初始化的代码
void init_task(void)
{
    
}
