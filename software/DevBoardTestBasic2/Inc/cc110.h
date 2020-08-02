/*
 * cc110.h
 *
 *  Created on: Jul 27, 2020
 *      Author: jfrit
 */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef CC110_H_
#define CC110_H_

/* Includes ------------------------------------------------------------------*/
#include <cc110_types.h>

/* Global Definitions --------------------------------------------------------*/
// Function return status codes
#define CC110_OK    1
#define CC110_FAIL  0

/* Global Variables ----------------------------------------------------------*/

/* Public Function Prototypes ------------------------------------------------*/
extern void cc110_spi_xfer(uint8_t* txData, uint8_t* rxData, uint16_t len);
int8_t cc110_init(void);
void cc110_test(void);

#endif /* CC110_H_ */
