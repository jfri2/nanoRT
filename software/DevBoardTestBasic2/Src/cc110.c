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
#include <logger.h>

/* Private variables ---------------------------------------------------------*/
// Status Registers
CC110_PARTNUM_REG_t CC110_PARTNUM_DATA;
CC110_VERSION_REG_t CC110_VERSION_DATA;
CC110_FREQEST_REG_t CC110_FREQEST_DATA;
CC110_CRCOK_REG_t CC110_CRCOK_DATA;
CC110_RSSI_REG_t CC110_RSSI_DATA;
CC110_MARCSTATE_REG_t CC110_MARCSTATE_DATA;
CC110_PKTSTATUS_REG_t CC110_PKTSTATUS_DATA;
CC110_TXBYTES_REG_t CC110_TXBYTES_DATA;
CC110_RXBYTES_REG_t CC110_RXBYTES_DATA;

// Configuration Registers
CC110_IOCFG2_REG_t CC110_IOCFG2_DATA;
CC110_IOCFG1_REG_t CC110_IOCFG1_DATA;
CC110_IOCFG0_REG_t CC110_IOCFG0_DATA;
CC110_FIFOTHR_REG_t CC110_FIFOTHR_DATA;
CC110_SYNC1_REG_t CC110_SYNC1_DATA;
CC110_SYNC2_REG_t CC110_SYNC2_DATA;
CC110_PKTLEN_REG_t CC110_PKTLEN_DATA;
CC110_PKTCTRL1_REG_t CC110_PKTCTRL1_DATA;
CC110_PKTCTRL0_REG_t CC110_PKTCTRL0_DATA;
CC110_ADDR_REG_t CC110_ADDR_DATA;
CC110_CHANNR_REG_t CC110_CHANNR_DATA;
CC110_FSCTRL1_REG_t CC110_FSCTRL1_DATA;
CC110_FSCTRL0_REG_t CC110_FSCTRL0_DATA;
CC110_FREQ2_REG_t CC110_FREQ2_DATA;
CC110_FREQ1_REG_t CC110_FREQ1_DATA;
CC110_FREQ0_REG_t CC110_FREQ0_DATA;
CC110_MDMCFG4_REG_t CC110_MDMCFG4_DATA;
CC110_MDMCFG3_REG_t CC110_MDMCFG3_DATA;
CC110_MDMCFG2_REG_t CC110_MDMCFG2_DATA;
CC110_MDMCFG1_REG_t CC110_MDMCFG1_DATA;
CC110_MDMCFG0_REG_t CC110_MDMCFG0_DATA;
CC110_DEVIATN_REG_t CC110_DEVIATN_DATA;
CC110_MCSM2_REG_t CC110_MCSM2_DATA;
CC110_MCSM1_REG_t CC110_MCSM1_DATA;
CC110_MCSM0_REG_t CC110_MCSM0_DATA;
CC110_FOCCFG_REG_t CC110_FOCCFG_DATA;
CC110_BSCFG_REG_t CC110_BSCFG_DATA;
CC110_AGCCTRL2_REG_t CC110_AGCCTRL2_DATA;
CC110_AGCCTRL1_REG_t CC110_AGCCTRL1_DATA;
CC110_AGCCTRL0_REG_t CC110_AGCCTRL0_DATA;
CC110_FREND1_REG_t CC110_FREND1_DATA;
CC110_FREND0_REG_t CC110_FREND0_DATA;
CC110_FSCAL3_REG_t CC110_FSCAL3_DATA;
CC110_FSCAL2_REG_t CC110_FSCAL2_DATA;
CC110_FSCAL1_REG_t CC110_FSCAL1_DATA;
CC110_FSCAL0_REG_t CC110_FSCAL0_DATA;
CC110_TEST2_REG_t CC110_TEST2_DATA;
CC110_TEST1_REG_t CC110_TEST1_DATA;
CC110_TEST0_REG_t CC110_TEST0_DATA;

/* Private function prototypes -----------------------------------------------*/
uint8_t cc110_readByte(uint8_t addr);
void cc110_getStatus(void);
void cc110_getConfig(void);
void cc110_dumpRegisters(void);

/* Public function definitions -----------------------------------------------*/
void cc110_spi_xfer(uint8_t* txData, uint8_t* rxData, uint16_t len)
{
    HAL_SPI_TransmitReceive(&hspi2, txData, rxData, len, HAL_MAX_DELAY);
}

int8_t cc110_init(void)
{
    int8_t retval = CC110_FAIL;
    // TODO: Init code

    return (retval);
}

void cc110_test(void)
{
    cc110_dumpRegisters();
}

/* Private function definitions ----------------------------------------------*/
uint8_t cc110_readByte(uint8_t addr)
{
    uint8_t tmp = 0;
    uint8_t tx_buf[2] = {
            CC110_BYTE_READ | addr,
            tmp };
    uint8_t rx_buf[2] = { 0 };

    cc110_spi_xfer(tx_buf, rx_buf, 2);

    return (rx_buf[1]);
}

