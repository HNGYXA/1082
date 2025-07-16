#include "API.h"
#include "FreeRTOS.h"
#include "cmsis_os.h"
#include "cmsis_os2.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "event_groups.h"
#include "timers.h"
// Semaphore:信号量
/***
	功能:尝试获取二值信号量
	参数1:Freq_SemHandle -->句柄
	参数2:portMAX_DELAY -->等待时间--ms
	返回值：pdTRUE（成功获取）或 pdFALSE（超时）。
	xSemaphoreTake(SemaphoreHandle_t Freq_SemHandle,portMAX_DELAY); 
***/

/***
	功能:释放二值信号量
	参数1:Freq_SemHandle -->句柄
	返回值：pdTRUE（成功）或 pdFALSE。
	xSemaphoreGive(SemaphoreHandle_t Freq_SemHandle);
***/

/***
	功能:在中断中获取二值信号量
	参数1:xBinarySemaphore -->句柄
	参数2: &xHigherPriorityTaskWoken -->指向一个 BaseType_t 变量的指针，用于指示是否有更高优先级的任务因获取信号量而被唤醒。
	返回值：pdTRUE（成功）或 pdFALSE。
	xSemaphoreTakeFromISR(SemaphoreHandle_t xBinarySemaphore, &xHigherPriorityTaskWoken);
***/

/***
	功能:在中断中释放二值信号量
	参数1:xBinarySemaphore -->句柄
	参数2: &xHigherPriorityTaskWoken -->指向一个 BaseType_t 变量的指针，用于指示是否有更高优先级的任务因获取信号量而被唤醒。
	返回值：pdTRUE（成功）或 pdFALSE。
	xSemaphoreGiveFromISR(SemaphoreHandle_t xBinarySemaphore, &xHigherPriorityTaskWoken);
***/

/***
	功能:删除二值信号量
	参数1:xSemaphore -->句柄
	返回值：无
	void vSemaphoreDelete(SemaphoreHandle_t xSemaphore);
***/

// Queue:队列
/***
	功能:删除队列
	参数1:xQueue -->句柄
	返回值：无
	void vQueueDelete(QueueHandle_t xQueue);
***/

/***
	功能:重置队列--清空所有数据，恢复初始状态
	参数1:xQueue -->句柄
	返回值：无
	void vQueueReset(QueueHandle_t xQueue);
***/

/***
	功能:向队列发送数据(尾部)
	参数1:xQueue：句柄
	参数2:pvItemToQueue：指向要发送的数据的指针
	参数3:xTicksToWait：等待时间
	返回值：pdTRUE（成功）或 pdFALSE。
	BaseType_t xQueueSend(
    QueueHandle_t xQueue,
    const void *pvItemToQueue,
    TickType_t xTicksToWait
);
***/

/***
	功能:中断中向队列发送数据(尾部)
	参数1:xQueue：句柄
	参数2:pvItemToQueue：指向要发送的数据的指针
	参数3:pxHigherPriorityTaskWoken：指向一个 BaseType_t 变量的指针，用于指示是否有更高优先级的任务因获取信号量而被唤醒。
	返回值：pdTRUE（成功）或 pdFALSE。
	BaseType_t xQueueSendFromISR(
    QueueHandle_t xQueue,
    const void *pvItemToQueue,
    BaseType_t *pxHigherPriorityTaskWoken
);
***/

/***
	功能:队列接收数据
	参数1:xQueue：句柄
	参数2:pvBuffer：指向要接收的数据的指针
	参数3:xTicksToWait：等待时间
	返回值：pdTRUE（成功）或 pdFALSE。
	BaseType_t xQueueReceive(
			QueueHandle_t xQueue,
			void *pvBuffer,
			TickType_t xTicksToWait
	);
***/

