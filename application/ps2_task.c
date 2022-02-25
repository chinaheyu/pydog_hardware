#include "cmsis_os.h"
#include "pstwo.h"

void ps2_task(void *argument)
{
    /* 手柄 */
    PS2_Init();
    PS2_SetInit();
   
    // 开始循环读传感器
    while(1)
    {
        if(osKernelLock() == osOK)
        {
            // 接收手柄数据
            PS2_Receive();
            osKernelUnlock();
        }

        osDelay(20);
    }
}
