//EVENT �¼���־�� RTOS API5.0
//�� FreeRTOS �У��¼���־�飨Event Groups�� ��һ��ͬ�����ƣ������������������жϼ���¼�֪ͨ��
//��ͨ��һ�� 32 λ�ı�־λ���ϣ�ÿһλ����һ���¼���ʵ�ָ�Ч���¼���־����

/***
	���ܣ������¼���־��
	����1��attr���¼���־�����Խṹ��ָ�룬���������ơ��ڴ���䷽ʽ�ȡ���ΪNULL����ʹ��Ĭ�����ԡ�
	����ֵ��
		�ɹ����¼���־��������NULL����
		ʧ�ܣ�NULL�����ڴ治��������Ч��
	osEventFlagsId_t osEventFlagsNew(const osEventFlagsAttr_t *attr);
	
	const osEventFlagsAttr_t Event1_attributes = {
  .name = "Event1"
	};
***/

/***
	���ܣ������¼���־
	����1��ef_id���¼���־������
	����2��flags��Ҫ���õı�־λ��λ���룬��0x01��ʾ���õ� 0 λ����
	����ֵ��
		�ɹ������ò�����ĵ�ǰ��־ֵ��
		ʧ�ܣ�0xFFFFFFFF����ef_id��Ч����
	uint32_t osEventFlagsSet(osEventFlagsId_t ef_id, uint32_t flags);
	
	���ܣ�����¼���־
	����1��ef_id���¼���־������
	����2��flags��Ҫ����ı�־λ��λ���룬��0x01��ʾ����� 0 λ������Ϊ0����������б�־��
	����ֵ��
		�ɹ����������ǰ�ı�־ֵ��
		ʧ�ܣ�0xFFFFFFFF����ef_id��Ч����
	uint32_t osEventFlagsClear(osEventFlagsId_t ef_id, uint32_t flags);
	
	���ܣ��ȴ��¼���־
	����1��ef_id���¼���־������
	����2��flags��Ҫ�ȴ��ı�־λ��λ���룩��
	����3��options���ȴ�ѡ���λ����ϣ���
		osFlagsWaitAll���ȴ����б�־λ�����á�
		osFlagsWaitAny���ȴ������־λ�����á�
		osFlagsNoClear���ȴ��ɹ����Զ������־λ��Ĭ�ϻ�������� *�˲�������ǰ����������һ����
	����4��timeout���ȴ���ʱʱ�䣨ms����֧�֣�
		0���������أ�����������
		osWaitForever�����õȴ�����������
	����ֵ��
		�ɹ������������ı�־ֵ���Զ����ǰ��״̬������ָ��osFlagsNoClear����
		��ʱ��0����timeout�����ұ�־δ������������
		ʧ�ܣ�-1����ef_id��Ч��options�������󣩡�
	int32_t osEventFlagsWait(osEventFlagsId_t ef_id, uint32_t flags, uint32_t options, uint32_t timeout);
	
	���ܣ�ɾ���¼���־��
	����1��ef_id���¼���־������
	����ֵ��
		osOK���¼���־��ɹ�ɾ����
		osErrorResource���¼���־���ѱ�ɾ���򲻴��ڡ�
		osErrorParameter��ef_idΪNULL��
	osStatus_t osEventFlagsDelete(osEventFlagsId_t ef_id);
***/


//EVENT �¼���־�� RTOS API2.0
//�� FreeRTOS �У��¼���־�飨Event Groups�� ��һ��ͬ�����ƣ������������������жϼ���¼�֪ͨ��
//��ͨ��һ�� 32 λ�ı�־λ���ϣ�ÿһλ����һ���¼���ʵ�ָ�Ч���¼���־����

