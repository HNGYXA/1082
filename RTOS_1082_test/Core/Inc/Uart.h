#ifndef  __Uart_H__
#define  __Uart_H__

#define Uart_Rxbuffer 128
typedef enum {
    SYS_IDLE,           // ����״̬
    SYS_FREQ_CONFIG,    // Ƶ��������
    SYS_AIMMING,        // ���������
    SYS_ERROR           // ����״̬
} SystemState_t;

void UartTask(void *argument);

#endif
