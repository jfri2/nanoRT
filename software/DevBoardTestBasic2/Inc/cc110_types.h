/*
 * cc110_types.h
 *
 *  Created on: Jul 27, 2020
 *      Author: jfrit
 */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef CC110_TYPES_H_
#define CC110_TYPES_H_

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

/* Global Definitions --------------------------------------------------------*/
// Register Addresses
#define CC110_BYTE_WRITE        0x00    // Byte write header
#define CC110_BYTE_READ         0x80    // Byte read header
#define CC110_BURST_WRITE       0x40    // Burst write header
#define CC110_BURST_READ        0xC0    // Burst read header

// Command Strobe Addresses
// Note: Command strobes can only be accessed using BYTE_WRITEs or BYTE_READS
#define CC110_SRES_ADDR         0x30    // Reset chip
#define CC110_SFSTXON_ADDR      0x31    // En and cal freq synth and turn it off
#define CC110_SXOFF_ADDR        0x32    // Turn off crystal osc
#define CC110_SCAL_ADDR         0x33    // Cal freq synth and turn it off
#define CC110_SRX_ADDR          0x34    // In IDLE, enable Rx
#define CC110_STX_ADDR          0x35    // In IDLE, enable Tx
#define CC110_SIDLE_ADDR        0x36    // Enter IDLE
#define CC110_SPWD_ADDR         0x39    // Enter power down when CSn goes high
#define CC110_SFRX_ADDR         0x3A    // Flush Rx FIFO
#define CC110_SFTX_ADDR         0x3B    // Flush Tx FIFO
#define CC110_SNOP_ADDR         0x3D    // No operation

// Configuration Register Addresses
#define CC110_IOCFG2_ADDR       0x00    // GDO2 output pin config
#define CC110_IOCFG1_ADDR       0x01    // GDO1 output pin config
#define CC110_IOCFG0_ADDR       0x02    // GDO0 output pin config
#define CC110_FIFOTHR_ADDR      0x03    // Tx & Rx FIFO threshold
#define CC110_SYNC1_ADDR        0x04    // Sync Word high byte
#define CC110_SYNC0_ADDR        0x05    // Sync Word low byte
#define CC110_PKTLEN_ADDR       0x06    // Packet length
#define CC110_PKTCTRL1_ADDR     0x07    // Packet automation control 1
#define CC110_PKTCTRL0_ADDR     0x08    // Packet automation control 0
#define CC110_ADDR_ADDR         0x09    // Device address
#define CC110_CHANNR_ADDR       0xDA    // Channel number
#define CC110_FSCTRL1_ADDR      0xDB    // Freq synth control 1
#define CC110_FSCTRL0_ADDR      0x0C    // Freq synth control 0
#define CC110_FREQ2_ADDR        0x0D    // Freq control high byte
#define CC110_FREQ1_ADDR        0x0E    // Freq control mid byte
#define CC110_FREQ0_ADDR        0x0F    // Freq control low byte
#define CC110_MDMCFG4_ADDR      0x10    // MODEM config 4
#define CC110_MDMCFG3_ADDR      0x11    // MODEM config 3
#define CC110_MDMCFG2_ADDR      0x12    // MODEM config 2
#define CC110_MDMCFG1_ADDR      0x13    // MODEM config 1
#define CC110_MDMCFG0_ADDR      0x14    // MODEM config 0
#define CC110_DEVIATN_ADDR      0x15    // MODEM deviation setting
#define CC110_MCSM2_ADDR        0x16    // Radio control FSM config 2
#define CC110_MCSM1_ADDR        0x17    // Radio control FSM config 1
#define CC110_MCSM0_ADDR        0x18    // Radio control FSM config 0
#define CC110_FOCCFG_ADDR       0x19    // Freq offset compensation config
#define CC110_BSCFG_ADDR        0x1A    // Bit synch config
#define CC110_AGCCTRL2_ADDR     0x1B    // AGC Control 2
#define CC110_AGCCTRL1_ADDR     0x1C    // AGC Control 1
#define CC110_AGCCTRL0_ADDR     0x1D    // AGC Control 0
#define CC110_FREND1_ADDR       0x21    // Front End Rx config
#define CC110_FREND0_ADDR       0x22    // Front End Tx config
#define CC110_FSCAL3_ADDR       0x23    // Freq synth cal 3
#define CC110_FSCAL2_ADDR       0x24    // Freq synth cal 2
#define CC110_FSCAL1_ADDR       0x25    // Freq synth cal 1
#define CC110_FSCAL0_ADDR       0x26    // Freq synth cal 0
#define CC110_TEST2_ADDR        0x2C    // Test 2
#define CC110_TEST1_ADDR        0x2D    // Test 1
#define CC110_TEST0_ADDR        0x2E    // Test 0

