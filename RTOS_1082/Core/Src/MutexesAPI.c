//Mutexes:������ RTOS API5.0
//��ʵʱ����ϵͳ��RTOS���У���������Mutex�� ��ʵ����Դ������ʵĺ��Ļ��ƣ����ڽ�������񻷾��µľ�̬����
//Mutexes�������ж���ʹ�� -- mutexes���ڴ�������֮������ȼ���ת���⣬�ж�û�����ȼ�
//��ȡ���ͷ�Ҫ��ͬһ������
/***
	����:������̬������
	����ֵ���ɹ����ػ����������ʧ�ܷ��� NULL��
	������attr�����������Խṹ��ָ�룬���������ơ��ڴ���䷽ʽ���ݹ����Եȡ���ΪNULL����ʹ��Ĭ�����ԡ�
	osMutexId_t osMutexNew(const osMutexAttr_t *attr);
	
	const osMutexAttr_t UartMutexes_attributes = {
  .name = "UartMutexes"
	};
***/

/***
	����: ��ȡ������	
	����1��mutex_id�������������
	����2��timeout���ȴ��������ĳ�ʱʱ�䣨ms����
		֧�֣�0���������أ�����������
		osWaitForever�����õȴ���������
	����ֵ��
		osOK���ɹ���ȡ��������
		osErrorTimeout����ʱǰδ��ȡ����������
		osErrorResource���������ѱ�ռ����timeout=0��
		osErrorParameter��������Ч����mutex_idΪNULL����
		osErrorISR���� ISR ���ô˺����������������� ISR ��ʹ�ã���
	osStatus_t osMutexAcquire(osMutexId_t mutex_id, uint32_t timeout);
	
	����: �ͷŻ�����
	����1��mutex_id�������������
	����ֵ��
		osOK���ɹ��ͷŻ�������
		osErrorResource����ǰ�߳�δ���иû�������
		osErrorParameter��mutex_idΪNULL��
		osErrorISR���� ISR ���ô˺�����
	osStatus_t osMutexRelease(osMutexId_t mutex_id);
	
	����: ɾ��������
	����1��mutex_id�������������
	����ֵ��
		osOK���������ɹ�ɾ����
		osErrorResource���������ѱ�ɾ���򲻴��ڡ�
		osErrorParameter��mutex_idΪNULL��
	osStatus_t osMutexDelete(osMutexId_t mutex_id);
***/






//Mutexes:������ RTOS API2.0
//��ʵʱ����ϵͳ��RTOS���У���������Mutex�� ��ʵ����Դ������ʵĺ��Ļ��ƣ����ڽ�������񻷾��µľ�̬����
/***
	����:������̬����������ʼ����ֵΪ 1����ʾ��Դ���ã���
	����ֵ���ɹ����ػ����������ʧ�ܷ��� NULL��
	SemaphoreHandle_t xSemaphoreCreateMutex(void);
	ע�⣺�����󼴿�ʹ�ã���������ʼ����
***/

/***
	����:��̬���������������趯̬�ڴ���䣩��
	����1��pxMutexBuffer ָ��̬�洢���Ľṹ�塣
	����ֵ���ɹ����ػ����������ʧ�ܷ��� NULL��
	SemaphoreHandle_t xSemaphoreCreateMutexStatic(StaticSemaphore_t *pxMutexBuffer);
***/

/***
	����:��ȡ������������ģʽ����
	����1��xMutex ���������
	����2;xTicksToWait �ȴ���ʱʱ�䣨portMAX_DELAY ��ʾ���õȴ���
	����ֵ��pdTRUE���ɹ���ȡ�������� pdFALSE����ʱ��δ��ȡ����
	BaseType_t xSemaphoreTake(
    SemaphoreHandle_t xMutex,
    TickType_t xTicksToWait
	);
***/

/***
	����:�ͷŻ������������ɳ��л������������ͷţ�
	����1��xMutex ���������
	����ֵ��pdTRUE���ɹ���ȡ�������� pdFALSE��ʧ�ܣ��糢���ͷ�δ���еĻ���������
	BaseType_t xSemaphoreGive(SemaphoreHandle_t xMutex);
	���ѵȴ��û�������������ȼ�������Ϊ�����ȼ��̳С�
	�������������жϷ������ISR�����ͷţ���Ϊ�ж��޷����л�������
***/

/***
	����:�����ݹ黥����

	����ֵ���ɹ����ؾ����ʧ�ܷ��� NULL��
	��̬��SemaphoreHandle_t xSemaphoreCreateRecursiveMutex(void);
	��̬��SemaphoreHandle_t xSemaphoreCreateRecursiveMutexStatic(StaticSemaphore_t *pxMutexBuffer);
	
	���ܣ��ݹ��ȡ��������ͬһ����ɶ�λ�ȡ����
	BaseType_t xSemaphoreTakeRecursive(
    SemaphoreHandle_t xMutex,
    TickType_t xTicksToWait
	);
	�����뷵��ֵ��ͬ xSemaphoreTake()��
	��Ϊ��
	ÿ�λ�ȡʱ�ڲ��������� 1��
	���Ӧ�������ͷŲ�����ȫ������
	
	
	���ܣ��ݹ��ͷŻ������������ڲ�����������
	����ֵ��pdTRUE���ɹ��ͷš�pdFALSE��ʧ�ܣ����ͷŴ���������ȡ��������
	BaseType_t xSemaphoreGiveRecursive(SemaphoreHandle_t xMutex);
	
	���ܣ�ɾ��������
	void vSemaphoreDelete(SemaphoreHandle_t xMutex);
***/