/***
	功能:中断中队列接收数据
	参数1:xQueue：句柄
	参数2:pvBuffer：指向要接收的数据的指针
	参数3:pxHigherPriorityTaskWoken：指向一个 BaseType_t 变量的指针，用于指示是否有更高优先级的任务因获取信号量而被唤醒。
	返回值：pdTRUE（成功）或 pdFALSE。
	BaseType_t xQueueReceiveFromISR(
			QueueHandle_t xQueue,
			void *pvBuffer,
			BaseType_t *pxHigherPriorityTaskWoken
	);
***/

/***
	功能:获取队列中当前的消息数量
	参数1:xQueue：句柄
	返回值：UBaseType_t的整数
	UBaseType_t uxQueueMessagesWaiting(const QueueHandle_t xQueue);
***/

/***
	功能:获取队列中剩余的空闲空间
	参数1:xQueue：句柄
	返回值：UBaseType_t的整数
	UBaseType_t uxQueueSpacesAvailable(const QueueHandle_t xQueue);
***/

/***
	功能:在中断中检查队列是否为空或者已满
	参数1:xQueue：句柄
	返回值：UBaseType_t的整数
	BaseType_t xQueueIsQueueEmptyFromISR(const QueueHandle_t xQueue);
	BaseType_t xQueueIsQueueFullFromISR(const QueueHandle_t xQueue);
***/

//Mutexes:互斥量
/***
	功能:创建动态互斥量（初始计数值为 1，表示资源可用）。
	返回值：成功返回互斥量句柄，失败返回 NULL。
	SemaphoreHandle_t xSemaphoreCreateMutex(void);
	注意：创建后即可使用，无需额外初始化。
***/

/***
	功能:静态创建互斥量（无需动态内存分配）。
	参数1：pxMutexBuffer 指向静态存储区的结构体。
	返回值：成功返回互斥量句柄，失败返回 NULL。
	SemaphoreHandle_t xSemaphoreCreateMutexStatic(StaticSemaphore_t *pxMutexBuffer);
***/

/***
	功能:获取互斥量（阻塞模式）。
	参数1：xMutex 互斥量句柄
	参数2;xTicksToWait 等待超时时间（portMAX_DELAY 表示永久等待）
	返回值：pdTRUE：成功获取互斥量。 pdFALSE：超时仍未获取到。
	BaseType_t xSemaphoreTake(
    SemaphoreHandle_t xMutex,
    TickType_t xTicksToWait
	);
***/

/***
	功能:释放互斥量（必须由持有互斥量的任务释放）
	参数1：xMutex 互斥量句柄
	返回值：pdTRUE：成功获取互斥量。 pdFALSE：失败（如尝试释放未持有的互斥量）。
	BaseType_t xSemaphoreGive(SemaphoreHandle_t xMutex);
	唤醒等待该互斥量的最高优先级任务，因为有优先级继承。
	互斥量不可在中断服务程序（ISR）中释放，因为中断无法持有互斥量。
***/

/***
	功能:创建递归互斥量

	返回值：成功返回句柄，失败返回 NULL。
	动态：SemaphoreHandle_t xSemaphoreCreateRecursiveMutex(void);
	静态：SemaphoreHandle_t xSemaphoreCreateRecursiveMutexStatic(StaticSemaphore_t *pxMutexBuffer);
	
	功能：递归获取互斥量（同一任务可多次获取）。
	BaseType_t xSemaphoreTakeRecursive(
    SemaphoreHandle_t xMutex,
    TickType_t xTicksToWait
	);
	参数与返回值：同 xSemaphoreTake()。
	行为：
	每次获取时内部计数器加 1。
	需对应次数的释放才能完全解锁。
	
	
	功能：递归释放互斥量（减少内部计数器）。
	返回值：pdTRUE：成功释放。pdFALSE：失败（如释放次数超过获取次数）。
	BaseType_t xSemaphoreGiveRecursive(SemaphoreHandle_t xMutex);
	
	功能：删除互斥量
	void vSemaphoreDelete(SemaphoreHandle_t xMutex);
***/

//EVENT 事件标志组
//在 FreeRTOS 中，事件标志组（Event Groups） 是一种同步机制，用于任务间或任务与中断间的事件通知。
//它通过一个 32 位的标志位集合（每一位代表一个事件）实现高效的事件标志管理。