// PA Table
#define CC110_PATABLE_ADDR      0x3E    // PA Table

// Status Register Addresses
// Note: Status Registers can only be accessed using BURST_READs
#define CC110_PARTNUM_ADDR      0x30    // CC110x Part Number
#define CC110_VERSION_ADDR      0x31    // Current version number
#define CC110_FREQEST_ADDR      0x32    // Frequency offset estimate
#define CC110_CRCOK_ADDR       0x33    // CRC OK
#define CC110_RSSI_ADDR         0x34    // Receive Signal Strength Indicator
#define CC110_MARCSTATE_ADDR    0x35    // Control State Machine State
#define CC110_PKTSTATUS_ADDR    0x38    // Current GDOx status and packet status
#define CC110_TXBYTES_ADDR      0x3A    // Underflow and number of bytes in Tx FIFO
#define CC110_RXBYTES_ADDR      0x3B    // Overflow and number of bytes in Rx FIFO

// FIFO Addresses
// Note: Tx FIFO is accessed with a BYTE_WRITE or BURST_WRITE, 
//       Rx FIFO is accessed with a BYTE_READ or BURST_READ
#define CC110_TXFIFO_ADDR       0x3F
#define CC110_RXFIFO_ADDR       0x3F

/* Global TypeDefs -----------------------------------------------------------*/
// Status Byte
typedef enum
{
    CC110_STATUS_BYTE_STATE_IDLE = 0x00,
    CC110_STATUS_BYTE_STATE_RX,
    CC110_STATUS_BYTE_STATE_TX,
    CC110_STATUS_BYTE_STATE_FSTXON,
    CC110_STATUS_BYTE_STATE_CALIBRATE,
    CC110_STATUS_BYTE_STATE_SETTLING,
    CC110_STATUS_BYTE_STATE_RXFIFO_OVERFLOW,
    CC110_STATUS_BYTE_STATE_TXFIFO_UNDERFLOW
} CC110_STATUS_BYTE_STATE_t;

typedef union
{
    uint8_t data;
    struct
    {
        uint8_t FIFO_BYTES_AVAILABLE :4;
        CC110_STATUS_BYTE_STATE_t STATE :3;
        uint8_t CHIP_RDYn :1;
    };
} CC110_STATUS_BYTE_t;

// Status Registers - Note: struct goes LSB = Top; MSB = Bottom
// TODO: Convert these to enum types and update below registers, also add comments
typedef union
{
    uint8_t data;
    struct
    {   
        uint8_t PARTNUM :8;
    };
} CC110_PARTNUM_REG_t;

typedef union
{
    uint8_t data;
    struct
    {   
        uint8_t VERSION :5;
    };
} CC110_VERSION_REG_t;

typedef union
{
    uint8_t data;
    struct
    {   
        uint8_t FREQOFF_EST :8;
    };
} CC110_FREQEST_REG_t;

typedef union
{
    uint8_t data;
    struct
    {   
        uint8_t unused :6;
        uint8_t CRC_OK :2;
    };
} CC110_CRCOK_REG_t;

typedef union
{
    uint8_t data;
    struct
    {   
        uint8_t RSSI :8;
    };
} CC110_RSSI_REG_t;

