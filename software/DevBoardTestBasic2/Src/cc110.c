/*
 * cc110.c
 *
 *  Created on: Jul 27, 2020
 *      Author: jfrit
 */
/* Includes ------------------------------------------------------------------*/
#include <cc110.h>
#include <cc110_types.h>
#include <spi.h>
#include <gpio.h>
#include <logger.h>
#include <ustr.h>
#include <tim.h>

/* Private variables ---------------------------------------------------------*/
// Status Byte
static CC110_STATUS_BYTE_t CC110_STATUS_DATA;

// Status Registers
static CC110_PARTNUM_REG_t CC110_PARTNUM_DATA;
static CC110_VERSION_REG_t CC110_VERSION_DATA;
static CC110_FREQEST_REG_t CC110_FREQEST_DATA;
static CC110_CRCOK_REG_t CC110_CRCOK_DATA;
static CC110_RSSI_REG_t CC110_RSSI_DATA;
static CC110_MARCSTATE_REG_t CC110_MARCSTATE_DATA;
static CC110_PKTSTATUS_REG_t CC110_PKTSTATUS_DATA;
static CC110_TXBYTES_REG_t CC110_TXBYTES_DATA;
static CC110_RXBYTES_REG_t CC110_RXBYTES_DATA;

// Configuration Registers
static CC110_IOCFG2_REG_t CC110_IOCFG2_DATA;
static CC110_IOCFG1_REG_t CC110_IOCFG1_DATA;
static CC110_IOCFG0_REG_t CC110_IOCFG0_DATA;
static CC110_FIFOTHR_REG_t CC110_FIFOTHR_DATA;
static CC110_SYNC1_REG_t CC110_SYNC1_DATA;
static CC110_SYNC2_REG_t CC110_SYNC2_DATA;
static CC110_PKTLEN_REG_t CC110_PKTLEN_DATA;
static CC110_PKTCTRL1_REG_t CC110_PKTCTRL1_DATA;
static CC110_PKTCTRL0_REG_t CC110_PKTCTRL0_DATA;
static CC110_ADDR_REG_t CC110_ADDR_DATA;
static CC110_CHANNR_REG_t CC110_CHANNR_DATA;
static CC110_FSCTRL1_REG_t CC110_FSCTRL1_DATA;
static CC110_FSCTRL0_REG_t CC110_FSCTRL0_DATA;
static CC110_FREQ2_REG_t CC110_FREQ2_DATA;
static CC110_FREQ1_REG_t CC110_FREQ1_DATA;
static CC110_FREQ0_REG_t CC110_FREQ0_DATA;
static CC110_MDMCFG4_REG_t CC110_MDMCFG4_DATA;
static CC110_MDMCFG3_REG_t CC110_MDMCFG3_DATA;
static CC110_MDMCFG2_REG_t CC110_MDMCFG2_DATA;
static CC110_MDMCFG1_REG_t CC110_MDMCFG1_DATA;
static CC110_MDMCFG0_REG_t CC110_MDMCFG0_DATA;
static CC110_DEVIATN_REG_t CC110_DEVIATN_DATA;
static CC110_MCSM2_REG_t CC110_MCSM2_DATA;
static CC110_MCSM1_REG_t CC110_MCSM1_DATA;
static CC110_MCSM0_REG_t CC110_MCSM0_DATA;
static CC110_FOCCFG_REG_t CC110_FOCCFG_DATA;
static CC110_BSCFG_REG_t CC110_BSCFG_DATA;
static CC110_AGCCTRL2_REG_t CC110_AGCCTRL2_DATA;
static CC110_AGCCTRL1_REG_t CC110_AGCCTRL1_DATA;
static CC110_AGCCTRL0_REG_t CC110_AGCCTRL0_DATA;
static CC110_FREND1_REG_t CC110_FREND1_DATA;
static CC110_FREND0_REG_t CC110_FREND0_DATA;
static CC110_FSCAL3_REG_t CC110_FSCAL3_DATA;
static CC110_FSCAL2_REG_t CC110_FSCAL2_DATA;
static CC110_FSCAL1_REG_t CC110_FSCAL1_DATA;
static CC110_FSCAL0_REG_t CC110_FSCAL0_DATA;
static CC110_TEST2_REG_t CC110_TEST2_DATA;
static CC110_TEST1_REG_t CC110_TEST1_DATA;
static CC110_TEST0_REG_t CC110_TEST0_DATA;

// RSSI
static int16_t cc110_rssiDbm = 0;

// Busy/Tx/Rx main status for writing to/from FIFOs
// TODO: Convert as much as possible to structs and enums
static int8_t cc110_connectedStatus = CC110_OK;
static uint8_t cc110_txRxStatus = CC110_TXRX_BUSY;

// Buffers for OTA data in/out of CC110
static uint8_t cc110_dmaBufTx[CC110_FIFO_SIZE + 1] = { 0 };
static uint8_t cc110_dmaBufRx[CC110_FIFO_SIZE + 1] = { 0 };

static uint8_t cc110_txBuf[CC110_TXRX_BUF_SIZE] = { 0 };
static uint32_t cc110_txBufReadIndex = 0;
static uint32_t cc110_txBufWriteIndex = 0;
static int8_t cc110_txBufStatus = CC110_BUF_EMPTY;
static uint8_t cc110_txBufWriteLock = CC110_BUF_WRITE_FREE;

