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