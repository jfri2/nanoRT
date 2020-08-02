/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * File Name          : freertos.c
 * Description        : Code for freertos applications
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under Ultimate Liberty license
 * SLA0044, the "License"; You may not use this file except in compliance with
 * the License. You may obtain a copy of the License at:
 *                             www.st.com/SLA0044
 *
 ******************************************************************************
 */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */     
#include "logger.h"
#include "ssd1315.h"
#include "cc110.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
osThreadId_t consoleTaskHandle;
const osThreadAttr_t consoleTask_attributes = { .name = "consoleTask", .priority =
        (osPriority_t) osPriorityHigh + 1, .stack_size = 1024 * 4 };

osThreadId_t displayTaskHandle;
const osThreadAttr_t displayTask_attributes = { .name = "displayTask", .priority =
        (osPriority_t) osPriorityNormal, .stack_size = 1024 * 4 };

osThreadId_t radioTaskHandle;
const osThreadAttr_t radioTask_attributes = { .name = "radioTask", .priority =
        (osPriority_t) osPriorityHigh, .stack_size = 1024 * 4 };

/* USER CODE END Variables */
/* Definitions for heartbeatTask */
osThreadId_t heartbeatTaskHandle;
const osThreadAttr_t heartbeatTask_attributes = {
  .name = "heartbeatTask",
  .priority = (osPriority_t) osPriorityLow,
  .stack_size = 1024 * 4
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
void StartConsoleTask(void* argument);
void StartDisplayTask(void* argument);
void StartRadioTask(void* argument);
/* USER CODE END FunctionPrototypes */

void StartHeartbeatTask(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
    /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
    /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
    /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
    /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of heartbeatTask */
  heartbeatTaskHandle = osThreadNew(StartHeartbeatTask, NULL, &heartbeatTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
    consoleTaskHandle = osThreadNew(StartConsoleTask, NULL, &consoleTask_attributes);
    displayTaskHandle = osThreadNew(StartDisplayTask, NULL, &displayTask_attributes);
    radioTaskHandle = osThreadNew(StartRadioTask, NULL, &radioTask_attributes);
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StartHeartbeatTask */
/**
 * @brief  Function implementing the heartbeatTask thread.
 * @param  argument: Not used 
 * @retval None
 */
/* USER CODE END Header_StartHeartbeatTask */
void StartHeartbeatTask(void *argument)
{
  /* USER CODE BEGIN StartHeartbeatTask */
    log_info("Starting heartbeat task");
    /* Infinite loop */
    while (1)
    {
        HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
        osDelay(100);
    }
  /* USER CODE END StartHeartbeatTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
void StartConsoleTask(void* argument)
{
    log_info("Starting console task");
    while (1)
    {
        log_read();
        osDelay(1);
    }
}

void StartDisplayTask(void* argument)
{
    // Init display
    log_info("Starting display task");
    ssd1315_init();
    while (1)
    {
        ssd1315_demo();
        osDelay(100); // Do nothing
    }
}

void StartRadioTask(void* argument)
{
    // Init display
    log_info("Starting radio task");
    cc110_init();
    cc110_test();
    while (1)
    {
        osDelay(100); // Do nothing
    }
}

/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
