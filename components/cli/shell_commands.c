#include "shell_commands.h"

/* FreeRTOS includes. */
#include "FreeRTOS.h"
#include "task.h"

/* Standard includes. */
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* FreeRTOS+CLI includes. */
#include "FreeRTOS_CLI.h"

#include "bsp_sram.h"
#include "adc.h"
#include "w25qxx.h"
#include "read_uid.h"
#include "fatfs.h"

#include "ssd1306_tests.h"


extern void fsResultVarToString(FRESULT r, char* out);


static BaseType_t prvReadUIDCommand( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString );
static BaseType_t prvTestExternalSramCommand( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString );
static BaseType_t prvTestExternalFlashCommand( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString );
static BaseType_t prvEraseFlashChipCommand( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString );
static BaseType_t prvFlashReadPageCommand( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString );
static BaseType_t prvWriteFileToFlashCommand( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString );
static BaseType_t prvHeapDetailCommand( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString );
static BaseType_t prvMeasureTempCommand( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString );
static BaseType_t prvTestOledCommand( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString );


static const CLI_Command_Definition_t xReadUID =
{
	"read-uid", /* The command string to type. */
	"\r\nread-uid:\r\n Read the Unique Device ID of stm32 chip. \r\n",
	prvReadUIDCommand, /* The function to run. */
	0 /* No parameters are expected. */
};

static const CLI_Command_Definition_t xTestExternalSram =
{
	"test-external-sram", /* The command string to type. */
	"\r\ntest-external-sram:\r\n Test the external sram size. \r\n",
	prvTestExternalSramCommand, /* The function to run. */
	0 /* No parameters are expected. */
};

static const CLI_Command_Definition_t xTestExternalFlash =
{
	"test-external-flash", /* The command string to type. */
	"\r\ntest-external-flash:\r\n Test the external flash size. \r\n",
	prvTestExternalFlashCommand, /* The function to run. */
	0 /* No parameters are expected. */
};

static const CLI_Command_Definition_t xEraseFlashChip =
{
	"erase-flash-chip", /* The command string to type. */
	"\r\nerase-flash-chip:\r\n Erase the entile flash chip. This operation takes a long time. \r\n",
	prvEraseFlashChipCommand, /* The function to run. */
	0 /* No parameters are expected. */
};

static const CLI_Command_Definition_t xFlashReadPage =
{
	"flash-read-page", /* The command string to type. */
	"\r\nflash-read-page <page-num>:\r\n Read the specified page of enternal flash. \r\n",
	prvFlashReadPageCommand, /* The function to run. */
	1 /* No parameters are expected. */
};

static const CLI_Command_Definition_t xWriteFileToFlash =
{
	"write-file-to-flash", /* The command string to type. */
	"\r\nwrite-file-to-flash <write-address> <filename>:\r\n Write the file context to enternal flash. \r\n",
	prvWriteFileToFlashCommand, /* The function to run. */
	2 /* No parameters are expected. */
};

static const CLI_Command_Definition_t xHeapDetail =
{
	"heap-detail", /* The command string to type. */
	"\r\nheap-detail:\r\n Print the details of heap state. \r\n",
	prvHeapDetailCommand, /* The function to run. */
	0 /* No parameters are expected. */
};

static const CLI_Command_Definition_t xMeasureTemp =
{
	"measure-temp", /* The command string to type. */
	"\r\nmeasure-temp:\r\n Measure the temperature of MCU through the inside sensor. \r\n",
	prvMeasureTempCommand, /* The function to run. */
	0 /* No parameters are expected. */
};

static const CLI_Command_Definition_t xTestOled =
{
	"test-oled", /* The command string to type. */
	"\r\ntest-oled:\r\n Test the external oled screen. \r\n",
	prvTestOledCommand, /* The function to run. */
	0 /* No parameters are expected. */
};

static BaseType_t prvReadUIDCommand( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString )
{
    /* Remove compile time warnings about unused parameters, and check the
	write buffer is not NULL.  NOTE - for simplicity, this example assumes the
	write buffer length is adequate, so does not check for buffer overflows. */
	( void ) pcCommandString;
	( void ) xWriteBufferLen;
	configASSERT( pcWriteBuffer );
    
    int len = 0;
    int i;
    
    uint8_t* buf = pvPortMalloc(UNIQUE_ID_BYTE_SIZE);
    
    read_unique_device_ID(buf, UNIQUE_ID_BYTE_SIZE);
    
    for(i = 0; i < UNIQUE_ID_BYTE_SIZE - 1; ++i)
    {
        len += snprintf(pcWriteBuffer + len, xWriteBufferLen - len, "%02X-", buf[i]);
    }
    len += snprintf(pcWriteBuffer + len, xWriteBufferLen - len, "%02X\r\n", buf[i]);
    
    vPortFree(buf);

	/* There is no more data to return after this single string, so return
	pdFALSE. */
	return pdFALSE;
}

