#include "memory_management.h"
#include "cmsis_os.h"
#include "log.h"


// 将外部SRAM用作FreeRTOS的堆
#define EXT_SRAM_START_ADDRESS ( ( uint8_t * ) 0x68000000 )
#define EXT_SRAM_SIZE ( 1024 * 1024 )
uint8_t ucHeap[configTOTAL_HEAP_SIZE];

HeapRegion_t xHeapRegions[] = {
  { ucHeap,                 configTOTAL_HEAP_SIZE },
  { EXT_SRAM_START_ADDRESS, EXT_SRAM_SIZE         },
  { NULL,                   0                     }
};

uint32_t get_total_heap_size(void)
{
    return configTOTAL_HEAP_SIZE + EXT_SRAM_SIZE;
}

void application_stack_overflow_hook(xTaskHandle xTask, signed char *pcTaskName)
{
    verbose_log("Task '%s' is stack overflow.", pcTaskName);
    
    while(1)
    {
    }
}

void application_malloc_failed_hook(void)
{
    verbose_log("pvPortMalloc failed, maybe out of memory.");
    
    while(1)
    {
    }
}