#ifndef  __VAS1082_H__
#define  __VAS1082_H__
#include "tim.h"
#include "cmsis_os2.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "queue.h"
#include "stdio.h"

extern osMessageQueueId_t UartQueueHandle;
extern osSemaphoreId_t Uart_SemHandle;
extern osSemaphoreId_t Freq_SemHandle;
extern osSemaphoreId_t defaultTaskHandle;
extern osSemaphoreId_t Aimming_SemHandle;
extern osMutexId_t UartMutexesHandle;
extern osThreadId_t OtherTaskHandle;
#endif