typedef enum
{
    CC110_MARCSTATE_SLEEP = 0x00,
    CC110_MARCSTATE_IDLE,
    CC110_MARCSTATE_XOFF,
    CC110_MARCSTATE_VCOON_MC,
    CC110_MARCSTATE_REGON_MC,
    CC110_MARCSTATE_MANCAL,
    CC110_MARCSTATE_VCOON,
    CC110_MARCSTATE_REGON,
    CC110_MARCSTATE_STARTCAL,
    CC110_MARCSTATE_BWBOOST,
    CC110_MARCSTATE_FS_LOCK,
    CC110_MARCSTATE_IFADCON,
    CC110_MARCSTATE_ENDCAL,
    CC110_MARCSTATE_RX,
    CC110_MARCSTATE_RX_END,
    CC110_MARCSTATE_RX_RST,
    CC110_MARCSTATE_TXRX_SWITCH,
    CC110_MARCSTATE_RXFIFO_OVERFLOW,
    CC110_MARCSTATE_FSTXON,
    CC110_MARCSTATE_TX,
    CC110_MARCSTATE_TX_END,
    CC110_MARCSTATE_RXTX_SWITCH,
    CC110_MARCSTATE_TXFIFO_UNDERFLOW
} CC110_MARCSTATE_t;

typedef union
{
    uint8_t data;
    struct
    {   
        CC110_MARCSTATE_t MARC_STATE :5;
        uint8_t unused :3;
    };
} CC110_MARCSTATE_REG_t;

typedef union
{
    uint8_t data;
    struct
    {   
        uint8_t GDO0 :1;
        uint8_t unused0 :1;
        uint8_t GDO2 :1;
        uint8_t SFD :1;
        uint8_t CCA :1;
        uint8_t unused1 :1;
        uint8_t CS :1;
        uint8_t CRC_OK :1;
    };
} CC110_PKTSTATUS_REG_t;

typedef union
{
    uint8_t data;
    struct
    {   
        uint8_t NUM_TXBYTES :7;
        uint8_t TXFIFO_UNDERFLOW :1;
    };
} CC110_TXBYTES_REG_t;

typedef union
{
    uint8_t data;
    struct
    {   
        uint8_t NUM_RXBYTES :7;
        uint8_t TXFIFO_OVERFLOW :1;
    };
} CC110_RXBYTES_REG_t;

// Configuration Registers - Note: struct goes LSB = Top; MSB = Bottom

// TODO: Convert these to enum types and update below registers
// GDO_INV
#define CC110_GDO_INV_HIGH              0   // GDO Active High
#define CC110_GDO_INV_ALOW              1   // GDO Active Low

