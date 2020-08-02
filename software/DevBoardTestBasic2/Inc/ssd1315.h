/*
 * ssd1315.h
 *
 *  Created on: Jul 6, 2020
 *      Author: jfrit
 */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef SSD1315_H_
#define SSD1315_H_

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "u8g2.h"

/* Global Definitions --------------------------------------------------------*/
#define DATA_BUFFER_SIZE 128   //the size of buffer depends on how many pages are transfered at once e.g. one page are 128byte
#define I2C_TIMEOUT 1000
#define DEVICE_ADDRESS 0x78    //device address is added
#define I2C_HANDLER hi2c1

/* Global Variables ----------------------------------------------------------*/
enum ssd1315_return {
    SSD1315_FAIL = -1,
    SSD1315_OK = 1
};

/* Public Function Prototypes ------------------------------------------------*/
int8_t ssd1315_init(void);
void ssd1315_demo(void);

#endif /* SSD1315_H_ */