static uint8_t cc110_rxBuf[CC110_TXRX_BUF_SIZE] = { 0 };
static uint32_t cc110_rxBufReadIndex = 0;
static uint32_t cc110_rxBufWriteIndex = 0;
static int8_t cc110_rxBufStatus = CC110_BUF_EMPTY;
static uint8_t cc110_rxBufWriteLock = CC110_BUF_WRITE_FREE;

/* Private function prototypes -----------------------------------------------*/
static inline void cc110_spi_xfer(uint8_t* txData, uint8_t* rxData, uint16_t len);
static int8_t cc110_reset(void);
static inline uint8_t cc110_readRegister(uint8_t addr);
static inline uint8_t cc110_burstReadRegister(uint8_t addr);
static inline void cc110_writeRegister(uint8_t addr, uint8_t data);
static inline void cc110_burstWrite(uint8_t addr, uint8_t* data, uint8_t len);
static inline void cc110_strobeCmd(uint8_t cmd);
static void cc110_getStatus(void);
static void cc110_getConfig(void);
static void cc110_dumpRegisters(void);

/* Public function definitions -----------------------------------------------*/
int8_t cc110_init(void)
{
    int8_t retval = CC110_OK;

    // De-init timer that takes care of tx/rx operations (this should already be done)
    HAL_TIM_Base_MspDeInit(&htim10);

    // Reset Device
    cc110_connectedStatus = cc110_reset();
    if (cc110_connectedStatus == CC110_OK)
    {
        log_info("CC110 Connected");

        // Force to IDLE state
        cc110_strobeCmd(CC110_SIDLE_ADDR);

        // Flush FIFOs
        cc110_strobeCmd(CC110_SFRX_ADDR);
        cc110_strobeCmd(CC110_SFTX_ADDR);

        // Grab latest status from status registers
        cc110_getStatus();

        // From SmartRF Studio
        // Settings are as follows
        // Base Freq: 914.999908 MHz
        // Channel Number: 0
        // Channel Spacing: 199.814 kHz
        // Xtal Freq: 27.0 MHz
        // Data rate: 1.201 kbaud
        // Rx filter BW: 60.267 kHz
        // Manchester Encoding Disabled
        // Modulation format: GFSK
        // Deviation: 4.944 kHz
        // Tx Power: -15 dBm
        // Packet

        // Default GDO2 and GDO1 Config
#define SMARTRF_SETTING_IOCFG0          0x06    // 0x06 for GDO0 config
        // 0xD391 Sync word
        // Packet len of 255 (changed later)
        // CRC autoflush disabled; append status enabled,
//#define SMARTRF_SETTING_FIFOTHR         0x47    // ADC retention enabled; Close in RX 0; FIFO thr 32/33
#define SMARTRF_SETTING_FIFOTHR         0xb00110111     // ADC retention disabled; Close in RX 18 dB; FIFO thr 32/33
#define SMARTRF_SETTING_PKTCTRL0        0x05    //
#define SMARTRF_SETTING_FSCTRL1         0x06
#define SMARTRF_SETTING_FREQ2           0x21
#define SMARTRF_SETTING_FREQ1           0xE3
#define SMARTRF_SETTING_FREQ0           0x8E
#define SMARTRF_SETTING_MDMCFG4         0xF5
#define SMARTRF_SETTING_MDMCFG3         0x75
//#define SMARTRF_SETTING_MDMCFG2         0x13    // GFSK, 30/32 + carrier sense above thresh
#define SMARTRF_SETTING_MDMCFG2         0x11    // GFSK, 15/16 Sync words detected
#define SMARTRF_SETTING_MDMCFG0         0xE5
#define SMARTRF_SETTING_DEVIATN         0x14
#define SMARTRF_SETTING_MCSM0           0x18
#define SMARTRF_SETTING_FOCCFG          0x16
#define SMARTRF_SETTING_RESERVED_0X20   0xFB
#define SMARTRF_SETTING_FSCAL3          0xE9
#define SMARTRF_SETTING_FSCAL2          0x2A
#define SMARTRF_SETTING_FSCAL1          0x00
#define SMARTRF_SETTING_FSCAL0          0x1F
#define SMARTRF_SETTING_TEST2           0x81
#define SMARTRF_SETTING_TEST1           0x35
#define SMARTRF_SETTING_TEST0           0x09

        cc110_writeRegister(CC110_IOCFG0_ADDR, SMARTRF_SETTING_IOCFG0);
        cc110_writeRegister(CC110_FIFOTHR_ADDR, SMARTRF_SETTING_FIFOTHR);
        //cc110_writeRegister(CC110_PKTLEN_ADDR, SMARTRF_SETTING_PKTLEN);
        cc110_writeRegister(CC110_PKTCTRL0_ADDR, SMARTRF_SETTING_PKTCTRL0);
        cc110_writeRegister(CC110_FSCTRL1_ADDR, SMARTRF_SETTING_FSCTRL1);
        cc110_writeRegister(CC110_FREQ2_ADDR, SMARTRF_SETTING_FREQ2);
        cc110_writeRegister(CC110_FREQ1_ADDR, SMARTRF_SETTING_FREQ1);
        cc110_writeRegister(CC110_FREQ0_ADDR, SMARTRF_SETTING_FREQ0);
        cc110_writeRegister(CC110_MDMCFG4_ADDR, SMARTRF_SETTING_MDMCFG4);
        cc110_writeRegister(CC110_MDMCFG3_ADDR, SMARTRF_SETTING_MDMCFG3);
        cc110_writeRegister(CC110_MDMCFG2_ADDR, SMARTRF_SETTING_MDMCFG2);
        cc110_writeRegister(CC110_MDMCFG0_ADDR, SMARTRF_SETTING_MDMCFG0);
        cc110_writeRegister(CC110_MCSM0_ADDR, SMARTRF_SETTING_MCSM0);
        cc110_writeRegister(CC110_FOCCFG_ADDR, SMARTRF_SETTING_FOCCFG);
        cc110_writeRegister(0x20, SMARTRF_SETTING_RESERVED_0X20);
        cc110_writeRegister(CC110_FSCAL3_ADDR, SMARTRF_SETTING_FSCAL3);
        cc110_writeRegister(CC110_FSCAL2_ADDR, SMARTRF_SETTING_FSCAL2);
        cc110_writeRegister(CC110_FSCAL1_ADDR, SMARTRF_SETTING_FSCAL1);
        cc110_writeRegister(CC110_FSCAL0_ADDR, SMARTRF_SETTING_FSCAL0);
        cc110_writeRegister(CC110_TEST2_ADDR, SMARTRF_SETTING_TEST2);
        cc110_writeRegister(CC110_TEST1_ADDR, SMARTRF_SETTING_TEST1);
        cc110_writeRegister(CC110_TEST0_ADDR, SMARTRF_SETTING_TEST0);
        cc110_writeRegister(CC110_PATABLE_ADDR, POWER_m30DBM);

        cc110_writeRegister(CC110_PKTLEN_ADDR, 0x02);           // Packet length of 2 bytes
        cc110_writeRegister(CC110_PKTCTRL1_ADDR, 0b00000000);   // No addr checking, append status disable
        cc110_writeRegister(CC110_PKTCTRL0_ADDR, 0b00000000); // Set to fixed packet length mode, CRC disabled
        //cc110_writeRegister(CC110_MCSM1_ADDR, 0b00001110);      // Set always CCA, stay in Rx after rx packet, and stay in tx after sending packet
        cc110_writeRegister(CC110_MCSM1_ADDR, 0b00001100); // Set always CCA, stay in Rx after rx packet, and goto IDLE after sending packet

        cc110_dumpRegisters();
        cc110_txRxStatus = CC110_TXRX_BUSY;

        // Re-init the timer that takes care of reading/writing to/from the FIFOs
        HAL_TIM_Base_MspInit(&htim10);

    }
    else
    {
        retval = CC110_FAIL;
        log_warn("CC110 device not found/unable to reset");
    }

    return (retval);
}

