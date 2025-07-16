/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
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
#include "stdio.h"
#include "VAS1082.h"
#include "event_groups.h"
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

/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for Task_Freq */
osThreadId_t Task_FreqHandle;
const osThreadAttr_t Task_Freq_attributes = {
  .name = "Task_Freq",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for Task_Aimming */
osThreadId_t Task_AimmingHandle;
const osThreadAttr_t Task_Aimming_attributes = {
  .name = "Task_Aimming",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for Task_Uart */
osThreadId_t Task_UartHandle;
const osThreadAttr_t Task_Uart_attributes = {
  .name = "Task_Uart",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal4,
};
/* Definitions for UartQueue */
osMessageQueueId_t UartQueueHandle;
const osMessageQueueAttr_t UartQueue_attributes = {
  .name = "UartQueue"
};
/* Definitions for myTimer01 */
osTimerId_t myTimer01Handle;
const osTimerAttr_t myTimer01_attributes = {
  .name = "myTimer01"
};
/* Definitions for UartMutexes */
osMutexId_t UartMutexesHandle;
const osMutexAttr_t UartMutexes_attributes = {
  .name = "UartMutexes"
};
/* Definitions for Freq_Sem */
osSemaphoreId_t Freq_SemHandle;
const osSemaphoreAttr_t Freq_Sem_attributes = {
  .name = "Freq_Sem"
};
/* Definitions for Aimming_Sem */
osSemaphoreId_t Aimming_SemHandle;
const osSemaphoreAttr_t Aimming_Sem_attributes = {
  .name = "Aimming_Sem"
};
/* Definitions for Uart_Sem */
osSemaphoreId_t Uart_SemHandle;
const osSemaphoreAttr_t Uart_Sem_attributes = {
  .name = "Uart_Sem"
};
/* Definitions for Event1 */
osEventFlagsId_t Event1Handle;
const osEventFlagsAttr_t Event1_attributes = {
  .name = "Event1"
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
#define EVENT_BIT_0 (1 << 0)  // 第0位
#define EVENT_BIT_1 (1 << 1)  // 第1位
/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
void Freq(void *argument);
void Aimming(void *argument);
void Uart(void *argument);
void Callback01(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* Hook prototypes */
void vApplicationStackOverflowHook(xTaskHandle xTask, signed char *pcTaskName);

/* USER CODE BEGIN 4 */
void vApplicationStackOverflowHook(xTaskHandle xTask, signed char *pcTaskName)
{
		printf("%s:overflow\r\n",pcTaskName);//���Ӻ�����ջ�������
		
   /* Run time stack overflow checking is performed if
   configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2. This hook function is
   called if a stack overflow is detected. */
}
/* USER CODE END 4 */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */
  /* Create the mutex(es) */
  /* creation of UartMutexes */
  UartMutexesHandle = osMutexNew(&UartMutexes_attributes);

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* Create the semaphores(s) */
  /* creation of Freq_Sem */
  Freq_SemHandle = osSemaphoreNew(1, 0, &Freq_Sem_attributes);

  /* creation of Aimming_Sem */
  Aimming_SemHandle = osSemaphoreNew(1, 0, &Aimming_Sem_attributes);

  /* creation of Uart_Sem */
  Uart_SemHandle = osSemaphoreNew(1, 0, &Uart_Sem_attributes);

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* Create the timer(s) */
  /* creation of myTimer01 */
  myTimer01Handle = osTimerNew(Callback01, osTimerPeriodic, NULL, &myTimer01_attributes);

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* creation of UartQueue */
  UartQueueHandle = osMessageQueueNew (128, sizeof(uint8_t), &UartQueue_attributes);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* creation of Task_Freq */
  Task_FreqHandle = osThreadNew(Freq, NULL, &Task_Freq_attributes);

  /* creation of Task_Aimming */
  Task_AimmingHandle = osThreadNew(Aimming, NULL, &Task_Aimming_attributes);

  /* creation of Task_Uart */
  Task_UartHandle = osThreadNew(Uart, NULL, &Task_Uart_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* creation of Event1 */
  Event1Handle = osEventFlagsNew(&Event1_attributes);

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;)
  {
		osTimerStart(myTimer01Handle, 1000); 
		printf("hello\r\n");
		//vTaskDelete(defaultTaskHandle);
    osDelay(1);
		EventBits_t uxBits;
		uxBits = xEventGroupWaitBits(
    Event1Handle,
    1 | 2,
    pdTRUE,          // 退出时清除位
    pdTRUE,         // 逻辑或：任意位设置即可
    portMAX_DELAY
		);
		
			printf("1\r\n");
	

  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_Freq */
/**
* @brief Function implementing the Task_Freq thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Freq */
void Freq(void *argument)
{
  /* USER CODE BEGIN Freq */
  /* Infinite loop */
  for(;;)
  {
		Freq_Sel(); 
    osDelay(1);
  }
  /* USER CODE END Freq */
}

/* USER CODE BEGIN Header_Aimming */
/**
* @brief Function implementing the Task_Aimming thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Aimming */
void Aimming(void *argument)
{
  /* USER CODE BEGIN Aimming */
  /* Infinite loop */
  for(;;)
  {
		Aimming_Sel();
    osDelay(1);
  }
  /* USER CODE END Aimming */
}

/* USER CODE BEGIN Header_Uart */
/**
* @brief Function implementing the Task_Uart thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Uart */
void Uart(void *argument)
{
  /* USER CODE BEGIN Uart */
  /* Infinite loop */
  for(;;)
  {
		Uart_Receive();
    osDelay(1);
  }
  /* USER CODE END Uart */
}

/* Callback01 function */
void Callback01(void *argument)
{
  /* USER CODE BEGIN Callback01 */
	static uint16_t count = 0;
	if(count%2)
		 xEventGroupSetBits(Event1Handle, 1);
	else xEventGroupSetBits(Event1Handle, 2);
	count ++;
  /* USER CODE END Callback01 */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

