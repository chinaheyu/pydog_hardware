#include "memory_management.h"
#include "cmsis_os.h"
#include "log.h"


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