#include "API.h"
#include "FreeRTOS.h"
#include "cmsis_os.h"
#include "cmsis_os2.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "event_groups.h"
#include "timers.h"
// Semaphore:�ź���
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

// Queue:����
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

//Mutexes:������
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