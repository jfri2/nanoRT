/*
 * logger.c
 *
 *  Created on: Jul 6, 2020
 *      Author: jfrit
 */
/* Includes ------------------------------------------------------------------*/
#include "logger.h"
#include "ustr.h"
#include "usart.h"
#include <freertos.h>
#include <task.h>
#include <stdarg.h>

/* Global Variable Definitions -----------------------------------------------*/

/* Private Variables ---------------------------------------------------------*/
static uint8_t log_buf[LOG_BUF_SIZE] = { 0 };
static uint32_t log_readIndex = 0;
static uint32_t log_writeIndex = 0;
static int8_t log_status = LOG_EMPTY;
static uint8_t log_writeLock = LOG_WRITE_FREE;
static uint8_t log_msg_dma[LOG_MSG_MAX_SIZE] = { 0 };

/* Private Function Prototypes ---------------------------------------------*/
int8_t log_write(char* str, uint32_t len);
int8_t log_writeByte(uint8_t byte);

/* Public Function Definitions -----------------------------------------------*/
void log_event(uint8_t level, const char* file, unsigned int line, const char* format, ...)
{
    // Temporary buffers
    char msg[LOG_MSG_MAX_SIZE] = { 0 };
    char tag[LOG_MSG_MAX_SIZE] = { 0 };

    // Get and format current time in ss:ms
    unsigned int timeMs = HAL_GetTick();
    unsigned int timeS = timeMs / 1000;
    timeMs = timeMs % 1000;

    if (level <= LOG_LEVEL)
    {
        // Create tag for log message
        switch (level)
        {
            case LOG_ERROR:
                snprintf(tag, LOG_MSG_MAX_SIZE, "\n\r%03u.%03u [ERROR] %s(%u) ", timeS, timeMs,
                         file, line);
                break;

            case LOG_WARN:
                snprintf(tag, LOG_MSG_MAX_SIZE, "\n\r%03u.%03u [WARN] %s(%u) ", timeS, timeMs, file,
                         line);
                break;

            case LOG_INFO:
                snprintf(tag, LOG_MSG_MAX_SIZE, "\n\r%03u.%03u [INFO] %s(%u) ", timeS, timeMs, file,
                         line);
                break;

            case LOG_DEBUG:
                snprintf(tag, LOG_MSG_MAX_SIZE, "\n\r%03u.%03u [DEBUG] %s(%u) ", timeS, timeMs,
                         file, line);
                break;

            default:
                // Do nothing
                break;
        }

        // Copy log message
        va_list arg;
        va_start(arg, format);
        vsprintf(msg, format, arg);
        va_end(arg);

        // Prepend tag to message
        strcat(tag, msg);

        // Write to the log buffer
        while (log_writeLock == LOG_WRITE_BUSY)
        {
            // Do nothing
        }
        log_write(tag, strlen(tag) + 1);
    }
}

int8_t log_bufReadByte(uint8_t* ret_byte)
{
    if (log_writeIndex == log_readIndex)
    {
        log_status = LOG_EMPTY;
    }
    else
    {
        *ret_byte = log_buf[log_readIndex];
        log_readIndex = (log_readIndex + 1) % LOG_BUF_SIZE;
    }

    // Check after the last read if the log is empty
    if (log_writeIndex == log_readIndex)
    {
        log_status = LOG_EMPTY;
    }

    return (log_status);
}

// Example log_read implementation sending logs to a UART
void log_read(void)
{
    static uint8_t log_errorMsg[] = "\n\r\[ERROR] Log buffer overflow!\n\r";
    uint32_t msg_index = 0;
    uint32_t timeMs = 0;
    uint32_t timeoutMs = 10000;

    // Wait for previous transfer to complete if it hasn't already
    while ((Usart_DmaTxComplete != 1) && (timeMs < timeoutMs))
    {
        HAL_Delay(1);
        timeMs++;
    }

    // Read out string from log buffer, stopping when reaching a null character
    if (log_status != LOG_EMPTY)
    {
        // If the log got full, output an error immediately
        if (log_status == LOG_FULL)
        {
            HAL_UART_Transmit(&huart1, log_errorMsg, ustrlen(log_errorMsg), HAL_MAX_DELAY);
        }
        else
        {
            // Copy last string
            do
            {
                log_bufReadByte(&log_msg_dma[msg_index]);
                msg_index++;
            }
            while ((log_msg_dma[msg_index - 1] != '\0') && (msg_index < (LOG_MSG_MAX_SIZE - 1)));
        }

        // Reset transfer flag and send new string via DMA to UART
        Usart_DmaTxComplete = 0;
        HAL_UART_Transmit_DMA(&huart1, log_msg_dma, (msg_index - 1));
    }
}

/* Private Function Definitions ----------------------------------------------*/
int8_t log_write(char* str, uint32_t len)
{
    uint32_t i = 0;

    if (len >= LOG_MSG_MAX_SIZE)
    {
        log_warn("Something tried to write a log message >= %d!!!", LOG_MSG_MAX_SIZE);
    }

    // TODO: Wrap this in a mutex
    // Write data to buffer, terminating if log is full
    while ((i < len) && (log_status != LOG_FULL))
    {
        log_writeByte(str[i]);
        i++;
    }

    return (log_status);
}

int8_t log_writeByte(uint8_t byte)
{
    // Check to see if log buffer is full
    if (log_writeIndex == ((log_readIndex - 1 + LOG_BUF_SIZE) % LOG_BUF_SIZE))
    {
        log_status = LOG_FULL;
    }
    else    // Copy byte if not full
    {
        log_status = LOG_OK;
        log_writeLock = LOG_WRITE_BUSY;
        log_buf[log_writeIndex] = byte;
        log_writeLock = LOG_WRITE_FREE;
        log_writeIndex = (log_writeIndex + 1) % LOG_BUF_SIZE;
    }

    return (log_status);
}
/************************ (C) COPYRIGHT John Fritz *****END OF FILE****/
