//Timer �����ʱ��
//��FreeRTOS �У������ʱ����Software Timers�� �ǻ���ϵͳʱ��ʵ�ֵĶ�ʱ�����ƣ�
//����������ж���ָ��ʱ���ִ�лص�����

/***
	���ܣ�������̬�����ʱ����
	����1��pcTimerName����ʱ�����ƣ����ڵ��ԣ���Ϊ NULL����
	����2��xTimerPeriod����ʱ�����ڣ���λ��ϵͳ���ģ�pdMS_TO_TICKS(ms) ת������
	����3��xAutoReload��pdTRUE�����ڶ�ʱ�����Զ���������pdFALSE�����ζ�ʱ����ִ��һ�κ�ֹͣ����
	����4��pvTimerID����ʱ�� ID���û����ݣ���ͨ�� pvTimerGetTimerID() ��ȡ����
	����5��pxCallback����ʱ���ص�����ָ�루ԭ�ͣ�void callback(TimerHandle_t xTimer)����
	����ֵ���ɹ������ض�ʱ�������TimerHandle_t����ʧ�ܣ����� NULL���ڴ����ʧ�ܣ���
	TimerHandle_t xTimerCreate(
			const char * const pcTimerName,
			const TickType_t xTimerPeriod,
			const BaseType_t xAutoReload,
			void * const pvTimerID,
			TimerCallbackFunction_t pxCallback
	);
***/

/***
	���ܣ�������������ʱ�������񻷾�����
	����1��xTimer��������
	����2��xTicksToWait���ȴ���ʱ��������еĳ�ʱʱ�䣨portMAX_DELAY ��ʾ���õȴ�����
	����ֵ��pdPASS������ɹ�������У�����֤��ʱ��������������pdFAIL����������������޷����롣
	BaseType_t xTimerStart(TimerHandle_t xTimer, TickType_t xTicksToWait);
	
	���ܣ����ö�ʱ�������¿�ʼ��ʱ�����۵�ǰ״̬����
	����1��xTimer��������
	����2��xTicksToWait���ȴ���ʱ��������еĳ�ʱʱ�䣨portMAX_DELAY ��ʾ���õȴ�����
	����ֵ��pdPASS������ɹ�������У�����֤��ʱ��������������pdFAIL����������������޷����롣
	BaseType_t xTimerReset(TimerHandle_t xTimer, TickType_t xTicksToWait);
	
	���ܣ����жϷ������ISR����������ʱ����
	BaseType_t xTimerStartFromISR(
    TimerHandle_t xTimer,
    BaseType_t *pxHigherPriorityTaskWoken
	);
	
	���ܣ�ֹͣ��ʱ�������񻷾�����
	BaseType_t xTimerStop(TimerHandle_t xTimer, TickType_t xTicksToWait);
	
	���ܣ��� ISR ��ֹͣ��ʱ����
	BaseType_t xTimerStopFromISR(
    TimerHandle_t xTimer,
    BaseType_t *pxHigherPriorityTaskWoken
	);
	
	���ܣ���̬���Ķ�ʱ�����ڣ������������еĶ�ʱ����
	����1�����
	����2��xNewPeriod���µĶ�ʱ�����ڣ�ϵͳ���ģ���
	����3���ȴ�ʱ��
	����ֵ��pdPASS������ɹ�������С�pdFAIL���������������
	BaseType_t xTimerChangePeriod(
    TimerHandle_t xTimer,
    TickType_t xNewPeriod,
    TickType_t xTicksToWait
	);
	
	���ܣ��� ISR �и��Ķ�ʱ�����ڡ�
	BaseType_t xTimerChangePeriodFromISR(
    TimerHandle_t xTimer,
    TickType_t xNewPeriod,
    BaseType_t *pxHigherPriorityTaskWoken
	);
***/

/***
���ܣ���鶨ʱ���Ƿ��ڻ״̬������������
����ֵ��
pdTRUE����ʱ����������
pdFALSE����ʱ��δ��������ֹͣ��
BaseType_t xTimerIsTimerActive(const TimerHandle_t xTimer);

���ܣ���ȡ��ʱ���ĵ�ǰ���ڡ�
����ֵ����ʱ�����ڣ�ϵͳ���ģ���
TickType_t xTimerGetPeriod(const TimerHandle_t xTimer);

���ܣ���ȡ��ʱ�����û����� ID��
����ֵ��������ʱ��ʱ���õ� pvTimerID��
void *pvTimerGetTimerID(const TimerHandle_t xTimer);

���ܣ�ɾ����ʱ�����ͷ��ڴ棨��̬�����Ķ�ʱ����
void vTimerDelete(TimerHandle_t xTimer, TickType_t xTicksToWait);
ע�⣺ɾ��ǰ��ȷ����ʱ����ֹͣ��������Դй©
***/