static BaseType_t prvTestExternalSramCommand( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString )
{
	/* Remove compile time warnings about unused parameters, and check the
	write buffer is not NULL.  NOTE - for simplicity, this example assumes the
	write buffer length is adequate, so does not check for buffer overflows. */
	( void ) pcCommandString;
	( void ) xWriteBufferLen;
	configASSERT( pcWriteBuffer );
    
    const char *const pcHeader = "External SRAM Memory Size: ";
    strcpy(pcWriteBuffer, pcHeader);
    
    sprintf(pcWriteBuffer + strlen(pcHeader), "%d bytes\r\n", FSMC_SRAM_TestSize());//显示内存容量

	/* There is no more data to return after this single string, so return
	pdFALSE. */
	return pdFALSE;
}

static BaseType_t prvTestExternalFlashCommand( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString )
{
	/* Remove compile time warnings about unused parameters, and check the
	write buffer is not NULL.  NOTE - for simplicity, this example assumes the
	write buffer length is adequate, so does not check for buffer overflows. */
	( void ) pcCommandString;
	configASSERT( pcWriteBuffer );
    
    int len = 0;
    int i;
    
    switch (w25qxx.ID)
	{
	case W25Q512: // 	w25q512
        len += snprintf(pcWriteBuffer + len, xWriteBufferLen - len, "Device: W25Q512\r\n");
		break;
	case W25Q256: // 	w25q256
        len += snprintf(pcWriteBuffer + len, xWriteBufferLen - len, "Device: W25Q256\r\n");
		break;
	case W25Q128: // 	w25q128
        len += snprintf(pcWriteBuffer + len, xWriteBufferLen - len, "Device: W25Q128\r\n");
		break;
	case W25Q64: //	w25q64
        len += snprintf(pcWriteBuffer + len, xWriteBufferLen - len, "Device: W25Q64\r\n");
		break;
	case W25Q32: //	w25q32
        len += snprintf(pcWriteBuffer + len, xWriteBufferLen - len, "Device: W25Q32\r\n");
		break;
	case W25Q16: //	w25q16
        len += snprintf(pcWriteBuffer + len, xWriteBufferLen - len, "Device: W25Q16\r\n");
		break;
	case W25Q80: //	w25q80
        len += snprintf(pcWriteBuffer + len, xWriteBufferLen - len, "Device: W25Q80\r\n");
		break;
	case W25Q40: //	w25q40
        len += snprintf(pcWriteBuffer + len, xWriteBufferLen - len, "Device: W25Q40\r\n");
		break;
	case W25Q20: //	w25q20
        len += snprintf(pcWriteBuffer + len, xWriteBufferLen - len, "Device: W25Q20\r\n");
		break;
	case W25Q10: //	w25q10
        len += snprintf(pcWriteBuffer + len, xWriteBufferLen - len, "Device: W25Q10\r\n");
		break;
	default: // w25qxx Unknown ID
        len += snprintf(pcWriteBuffer + len, xWriteBufferLen - len, "Device: Unknown\r\n");
        break;
	}
    
    uint64_t Uid = 0;
    for(i = 0; i < 8; ++i)
    {
        Uid += w25qxx.UniqID[i];
        Uid <<= 8;
    }
    len += snprintf(pcWriteBuffer + len, xWriteBufferLen - len, "UniqID: 0x%016llx\r\n", Uid);
    
    len += snprintf(pcWriteBuffer + len, xWriteBufferLen - len, "PageSize: %d\r\n", w25qxx.PageSize);
    
    len += snprintf(pcWriteBuffer + len, xWriteBufferLen - len, "PageCount: %d\r\n", w25qxx.PageCount);
    
    len += snprintf(pcWriteBuffer + len, xWriteBufferLen - len, "SectorSize: %d\r\n", w25qxx.SectorSize);
    
    len += snprintf(pcWriteBuffer + len, xWriteBufferLen - len, "SectorCount: %d\r\n", w25qxx.SectorCount);
    
    len += snprintf(pcWriteBuffer + len, xWriteBufferLen - len, "BlockSize: %d\r\n", w25qxx.BlockSize);
    
    len += snprintf(pcWriteBuffer + len, xWriteBufferLen - len, "BlockCount: %d\r\n", w25qxx.BlockCount);
    
    len += snprintf(pcWriteBuffer + len, xWriteBufferLen - len, "Capacity: %d kbytes\r\n", w25qxx.CapacityInKiloByte);
    
    int test_count = 16;
    len += snprintf(pcWriteBuffer + len, xWriteBufferLen - len, "\r\nRead first %d bytes:\r\n", test_count);
    uint8_t* buf = pvPortMalloc(test_count * sizeof(uint8_t));
    W25qxx_ReadBytes(buf, 0, test_count);
    for(i = 0; i < test_count; ++i)
    {
        len += snprintf(pcWriteBuffer + len, xWriteBufferLen - len, "%02x ", buf[i]);
    }
    vPortFree(buf);
    len += snprintf(pcWriteBuffer + len, xWriteBufferLen - len, "\r\n");

	/* There is no more data to return after this single string, so return
	pdFALSE. */
	return pdFALSE;
}

