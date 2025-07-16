// Queue:���� RTOS API5.0
//���У�Queue�� ��ʵ�������ͨ�ź�ͬ���ĺ��Ļ��ơ�
/***
	���ܣ���������ʼ��һ����Ϣ���С�
	����1��msg_count�����п����ɵ������Ϣ������
	����2��msg_size��ÿ����Ϣ�Ĵ�С���ֽڣ���
	����3��attr���������ԣ������ơ��ڴ����ȣ���
	����ֵ���ɹ������о������NULL����ʧ�ܣ�NULL�����ڴ治��������Ч��
	osMessageQueueId_t osMessageQueueNew(uint32_t msg_count, uint32_t msg_size, const osMessageQueueAttr_t *attr);
***/

/***
	���ܣ�������Ϣ������
	����1��mq_id�����о����
	����2��msg_ptr��ָ���������Ϣ��ָ�룬��Ϣ��С����Ϊmsg_size����������ʱָ������
	����3��msg_prio����Ϣ���ȼ���0 Ϊ��׼���ȼ������� RTOS ֧�ָ������ȼ�����
	����4��timeout���ȴ����пռ�ĳ�ʱʱ�䣨ms����֧�֣�
		0���������أ�����������
		osWaitForever�����õȴ�����������
	����ֵ��
		osOK����Ϣ�ɹ���ӡ�
		osErrorTimeout����ʱǰδ��ȡ�����пռ䡣
		osErrorResource������������timeout=0��
		osErrorParameter��������Ч����mq_idΪNULL����
		osErrorISR���� ISR ���ô˺�����Ӧʹ��PutFromISR����
	osStatus_t osMessageQueuePut(osMessageQueueId_t mq_id, const void *msg_ptr, uint8_t msg_prio, uint32_t timeout);
	
	���ܣ�������Ϣ�����У��жϣ�
	����1��mq_id�����о����
	����2��msg_ptr��ָ���������Ϣ��ָ�룬��Ϣ��С����Ϊmsg_size����������ʱָ������
	����3��msg_prio����Ϣ���ȼ���0 Ϊ��׼���ȼ������� RTOS ֧�ָ������ȼ�����
	����4��task_woken��ָ��uint32_t��ָ�룬����ָʾ�Ƿ�����������Ϣ��Ӷ������ѣ�
		���������񣬺����Ὣ*task_woken��Ϊ����ֵ������ ISR ����ǰ����osThreadYield()�����������л���
		��ΪNULL������Ի���״̬��
	����ֵ��
		osOK����Ϣ�ɹ���ӡ�
		osErrorTimeout����ʱǰδ��ȡ�����пռ䡣
		osErrorResource������������timeout=0��
		osErrorParameter��������Ч����mq_idΪNULL����
	osStatus_t osMessageQueuePutFromISR(osMessageQueueId_t mq_id, const void *msg_ptr, uint8_t msg_prio, uint32_t *task_woken);
***/

/***
	���ܣ��Ӷ��н�����Ϣ
	����1��mq_id�����о����
	����2��msg_ptr���洢���յ�����Ϣ�Ļ�����ָ�룬��С���msg_size��
	����3��msg_prio��ָ��uint8_t��ָ�룬���ڴ洢��Ϣ���ȼ�����ΪNULL���������ȼ�����
	����4��timeout���ȴ���Ϣ�ĳ�ʱʱ�䣨ms����֧��0��osWaitForever��
	����ֵ��
		osOK���ɹ���ȡ��Ϣ��
		osErrorTimeout����ʱǰ������Ϊ�ա�
		osErrorResource������Ϊ����timeout=0��
		osErrorParameter��������Ч����mq_idΪNULL��msg_ptrΪNULL����
		osErrorISR���� ISR ���ô˺�����Ӧʹ��GetFromISR��
	osStatus_t osMessageQueueGet(osMessageQueueId_t mq_id, void *msg_ptr, uint8_t *msg_prio, uint32_t timeout);
	
	���ܣ��Ӷ��н�����Ϣ���жϣ�
	����1��mq_id�����о����
	����2��msg_ptr���洢���յ�����Ϣ�Ļ�����ָ�룬��С���msg_size��
	����3��msg_prio��ָ��uint8_t��ָ�룬���ڴ洢��Ϣ���ȼ�����ΪNULL���������ȼ�����	
	����ֵ��
		osOK���ɹ���ȡ��Ϣ��
		osErrorResource������Ϊ�ա�
		osErrorParameter��������Ч��
	osStatus_t osMessageQueueGetFromISR(osMessageQueueId_t mq_id, void *msg_ptr, uint8_t *msg_prio);
***/

