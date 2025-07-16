//EVENT �¼���־��
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