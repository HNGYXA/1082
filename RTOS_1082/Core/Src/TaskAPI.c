#include "API.h"
#include "FreeRTOS.h"
#include "cmsis_os.h"
#include "cmsis_os2.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "event_groups.h"
#include "timers.h"
// Task ����  RTOS2 API5.0
/***
	���ܣ���������������
	����1��func��������ָ�루ԭ�ͣ�void func(void *argument)����
	����2��argument�����ݸ�����Ĳ�������Ϊ NULL����
	����3��attr���������ԣ������ơ�ջ��С�����ȼ�����Ϊ NULL����
	����ֵ���ɹ����������� ID��osThreadId_t����ʧ�ܣ����� NULL��
	osThreadId_t osThreadNew(
    osThreadFunc_t func,
    void *argument,
    const osThreadAttr_t *attr
	);
	defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);
	const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",  //����
  .stack_size = 128 * 4,	//�ڴ�  128word
  .priority = (osPriority_t) osPriorityNormal, //���ȼ�
	};
***/

/***
	���ܣ���ָֹ������
	����1��thread_id������ ID��NULL ��ʾ��ǰ���񣩡�
	����ֵ��osOK�������ɹ���osErrorResource�����񲻴��ڻ�����ֹ��
	osStatus_t osThreadTerminate(osThreadId_t thread_id);
	
	���ܣ�����ָ������
	����1��thread_id������ ID��NULL ��ʾ��ǰ���񣩡�
	����ֵ��osOK�������ɹ���osErrorResource�����񲻴��ڻ�����ֹ��
	osStatus_t osThreadSuspend(osThreadId_t thread_id);
	
	���ܣ��ָ������������
	����1��thread_id������ ID��NULL ��ʾ��ǰ���񣩡�
	����ֵ��osOK�������ɹ���osErrorResource�����񲻴��ڻ�����ֹ��
	osStatus_t osThreadResume(osThreadId_t thread_id);
	
	���ܣ������������ȼ���
	����1��thread_id������ ID��NULL ��ʾ��ǰ���񣩡�
	����2��priority�������ȼ���ö��ֵ���� osPriorityNormal����
	����ֵ��osOK�������ɹ���osErrorResource�����񲻴��ڻ�����ֹ��
	osStatus_t osThreadSetPriority(osThreadId_t thread_id, osPriority_t priority);
	
	���ܣ���ȡ�������ȼ���
	����1��thread_id������ ID��NULL ��ʾ��ǰ���񣩡�
	����ֵ������ĵ�ǰ���ȼ���
	osPriority_t osThreadGetPriority(osThreadId_t thread_id);
	
	���ܣ���ǰ������ʱָ��ʱ�䡣
	����1��ticks����ʱ��ϵͳ��������
	����ֵ��osOK����ʱ�ɹ���osErrorParameter��������Ч��
	osStatus_t osDelay(uint32_t ticks);
***/

/***
	���Ĺ���:�����ó� CPU
	��ǰ���е��߳���ʱ����ִ��Ȩ�����������ѡ����������̬��ͬ���ȼ���������ȼ��߳�ִ�С�
	�����������л�
	ǿ�Ƶ��������������߳����ȼ���������һ��ִ�е��̡߳���û�и������ȼ��߳̾�������ǰ�߳̿��ܻᱻ���µ��ȡ�
	void osThreadYield(void);
***/


//Task  ���� RTOS API2.0
/***
	���ܣ�������̬����
	����1��pxTaskCode��������ָ�루ԭ�ͣ�void vTaskFunction(void *pvParameters)����
	����2��pcName���������ƣ����ڵ��ԣ���Ϊ NULL����
	����3��usStackDepth������ջ��С����λ���֣��� 1024 ��ʾ 1024��4 �ֽڣ���
	����4��pvParameters�����ݸ�����Ĳ�������Ϊ NULL����
	����5��uxPriority���������ȼ���0 Ϊ������ȼ�����
	����6��pxCreatedTask�����ڷ�������������Ϊ NULL����
	����ֵ��pdPASS�����񴴽��ɹ���errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY���ڴ����ʧ�ܡ�
	BaseType_t xTaskCreate(
    TaskFunction_t pxTaskCode,
    const char * const pcName,
    configSTACK_DEPTH_TYPE usStackDepth,
    void *pvParameters,
    UBaseType_t uxPriority,
    TaskHandle_t *pxCreatedTask
	);
***/

/*** 
	���ܣ�ɾ�������ͷ���Դ��
	����1��xTaskToDelete��Ҫɾ������������NULL ��ʾɾ����ǰ���񣩡�
	void vTaskDelete(TaskHandle_t xTaskToDelete);
	
	���ܣ�����ָ��������ִͣ�У���
	void vTaskSuspend(TaskHandle_t xTaskToSuspend);
	
	���ܣ��ָ������������
	����ֵ��pdTRUE������ָ�����Ҫ�����������л���pdFALSE�������л���
	BaseType_t xTaskResume(TaskHandle_t xTaskToResume);
	
	���ܣ����ж��лָ�����
	����ֵ��pdTRUE������ָ�����Ҫ�����������л���pdFALSE�������л���
	BaseType_t xTaskResumeFromISR(TaskHandle_t xTaskToResume);
	
	���ܣ������������ȼ���
	����1��xTask��Ŀ����������NULL ��ʾ��ǰ���񣩡�
	����2��uxNewPriority�������ȼ�ֵ��
	void vTaskPrioritySet(TaskHandle_t xTask, UBaseType_t uxNewPriority);
	
	���ܣ���ȡ�������ȼ���
	����ֵ������ĵ�ǰ���ȼ���
	UBaseType_t uxTaskPriorityGet(TaskHandle_t xTask);
	
	���ܣ���ǰ������ʱָ��ʱ�䣨������ʱ����
	����1��xTicksToWait����ʱ��ϵͳ����������ͨ�� pdMS_TO_TICKS(ms) ת������
	void vTaskDelay(TickType_t xTicksToWait);
	
	���ܣ�ʵ�����������������ʱ��
	����1��pxPreviousWakeTime���ϴλ���ʱ�䣨���ʼ��Ϊ��ǰʱ�䣩��
	����2��xTimeIncrement������ʱ�䣨ϵͳ����������
	����ֵ��pdTRUE������Ԥ��ִ�С�pdFALSE������ִ�г�ʱ�����������ȼ����ͣ���
	ע�⣺TickType_t pxPreviousWakeTime; �趨��
	BaseType_t xTaskDelayUntil(TickType_t *pxPreviousWakeTime, TickType_t xTimeIncrement);
***/