void cc110_dumpRegisters(void)
{
    cc110_getStatus();
    cc110_getConfig();

    // Dump status registers
    log_info("CC110_PARTNUM_DATA: %x", CC110_PARTNUM_DATA.data);
    log_info("CC110_VERSION_DATA: %x", CC110_VERSION_DATA.data);
    log_info("CC110_FREQEST_DATA: %x", CC110_FREQEST_DATA.data);
    log_info("CC110_CRCOK_DATA: %x", CC110_CRCOK_DATA.data);
    log_info("CC110_RSSI_DATA: %x", CC110_RSSI_DATA.data);
    log_info("CC110_MARCSTATE_DATA: %x", CC110_MARCSTATE_DATA.data);
    log_info("CC110_PKTSTATUS_DATA: %x", CC110_PKTSTATUS_DATA.data);
    log_info("CC110_TXBYTES_DATA: %x", CC110_TXBYTES_DATA.data);
    log_info("CC110_RXBYTES_DATA: %x", CC110_RXBYTES_DATA.data);

    // Dump config registers
    log_info("CC110_IOCFG2_DATA: %x", CC110_IOCFG2_DATA.data);
    log_info("CC110_IOCFG1_DATA: %x", CC110_IOCFG1_DATA.data);
    log_info("CC110_IOCFG0_DATA: %x", CC110_IOCFG0_DATA.data);
    log_info("CC110_FIFOTHR_DATA: %x", CC110_FIFOTHR_DATA.data);
    log_info("CC110_SYNC1_DATA: %x", CC110_SYNC1_DATA.data);
    log_info("CC110_SYNC2_DATA: %x", CC110_SYNC2_DATA.data);
    log_info("CC110_PKTLEN_DATA: %x", CC110_PKTLEN_DATA.data);
    log_info("CC110_PKTCTRL1_DATA: %x", CC110_PKTCTRL1_DATA.data);
    log_info("CC110_PKTCTRL0_DATA: %x", CC110_PKTCTRL0_DATA.data);
    log_info("CC110_ADDR_DATA: %x", CC110_ADDR_DATA.data);
    log_info("CC110_CHANNR_DATA: %x", CC110_CHANNR_DATA.data);
    log_info("CC110_FSCTRL1_DATA: %x", CC110_FSCTRL1_DATA.data);
    log_info("CC110_FSCTRL0_DATA: %x", CC110_FSCTRL0_DATA.data);
    log_info("CC110_FREQ2_DATA: %x", CC110_FREQ2_DATA.data);
    log_info("CC110_FREQ1_DATA: %x", CC110_FREQ1_DATA.data);
    log_info("CC110_FREQ0_DATA: %x", CC110_FREQ0_DATA.data);
    log_info("CC110_MDMCFG4_DATA: %x", CC110_MDMCFG4_DATA.data);
    log_info("CC110_MDMCFG3_DATA: %x", CC110_MDMCFG3_DATA.data);
    log_info("CC110_MDMCFG2_DATA: %x", CC110_MDMCFG2_DATA.data);
    log_info("CC110_MDMCFG1_DATA: %x", CC110_MDMCFG1_DATA.data);
    log_info("CC110_MDMCFG0_DATA: %x", CC110_MDMCFG0_DATA.data);
    log_info("CC110_DEVIATN_DATA: %x", CC110_DEVIATN_DATA.data);
    log_info("CC110_MCSM2_DATA: %x", CC110_MCSM2_DATA.data);
    log_info("CC110_MCSM1_DATA: %x", CC110_MCSM1_DATA.data);
    log_info("CC110_MCSM0_DATA: %x", CC110_MCSM0_DATA.data);
    log_info("CC110_FOCCFG_DATA: %x", CC110_FOCCFG_DATA.data);
    log_info("CC110_BSCFG_DATA: %x", CC110_BSCFG_DATA.data);
    log_info("CC110_AGCCTRL2_DATA: %x", CC110_AGCCTRL2_DATA.data);
    log_info("CC110_AGCCTRL1_DATA: %x", CC110_AGCCTRL1_DATA.data);
    log_info("CC110_AGCCTRL0_DATA: %x", CC110_AGCCTRL0_DATA.data);
    log_info("CC110_FREND1_DATA: %x", CC110_FREND1_DATA.data);
    log_info("CC110_FREND0_DATA: %x", CC110_FREND0_DATA.data);
    log_info("CC110_FSCAL3_DATA: %x", CC110_FSCAL3_DATA.data);
    log_info("CC110_FSCAL2_DATA: %x", CC110_FSCAL2_DATA.data);
    log_info("CC110_FSCAL1_DATA: %x", CC110_FSCAL1_DATA.data);
    log_info("CC110_FSCAL0_DATA: %x", CC110_FSCAL0_DATA.data);
    log_info("CC110_TEST2_DATA: %x", CC110_TEST2_DATA.data);
    log_info("CC110_TEST1_DATA: %x", CC110_TEST1_DATA.data);
    log_info("CC110_TEST0_DATA: %x", CC110_TEST0_DATA.data);
}

