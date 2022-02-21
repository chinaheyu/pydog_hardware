#include "cmsis_os.h"
#include "ssd1306.h"
#include "w25qxx.h"
#include "log.h"


#define SSD1306_INCLUDE_FONT_6x8_ADDRESS SSD1306_BUFFER_SIZE

#define LOGO_ADDRESS (FONT_BASE_ADDRESS + FONT_16x26_ADDRESS + FONT_16x26_SIZE) // 0x000039E4 14820
#define LOGO_SIZE SSD1306_BUFFER_SIZE

void oled_task(void const * argument)
{
#ifndef USE_FLASH_FONT
    extern void WriteFontsToFlash(void);
    WriteFontsToFlash();
#endif
#ifdef USE_FLASH_FONT
    extern void ReadFontsFromFlash(void);
    ReadFontsFromFlash();
#endif
    ssd1306_Init();

    // 读取放在spi flash中的logo
    uint8_t *logo_image = pvPortMalloc(LOGO_SIZE);
    W25qxx_ReadBytes(logo_image, LOGO_ADDRESS, LOGO_SIZE);
    
    ssd1306_FastUpdateScreen(logo_image);
    
    while(1)
    {
        
        osDelay(100);
    }
}


