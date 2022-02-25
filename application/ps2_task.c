#include "cmsis_os.h"
#include "pstwo.h"

void ps2_task(void *argument)
{
    /* �ֱ� */
    PS2_Init();
    PS2_SetInit();
   
    // ��ʼѭ����������
    while(1)
    {
        if(osKernelLock() == osOK)
        {
            // �����ֱ�����
            PS2_Receive();
            osKernelUnlock();
        }

        osDelay(20);
    }
}
