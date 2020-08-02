/*
 * SSD1315.c
 *
 *  Created on: Jul 6, 2020
 *      Author: jfrit
 */
/* Includes ------------------------------------------------------------------*/
#include "ssd1315.h"
#include "logger.h"
#include "i2c.h"
#include "testImg.h"
#include "u8g2.h"

/* Private variables ---------------------------------------------------------*/
uint8_t SSD1315_ADDR = 0x78;
static u8g2_t u8g2;

#define hare1_width 25
#define hare1_height 28
static unsigned char hare1_bits[] = {
        0x00,
        0x08,
        0x00,
        0x00,
        0x00,
        0x77,
        0x00,
        0x00,
        0x80,
        0x80,
        0x01,
        0x00,
        0x00,
        0x01,
        0x02,
        0x00,
        0x00,
        0x0e,
        0x0c,
        0x00,
        0x00,
        0x01,
        0x30,
        0x00,
        0x80,
        0x00,
        0x40,
        0x00,
        0x80,
        0x00,
        0x88,
        0x00,
        0x40,
        0x1c,
        0x1c,
        0x01,
        0x80,
        0x23,
        0x1c,
        0x01,
        0x00,
        0x20,
        0x08,
        0x01,
        0x00,
        0x1c,
        0x80,
        0x00,
        0x00,
        0x03,
        0x40,
        0x00,
        0x80,
        0x00,
        0x20,
        0x00,
        0x60,
        0x00,
        0x20,
        0x00,
        0x10,
        0x00,
        0x20,
        0x00,
        0x08,
        0x00,
        0x40,
        0x00,
        0x04,
        0x00,
        0x40,
        0x00,
        0x02,
        0x80,
        0x87,
        0x00,
        0x01,
        0x40,
        0x88,
        0x00,
        0x01,
        0x20,
        0x48,
        0x00,
        0x03,
        0x20,
        0x30,
        0x00,
        0x1c,
        0xc0,
        0x00,
        0x00,
        0x20,
        0x00,
        0x01,
        0x00,
        0x40,
        0x00,
        0x01,
        0x00,
        0x80,
        0x80,
        0x00,
        0x00,
        0x00,
        0x67,
        0x00,
        0x00,
        0x00,
        0x18,
        0x00,
        0x00 };

#define hare2_width 30
#define hare2_height 28
static unsigned char hare2_bits[] = {
        0x00,
        0xc0,
        0x03,
        0x00,
        0x00,
        0x20,
        0x04,
        0x00,
        0x00,
        0x20,
        0x08,
        0x00,
        0x00,
        0x40,
        0x10,
        0x00,
        0x00,
        0x80,
        0x20,
        0x00,
        0x00,
        0x70,
        0x40,
        0x00,
        0x00,
        0x08,
        0x80,
        0x01,
        0x00,
        0x04,
        0x00,
        0x02,
        0x00,
        0xf8,
        0x00,
        0x04,
        0x00,
        0x00,
        0x81,
        0x08,
        0x00,
        0x00,
        0xc1,
        0x11,
        0x00,
        0xfc,
        0xc0,
        0x11,
        0x00,
        0x03,
        0x80,
        0x10,
        0x80,
        0x00,
        0x00,
        0x08,
        0x70,
        0x00,
        0x00,
        0x04,
        0x08,
        0x00,
        0x00,
        0x03,
        0x06,
        0x00,
        0xc0,
        0x00,
        0x01,
        0x00,
        0x20,
        0x00,
        0x01,
        0x00,
        0x10,
        0x00,
        0x01,
        0x00,
        0x10,
        0x00,
        0x06,
        0x00,
        0x20,
        0x00,
        0x08,
        0x00,
        0x40,
        0x00,
        0x08,
        0xf0,
        0x41,
        0x00,
        0x08,
        0x08,
        0x46,
        0x00,
        0x08,
        0x04,
        0x48,
        0x00,
        0x08,
        0x08,
        0x30,
        0x00,
        0x30,
        0x10,
        0x00,
        0x00,
        0xc0,
        0x0f,
        0x00,
        0x00 };

uint8_t max = 0;
uint8_t step = 1;
uint8_t picture = 4;

/* Private function prototypes -----------------------------------------------*/
void u8g2_prepare();
uint8_t u8x8_byte_hw_i2c(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);
uint8_t u8x8_stm32_gpio_and_delay_cb(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);
uint8_t u8x8_stm32_gpio_and_delay_cb(U8X8_UNUSED u8x8_t* u8x8, U8X8_UNUSED uint8_t msg,
        U8X8_UNUSED uint8_t arg_int, U8X8_UNUSED void* arg_ptr);