/***
	功能：创建动态事件标志组。
	返回值：成功：返回事件标志组句柄（EventGroupHandle_t）。失败：返回 NULL（内存分配失败）。
	EventGroupHandle_t xEventGroupCreate(void);
	注意：需在 FreeRTOSConfig.h 中启用 configUSE_EVENT_GROUPS。
	
	功能：静态创建事件标志组（无需动态内存分配）。
	返回值：成功：返回事件标志组句柄（EventGroupHandle_t）。失败：返回 NULL（内存分配失败）。
	参数：pxEventGroupBuffer 指向静态存储区的结构体。
	EventGroupHandle_t xEventGroupCreateStatic(StaticEventGroup_t *pxEventGroupBuffer);
***/

/***
	功能：在任务中设置事件标志组的指定位。
	参数1：xEventGroup：事件标志组句柄。
	参数2：uxBitsToSet：要设置的位掩码（如 0x01 表示第 0 位）。
	返回值：设置位后事件标志组的当前值。
	EventBits_t xEventGroupSetBits(
			EventGroupHandle_t xEventGroup,
			const EventBits_t uxBitsToSet
	);
	

	功能：在中断服务程序（ISR）中设置事件标志组的指定位。
	参数1：xEventGroup：事件标志组句柄。
	参数2：uxBitsToSet：要设置的位掩码（如 0x01 表示第 0 位）。
	参数3：pxHigherPriorityTaskWoken：用于标记是否需要触发上下文切换。
	返回值：pdPASS：操作成功。pdFAIL：事件标志组已被删除（极少发生）
	BaseType_t xEventGroupSetBitsFromISR(
    EventGroupHandle_t xEventGroup,
    const EventBits_t uxBitsToSet,
    BaseType_t *pxHigherPriorityTaskWoken
	);
	
	功能：清除事件标志组的指定位。
	参数1：xEventGroup：事件标志组句柄
	参数2：uxBitsToClear：要清除的位掩码。
	返回值：清除位前事件标志组的值。
	EventBits_t xEventGroupClearBits(
    EventGroupHandle_t xEventGroup,
    const EventBits_t uxBitsToClear
	);
***/

/***
	功能：等待指定事件标志位被设置（阻塞模式）。
	参数1：xEventGroup：事件标志组句柄
	参数2：uxBitsToWaitFor：等待的位掩码（如 0x03 表示等待第 0 位和第 1 位）。
	参数3：xClearOnExit：pdTRUE：退出前自动清除等待的位。pdFALSE：不清除。
	参数4：xWaitForAllBits：pdTRUE：所有等待的位都被设置时才唤醒（逻辑与）pdFALSE：任意一个等待的位被设置时就唤醒（逻辑或）。
	参数5：xTicksToWait：超时时间（portMAX_DELAY 表示永久等待）。
	EventBits_t xEventGroupWaitBits(
			EventGroupHandle_t xEventGroup,
			const EventBits_t uxBitsToWaitFor,
			const BaseType_t xClearOnExit,
			const BaseType_t xWaitForAllBits,
			TickType_t xTicksToWait
	);
***/

/***
	功能：获取事件标志组的当前值（非阻塞）。
	返回值：事件标志组的当前值。
	EventBits_t xEventGroupGetBits(EventGroupHandle_t xEventGroup);
	
	功能：在 ISR 中获取事件标志组的当前值。
	返回值：事件标志组的当前值。
	EventBits_t xEventGroupGetBitsFromISR(EventGroupHandle_t xEventGroup);
	
	功能：删除事件标志组并释放内存（动态创建的）
	注意：删除前需确保无任务等待该事件标志组。
	void vEventGroupDelete(EventGroupHandle_t xEventGroup);
***/

//Timer 软件定时器
//在FreeRTOS 中，软件定时器（Software Timers） 是基于系统时钟实现的定时器机制，
//允许任务或中断在指定时间后执行回调函数