// GDO_CFG
#define CC110_GDO_CFG_RXFIFO_FULL   0   // Associated to the RX FIFO: Asserts when RX FIFO is filled at or above the RX FIFO threshold. Deasserts when RX FIFO is drained below the same threshold.
#define CC110_GDO_CFG_RXFIFO_FULLP  1   // Associated to the RX FIFO: Asserts when RX FIFO is filled at or above the RX FIFO threshold or the end of packet is reached. Deasserts when the RX FIFO is empty.
#define CC110_GDO_CFG_TXFIFO_FULLT  2   // Associated to the TX FIFO: Asserts when the TX FIFO is filled at or above the TX FIFO threshold. Deasserts when the TX FIFO is below the same threshold.
#define CC110_GDO_CFG_TXFIFO_FULL   3   // Associated to the TX FIFO: Asserts when TX FIFO is full. Deasserts when the TX FIFO is drained below the TX FIFO threshold.
#define CC110_GDO_CFG_RXFIFO_OVER   4   // Asserts when the RX FIFO has overflowed. Deasserts when the FIFO has been flushed.
#define CC110_GDO_CFG_TXFIFO_UNDER  5   // Asserts when the TX FIFO has underflowed. Deasserts when the FIFO is flushed.
#define CC110_GDO_CFG_SYNC          6   // Asserts when sync word has been sent / received, and de-asserts at the end of the packet. In RX, the pin will also de-assert when a packet is discarded due to address or maximum length filtering or when the radio enters RXFIFO_OVERFLOW state. In TX the pin will de-assert if the TX FIFO underflows.
#define CC110_GDO_CFG_CRCOKP        7   // Asserts when a packet has been received with CRC OK. Deasserts when the first byte is read from the RX FIFO
#define CC110_GDO_CFG_CLR_CH_ASS    9   // Clear channel assessment. High when RSSI level is below threshold (dependent on the current CCA_MODE setting).
#define CC110_GDO_CFG_PLL_LOCK      10  // Lock detector output. The PLL is in lock if the lock detector output has a positive transition or is constantly logic high. To check for PLL lock the lock detector output should be used as an interrupt for the MCU.
#define CC110_GDO_CFG_SCLK          11  // Serial Clock. Synchronous to the data in synchronous serial mode.In RX mode, data is set up on the falling edge by CC110L when GDOx_INV=0.In TX mode, data is sampled by CC110L on the rising edge of the serial clock when GDOx_INV=0.
#define CC110_GDO_CFG_SSDO          12  // Serial Synchronous Data Output. Used for synchronous serial mode.
#define CC110_GDO_CFG_ASDO          13  // Serial Data Output. Used for asynchronous serial mode.
#define CC110_GDO_CFG_CARRIER_SEN   14  // Carrier sense. High if RSSI level is above threshold. Cleared when entering IDLE mode.
#define CC110_GDO_CFG_CRCOK         15  // CRC_OK. The last CRC comparison matched. Cleared when entering/restarting RX mode.
#define CC110_GDO_CFG_PA_PD         27  // PA_PD. Note: PA_PD will have the same signal level in SLEEP and TX states. To control an external PA or RX/TX switch in applications where the SLEEP state is used it is recommended to use GDOx_CFGx=0x2F instead.
#define CC110_GDO_CFG_LNA_PD        28  // LNA_PD. Note: LNA_PD will have the same signal level in SLEEP and RX states. To control an external LNA or RX/TX switch in applications where the SLEEP state is used it is recommended to use GDOx_CFGx=0x2F instead.
#define CC110_GDO_CFG_CLK_32K       39
#define CC110_GDO_CFG_RDYn          41
#define CC110_GDO_CFG_XOSC_STABLE   43
#define CC110_GDO_CFG_HIGHZ         46
#define CC110_GDO_CFG_HW_TO_0       47  // HW to 0 (HW1 achieved by setting GDOx_INV=1). Can be used to control an external LNA/PA or RX/TX switch.
#define CC110_GDO_CFG_CLK_XOSC1     48
#define CC110_GDO_CFG_CLK_XOSC1_5   49  
#define CC110_GDO_CFG_CLK_XOSC2     50
#define CC110_GDO_CFG_CLK_XOSC3     51
#define CC110_GDO_CFG_CLK_XOSC4     52
#define CC110_GDO_CFG_CLK_XOSC6     53
#define CC110_GDO_CFG_CLK_XOSC8     54
#define CC110_GDO_CFG_CLK_XOSC12    55
#define CC110_GDO_CFG_CLK_XOSC16    56
#define CC110_GDO_CFG_CLK_XOSC24    57
#define CC110_GDO_CFG_CLK_XOSC32    58
#define CC110_GDO_CFG_CLK_XOSC48    59
#define CC110_GDO_CFG_CLK_XOSC64    60
#define CC110_GDO_CFG_CLK_XOSC96    61
#define CC110_GDO_CFG_CLK_XOSC128   62
#define CC110_GDO_CFG_CLK_XOSC192   63

typedef union
{
    uint8_t data;
    struct
    {
        uint8_t GDO2_CFG :6;     // Default is CHP_RDYn
        uint8_t GDO2_INV :1;     // Invert output: Low = 1; 0 = High
        uint8_t unused :1;
    };
} CC110_IOCFG2_REG_t;

// GDO_DS
#define CC110_GDO_DS_HIGH   1
#define CC110_GDO_DS_LOW    0

typedef union
{
    uint8_t data;
    struct
    {
        uint8_t GDO1_CFG :6;    // Default is 3-state
        uint8_t GDO1_INV :1;    // Invert output: Low = 1; 0 = High
        uint8_t GDO_DS :1;      // Set high or low output drive strength on GDO pins


    };
} CC110_IOCFG1_REG_t;

typedef union
{
    uint8_t data;
    struct
    {
        uint8_t GDO0_CFG :6;    // Default is CLK_XOSC / 192
        uint8_t GDO0_INV :1;    // Invert output: Low = 1; 0 = High
        uint8_t unused :1;
    };
} CC110_IOCFG0_REG_t;