static BaseType_t prvEraseFlashChipCommand( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString )
{
    ( void ) pcCommandString;
    ( void ) xWriteBufferLen;
	configASSERT( pcWriteBuffer );
    
    W25qxx_EraseChip();
    sprintf(pcWriteBuffer, "Erase finished.\r\n");
    
    return pdFALSE;
}

static BaseType_t prvFlashReadPageCommand( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString )
{
    const char *pcParameter;
    BaseType_t xParameterStringLength;
    configASSERT( pcWriteBuffer );
    
    BaseType_t ret = pdTRUE;
    
	/* Obtain the parameter string. */
	pcParameter = FreeRTOS_CLIGetParameter
					(
						pcCommandString,		/* The command string itself. */
						1,						/* Return the first parameter. */
						&xParameterStringLength	/* Store the parameter string length. */
					);
    
    static uint32_t current_address = 0;
    static uint8_t unfinished_flag = 0;
    static uint32_t page_idx = 0;
    
    int len = 0;
    int i;
    
    if(!unfinished_flag)
    {
        current_address = 0;
        unfinished_flag = 1;
        page_idx = atoi(pcParameter);
    }
    
    if(current_address < w25qxx.PageSize)
    {
        uint8_t* buf = pvPortMalloc(16 * sizeof(uint8_t));
        W25qxx_ReadBytes(buf, page_idx * w25qxx.PageSize + current_address, 16);
        
        len += snprintf(pcWriteBuffer + len, xWriteBufferLen - len, "0x%08X: ", page_idx * w25qxx.PageSize + current_address);
        
        for(i = 0; i < 8; ++i)
        {
            len += snprintf(pcWriteBuffer + len, xWriteBufferLen - len, "%02X ", buf[i]);
        }
        
        len += snprintf(pcWriteBuffer + len, xWriteBufferLen - len, "- ");
        
        for(i = 8; i < 16; ++i)
        {
            len += snprintf(pcWriteBuffer + len, xWriteBufferLen - len, "%02X ", buf[i]);
        }
        
        len += snprintf(pcWriteBuffer + len, xWriteBufferLen - len, " | ");
        
        for(i = 0; i < 16; ++i)
        {
            if(buf[i] >= 32 && buf[i] <= 126)
            {
                pcWriteBuffer[len + i] = buf[i];
            }
            else
            {
                pcWriteBuffer[len + i] = '.';
            }
        }
        len += 16;
        
        len += snprintf(pcWriteBuffer + len, xWriteBufferLen - len, "\r\n");
        
        vPortFree(buf);
        current_address += 16;
    }
    else
    {
        pcWriteBuffer[0] = '\0';
        unfinished_flag = 0;
        ret = pdFALSE;
    }
    
	/* There is no more data to return after this single string, so return
	pdFALSE. */
	return ret;
}

static BaseType_t prvWriteFileToFlashCommand( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString )
{
    const char *write_address;
    const char *filename;
    BaseType_t xParameterStringLength;
    configASSERT( pcWriteBuffer );
    
	/* Obtain the parameter string. */
	write_address = FreeRTOS_CLIGetParameter
					(
						pcCommandString,		/* The command string itself. */
						1,						/* Return the first parameter. */
						&xParameterStringLength	/* Store the parameter string length. */
					);
    filename = FreeRTOS_CLIGetParameter
					(
						pcCommandString,		/* The command string itself. */
						2,						/* Return the first parameter. */
						&xParameterStringLength	/* Store the parameter string length. */
					);
    
    // 申请内存
    FIL* fp = pvPortMalloc(sizeof(FIL));

    retSD = f_open(fp, filename, FA_READ);
    if(retSD != FR_OK)
    {
        fsResultVarToString(retSD, pcWriteBuffer);
        strcat( pcWriteBuffer, "\r\n" );
    }
    else
    {
        uint32_t file_size = f_size(fp);
        
        uint8_t* buf = pvPortMalloc(file_size);

        uint32_t written_size;
        uint32_t addr = atof(write_address);

        retSD = f_read(fp, buf, file_size, &written_size);
        W25qxx_WriteAnyBytes(addr, written_size, buf);
        
        snprintf(pcWriteBuffer, xWriteBufferLen, "Flash Finished.\r\nTotal file size: %d bytes. Successfully written size: %d bytes\r\nYou can check the result at page %d address 0x%08X\r\n", file_size, written_size, addr / w25qxx.PageSize, addr);

        vPortFree(buf);
    }
    
    retSD = f_close(fp);
    
    // 释放内存
    vPortFree(fp);

    return pdFALSE;
}