/***
	���ܣ�������̬�¼���־�顣
	����ֵ���ɹ��������¼���־������EventGroupHandle_t����ʧ�ܣ����� NULL���ڴ����ʧ�ܣ���
	EventGroupHandle_t xEventGroupCreate(void);
	ע�⣺���� FreeRTOSConfig.h ������ configUSE_EVENT_GROUPS��
	
	���ܣ���̬�����¼���־�飨���趯̬�ڴ���䣩��
	����ֵ���ɹ��������¼���־������EventGroupHandle_t����ʧ�ܣ����� NULL���ڴ����ʧ�ܣ���
	������pxEventGroupBuffer ָ��̬�洢���Ľṹ�塣
	EventGroupHandle_t xEventGroupCreateStatic(StaticEventGroup_t *pxEventGroupBuffer);
***/

/***
	���ܣ��������������¼���־���ָ��λ��
	����1��xEventGroup���¼���־������
	����2��uxBitsToSet��Ҫ���õ�λ���루�� 0x01 ��ʾ�� 0 λ����
	����ֵ������λ���¼���־��ĵ�ǰֵ��
	EventBits_t xEventGroupSetBits(
			EventGroupHandle_t xEventGroup,
			const EventBits_t uxBitsToSet
	);
	

	���ܣ����жϷ������ISR���������¼���־���ָ��λ��
	����1��xEventGroup���¼���־������
	����2��uxBitsToSet��Ҫ���õ�λ���루�� 0x01 ��ʾ�� 0 λ����
	����3��pxHigherPriorityTaskWoken�����ڱ���Ƿ���Ҫ�����������л���
	����ֵ��pdPASS�������ɹ���pdFAIL���¼���־���ѱ�ɾ�������ٷ�����
	BaseType_t xEventGroupSetBitsFromISR(
    EventGroupHandle_t xEventGroup,
    const EventBits_t uxBitsToSet,
    BaseType_t *pxHigherPriorityTaskWoken
	);
	
	���ܣ�����¼���־���ָ��λ��
	����1��xEventGroup���¼���־����
	����2��uxBitsToClear��Ҫ�����λ���롣
	����ֵ�����λǰ�¼���־���ֵ��
	EventBits_t xEventGroupClearBits(
    EventGroupHandle_t xEventGroup,
    const EventBits_t uxBitsToClear
	);
***/

/***
	���ܣ��ȴ�ָ���¼���־λ�����ã�����ģʽ����
	����1��xEventGroup���¼���־����
	����2��uxBitsToWaitFor���ȴ���λ���루�� 0x03 ��ʾ�ȴ��� 0 λ�͵� 1 λ����
	����3��xClearOnExit��pdTRUE���˳�ǰ�Զ�����ȴ���λ��pdFALSE���������
	����4��xWaitForAllBits��pdTRUE�����еȴ���λ��������ʱ�Ż��ѣ��߼��룩pdFALSE������һ���ȴ���λ������ʱ�ͻ��ѣ��߼��򣩡�
	����5��xTicksToWait����ʱʱ�䣨portMAX_DELAY ��ʾ���õȴ�����
	EventBits_t xEventGroupWaitBits(
			EventGroupHandle_t xEventGroup,
			const EventBits_t uxBitsToWaitFor,
			const BaseType_t xClearOnExit,
			const BaseType_t xWaitForAllBits,
			TickType_t xTicksToWait
	);
***/

/***
	���ܣ���ȡ�¼���־��ĵ�ǰֵ������������
	����ֵ���¼���־��ĵ�ǰֵ��
	EventBits_t xEventGroupGetBits(EventGroupHandle_t xEventGroup);
	
	���ܣ��� ISR �л�ȡ�¼���־��ĵ�ǰֵ��
	����ֵ���¼���־��ĵ�ǰֵ��
	EventBits_t xEventGroupGetBitsFromISR(EventGroupHandle_t xEventGroup);
	
	���ܣ�ɾ���¼���־�鲢�ͷ��ڴ棨��̬�����ģ�
	ע�⣺ɾ��ǰ��ȷ��������ȴ����¼���־�顣
	void vEventGroupDelete(EventGroupHandle_t xEventGroup);
***/