void cc110_txTest(void)
{
    static uint32_t index;                // Keeps track of number of calls to this function
    uint8_t msg[] = "Hello World n";
#define RX_MSG_BYTES         10
    static uint8_t rx_msg[RX_MSG_BYTES];

    if (cc110_connectedStatus == CC110_OK)
    {
        // Turn LED on to indicate we're starting the loop
        HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);

        // Grab latest status
        cc110_getStatus();

        // Tell console we're doing something
        msg[13] = ('%c', index);
        log_info("%d - Writing %s to tx cbuf and tx'ing", index, msg);

        // Write a bit of dummy data to txBuf
        cc110_writeTxBuf(msg, ustrlen(msg));

        // Set state to tx
        cc110_txRxStatus = CC110_TXRX_TX;

        // Check current state
        if (CC110_STATUS_DATA.STATE == CC110_STATUS_BYTE_STATE_TX)
        {
            log_info("CC110 in TX state");
        }
        else
        {
            log_info("CC110 in %x state", CC110_STATUS_DATA.STATE);
        }

        /*
         // Switch to Rx mode
         log_info("Switching to Rx Mode...");

         // TODO: Make a function call around tx/rx that sets this state
         cc110_txRxStatus = CC110_TXRX_RX;
         cc110_strobeCmd(CC110_SRX_ADDR);

         HAL_Delay(1);

         // Check current state
         if (CC110_STATUS_DATA.STATE == CC110_STATUS_BYTE_STATE_RX)
         {
         log_info("CC110 in RX state, RSSI: %d dBm", cc110_rssiDbm);
         }
         else
         {
         log_info("CC110 in %x state", CC110_STATUS_DATA.STATE);
         }

         // Print out last RX_MSG_BYTES from rx buffer
         for (uint8_t i = 0; i<RX_MSG_BYTES; i++)
         {
         cc110_readRxBuf(rx_msg, RX_MSG_BYTES);
         log_info("RX'd: %d", rx_msg[i]);
         }
         */

        index++;

        // Turn off LED at end of function call
        HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);
    }
}