// FIFO_THR
#define CC110_FIFO_THRES_RX4_TX61       0
#define CC110_FIFO_THRES_RX8_TX57       1
#define CC110_FIFO_THRES_RX12_TX53      2
#define CC110_FIFO_THRES_RX16_TX49      3
#define CC110_FIFO_THRES_RX20_TX45      4
#define CC110_FIFO_THRES_RX24_TX41      5
#define CC110_FIFO_THRES_RX28_TX37      6
#define CC110_FIFO_THRES_RX32_TX33      7
#define CC110_FIFO_THRES_RX36_TX29      8
#define CC110_FIFO_THRES_RX40_TX25      9
#define CC110_FIFO_THRES_RX44_TX21      10
#define CC110_FIFO_THRES_RX48_TX17      11
#define CC110_FIFO_THRES_RX52_TX13      12
#define CC110_FIFO_THRES_RX56_TX9       13
#define CC110_FIFO_THRES_RX60_TX5       14
#define CC110_FIFO_THRES_RX64_TX1       15
#define CC110_FIFO_THRES_RX64_TX1       15

// CLOSE_IN_RX
#define CC110_CLOSE_IN_RX_0DB           0
#define CC110_CLOSE_IN_RX_6DB           1
#define CC110_CLOSE_IN_RX_12DB          2
#define CC110_CLOSE_IN_RX_18DB          3

// ADC_RETENTION
#define CC110_ADC_RETENTION_RESET       0
#define CC110_ADC_RETENTION_SET         1

typedef union
{
    uint8_t data;
    struct
    {
        uint8_t FIFO_THR :4;        // Set threshold for rx and tx FIFO, see datasheet
        uint8_t CLOSE_IN_RX :2;     // Attenuation ctrl: 0: 0 dB, 1: 6 dB, 2: 12 dB, 3: 18 dB
        uint8_t ADC_RETENTION :1;   // Set to 1 before going into sleep

        uint8_t unused :1;
    };
} CC110_FIFOTHR_REG_t;

// TODO: Add definitions for values in below registers
typedef union
{
    uint8_t data;
    struct
    {
        uint8_t MSB :8;             // MSB of 16-bit sync word
    };
} CC110_SYNC1_REG_t;

typedef union
{
    uint8_t data;
    struct
    {
        uint8_t LSB :8;             // LSB of 16-bit sync word
    };
} CC110_SYNC0_REG_t;

typedef union
{
    uint8_t data;
    struct
    {
        uint8_t PACKET_LENGTH :8;   // Packet Length when fixed packet length mode is en
    };
} CC110_PKTLEN_REG_t;

typedef union
{
    uint8_t data;
    struct
    {
        uint8_t ADR_CHK :2;         // Controls addr check config: 0: No check; 1: Addr check, no broadcast; 2: Addr check and 0x00 broadcast; 3: Addr check and 0x00 and 0xFF broadcast
        uint8_t APPEND_STATUS :1;   // When En, two status bytes are appended to payload of packet with RSSI and CRC OK
        uint8_t CRC_AUTOFLUSH :1;   // Enables autoflush of Rx FIFO when CRC is not OK
        uint8_t unused :4;

    };
} CC110_PKTCTRL1_REG_t;

typedef union
{
    uint8_t data;
    struct
    {
        uint8_t LENGTH_CONFIG :2; // 0: Fixed packet length mode; 1: Variable packet length mode; 2: Infinite packet length mode; 3: Reserved
        uint8_t CRC_EN :1;          // 0: CRC calc disabled; 1: CRC calc enabled
        uint8_t unused0 :1;
        uint8_t PKT_FORMAT :2;
        uint8_t unused1 :2;

    };
} CC110_PKTCTRL0_REG_t;

typedef union
{
    uint8_t data;
    struct
    {
        uint8_t DEVICE_ADDR :8; // Address used for packet filtering, optional addrs are 0x00 and 0xFF for broadcast 
    };
} CC110_ADDR_REG_t;

typedef union
{
    uint8_t data;
    struct
    {
        uint8_t CHAN :8;            // Channel number 
    };
} CC110_CHANNR_REG_t;