uint8_t u8x8_byte_hw_i2c(u8x8_t* u8x8, uint8_t msg, uint8_t arg_int, void* arg_ptr);

// Demo functions
void u8g2_me(uint8_t a);
void u8g2_testImg(void);
void u8g2_drawLogo(void);
void u8g2_box_frame(uint8_t a);
void u8g2_disc_circle(uint8_t a);
void u8g2_string(uint8_t a);
void u8g2_line(uint8_t a);
void u8g2_hare(uint8_t a);
void draw(void);

/* Public function definitions -----------------------------------------------*/
int8_t ssd1315_init(void)
{
    u8g2_Setup_ssd1306_i2c_128x64_noname_1(&u8g2, U8G2_R0, u8x8_byte_hw_i2c,
                                           u8x8_stm32_gpio_and_delay_cb); // init u8g2 structure
    u8g2_InitDisplay(&u8g2); // send init sequence to the display, display is in sleep mode after this,
    u8g2_SetPowerSave(&u8g2, 0); // wake up display
    return (SSD1315_OK);
}

void ssd1315_demo(void)
{
    u8g2_FirstPage(&u8g2);
    do
    {
        draw();
    }
    while (u8g2_NextPage(&u8g2)); // 8 times running

    if (step <= max)
        step += 3;
    else
    {
        step = 0;
        picture++;
        if (picture >= 7)
        {
            picture = 0;
        }
    }
}

/* Private function definitions ----------------------------------------------*/
void u8g2_prepare()
{
    u8g2_SetFont(&u8g2, u8g2_font_courB10_tr);
    u8g2_SetFontRefHeightExtendedText(&u8g2);
    u8g2_SetDrawColor(&u8g2, 1);
    u8g2_SetFontPosTop(&u8g2);
    u8g2_SetFontDirection(&u8g2, 0);
}

void u8g2_me(uint8_t a)
{
    u8g2_DrawStr(&u8g2, 0, 0, "DrawDisc");
    u8g2_DrawDisc(&u8g2, 10, 22, 7, U8G2_DRAW_ALL);
    u8g2_DrawDisc(&u8g2, 24 + 5*a, 20, 5, U8G2_DRAW_ALL);
    u8g2_DrawStr(&u8g2, 0, 32, "Hello World!!!");
    u8g2_DrawCircle(&u8g2, 10, 22 + 32, 7, U8G2_DRAW_ALL);
    u8g2_DrawCircle(&u8g2, 24 + a, 20 + 32, 5, U8G2_DRAW_ALL);
}

void u8g2_drawLogo(void)
{
    u8g2_SetFontMode(&u8g2, 1);  // Transparent
    u8g2_SetDrawColor(&u8g2, 1);

    u8g2_SetFontDirection(&u8g2, 0);
    u8g2_SetFont(&u8g2, u8g2_font_inb24_mf);
    u8g2_DrawStr(&u8g2, 0, 5, "U");

    u8g2_SetFontDirection(&u8g2, 1);
    u8g2_SetFont(&u8g2, u8g2_font_inb30_mn);
    u8g2_DrawStr(&u8g2, 51, 8, "8");

    u8g2_SetFontDirection(&u8g2, 0);
    u8g2_SetFont(&u8g2, u8g2_font_inb24_mf);
    u8g2_DrawStr(&u8g2, 51, 5, "g");
    u8g2_DrawStr(&u8g2, 67, 5, "\xb2");

    u8g2_DrawHLine(&u8g2, 2, 35, 47);
    u8g2_DrawHLine(&u8g2, 3, 36, 47);
    u8g2_DrawVLine(&u8g2, 45, 32, 12);
    u8g2_DrawVLine(&u8g2, 46, 33, 12);

    u8g2_SetFont(&u8g2, u8g2_font_5x8_tr);

    u8g2_DrawStr(&u8g2, 1, 54, "github.com/olikraus/u8g2");
}

void u8g2_box_frame(uint8_t a)
{
    u8g2_DrawStr(&u8g2, 0, 0, "DrawBox");
    u8g2_DrawBox(&u8g2, 0, 15, 20, 10);
    u8g2_DrawBox(&u8g2, 0 + a, 15, 30, 7);
    u8g2_DrawStr(&u8g2, 0, 32, "DrawFrame");
    u8g2_DrawFrame(&u8g2, 0, 15 + 32, 20, 10);
    u8g2_DrawFrame(&u8g2, 0 + a, 15 + 32, 30, 7);
}