void cc110_rxTest(void)
{
#define RX_MSG_BYTES         10
    static uint8_t rx_msg[RX_MSG_BYTES];

    // Set Rx State
    cc110_txRxStatus = CC110_TXRX_RX;

    // Grab latest status
    cc110_getStatus();

    log_info("RSSI: %d dBm", cc110_rssiDbm);

    cc110_readRxBuf(rx_msg, RX_MSG_BYTES);
    log_info("MSG: %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x ", rx_msg[0], rx_msg[1],
             rx_msg[2], rx_msg[3], rx_msg[4], rx_msg[5], rx_msg[6], rx_msg[7], rx_msg[8],
             rx_msg[9]);
    log_info("MSG: %c%c%c%c%c%c%c%c%c%c", rx_msg[0], rx_msg[1], rx_msg[2], rx_msg[3], rx_msg[4],
             rx_msg[5], rx_msg[6], rx_msg[7], rx_msg[8], rx_msg[9]);
}

// Handles tx and rx FIFO operations.
// This function should be called in a timer ISR that is set to trigger at a 33.784 kHz rate (~67.5 kbps OTA)
// TX/RX DMA Complete callback should reset the Spi_DmaTxRxComplete flag
// Every time this function is called the next available data from the tx buf will be transmitted and the
// last data received from the CC110 will be put into the rx buf
void cc110_txrx(void)
{
    static uint8_t tx_bytes_free;
    static uint8_t rx_bytes_avaiable;
    static uint8_t rx_bytes_avaiable_previous;
    static uint8_t rx_data_ready = 0;
    static uint8_t dma_index = 0;

    // Check for previous transfer complete
    if (Spi_DmaTxRxComplete != 1)
    {
        log_error("SPI DMA TxRx not complete!");
    }
    else if (Spi_Busy != 0)
    {
        // Do nothing if SPI is still busy
    }
    else
    {
        switch (cc110_txRxStatus)
        {
            case CC110_TXRX_BUSY:
                // Do nothing if we're not ready to read/write from FIFOs
                rx_data_ready = 0; // When not in Rx mode set the flag that a new SPI xfer has to happen to have valid data
                break;

            case CC110_TXRX_TX:     // Transmit mode
                rx_data_ready = 0; // When not in Rx mode set the flag that a new SPI xfer has to happen to have valid data
                // Get number of bytes available in CC110 TX FIFO
                CC110_TXBYTES_DATA.data = cc110_burstReadRegister(CC110_TXBYTES_ADDR);
                tx_bytes_free = CC110_FIFO_SIZE - CC110_TXBYTES_DATA.NUM_TXBYTES;

                // Ack any underflow if there is one
                if (CC110_STATUS_DATA.STATE == CC110_STATUS_BYTE_STATE_TXFIFO_UNDERFLOW)
                {
                    cc110_strobeCmd(CC110_SFTX_ADDR);
                }

                // Copy TX bytes for the next transfer from the tx cbuf to the DMA buffer
                // Check if Tx buf is empty
                if (cc110_txBufStatus != CC110_BUF_EMPTY)
                {
                    // Copy command word into first byte of DMA buffer
                    cc110_dmaBufTx[0] = CC110_BURST_WRITE | CC110_TXFIFO_ADDR;

                    // Reset DMA index
                    dma_index = 1;

                    // Copy as much data as possible to DMA buffer (bytes free in tx buffer or until tx buf is empty)
                    while ((cc110_txBufReadIndex != cc110_txBufWriteIndex)
                            && (dma_index < tx_bytes_free))
                    {
                        cc110_dmaBufTx[dma_index] = cc110_txBuf[cc110_txBufReadIndex];
                        cc110_txBufReadIndex = (cc110_txBufReadIndex + 1) % CC110_TXRX_BUF_SIZE;
                        dma_index++;
                    }

                    // Update buffer status if we emptied the whole buffer
                    if (cc110_txBufReadIndex == cc110_txBufWriteIndex)
                    {
                        cc110_txBufStatus = CC110_BUF_EMPTY;
                    }

                    // Start SPI DMA
                    // dma_index will indicate total number of bytes to transfer (including cmd byte)
                    // TODO: do this with DMA
                    cc110_burstWrite(CC110_TXFIFO_ADDR, &cc110_dmaBufTx[1], dma_index - 1);
                    //cc110_spi_xfer(cc110_dmaBufTx, cc110_dmaBufRx, dma_index);
                    //HAL_SPI_TransmitReceive_DMA(&hspi2, cc110_dmaBufTx, cc110_dmaBufRx, dma_index);

                    // Set CC110 to Tx state now that there is data in the FIFO
                    cc110_strobeCmd(CC110_STX_ADDR);
                }
                else
                {
                    // Do nothing, buffer is empty and we can't write data to the CC110
                    //log_debug("No data to send, Tx buf is empty...");
                }
                break;

            case CC110_TXRX_RX:     // Receive mode

                //DEBUG:
                cc110_getStatus();

                // If there's an overflow ack it and tell the radio data is not ready
                if (CC110_STATUS_DATA.STATE == CC110_STATUS_BYTE_STATE_RXFIFO_OVERFLOW)
                {
                    cc110_strobeCmd(CC110_SFRX_ADDR);
                    rx_data_ready = 0;
                }

                // Get number of bytes available in CC110 RX FIFO
                rx_bytes_avaiable_previous = rx_bytes_avaiable;
                CC110_RXBYTES_DATA.data = cc110_burstReadRegister(CC110_RXBYTES_ADDR);
                rx_bytes_avaiable = CC110_RXBYTES_DATA.NUM_RXBYTES;

                if (rx_data_ready == 1)
                {
                    // Copy command word into first byte of DMA buffer
                    cc110_dmaBufTx[0] = CC110_BURST_READ | CC110_RXFIFO_ADDR;

                    // Reset DMA index
                    dma_index = 1;

                    // Copy RX bytes from last transfer from the DMA buffer to the rx cbuf
                    // Check if the Rx buf is full
                    if (cc110_rxBufWriteIndex
                            == ((cc110_rxBufReadIndex - 1 + CC110_TXRX_BUF_SIZE)
                                    % CC110_TXRX_BUF_SIZE))
                    {
                        cc110_rxBufStatus = CC110_BUF_FULL;
                    }
                    else
                    {
                        // Read out all data from previous SPI xfer until the DMA buffer is empty or the rx buffer is full
                        cc110_rxBufStatus = CC110_BUF_OK;
                        cc110_rxBufWriteLock = CC110_BUF_WRITE_BUSY;
                        while ((cc110_rxBufWriteIndex
                                != ((cc110_rxBufReadIndex - 1 + CC110_TXRX_BUF_SIZE)
                                        % CC110_TXRX_BUF_SIZE))
                                && (dma_index < rx_bytes_avaiable_previous))
                        {
                            cc110_rxBuf[cc110_rxBufWriteIndex] = cc110_dmaBufRx[dma_index];
                            dma_index++;
                            cc110_rxBufWriteIndex = (cc110_rxBufWriteIndex + 1)
                                    % CC110_TXRX_BUF_SIZE;
                        }
                        cc110_rxBufWriteLock = CC110_BUF_WRITE_FREE;
                    }
                }
                else
                {
                    // Set radio to RX state
                    cc110_strobeCmd(CC110_SRX_ADDR);
                    rx_data_ready = 1;       // Reset flag for data ready
                }

                // Perform SPI DMA transfer for number of bytes available in the RX FIFO
                // TODO: do this with DMA
                //HAL_SPI_TransmitReceive_DMA(&hspi2, cc110_dmaBufTx, cc110_dmaBufRx, rx_bytes_avaiable + 1);
                cc110_spi_xfer(cc110_dmaBufTx, cc110_dmaBufRx, rx_bytes_avaiable + 1);
                break;

            default:
                rx_data_ready = 0; // When not in Rx mode set the flag that a new SPI xfer has to happen to have valid data
                break;
        }
    }
}

