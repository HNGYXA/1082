// Queue:队列 RTOS API5.0
//队列（Queue） 是实现任务间通信和同步的核心机制。
/***
	功能：创建并初始化一个消息队列。
	参数1：msg_count：队列可容纳的最大消息数量。
	参数2：msg_size：每条消息的大小（字节）。
	参数3：attr：队列属性（如名称、内存分配等）。
	返回值：成功：队列句柄（非NULL）。失败：NULL（如内存不足或参数无效）
	osMessageQueueId_t osMessageQueueNew(uint32_t msg_count, uint32_t msg_size, const osMessageQueueAttr_t *attr);
***/

/***
	功能：发送消息到队列
	参数1：mq_id：队列句柄。
	参数2：msg_ptr：指向待发送消息的指针，消息大小必须为msg_size（创建队列时指定）。
	参数3：msg_prio：消息优先级（0 为标准优先级，部分 RTOS 支持更高优先级）。
	参数4：timeout：等待队列空间的超时时间（ms），支持：
		0：立即返回（非阻塞）。
		osWaitForever：永久等待（阻塞）。
	返回值：
		osOK：消息成功入队。
		osErrorTimeout：超时前未获取到队列空间。
		osErrorResource：队列已满且timeout=0。
		osErrorParameter：参数无效（如mq_id为NULL）。
		osErrorISR：从 ISR 调用此函数（应使用PutFromISR）。
	osStatus_t osMessageQueuePut(osMessageQueueId_t mq_id, const void *msg_ptr, uint8_t msg_prio, uint32_t timeout);
	
	功能：发送消息到队列（中断）
	参数1：mq_id：队列句柄。
	参数2：msg_ptr：指向待发送消息的指针，消息大小必须为msg_size（创建队列时指定）。
	参数3：msg_prio：消息优先级（0 为标准优先级，部分 RTOS 支持更高优先级）。
	参数4：task_woken：指向uint32_t的指针，用于指示是否有任务因消息入队而被唤醒：
		若唤醒任务，函数会将*task_woken设为非零值，需在 ISR 返回前调用osThreadYield()触发上下文切换。
		若为NULL，则忽略唤醒状态。
	返回值：
		osOK：消息成功入队。
		osErrorTimeout：超时前未获取到队列空间。
		osErrorResource：队列已满且timeout=0。
		osErrorParameter：参数无效（如mq_id为NULL）。
	osStatus_t osMessageQueuePutFromISR(osMessageQueueId_t mq_id, const void *msg_ptr, uint8_t msg_prio, uint32_t *task_woken);
***/

/***
	功能：从队列接收消息
	参数1：mq_id：队列句柄。
	参数2：msg_ptr：存储接收到的消息的缓冲区指针，大小需≥msg_size。
	参数3：msg_prio：指向uint8_t的指针，用于存储消息优先级（可为NULL，忽略优先级）。
	参数4：timeout：等待消息的超时时间（ms），支持0、osWaitForever。
	返回值：
		osOK：成功获取消息。
		osErrorTimeout：超时前队列仍为空。
		osErrorResource：队列为空且timeout=0。
		osErrorParameter：参数无效（如mq_id为NULL或msg_ptr为NULL）。
		osErrorISR：从 ISR 调用此函数（应使用GetFromISR）
	osStatus_t osMessageQueueGet(osMessageQueueId_t mq_id, void *msg_ptr, uint8_t *msg_prio, uint32_t timeout);
	
	功能：从队列接收消息（中断）
	参数1：mq_id：队列句柄。
	参数2：msg_ptr：存储接收到的消息的缓冲区指针，大小需≥msg_size。
	参数3：msg_prio：指向uint8_t的指针，用于存储消息优先级（可为NULL，忽略优先级）。	
	返回值：
		osOK：成功获取消息。
		osErrorResource：队列为空。
		osErrorParameter：参数无效。
	osStatus_t osMessageQueueGetFromISR(osMessageQueueId_t mq_id, void *msg_ptr, uint8_t *msg_prio);
***/

/***
	功能：查看队列中的消息数量
	参数1：mq_id：队列句柄。
	返回值：
		当前队列中的消息数量（0 ≤ count ≤ msg_count）。
		若mq_id无效，返回值未定义（通常为 0 或非预期值）。
	uint32_t osMessageQueueGetCount(osMessageQueueId_t mq	_id);
	
	功能：查看队列中的可用空间
	参数1：mq_id：队列句柄。
	返回值：
		队列中剩余的可用空间（即还能存放的消息数量）。
		若mq_id无效，返回值未定义。
	uint32_t osMessageQueueGetSpace(osMessageQueueId_t mq_id);
	
	功能：删除队列
	参数1：mq_id：队列句柄
	返回值：
		osOK：队列成功删除。
		osErrorResource：队列已被删除或不存在。
		osErrorParameter：mq_id为NULL。
	osStatus_t osMessageQueueDelete(osMessageQueueId_t mq_id);
***/

/***
	核心功能
	主动让出 CPU
	当前运行的线程暂时放弃执行权，允许调度器选择其他就绪态的同优先级或更高优先级线程执行。
	触发上下文切换
	强制调度器重新评估线程优先级，决定下一个执行的线程。若没有更高优先级线程就绪，则当前线程可能会被重新调度。
	void osThreadYield(void);
***/







// Queue:队列 RTOS API2.0
//队列（Queue） 是实现任务间通信和同步的核心机制。
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