static BaseType_t prvHeapDetailCommand( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString )
{
    /* Remove compile time warnings about unused parameters, and check the
    write buffer is not NULL.  NOTE - for simplicity, this example assumes the
    write buffer length is adequate, so does not check for buffer overflows. */
    ( void ) pcCommandString;
    ( void ) xWriteBufferLen;
    configASSERT( pcWriteBuffer );
    
    HeapStats_t heap_state;
    vPortGetHeapStats(&heap_state);

    sprintf( pcWriteBuffer,
    "Available Heap Space: %d bytes\r\n"
    "Size Of Largest Free Block: %d bytes\r\n"
    "Size Of Smallest Free Block: %d bytes\r\n"
    "Number Of Free Blocks: %d\r\n"
    "Minimum Ever Free: %d bytes\r\n"
    "Number Of Successful Allocations: %d\r\n"
    "Number Of Successful Frees: %d\r\n",
    heap_state.xAvailableHeapSpaceInBytes,
    heap_state.xSizeOfLargestFreeBlockInBytes,
    heap_state.xSizeOfSmallestFreeBlockInBytes,
    heap_state.xNumberOfFreeBlocks,
    heap_state.xMinimumEverFreeBytesRemaining,
    heap_state.xNumberOfSuccessfulAllocations,
    heap_state.xNumberOfSuccessfulFrees
    );

    /* There is no more data to return after this single string, so return
    pdFALSE. */
    return pdFALSE;
}

static BaseType_t prvMeasureTempCommand( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString )
{
	/* Remove compile time warnings about unused parameters, and check the
	write buffer is not NULL.  NOTE - for simplicity, this example assumes the
	write buffer length is adequate, so does not check for buffer overflows. */
	( void ) pcCommandString;
	( void ) xWriteBufferLen;
	configASSERT( pcWriteBuffer );
    
    const char *const pcHeader = "Temperature: ";
    strcpy(pcWriteBuffer, pcHeader);
    
    HAL_ADC_Start(&hadc1);
    HAL_ADC_PollForConversion(&hadc1, 10);

    uint32_t AD_Value = HAL_ADC_GetValue(&hadc1);
    float Vol_Value = (float)AD_Value * ( 3.3f / 4096.0f); // 12bits ADC 转化为电压值
    float Temp_Value = (Vol_Value - 0.76f) / 0.0025f + 25.0f; // 电压转化为温度
    
    sprintf(pcWriteBuffer + strlen(pcHeader), "%f C\r\n", Temp_Value);//显示内存容量

	/* There is no more data to return after this single string, so return
	pdFALSE. */
	return pdFALSE;
}

static BaseType_t prvTestOledCommand( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString )
{
    /* Remove compile time warnings about unused parameters, and check the
	write buffer is not NULL.  NOTE - for simplicity, this example assumes the
	write buffer length is adequate, so does not check for buffer overflows. */
	( void ) pcCommandString;
	( void ) xWriteBufferLen;
	configASSERT( pcWriteBuffer );
    
    ssd1306_TestAll();

    sprintf(pcWriteBuffer, "Test finished\r\n");

	/* There is no more data to return after this single string, so return
	pdFALSE. */
	return pdFALSE;
}

void register_shell_commands(void)
{
    FreeRTOS_CLIRegisterCommand( &xReadUID );
    FreeRTOS_CLIRegisterCommand( &xTestExternalSram );
    FreeRTOS_CLIRegisterCommand( &xTestExternalFlash );
    FreeRTOS_CLIRegisterCommand( &xEraseFlashChip );
    FreeRTOS_CLIRegisterCommand( &xFlashReadPage );
    FreeRTOS_CLIRegisterCommand( &xWriteFileToFlash );
    FreeRTOS_CLIRegisterCommand( &xHeapDetail );
    FreeRTOS_CLIRegisterCommand( &xMeasureTemp );
    FreeRTOS_CLIRegisterCommand( &xTestOled );
    
}