void cc110_writeTxBuf(uint8_t* txData, uint16_t len)
{
    uint16_t index = 0;

    // Check if the Tx buf is full
    if (cc110_txBufWriteIndex
            == ((cc110_txBufReadIndex - 1 + CC110_TXRX_BUF_SIZE) % CC110_TXRX_BUF_SIZE))
    {
        cc110_txBufStatus = CC110_BUF_FULL;
        log_warn("Write attempted on CC110 txBuf when full");
    }
    else
    {
        // Write data to tx buf
        cc110_txBufStatus = CC110_BUF_OK;
        cc110_txBufWriteLock = CC110_BUF_WRITE_BUSY;
        while ((cc110_txBufWriteIndex != ((cc110_txBufReadIndex - 1 + CC110_TXRX_BUF_SIZE)))
                && (index < len))
        {
            cc110_txBuf[cc110_txBufWriteIndex] = txData[index];
            index++;
            cc110_txBufWriteIndex = (cc110_txBufWriteIndex + 1) % CC110_TXRX_BUF_SIZE;
        }
        cc110_txBufWriteLock = CC110_BUF_WRITE_FREE;
    }
}

void cc110_readRxBuf(uint8_t* rxData, uint16_t len)
{
    uint16_t index = 0;

    // Check if Rx buf is empty
    if (cc110_rxBufStatus != CC110_BUF_EMPTY)
    {
        // Copy as much data as possible to DMA buffer (bytes free in tx buffer or until tx buf is empty)
        while ((cc110_rxBufReadIndex != cc110_rxBufWriteIndex) && (index < len))
        {
            rxData[index] = cc110_rxBuf[cc110_rxBufReadIndex];
            cc110_rxBufReadIndex = (cc110_rxBufReadIndex + 1) % CC110_TXRX_BUF_SIZE;
            index++;
        }

        // Update buffer status if we emptied the whole buffer
        if (cc110_txBufReadIndex == cc110_txBufWriteIndex)
        {
            cc110_txBufStatus = CC110_BUF_EMPTY;
        }
    }
    else
    {
        log_warn("Read attempted on CC110 rxBuf when empty");
    }
}

/* Private function definitions ----------------------------------------------*/
static inline void cc110_spi_xfer(uint8_t* txData, uint8_t* rxData, uint16_t len)
{
    Spi_start();
    HAL_SPI_TransmitReceive(&hspi2, txData, rxData, len, HAL_MAX_DELAY);
    Spi_stop();
}