void cc110_getStatus(void)
{
    CC110_PARTNUM_DATA.data = cc110_readByte(CC110_PARTNUM_ADDR);
    CC110_VERSION_DATA.data = cc110_readByte(CC110_VERSION_ADDR);
    CC110_FREQEST_DATA.data = cc110_readByte(CC110_FREQEST_ADDR);
    CC110_CRCOK_DATA.data = cc110_readByte(CC110_CRCOK_ADDR);
    CC110_RSSI_DATA.data = cc110_readByte(CC110_RSSI_ADDR);
    CC110_MARCSTATE_DATA.data = cc110_readByte(CC110_MARCSTATE_ADDR);
    CC110_PKTSTATUS_DATA.data = cc110_readByte(CC110_PKTSTATUS_ADDR);
    CC110_TXBYTES_DATA.data = cc110_readByte(CC110_TXBYTES_ADDR);
    CC110_RXBYTES_DATA.data = cc110_readByte(CC110_RXBYTES_ADDR);
}

void cc110_getConfig(void)
{
    CC110_IOCFG2_DATA.data = cc110_readByte(CC110_IOCFG2_ADDR);
    CC110_IOCFG1_DATA.data = cc110_readByte(CC110_IOCFG1_ADDR);
    CC110_IOCFG0_DATA.data = cc110_readByte(CC110_IOCFG0_ADDR);
    CC110_FIFOTHR_DATA.data = cc110_readByte(CC110_FIFOTHR_ADDR);
    CC110_SYNC1_DATA.data = cc110_readByte(CC110_SYNC1_ADDR);
    CC110_SYNC2_DATA.data = cc110_readByte(CC110_SYNC2_ADDR);
    CC110_PKTLEN_DATA.data = cc110_readByte(CC110_PKTLEN_ADDR);
    CC110_PKTCTRL1_DATA.data = cc110_readByte(CC110_PKTCTRL1_ADDR);
    CC110_PKTCTRL0_DATA.data = cc110_readByte(CC110_PKTCTRL0_ADDR);
    CC110_ADDR_DATA.data = cc110_readByte(CC110_ADDR_ADDR);
    CC110_CHANNR_DATA.data = cc110_readByte(CC110_CHANNR_ADDR);
    CC110_FSCTRL1_DATA.data = cc110_readByte(CC110_FSCTRL1_ADDR);
    CC110_FSCTRL0_DATA.data = cc110_readByte(CC110_FSCTRL0_ADDR);
    CC110_FREQ2_DATA.data = cc110_readByte(CC110_FREQ2_ADDR);
    CC110_FREQ1_DATA.data = cc110_readByte(CC110_FREQ1_ADDR);
    CC110_FREQ0_DATA.data = cc110_readByte(CC110_FREQ0_ADDR);
    CC110_MDMCFG4_DATA.data = cc110_readByte(CC110_MDMCFG4_ADDR);
    CC110_MDMCFG3_DATA.data = cc110_readByte(CC110_MDMCFG3_ADDR);
    CC110_MDMCFG2_DATA.data = cc110_readByte(CC110_MDMCFG2_ADDR);
    CC110_MDMCFG1_DATA.data = cc110_readByte(CC110_MDMCFG1_ADDR);
    CC110_MDMCFG0_DATA.data = cc110_readByte(CC110_MDMCFG0_ADDR);
    CC110_DEVIATN_DATA.data = cc110_readByte(CC110_DEVIATN_ADDR);
    CC110_MCSM2_DATA.data = cc110_readByte(CC110_MCSM2_ADDR);
    CC110_MCSM1_DATA.data = cc110_readByte(CC110_MCSM1_ADDR);
    CC110_MCSM0_DATA.data = cc110_readByte(CC110_MCSM0_ADDR);
    CC110_FOCCFG_DATA.data = cc110_readByte(CC110_FOCCFG_ADDR);
    CC110_BSCFG_DATA.data = cc110_readByte(CC110_BSCFG_ADDR);
    CC110_AGCCTRL2_DATA.data = cc110_readByte(CC110_AGCCTRL2_ADDR);
    CC110_AGCCTRL1_DATA.data = cc110_readByte(CC110_AGCCTRL1_ADDR);
    CC110_AGCCTRL0_DATA.data = cc110_readByte(CC110_AGCCTRL0_ADDR);
    CC110_FREND1_DATA.data = cc110_readByte(CC110_FREND1_ADDR);
    CC110_FREND0_DATA.data = cc110_readByte(CC110_FREND0_ADDR);
    CC110_FSCAL3_DATA.data = cc110_readByte(CC110_FSCAL3_ADDR);
    CC110_FSCAL2_DATA.data = cc110_readByte(CC110_FSCAL2_ADDR);
    CC110_FSCAL1_DATA.data = cc110_readByte(CC110_FSCAL1_ADDR);
    CC110_FSCAL0_DATA.data = cc110_readByte(CC110_FSCAL0_ADDR);
    CC110_TEST2_DATA.data = cc110_readByte(CC110_TEST2_ADDR);
    CC110_TEST1_DATA.data = cc110_readByte(CC110_TEST1_ADDR);
    CC110_TEST0_DATA.data = cc110_readByte(CC110_TEST0_ADDR);
}

/************************ (C) COPYRIGHT John Fritz *****END OF FILE****/
