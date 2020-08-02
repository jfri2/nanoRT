/*
 * logger.h
 *
 *  Created on: Jul 6, 2020
 *      Author: jfrit
 */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef LOGGER_H_
#define LOGGER_H_

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stdio.h>

/* Global Definitions --------------------------------------------------------*/
#define LOG_NONE            0
#define LOG_ERROR           1
#define LOG_WARN            2
#define LOG_INFO            3
#define LOG_DEBUG           4
#define LOG_LEVEL           LOG_DEBUG
#define LOG_BUF_SIZE        4096        // bytes
#define LOG_MSG_MAX_SIZE    256         // bytes

#define LOG_FULL            -1
#define LOG_OK              0
#define LOG_EMPTY           1

#define LOG_WRITE_FREE      0
#define LOG_WRITE_BUSY      1

#define log_error(...) log_event(LOG_ERROR, __FILE__, __LINE__, __VA_ARGS__)
#define log_warn(...) log_event(LOG_WARN, __FILE__, __LINE__, __VA_ARGS__)
#define log_info(...) log_event(LOG_INFO, __FILE__, __LINE__, __VA_ARGS__)
#define log_debug(...) log_event(LOG_DEBUG, __FILE__, __LINE__, __VA_ARGS__)

/* Global Variables ----------------------------------------------------------*/
extern uint8_t log_buf[LOG_BUF_SIZE];
extern uint32_t log_readIndex;
extern uint32_t log_writeIndex;
extern int8_t  log_status;
extern uint8_t log_writeLock;

/* Public Function Prototypes ------------------------------------------------*/
void log_event(uint8_t level, const char *file, unsigned int line, const char *format, ...);
int8_t log_bufReadByte(uint8_t* ret_byte); // log_bufRead should call this function
extern void log_read(void);       // Must be defined by the log consumer

#endif // LOGGER_H_