typedef union
{
    uint8_t data;
    struct
    {
        uint8_t FREQ_IF :5;     // Desired IF Frequency for Rx
        uint8_t unused :3;
    };
} CC110_FSCTRL1_REG_t;

typedef union
{
    uint8_t data;
    struct
    {
        uint8_t FREQOFF :8;     // Frequency offset
    };
} CC110_FSCTRL0_REG_t;

typedef union
{
    uint8_t data;
    struct
    {
        uint8_t FREQ :6;        // Frequency Control Word High Byte
        uint8_t unused :2;
    };
} CC110_FREQ2_REG_t;

typedef union
{
    uint8_t data;
    struct
    {
        uint8_t FREQ :8;        // Frequency Control Word Middle Byte
    };
} CC110_FREQ1_REG_t;

typedef union
{
    uint8_t data;
    struct
    {
        uint8_t FREQ :8;        // Frequency Control Word Low Byte
    };
} CC110_FREQ0_REG_t;

typedef union
{
    uint8_t data;
    struct
    {
        uint8_t DRATE_E :4;     // Exponent of user specified data rate
        uint8_t CHANBW_M :2;    // Decimation ratio for ADC input stream
        uint8_t CHANBW_E: 2;    // TODO: Figure this out
    };
} CC110_MDMCFG4_REG_t;

typedef union
{
    uint8_t data;
    struct
    {
        uint8_t DRATE_M :8;     // Mantissa of user specified symbol rate
    };
} CC110_MDMCFG3_REG_t;

typedef union
{
    uint8_t data;
    struct
    {
        uint8_t SYNC_MODE :3;           // Combined sync-word qualifier mode, see datasheet
        uint8_t MANCHESTER_EN :1;       // 0: Disable Manchester Encoding, 1: Enable Manchester Encoding
        uint8_t MOD_FORMAT :3;          // Modulation format, see datasheet
        uint8_t DEM_DCFILT_OFF :1;      // DC blocking filter: 0: Enable, 1: Disable
    };
} CC110_MDMCFG2_REG_t;

typedef union
{
    uint8_t data;
    struct
    {
        uint8_t CHANSPC_E :2;           // 2 bit exponent of channel spacing
        uint8_t unused0 :1;
        uint8_t NUM_PREAMBLE :3;        // Sets minimum preamble bytes to be transmitted
        uint8_t unused1 :1;
    };
} CC110_MDMCFG1_REG_t;

typedef union
{
    uint8_t data;
    struct
    {
        uint8_t CHANSPC_M :8;           // 8-bit mantissa of channel spacing
    };
} CC110_MDMCFG0_REG_t;

typedef union
{
    uint8_t data;
    struct
    {
        uint8_t DEVIATION_M :3;         // Deviation 
        uint8_t unused0: 1;
        uint8_t DEVIATION_E :3;         // Deviation exponent
        uint8_t unused1: 1;
    };
} CC110_DEVIATN_REG_t;

typedef union
{
    uint8_t data;
    struct
    {
        uint8_t unused0 :4;
        uint8_t RX_TIME_RSSI :1;        // Direct RX termination based on RSSI
        uint8_t unused1: 3;
    };
} CC110_MCSM2_REG_t;

typedef union
{
    uint8_t data;
    struct
    {
        uint8_t TXOFF_MODE :2;          
        uint8_t RXOFF_MODE :2;
        uint8_t CCA_MODE :2;
        uint8_t unused :2;
    };
} CC110_MCSM1_REG_t;

typedef union
{
    uint8_t data;
    struct
    {
        uint8_t XOSC_FORCE_ON :1;       // Force XOSC to stay on in SLEEP
        uint8_t unused0 :1;
        uint8_t PO_TIMEOUT :2;          // Timeout after XOSC settle before CHP_RDY asserted
        uint8_t FS_AUTOCAL :2;          // Automatically calibrate when switching states between RX, TX, or IDLE
        uint8_t unused1 :2;
    };
} CC110_MCSM0_REG_t;

typedef union
{
    uint8_t data;
    struct
    {
        uint8_t FOC_LIMIT :2;           // Frequency Offset limit
        uint8_t FOC_POST_K :1;          // Freq compensation loop gain after sync word
        uint8_t FOC_PRE_K :2;           // Freq compensation loop gain before sync word
        uint8_t FOC_BS_CS_GATE :1;      // When set, freeze freq offset compensation until CS signal goes high
        uint8_t unused :2;
    };
} CC110_FOCCFG_REG_t;

