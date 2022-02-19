#include "bsp_sram.h"
#include "stm32f4xx.h"
#include "cmsis_os.h"


// ���ⲿSRAM����FreeRTOS�Ķ�
#define EXT_SRAM_START_ADDRESS ( ( uint8_t * ) 0x68000000 )
#define EXT_SRAM_SIZE ( 1024 * 1024 )
uint8_t ucHeap[configTOTAL_HEAP_SIZE];

HeapRegion_t xHeapRegions[] = {
  { ucHeap,                 configTOTAL_HEAP_SIZE },
  { EXT_SRAM_START_ADDRESS, EXT_SRAM_SIZE         },
  { NULL,                   0                     }
};


//��ָ����ַ(WriteAddr+Bank1_SRAM3_ADDR)��ʼ,������0
//WriteAddr:Ҫд��ĵ�ַ
//n:Ҫд����ֽ���
void FSMC_SRAM_ClearBuffer(uint32_t WriteAddr, uint32_t n)
{
	for(;n!=0;n--)
	{
		*(__IO uint8_t*)(Bank1_SRAM3_ADDR+WriteAddr) = 0;
		WriteAddr++;
	}
}

//��ָ����ַ(WriteAddr+Bank1_SRAM3_ADDR)��ʼ,����д��n���ֽ�.
//pBuffer:�ֽ�ָ��
//WriteAddr:Ҫд��ĵ�ַ
//n:Ҫд����ֽ���
void FSMC_SRAM_WriteBuffer(uint8_t *pBuffer, uint32_t WriteAddr, uint32_t n)
{
	for(;n!=0;n--)
	{
		*(__IO uint8_t*)(Bank1_SRAM3_ADDR+WriteAddr)=*pBuffer;
		WriteAddr++;
		pBuffer++;
	}
}

//��ָ����ַ((WriteAddr+Bank1_SRAM3_ADDR))��ʼ,��������n���ֽ�.
//pBuffer:�ֽ�ָ��
//ReadAddr:Ҫ��������ʼ��ַ
//n:Ҫд����ֽ���
void FSMC_SRAM_ReadBuffer(uint8_t *pBuffer,uint32_t ReadAddr,uint32_t n)
{
	for(;n!=0;n--)
	{
		*pBuffer++=*(__IO uint8_t*)(Bank1_SRAM3_ADDR+ReadAddr);
		ReadAddr++;
	}
}

uint32_t FSMC_SRAM_TestSize(void)
{
    uint32_t i;
    uint32_t strip;
	uint8_t temp[256];   // ԭ�е�����
    uint8_t wridx;       // ׼��д�������
	uint8_t sval;	     // �ڵ�ַi����������
    
    // 256 * 4k ����
    __disable_irq();
    strip = 4 * 1024;
    for(i = 0, wridx = 0; i < 256 * strip; i += strip)
    {
        FSMC_SRAM_ReadBuffer(temp + wridx, i, 1);
        FSMC_SRAM_WriteBuffer(&wridx, i, 1);
        wridx++;
    }
    for(i = 0, wridx = 0; i < 256 * strip; i += strip)
    {
        FSMC_SRAM_ReadBuffer(&sval, i, 1);
        if(sval != wridx)
            break;
        FSMC_SRAM_WriteBuffer(temp + wridx, i, 1);
        wridx++;
    }
    __enable_irq();
    
    return i;
}
