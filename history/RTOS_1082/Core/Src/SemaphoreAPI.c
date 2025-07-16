// Semaphore:信号量 RTOS API5.0
//在FreeRTOS中，二值信号量（Binary Semaphore） 是实现任务间同步和资源互斥的基础机制。
/***
	功能：创建二值信号量（通过 max_count=1 实现）。
	参数1：max_count：信号量最大计数值（二值信号量设为 1）。
	参数2：initial_count：初始计数值（0 或 1）。
	参数3：attr：信号量属性（如名称、内存区域，可为 NULL）。
	返回值：成功：返回信号量 ID（osSemaphoreId_t）。失败：返回 NULL。
	osSemaphoreId_t osSemaphoreNew(
			uint32_t max_count,
			uint32_t initial_count,
			const osSemaphoreAttr_t *attr
	);
***/

/***
	功能：获取信号量（阻塞模式）
	参数1：semaphore_id：信号量 ID。
	参数2：timeout：等待超时时间（osWaitForever 表示永久等待）。
	osStatus_t osSemaphoreAcquire(
			osSemaphoreId_t semaphore_id,
			uint32_t timeout
	);
***/

/***
	功能：释放信号量。
	参数1：semaphore_id：信号量 ID。
	返回值：osOK：成功释放。osErrorResource：信号量已删除或计数值已达最大值。
	osStatus_t osSemaphoreRelease(osSemaphoreId_t semaphore_id);
	
	功能：从中断服务程序中释放信号量。
	参数1：semaphore_id：信号量 ID。
	参数2：task_woken：指向uint32_t的指针，用于指示释放信号量是否唤醒了某个任务（非零值表示有任务被唤醒，需在中断返回前执行上下文切换）。
	osStatus_t osSemaphoreReleaseFromISR(osSemaphoreId_t semaphore_id, uint32_t *task_woken);
***/

/***
	功能：删除信号量并释放内存（动态创建的信号量）。
	参数1：semaphore_id：信号量 ID。
	void vSemaphoreDelete(SemaphoreHandle_t xSemaphore);
***/

/***
	功能：获取当前信号量的计数值。
	参数1：semaphore_id：信号量 ID。
	uint32_t osSemaphoreGetCount(osSemaphoreId_t semaphore_id);
***/







// Semaphore:信号量 RTOS API2.0
//在FreeRTOS中，二值信号量（Binary Semaphore） 是实现任务间同步和资源互斥的基础机制。
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