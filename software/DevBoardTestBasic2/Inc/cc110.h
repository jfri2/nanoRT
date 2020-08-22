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
#define CC110_FAIL              0
#define CC110_OK                1

#define CC110_TXRX_BUSY         0
#define CC110_TXRX_TX           1
#define CC110_TXRX_RX           2

#define CC110_FIFO_SIZE         64        // bytes
#define CC110_RSSI_OFFSET       74        // dB - from TI DN505

#define CC110_TXRX_BUF_SIZE     4096       // bytes
#define CC110_BUF_FULL          -1
#define CC110_BUF_OK            0
#define CC110_BUF_EMPTY         1
#define CC110_BUF_WRITE_FREE    0
#define CC110_BUF_WRITE_BUSY    1

/* Global Typedefs ----------------------------------------------------------*/

/* Public Function Prototypes ------------------------------------------------*/
int8_t cc110_init(void);
void cc110_test(void);
void cc110_printRssi(void);
void cc110_txrx(void);
void cc110_writeTxBuf(uint8_t* txData, uint16_t len);
void cc110_readRxBuf(uint8_t* rxData, uint16_t len);

#endif /* CC110_H_ */