void u8g2_disc_circle(uint8_t a)
{
    u8g2_DrawStr(&u8g2, 0, 0, "DrawDisc");
    u8g2_DrawDisc(&u8g2, 10, 22, 7, U8G2_DRAW_ALL);
    u8g2_DrawDisc(&u8g2, 24 + a, 20, 5, U8G2_DRAW_ALL);
    u8g2_DrawStr(&u8g2, 0, 32, "DrawCircle");
    u8g2_DrawCircle(&u8g2, 10, 22 + 32, 7, U8G2_DRAW_ALL);
    u8g2_DrawCircle(&u8g2, 24 + a, 20 + 32, 5, U8G2_DRAW_ALL);
}

void u8g2_string(uint8_t a)
{
    u8g2_SetFontDirection(&u8g2, 0);
    u8g2_DrawStr(&u8g2, 70 + a, 31, " 0");
    u8g2_SetFontDirection(&u8g2, 1);
    u8g2_DrawStr(&u8g2, 68, 0 + a, " 90");
    u8g2_SetFontDirection(&u8g2, 2);
    u8g2_DrawStr(&u8g2, 70 - a, 31, " 180");
    u8g2_SetFontDirection(&u8g2, 3);
    u8g2_DrawStr(&u8g2, 72, 64 - a, " 270");
}

void u8g2_line(uint8_t a)
{
    u8g2_DrawStr(&u8g2, 0, 0, "DrawLine");
    u8g2_DrawLine(&u8g2, 7 + a, 15, 60 - a, 60);
    u8g2_DrawLine(&u8g2, 7 + a * 2, 15, 80 - a, 60);
    u8g2_DrawLine(&u8g2, 7 + a * 3, 15, 100 - a, 60);
    u8g2_DrawLine(&u8g2, 7 + a * 4, 15, 120 - a, 60);
}

void u8g2_hare(uint8_t a)
{
    u8g2_DrawStr(&u8g2, 0, 0, "DrawXBM");
    if ((a & 1) == 0)
    {
        u8g2_DrawXBM(&u8g2, 0 + a, 30, hare1_width, hare1_height, hare1_bits);
    }
    else
    {
        u8g2_DrawXBM(&u8g2, 0 + a, 30, hare2_width, hare2_height, hare2_bits);
    }
}

void u8g2_testImg(void)
{
    u8g2_prepare();
    u8g2_DrawXBM(&u8g2, 0, 64, testImg_width, testImg_height, testImg_bits);
}

void draw(void)
{
    u8g2_prepare();
    switch (picture)
    {
        case 0:
            max = 150;
            u8g2_me(step);
            break;
        case 1:
            u8g2_drawLogo();
            break;
        case 2:
            max = 150;
            u8g2_box_frame(step);
            break;
        case 3:
            max = 150;
            u8g2_disc_circle(step);
            break;
        case 4:
            max = 60;
            u8g2_string(step);
            break;
        case 5:
            max = 120;
            u8g2_line(step);
            break;
        case 6:
            max = 120;
            u8g2_hare(step);
            break;
    }
}

