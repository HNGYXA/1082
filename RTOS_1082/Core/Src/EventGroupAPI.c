//EVENT 事件标志组 RTOS API5.0
//在 FreeRTOS 中，事件标志组（Event Groups） 是一种同步机制，用于任务间或任务与中断间的事件通知。
//它通过一个 32 位的标志位集合（每一位代表一个事件）实现高效的事件标志管理。

/***
	功能：创建事件标志组
	参数1：attr：事件标志组属性结构体指针，可设置名称、内存分配方式等。若为NULL，则使用默认属性。
	返回值：
		成功：事件标志组句柄（非NULL）。
		失败：NULL（如内存不足或参数无效）
	osEventFlagsId_t osEventFlagsNew(const osEventFlagsAttr_t *attr);
	
	const osEventFlagsAttr_t Event1_attributes = {
  .name = "Event1"
	};
***/

/***
	功能：设置事件标志
	参数1：ef_id：事件标志组句柄。
	参数2：flags：要设置的标志位（位掩码，如0x01表示设置第 0 位）。
	返回值：
		成功：设置操作后的当前标志值。
		失败：0xFFFFFFFF（如ef_id无效）。
	uint32_t osEventFlagsSet(osEventFlagsId_t ef_id, uint32_t flags);
	
	功能：清除事件标志
	参数1：ef_id：事件标志组句柄。
	参数2：flags：要清除的标志位（位掩码，如0x01表示清除第 0 位）。若为0，则清除所有标志。
	返回值：
		成功：清除操作前的标志值。
		失败：0xFFFFFFFF（如ef_id无效）。
	uint32_t osEventFlagsClear(osEventFlagsId_t ef_id, uint32_t flags);
	
	功能：等待事件标志
	参数1：ef_id：事件标志组句柄。
	参数2：flags：要等待的标志位（位掩码）。
	参数3：options：等待选项（按位或组合）：
		osFlagsWaitAll：等待所有标志位被设置。
		osFlagsWaitAny：等待任意标志位被设置。
		osFlagsNoClear：等待成功后不自动清除标志位（默认会清除）。 *此参数可与前两个的其中一个或
	参数4：timeout：等待超时时间（ms），支持：
		0：立即返回（非阻塞）。
		osWaitForever：永久等待（阻塞）。
	返回值：
		成功：满足条件的标志值（自动清除前的状态，除非指定osFlagsNoClear）。
		超时：0（当timeout到期且标志未满足条件）。
		失败：-1（如ef_id无效或options参数错误）。
	int32_t osEventFlagsWait(osEventFlagsId_t ef_id, uint32_t flags, uint32_t options, uint32_t timeout);
	
	功能：删除事件标志组
	参数1：ef_id：事件标志组句柄。
	返回值：
		osOK：事件标志组成功删除。
		osErrorResource：事件标志组已被删除或不存在。
		osErrorParameter：ef_id为NULL。
	osStatus_t osEventFlagsDelete(osEventFlagsId_t ef_id);
***/


//EVENT 事件标志组 RTOS API2.0
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
