#include "API.h"
#include "FreeRTOS.h"
#include "cmsis_os.h"
#include "cmsis_os2.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "event_groups.h"
#include "timers.h"
// Task 任务  RTOS2 API5.0
/***
	功能：创建并启动任务。
	参数1：func：任务函数指针（原型：void func(void *argument)）。
	参数2：argument：传递给任务的参数（可为 NULL）。
	参数3：attr：任务属性（如名称、栈大小、优先级，可为 NULL）。
	返回值：成功：返回任务 ID（osThreadId_t）。失败：返回 NULL。
	osThreadId_t osThreadNew(
    osThreadFunc_t func,
    void *argument,
    const osThreadAttr_t *attr
	);
	defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);
	const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",  //名字
  .stack_size = 128 * 4,	//内存  128word
  .priority = (osPriority_t) osPriorityNormal, //优先级
	};
***/

/***
	功能：终止指定任务。
	参数1：thread_id：任务 ID（NULL 表示当前任务）。
	返回值：osOK：操作成功。osErrorResource：任务不存在或已终止。
	osStatus_t osThreadTerminate(osThreadId_t thread_id);
	
	功能：挂起指定任务。
	参数1：thread_id：任务 ID（NULL 表示当前任务）。
	返回值：osOK：操作成功。osErrorResource：任务不存在或已终止。
	osStatus_t osThreadSuspend(osThreadId_t thread_id);
	
	功能：恢复被挂起的任务。
	参数1：thread_id：任务 ID（NULL 表示当前任务）。
	返回值：osOK：操作成功。osErrorResource：任务不存在或已终止。
	osStatus_t osThreadResume(osThreadId_t thread_id);
	
	功能：设置任务优先级。
	参数1：thread_id：任务 ID（NULL 表示当前任务）。
	参数2：priority：新优先级（枚举值，如 osPriorityNormal）。
	返回值：osOK：操作成功。osErrorResource：任务不存在或已终止。
	osStatus_t osThreadSetPriority(osThreadId_t thread_id, osPriority_t priority);
	
	功能：获取任务优先级。
	参数1：thread_id：任务 ID（NULL 表示当前任务）。
	返回值：任务的当前优先级。
	osPriority_t osThreadGetPriority(osThreadId_t thread_id);
	
	功能：当前任务延时指定时间。
	参数1：ticks：延时的系统节拍数。
	返回值：osOK：延时成功。osErrorParameter：参数无效。
	osStatus_t osDelay(uint32_t ticks);
***/

/***
	核心功能:主动让出 CPU
	当前运行的线程暂时放弃执行权，允许调度器选择其他就绪态的同优先级或更高优先级线程执行。
	触发上下文切换
	强制调度器重新评估线程优先级，决定下一个执行的线程。若没有更高优先级线程就绪，则当前线程可能会被重新调度。
	void osThreadYield(void);
***/


//Task  任务 RTOS API2.0
/***
	功能：创建动态任务。
	参数1：pxTaskCode：任务函数指针（原型：void vTaskFunction(void *pvParameters)）。
	参数2：pcName：任务名称（用于调试，可为 NULL）。
	参数3：usStackDepth：任务栈大小（单位：字，如 1024 表示 1024×4 字节）。
	参数4：pvParameters：传递给任务的参数（可为 NULL）。
	参数5：uxPriority：任务优先级（0 为最低优先级）。
	参数6：pxCreatedTask：用于返回任务句柄（可为 NULL）。
	返回值：pdPASS：任务创建成功。errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY：内存分配失败。
	BaseType_t xTaskCreate(
    TaskFunction_t pxTaskCode,
    const char * const pcName,
    configSTACK_DEPTH_TYPE usStackDepth,
    void *pvParameters,
    UBaseType_t uxPriority,
    TaskHandle_t *pxCreatedTask
	);
***/

/*** 
	功能：删除任务并释放资源。
	参数1：xTaskToDelete：要删除的任务句柄（NULL 表示删除当前任务）。
	void vTaskDelete(TaskHandle_t xTaskToDelete);
	
	功能：挂起指定任务（暂停执行）。
	void vTaskSuspend(TaskHandle_t xTaskToSuspend);
	
	功能：恢复被挂起的任务。
	返回值：pdTRUE：任务恢复后需要进行上下文切换。pdFALSE：无需切换。
	BaseType_t xTaskResume(TaskHandle_t xTaskToResume);
	
	功能：在中断中恢复任务。
	返回值：pdTRUE：任务恢复后需要进行上下文切换。pdFALSE：无需切换。
	BaseType_t xTaskResumeFromISR(TaskHandle_t xTaskToResume);
	
	功能：设置任务优先级。
	参数1：xTask：目标任务句柄（NULL 表示当前任务）。
	参数2：uxNewPriority：新优先级值。
	void vTaskPrioritySet(TaskHandle_t xTask, UBaseType_t uxNewPriority);
	
	功能：获取任务优先级。
	返回值：任务的当前优先级。
	UBaseType_t uxTaskPriorityGet(TaskHandle_t xTask);
	
	功能：当前任务延时指定时间（绝对延时）。
	参数1：xTicksToWait：延时的系统节拍数（可通过 pdMS_TO_TICKS(ms) 转换）。
	void vTaskDelay(TickType_t xTicksToWait);
	
	功能：实现周期性任务（相对延时）
	参数1：pxPreviousWakeTime：上次唤醒时间（需初始化为当前时间）。
	参数2：xTimeIncrement：周期时间（系统节拍数）。
	返回值：pdTRUE：任务按预期执行。pdFALSE：任务执行超时（可能因优先级过低）。
	注意：TickType_t pxPreviousWakeTime; 需定义
	BaseType_t xTaskDelayUntil(TickType_t *pxPreviousWakeTime, TickType_t xTimeIncrement);
***/