/***
	���ܣ��鿴�����е���Ϣ����
	����1��mq_id�����о����
	����ֵ��
		��ǰ�����е���Ϣ������0 �� count �� msg_count����
		��mq_id��Ч������ֵδ���壨ͨ��Ϊ 0 ���Ԥ��ֵ����
	uint32_t osMessageQueueGetCount(osMessageQueueId_t mq	_id);
	
	���ܣ��鿴�����еĿ��ÿռ�
	����1��mq_id�����о����
	����ֵ��
		������ʣ��Ŀ��ÿռ䣨�����ܴ�ŵ���Ϣ��������
		��mq_id��Ч������ֵδ���塣
	uint32_t osMessageQueueGetSpace(osMessageQueueId_t mq_id);
	
	���ܣ�ɾ������
	����1��mq_id�����о��
	����ֵ��
		osOK�����гɹ�ɾ����
		osErrorResource�������ѱ�ɾ���򲻴��ڡ�
		osErrorParameter��mq_idΪNULL��
	osStatus_t osMessageQueueDelete(osMessageQueueId_t mq_id);
***/

/***
	���Ĺ���
	�����ó� CPU
	��ǰ���е��߳���ʱ����ִ��Ȩ�����������ѡ����������̬��ͬ���ȼ���������ȼ��߳�ִ�С�
	�����������л�
	ǿ�Ƶ��������������߳����ȼ���������һ��ִ�е��̡߳���û�и������ȼ��߳̾�������ǰ�߳̿��ܻᱻ���µ��ȡ�
	void osThreadYield(void);
***/







// Queue:���� RTOS API2.0
//���У�Queue�� ��ʵ�������ͨ�ź�ͬ���ĺ��Ļ��ơ�
/***
	����:ɾ������
	����1:xQueue -->���
	����ֵ����
	void vQueueDelete(QueueHandle_t xQueue);
***/

/***
	����:���ö���--����������ݣ��ָ���ʼ״̬
	����1:xQueue -->���
	����ֵ����
	void vQueueReset(QueueHandle_t xQueue);
***/

/***
	����:����з�������(β��)
	����1:xQueue�����
	����2:pvItemToQueue��ָ��Ҫ���͵����ݵ�ָ��
	����3:xTicksToWait���ȴ�ʱ��
	����ֵ��pdTRUE���ɹ����� pdFALSE��
	BaseType_t xQueueSend(
    QueueHandle_t xQueue,
    const void *pvItemToQueue,
    TickType_t xTicksToWait
);
***/

/***
	����:�ж�������з�������(β��)
	����1:xQueue�����
	����2:pvItemToQueue��ָ��Ҫ���͵����ݵ�ָ��
	����3:pxHigherPriorityTaskWoken��ָ��һ�� BaseType_t ������ָ�룬����ָʾ�Ƿ��и������ȼ����������ȡ�ź����������ѡ�
	����ֵ��pdTRUE���ɹ����� pdFALSE��
	BaseType_t xQueueSendFromISR(
    QueueHandle_t xQueue,
    const void *pvItemToQueue,
    BaseType_t *pxHigherPriorityTaskWoken
);
***/

/***
	����:���н�������
	����1:xQueue�����
	����2:pvBuffer��ָ��Ҫ���յ����ݵ�ָ��
	����3:xTicksToWait���ȴ�ʱ��
	����ֵ��pdTRUE���ɹ����� pdFALSE��
	BaseType_t xQueueReceive(
			QueueHandle_t xQueue,
			void *pvBuffer,
			TickType_t xTicksToWait
	);
***/

/***
	����:�ж��ж��н�������
	����1:xQueue�����
	����2:pvBuffer��ָ��Ҫ���յ����ݵ�ָ��
	����3:pxHigherPriorityTaskWoken��ָ��һ�� BaseType_t ������ָ�룬����ָʾ�Ƿ��и������ȼ����������ȡ�ź����������ѡ�
	����ֵ��pdTRUE���ɹ����� pdFALSE��
	BaseType_t xQueueReceiveFromISR(
			QueueHandle_t xQueue,
			void *pvBuffer,
			BaseType_t *pxHigherPriorityTaskWoken
	);
***/

/***
	����:��ȡ�����е�ǰ����Ϣ����
	����1:xQueue�����
	����ֵ��UBaseType_t������
	UBaseType_t uxQueueMessagesWaiting(const QueueHandle_t xQueue);
***/

/***
	����:��ȡ������ʣ��Ŀ��пռ�
	����1:xQueue�����
	����ֵ��UBaseType_t������
	UBaseType_t uxQueueSpacesAvailable(const QueueHandle_t xQueue);
***/

/***
	����:���ж��м������Ƿ�Ϊ�ջ�������
	����1:xQueue�����
	����ֵ��UBaseType_t������
	BaseType_t xQueueIsQueueEmptyFromISR(const QueueHandle_t xQueue);
	BaseType_t xQueueIsQueueFullFromISR(const QueueHandle_t xQueue);
***/