#ifndef  __Uart_H__
#define  __Uart_H__

#define Uart_Rxbuffer 128
typedef enum {
    SYS_IDLE,           // 空闲状态
    SYS_FREQ_CONFIG,    // 频率配置中
    SYS_AIMMING,        // 调光控制中
    SYS_ERROR           // 错误状态
} SystemState_t;

void UartTask(void *argument);

#endif