typedef union
{
    uint8_t data;
    struct
    {
        uint8_t BS_LIMIT :2;            // Saturation point for data rate offset compensation algorithm
        uint8_t BS_POST_KP :1;          // Clock recovery feedback loop to be used after sync word proportional gain
        uint8_t BS_POST_KI :1;          // Clock recovery feedback loop to be used before sync word integral gain
        uint8_t BS_PRE_KP :2;           // Clock recovery feedback loop to be used before sync word proportional gain
        uint8_t BS_PRE_KI :2;           // Clock recovery feedback loop to be used before sync word integral gain        
    };
} CC110_BSCFG_REG_t;

// TODO: Add comments to the below registers
typedef union
{
    uint8_t data;
    struct
    {
        uint8_t MAGN_TARGET :3;
        uint8_t MAX_LNA_GAIN :3;
        uint8_t MAX_DVGA_GAIN :2;
    };
} CC110_AGCCTRL2_REG_t;

typedef union
{
    uint8_t data;
    struct
    {
        uint8_t CARRIER_SENS_ABS_THR :4;
        uint8_t CARRIER_SENS_REL_THR :2;
        uint8_t AGC_LNA_PRIORITY :1;
        uint8_t unused :1;
    };
} CC110_AGCCTRL1_REG_t;

typedef union
{
    uint8_t data;
    struct
    {
        uint8_t FILTER_LENGTH :2;
        uint8_t AGC_FREEZE :2;
        uint8_t WAIT_TIME :2;
        uint8_t HYST_LEVEL :2;
    };
} CC110_AGCCTRL0_REG_t;

typedef union
{
    uint8_t data;
    struct
    {
        uint8_t MIX_CURRENT :2;
        uint8_t LODIV_BUF_CURRENT_RX :2;
        uint8_t LNA2MIX_CURRENT :2;
        uint8_t LNA_CURRENT :2;
    };
} CC110_FREND1_REG_t;

typedef union
{
    uint8_t data;
    struct
    {
        uint8_t PA_POWER :2;
        uint8_t unused0 :1;
        uint8_t LODIV_BUF_CURRENT_TX :2;
        uint8_t unused1 :2;
    };
} CC110_FREND0_REG_t;

typedef union
{
    uint8_t data;
    struct
    {
        uint8_t FSCAL3_LOWER :4;
        uint8_t CHP_CURR_CAL_EN :2;
        uint8_t FSCAL3_UPPER :2;
    };
} CC110_FSCAL3_REG_t;

typedef union
{
    uint8_t data;
    struct
    {
        uint8_t FSCAL2 :5;
        uint8_t VCO_CORE_H_EN :1;
        uint8_t unused :2;
    };
} CC110_FSCAL2_REG_t;

typedef union
{
    uint8_t data;
    struct
    {
        uint8_t FSCAL1 :6;
        uint8_t unused :2;
    };
} CC110_FSCAL1_REG_t;

typedef union
{
    uint8_t data;
    struct
    {
        uint8_t FSCAL0 :7;
        uint8_t unused :1;
    };
} CC110_FSCAL0_REG_t;

typedef union
{
    uint8_t data;
    struct
    {
        uint8_t TEST2 :8;
    };
} CC110_TEST2_REG_t;

typedef union
{
    uint8_t data;
    struct
    {
        uint8_t TEST1 :8;
    };
} CC110_TEST1_REG_t;

typedef union
{
    uint8_t data;
    struct
    {
        uint8_t TEST0 :8;
    };
} CC110_TEST0_REG_t;

// PA Table
typedef enum
{
    POWER_m30DBM = 0x03,
    POWER_m20DBM = 0x0E,
    POWER_m15DBM = 0x1E,
    POWER_m10DBM = 0x27,
    POWER_0DBM = 0x8E,
    POWER_5DBM = 0xCD,
    POWER_7DBM = 0xC7,
    POWER_10DBM = 0xC0
} CC110_PATABLE_t;


#endif /* CC110_TYPES_H_ */
