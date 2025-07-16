//Mutexes:互斥量
//在实时操作系统（RTOS）中，互斥量（Mutex） 是实现资源互斥访问的核心机制，用于解决多任务环境下的竞态条件
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