/***
	功能：创建动态软件定时器。
	参数1：pcTimerName：定时器名称（用于调试，可为 NULL）。
	参数2：xTimerPeriod：定时器周期（单位：系统节拍，pdMS_TO_TICKS(ms) 转换）。
	参数3：xAutoReload：pdTRUE：周期定时器（自动重启）。pdFALSE：单次定时器（执行一次后停止）。
	参数4：pvTimerID：定时器 ID（用户数据，可通过 pvTimerGetTimerID() 获取）。
	参数5：pxCallback：定时器回调函数指针（原型：void callback(TimerHandle_t xTimer)）。
	返回值：成功：返回定时器句柄（TimerHandle_t）。失败：返回 NULL（内存分配失败）。
	TimerHandle_t xTimerCreate(
			const char * const pcTimerName,
			const TickType_t xTimerPeriod,
			const BaseType_t xAutoReload,
			void * const pvTimerID,
			TimerCallbackFunction_t pxCallback
	);
***/

/***
	功能：启动或重启定时器（任务环境）。
	参数1：xTimer：任务句柄
	参数2：xTicksToWait：等待定时器命令队列的超时时间（portMAX_DELAY 表示永久等待）。
	返回值：pdPASS：命令成功加入队列（不保证定时器立即启动）。pdFAIL：命令队列已满，无法加入。
	BaseType_t xTimerStart(TimerHandle_t xTimer, TickType_t xTicksToWait);
	
	功能：重置定时器（重新开始计时，无论当前状态）。
	参数1：xTimer：任务句柄
	参数2：xTicksToWait：等待定时器命令队列的超时时间（portMAX_DELAY 表示永久等待）。
	返回值：pdPASS：命令成功加入队列（不保证定时器立即启动）。pdFAIL：命令队列已满，无法加入。
	BaseType_t xTimerReset(TimerHandle_t xTimer, TickType_t xTicksToWait);
	
	功能：在中断服务程序（ISR）中启动定时器。
	BaseType_t xTimerStartFromISR(
    TimerHandle_t xTimer,
    BaseType_t *pxHigherPriorityTaskWoken
	);
	
	功能：停止定时器（任务环境）。
	BaseType_t xTimerStop(TimerHandle_t xTimer, TickType_t xTicksToWait);
	
	功能：在 ISR 中停止定时器。
	BaseType_t xTimerStopFromISR(
    TimerHandle_t xTimer,
    BaseType_t *pxHigherPriorityTaskWoken
	);
	
	功能：动态更改定时器周期（可用于运行中的定时器）
	参数1：句柄
	参数2：xNewPeriod：新的定时器周期（系统节拍）。
	参数3：等待时间
	返回值：pdPASS：命令成功加入队列。pdFAIL：命令队列已满。
	BaseType_t xTimerChangePeriod(
    TimerHandle_t xTimer,
    TickType_t xNewPeriod,
    TickType_t xTicksToWait
	);
	
	功能：在 ISR 中更改定时器周期。
	BaseType_t xTimerChangePeriodFromISR(
    TimerHandle_t xTimer,
    TickType_t xNewPeriod,
    BaseType_t *pxHigherPriorityTaskWoken
	);
***/

/***
功能：检查定时器是否处于活动状态（已启动）。
返回值：
pdTRUE：定时器已启动。
pdFALSE：定时器未启动或已停止。
BaseType_t xTimerIsTimerActive(const TimerHandle_t xTimer);

功能：获取定时器的当前周期。
返回值：定时器周期（系统节拍）。
TickType_t xTimerGetPeriod(const TimerHandle_t xTimer);

功能：获取定时器的用户定义 ID。
返回值：创建定时器时设置的 pvTimerID。
void *pvTimerGetTimerID(const TimerHandle_t xTimer);

功能：删除定时器并释放内存（动态创建的定时器）
void vTimerDelete(TimerHandle_t xTimer, TickType_t xTicksToWait);
注意：删除前需确保定时器已停止，避免资源泄漏
***/