uint8_t u8x8_stm32_gpio_and_delay_cb(U8X8_UNUSED u8x8_t* u8x8, U8X8_UNUSED uint8_t msg,
        U8X8_UNUSED uint8_t arg_int, U8X8_UNUSED void* arg_ptr)
{
    switch (msg)
    // there is no need for any delay
    {
        case U8X8_MSG_GPIO_AND_DELAY_INIT: // called once during init phase of u8g2/u8x8 -> Make with HAL_init
            //__NOP();
            break;                                                    // can be used to setup pins
        case U8X8_MSG_DELAY_NANO:                   // delay arg_int * 1 nano second
            //__NOP();
            break;
        case U8X8_MSG_DELAY_100NANO:                // delay arg_int * 100 nano seconds
            //__NOP();
            //__NOP();
            //__NOP();
            break;
        case U8X8_MSG_DELAY_10MICRO:                // delay arg_int * 10 micro seconds
            // for (uint16_t n = 0; n < (320*arg_int); n++)
            //      {
            //       __NOP();
            //      }
            break;
        case U8X8_MSG_DELAY_MILLI:                  // delay arg_int * 1 milli second
            //  HAL_Delay(arg_int);

            break;
        case U8X8_MSG_DELAY_I2C:             // arg_int is the I2C speed in 100KHz, e.g. 4 = 400 KHz
            //for (uint16_t n = 0; n < (160*arg_int); n++)
            //            {
            //           __NOP();
            //      }
            break;                          // arg_int=1: delay by 5us, arg_int = 4: delay by 1.25us
        case U8X8_MSG_GPIO_D0:                       // D0 or SPI clock pin: Output level in arg_int
            //case U8X8_MSG_GPIO_SPI_CLOCK:
            break;
        case U8X8_MSG_GPIO_D1:                        // D1 or SPI data pin: Output level in arg_int
            //case U8X8_MSG_GPIO_SPI_DATA:
            break;
        case U8X8_MSG_GPIO_D2:                              // D2 pin: Output level in arg_int
            break;
        case U8X8_MSG_GPIO_D3:                              // D3 pin: Output level in arg_int
            break;
        case U8X8_MSG_GPIO_D4:                              // D4 pin: Output level in arg_int
            break;
        case U8X8_MSG_GPIO_D5:                              // D5 pin: Output level in arg_int
            break;
        case U8X8_MSG_GPIO_D6:                              // D6 pin: Output level in arg_int
            break;
        case U8X8_MSG_GPIO_D7:                              // D7 pin: Output level in arg_int
            break;
        case U8X8_MSG_GPIO_E:                               // E/WR pin: Output level in arg_int
            break;
        case U8X8_MSG_GPIO_CS:                      // CS (chip select) pin: Output level in arg_int
            break;
        case U8X8_MSG_GPIO_DC:    // DC (data/cmd, A0, register select) pin: Output level in arg_int
            break;
        case U8X8_MSG_GPIO_RESET:                   // Reset pin: Output level in arg_int
            break;
        case U8X8_MSG_GPIO_CS1:                    // CS1 (chip select) pin: Output level in arg_int
            break;
        case U8X8_MSG_GPIO_CS2:                    // CS2 (chip select) pin: Output level in arg_int
            break;
        case U8X8_MSG_GPIO_I2C_CLOCK:               // arg_int=0: Output low at I2C clock pin
            break;                        // arg_int=1: Input dir with pullup high for I2C clock pin
        case U8X8_MSG_GPIO_I2C_DATA:                        // arg_int=0: Output low at I2C data pin
            break;                         // arg_int=1: Input dir with pullup high for I2C data pin

    }
    return 1;
}
//   case U8X8_MSG_GPIO_MENU_SELECT:
//     u8x8_SetGPIOResult(u8x8, /* get menu select pin state */ 0);
//     break;
//   case U8X8_MSG_GPIO_MENU_NEXT:
//     u8x8_SetGPIOResult(u8x8, /* get menu next pin state */ 0);
//     break;
//   case U8X8_MSG_GPIO_MENU_PREV:
//     u8x8_SetGPIOResult(u8x8, /* get menu prev pin state */ 0);
//     break;
//   case U8X8_MSG_GPIO_MENU_HOME:
//     u8x8_SetGPIOResult(u8x8, /* get menu home pin state */ 0);
//     break;
//   default:
//     u8x8_SetGPIOResult(u8x8, 1);    */              // default return value

uint8_t u8x8_byte_hw_i2c(u8x8_t* u8x8, uint8_t msg, uint8_t arg_int, void* arg_ptr)
{
    uint8_t* ptr;
    static uint8_t buffer_count;
    static uint8_t buffer[DATA_BUFFER_SIZE + 1]; //the size of buffer depends on how many pages are transfered at once
                                                 //e.g. one page are 128byte and one byte more for command

    switch (msg)
    {
        case U8X8_MSG_BYTE_SEND:                                    //collect
        {
            ptr = arg_ptr;
            for (int i = 1; i <= arg_int; i++)
            {
                buffer[buffer_count] = *(ptr++);
                buffer_count++;
            }
        }
            break;
        case U8X8_MSG_BYTE_INIT:
            break;
        case U8X8_MSG_BYTE_SET_DC:
            break;
        case U8X8_MSG_BYTE_START_TRANSFER:
            buffer_count = 0;                                               // start
            break;
        case U8X8_MSG_BYTE_END_TRANSFER:                    // send all at once
            HAL_I2C_Master_Transmit(&I2C_HANDLER, DEVICE_ADDRESS, (uint8_t*) buffer, buffer_count, I2C_TIMEOUT);
            break;
        default:
            return 0;
    }
    return 1;
}

/************************ (C) COPYRIGHT John Fritz *****END OF FILE****/