static int8_t cc110_reset(void)
{
    // TODO: Fix retval stuff & make timeouts shorter
    GPIO_InitTypeDef GPIO_InitStruct = { 0 };
    static const uint8_t timeoutMs = 10;
    static uint8_t timeMs = 0;
    int8_t retval = CC110_OK;

    // De-init SPI, set SCK output, 1; MOSI output, 0; MISO input
    HAL_SPI_MspDeInit(&hspi2);

    GPIO_InitStruct.Pin = SPI2_SCK_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(SPI2_SCK_Port, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = SPI2_MOSI_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(SPI2_MOSI_Port, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = SPI2_MISO_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(SPI2_MISO_Port, &GPIO_InitStruct);

    HAL_GPIO_WritePin(SPI2_SCK_Port, SPI2_SCK_Pin, GPIO_PIN_SET);
    HAL_Delay(1);
    HAL_GPIO_WritePin(SPI2_MOSI_Port, SPI2_MOSI_Pin, GPIO_PIN_RESET);

    // Strobe CSn Low/High, Hold CSn High for min 40 usec
    HAL_GPIO_WritePin(SPI2_NSS_GPIO_Port, SPI2_NSS_Pin, GPIO_PIN_RESET);
    HAL_Delay(1);
    HAL_GPIO_WritePin(SPI2_NSS_GPIO_Port, SPI2_NSS_Pin, GPIO_PIN_SET);
    HAL_Delay(1);

    // Pull CSn Low and wait for MISO to go low
    HAL_GPIO_WritePin(SPI2_NSS_GPIO_Port, SPI2_NSS_Pin, GPIO_PIN_RESET);
    while ((HAL_GPIO_ReadPin(SPI2_MISO_Port, SPI2_MISO_Pin) != GPIO_PIN_RESET)
            && (timeMs < timeoutMs))
    {
        HAL_Delay(1);
        timeMs++;
    }
    if (timeMs >= timeoutMs)
    {
        retval = CC110_FAIL;
        log_warn("Timed out waiting for MISO to go low");
    }

    // Re-init SPI
    HAL_SPI_MspInit(&hspi2);

    // Issues SRES strobe
    cc110_strobeCmd(CC110_SRES_ADDR);

    // Poll for CC110 status via NOP strobes until Status == 0x0F
    timeMs = 0;
    while ((CC110_STATUS_DATA.data != 0x0F) && (timeMs < timeoutMs))
    {
        cc110_strobeCmd(CC110_SNOP_ADDR);
        HAL_Delay(1);
        timeMs++;
    }

    if (timeMs >= timeoutMs)
    {
        retval = CC110_FAIL;
        ;
        log_warn("Timed out waiting for STATUS == 0x0F");
    }

    return (retval);
}

static inline uint8_t cc110_readRegister(uint8_t addr)
{
    uint8_t tmp = 0;
    uint8_t tx_buf[2] = {
    CC110_BYTE_READ | addr, tmp };
    uint8_t rx_buf[2] = { 0 };

    cc110_spi_xfer(tx_buf, rx_buf, 2);
    CC110_STATUS_DATA.data = rx_buf[0];
    return (rx_buf[1]);
}

static inline uint8_t cc110_burstReadRegister(uint8_t addr)
{
    uint8_t tmp = 0;
    uint8_t tx_buf[2] = {
    CC110_BURST_READ | addr, tmp };
    uint8_t rx_buf[2] = { 0 };

    cc110_spi_xfer(tx_buf, rx_buf, 2);
    CC110_STATUS_DATA.data = rx_buf[0];
    return (rx_buf[1]);
}

static inline void cc110_writeRegister(uint8_t addr, uint8_t data)
{
    uint8_t tx_buf[2] = { CC110_BYTE_WRITE | addr, data };
    uint8_t rx_buf[2] = { 0 };

    cc110_spi_xfer(tx_buf, rx_buf, 2);
    CC110_STATUS_DATA.data = rx_buf[0];
}

static inline void cc110_burstWrite(uint8_t addr, uint8_t* data, uint8_t len)
{
    static uint8_t rx_buf[CC110_FIFO_SIZE + 1];
    static uint8_t tx_buf[CC110_FIFO_SIZE + 1];
    tx_buf[0] = CC110_BURST_WRITE | addr;

    // Copy into FIFO
    memcpy(&tx_buf[1], data, len);

    // Write FIFO to CC110
    cc110_spi_xfer(tx_buf, rx_buf, len + 1);
    CC110_STATUS_DATA.data = rx_buf[0];
}

static inline void cc110_strobeCmd(uint8_t cmd)
{
    static uint8_t rx_buf[1];
    cmd = cmd | CC110_BYTE_WRITE;
    cc110_spi_xfer(&cmd, rx_buf, 1);
    CC110_STATUS_DATA.data = rx_buf[0];
}

static void cc110_dumpRegisters(void)
{
    cc110_getStatus();
    cc110_getConfig();

    // Dump status registers
    log_info("=== CC110 STATUS REGISTERS ===");
    log_info("CC110_PARTNUM_DATA: %02x", CC110_PARTNUM_DATA.data);
    log_info("CC110_VERSION_DATA: %02x", CC110_VERSION_DATA.data);
    log_info("CC110_FREQEST_DATA: %02x", CC110_FREQEST_DATA.data);
    log_info("CC110_CRCOK_DATA: %02x", CC110_CRCOK_DATA.data);
    log_info("CC110_RSSI_DATA: %02x", CC110_RSSI_DATA.data);
    log_info("CC110_MARCSTATE_DATA: %02x", CC110_MARCSTATE_DATA.data);
    log_info("CC110_PKTSTATUS_DATA: %02x", CC110_PKTSTATUS_DATA.data);
    log_info("CC110_TXBYTES_DATA: %02x", CC110_TXBYTES_DATA.data);
    log_info("CC110_RXBYTES_DATA: %02x", CC110_RXBYTES_DATA.data);

    // Dump config registers
    log_info("=== CC110 CONFIG REGISTERS ===");
    log_info("CC110_IOCFG2_DATA: %02x", CC110_IOCFG2_DATA.data);
    log_info("CC110_IOCFG1_DATA: %02x", CC110_IOCFG1_DATA.data);
    log_info("CC110_IOCFG0_DATA: %02x", CC110_IOCFG0_DATA.data);
    log_info("CC110_FIFOTHR_DATA: %02x", CC110_FIFOTHR_DATA.data);
    log_info("CC110_SYNC1_DATA: %02x", CC110_SYNC1_DATA.data);
    log_info("CC110_SYNC2_DATA: %02x", CC110_SYNC2_DATA.data);
    log_info("CC110_PKTLEN_DATA: %02x", CC110_PKTLEN_DATA.data);
    log_info("CC110_PKTCTRL1_DATA: %02x", CC110_PKTCTRL1_DATA.data);
    log_info("CC110_PKTCTRL0_DATA: %02x", CC110_PKTCTRL0_DATA.data);
    log_info("CC110_ADDR_DATA: %02x", CC110_ADDR_DATA.data);
    log_info("CC110_CHANNR_DATA: %02x", CC110_CHANNR_DATA.data);
    log_info("CC110_FSCTRL1_DATA: %02x", CC110_FSCTRL1_DATA.data);
    log_info("CC110_FSCTRL0_DATA: %02x", CC110_FSCTRL0_DATA.data);
    log_info("CC110_FREQ2_DATA: %02x", CC110_FREQ2_DATA.data);
    log_info("CC110_FREQ1_DATA: %02x", CC110_FREQ1_DATA.data);
    log_info("CC110_FREQ0_DATA: %02x", CC110_FREQ0_DATA.data);
    log_info("CC110_MDMCFG4_DATA: %02x", CC110_MDMCFG4_DATA.data);
    log_info("CC110_MDMCFG3_DATA: %02x", CC110_MDMCFG3_DATA.data);
    log_info("CC110_MDMCFG2_DATA: %02x", CC110_MDMCFG2_DATA.data);
    log_info("CC110_MDMCFG1_DATA: %02x", CC110_MDMCFG1_DATA.data);
    log_info("CC110_MDMCFG0_DATA: %02x", CC110_MDMCFG0_DATA.data);
    log_info("CC110_DEVIATN_DATA: %02x", CC110_DEVIATN_DATA.data);
    log_info("CC110_MCSM2_DATA: %02x", CC110_MCSM2_DATA.data);
    log_info("CC110_MCSM1_DATA: %02x", CC110_MCSM1_DATA.data);
    log_info("CC110_MCSM0_DATA: %02x", CC110_MCSM0_DATA.data);
    log_info("CC110_FOCCFG_DATA: %02x", CC110_FOCCFG_DATA.data);
    log_info("CC110_BSCFG_DATA: %02x", CC110_BSCFG_DATA.data);
    log_info("CC110_AGCCTRL2_DATA: %02x", CC110_AGCCTRL2_DATA.data);
    log_info("CC110_AGCCTRL1_DATA: %02x", CC110_AGCCTRL1_DATA.data);
    log_info("CC110_AGCCTRL0_DATA: %02x", CC110_AGCCTRL0_DATA.data);
    log_info("CC110_FREND1_DATA: %02x", CC110_FREND1_DATA.data);
    log_info("CC110_FREND0_DATA: %02x", CC110_FREND0_DATA.data);
    log_info("CC110_FSCAL3_DATA: %02x", CC110_FSCAL3_DATA.data);
    log_info("CC110_FSCAL2_DATA: %02x", CC110_FSCAL2_DATA.data);
    log_info("CC110_FSCAL1_DATA: %02x", CC110_FSCAL1_DATA.data);
    log_info("CC110_FSCAL0_DATA: %02x", CC110_FSCAL0_DATA.data);
    log_info("CC110_TEST2_DATA: %02x", CC110_TEST2_DATA.data);
    log_info("CC110_TEST1_DATA: %02x", CC110_TEST1_DATA.data);
    log_info("CC110_TEST0_DATA: %02x", CC110_TEST0_DATA.data);
}

static void cc110_getStatus(void)
{
    // Grab latest status
    CC110_PARTNUM_DATA.data = cc110_burstReadRegister(CC110_PARTNUM_ADDR);
    CC110_VERSION_DATA.data = cc110_burstReadRegister(CC110_VERSION_ADDR);
    CC110_FREQEST_DATA.data = cc110_burstReadRegister(CC110_FREQEST_ADDR);
    CC110_CRCOK_DATA.data = cc110_burstReadRegister(CC110_CRCOK_ADDR);
    CC110_RSSI_DATA.data = cc110_burstReadRegister(CC110_RSSI_ADDR);
    CC110_MARCSTATE_DATA.data = cc110_burstReadRegister(CC110_MARCSTATE_ADDR);
    CC110_PKTSTATUS_DATA.data = cc110_burstReadRegister(CC110_PKTSTATUS_ADDR);
    CC110_TXBYTES_DATA.data = cc110_burstReadRegister(CC110_TXBYTES_ADDR);
    CC110_RXBYTES_DATA.data = cc110_burstReadRegister(CC110_RXBYTES_ADDR);

    // Compute RSSI in dBm - See TI DN505
    if (CC110_RSSI_DATA.RSSI >= 128)
    {
        cc110_rssiDbm = (int16_t) (((int16_t) CC110_RSSI_DATA.RSSI - 256) / 2) - CC110_RSSI_OFFSET;
    }
    else
    {
        cc110_rssiDbm = (CC110_RSSI_DATA.RSSI / 2) - CC110_RSSI_OFFSET;
    }
}

static void cc110_getConfig(void)
{
    CC110_IOCFG2_DATA.data = cc110_readRegister(CC110_IOCFG2_ADDR);
    CC110_IOCFG1_DATA.data = cc110_readRegister(CC110_IOCFG1_ADDR);
    CC110_IOCFG0_DATA.data = cc110_readRegister(CC110_IOCFG0_ADDR);
    CC110_FIFOTHR_DATA.data = cc110_readRegister(CC110_FIFOTHR_ADDR);
    CC110_SYNC1_DATA.data = cc110_readRegister(CC110_SYNC1_ADDR);
    CC110_SYNC2_DATA.data = cc110_readRegister(CC110_SYNC2_ADDR);
    CC110_PKTLEN_DATA.data = cc110_readRegister(CC110_PKTLEN_ADDR);
    CC110_PKTCTRL1_DATA.data = cc110_readRegister(CC110_PKTCTRL1_ADDR);
    CC110_PKTCTRL0_DATA.data = cc110_readRegister(CC110_PKTCTRL0_ADDR);
    CC110_ADDR_DATA.data = cc110_readRegister(CC110_ADDR_ADDR);
    CC110_CHANNR_DATA.data = cc110_readRegister(CC110_CHANNR_ADDR);
    CC110_FSCTRL1_DATA.data = cc110_readRegister(CC110_FSCTRL1_ADDR);
    CC110_FSCTRL0_DATA.data = cc110_readRegister(CC110_FSCTRL0_ADDR);
    CC110_FREQ2_DATA.data = cc110_readRegister(CC110_FREQ2_ADDR);
    CC110_FREQ1_DATA.data = cc110_readRegister(CC110_FREQ1_ADDR);
    CC110_FREQ0_DATA.data = cc110_readRegister(CC110_FREQ0_ADDR);
    CC110_MDMCFG4_DATA.data = cc110_readRegister(CC110_MDMCFG4_ADDR);
    CC110_MDMCFG3_DATA.data = cc110_readRegister(CC110_MDMCFG3_ADDR);
    CC110_MDMCFG2_DATA.data = cc110_readRegister(CC110_MDMCFG2_ADDR);
    CC110_MDMCFG1_DATA.data = cc110_readRegister(CC110_MDMCFG1_ADDR);
    CC110_MDMCFG0_DATA.data = cc110_readRegister(CC110_MDMCFG0_ADDR);
    CC110_DEVIATN_DATA.data = cc110_readRegister(CC110_DEVIATN_ADDR);
    CC110_MCSM2_DATA.data = cc110_readRegister(CC110_MCSM2_ADDR);
    CC110_MCSM1_DATA.data = cc110_readRegister(CC110_MCSM1_ADDR);
    CC110_MCSM0_DATA.data = cc110_readRegister(CC110_MCSM0_ADDR);
    CC110_FOCCFG_DATA.data = cc110_readRegister(CC110_FOCCFG_ADDR);
    CC110_BSCFG_DATA.data = cc110_readRegister(CC110_BSCFG_ADDR);
    CC110_AGCCTRL2_DATA.data = cc110_readRegister(CC110_AGCCTRL2_ADDR);
    CC110_AGCCTRL1_DATA.data = cc110_readRegister(CC110_AGCCTRL1_ADDR);
    CC110_AGCCTRL0_DATA.data = cc110_readRegister(CC110_AGCCTRL0_ADDR);
    CC110_FREND1_DATA.data = cc110_readRegister(CC110_FREND1_ADDR);
    CC110_FREND0_DATA.data = cc110_readRegister(CC110_FREND0_ADDR);
    CC110_FSCAL3_DATA.data = cc110_readRegister(CC110_FSCAL3_ADDR);
    CC110_FSCAL2_DATA.data = cc110_readRegister(CC110_FSCAL2_ADDR);
    CC110_FSCAL1_DATA.data = cc110_readRegister(CC110_FSCAL1_ADDR);
    CC110_FSCAL0_DATA.data = cc110_readRegister(CC110_FSCAL0_ADDR);
    CC110_TEST2_DATA.data = cc110_readRegister(CC110_TEST2_ADDR);
    CC110_TEST1_DATA.data = cc110_readRegister(CC110_TEST1_ADDR);
    CC110_TEST0_DATA.data = cc110_readRegister(CC110_TEST0_ADDR);
}

/************************ (C) COPYRIGHT John Fritz *****END OF FILE****/
