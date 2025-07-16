// Semaphore:�ź��� RTOS API5.0
//��FreeRTOS�У���ֵ�ź�����Binary Semaphore�� ��ʵ�������ͬ������Դ����Ļ������ơ�
/***
	���ܣ�������ֵ�ź�����ͨ�� max_count=1 ʵ�֣���
	����1��max_count���ź���������ֵ����ֵ�ź�����Ϊ 1����
	����2��initial_count����ʼ����ֵ��0 �� 1����
	����3��attr���ź������ԣ������ơ��ڴ����򣬿�Ϊ NULL����
	����ֵ���ɹ��������ź��� ID��osSemaphoreId_t����ʧ�ܣ����� NULL��
	osSemaphoreId_t osSemaphoreNew(
			uint32_t max_count,
			uint32_t initial_count,
			const osSemaphoreAttr_t *attr
	);
***/

/***
	���ܣ���ȡ�ź���������ģʽ��
	����1��semaphore_id���ź��� ID��
	����2��timeout���ȴ���ʱʱ�䣨osWaitForever ��ʾ���õȴ�����
	osStatus_t osSemaphoreAcquire(
			osSemaphoreId_t semaphore_id,
			uint32_t timeout
	);
***/

/***
	���ܣ��ͷ��ź�����
	����1��semaphore_id���ź��� ID��
	����ֵ��osOK���ɹ��ͷš�osErrorResource���ź�����ɾ�������ֵ�Ѵ����ֵ��
	osStatus_t osSemaphoreRelease(osSemaphoreId_t semaphore_id);
	
	���ܣ����жϷ���������ͷ��ź�����
	����1��semaphore_id���ź��� ID��
	����2��task_woken��ָ��uint32_t��ָ�룬����ָʾ�ͷ��ź����Ƿ�����ĳ�����񣨷���ֵ��ʾ�����񱻻��ѣ������жϷ���ǰִ���������л�����
	osStatus_t osSemaphoreReleaseFromISR(osSemaphoreId_t semaphore_id, uint32_t *task_woken);
***/

/***
	���ܣ�ɾ���ź������ͷ��ڴ棨��̬�������ź�������
	����1��semaphore_id���ź��� ID��
	void vSemaphoreDelete(SemaphoreHandle_t xSemaphore);
***/

/***
	���ܣ���ȡ��ǰ�ź����ļ���ֵ��
	����1��semaphore_id���ź��� ID��
	uint32_t osSemaphoreGetCount(osSemaphoreId_t semaphore_id);
***/







// Semaphore:�ź��� RTOS API2.0
//��FreeRTOS�У���ֵ�ź�����Binary Semaphore�� ��ʵ�������ͬ������Դ����Ļ������ơ�
/***
	����:���Ի�ȡ��ֵ�ź���
	����1:Freq_SemHandle -->���
	����2:portMAX_DELAY -->�ȴ�ʱ��--ms
	����ֵ��pdTRUE���ɹ���ȡ���� pdFALSE����ʱ����
	xSemaphoreTake(SemaphoreHandle_t Freq_SemHandle,portMAX_DELAY); 
***/

/***
	����:�ͷŶ�ֵ�ź���
	����1:Freq_SemHandle -->���
	����ֵ��pdTRUE���ɹ����� pdFALSE��
	xSemaphoreGive(SemaphoreHandle_t Freq_SemHandle);
***/

/***
	����:���ж��л�ȡ��ֵ�ź���
	����1:xBinarySemaphore -->���
	����2: &xHigherPriorityTaskWoken -->ָ��һ�� BaseType_t ������ָ�룬����ָʾ�Ƿ��и������ȼ����������ȡ�ź����������ѡ�
	����ֵ��pdTRUE���ɹ����� pdFALSE��
	xSemaphoreTakeFromISR(SemaphoreHandle_t xBinarySemaphore, &xHigherPriorityTaskWoken);
***/

/***
	����:���ж����ͷŶ�ֵ�ź���
	����1:xBinarySemaphore -->���
	����2: &xHigherPriorityTaskWoken -->ָ��һ�� BaseType_t ������ָ�룬����ָʾ�Ƿ��и������ȼ����������ȡ�ź����������ѡ�
	����ֵ��pdTRUE���ɹ����� pdFALSE��
	xSemaphoreGiveFromISR(SemaphoreHandle_t xBinarySemaphore, &xHigherPriorityTaskWoken);
***/

/***
	����:ɾ����ֵ�ź���
	����1:xSemaphore -->���
	����ֵ����
	void vSemaphoreDelete(SemaphoreHandle_t xSemaphore);
***/