#include "Freq.h"
#include "Aimming.h"
#include "Uart.h"
#include "VAS1082.h"
extern uint8_t RxBuffer[Uart_Rxbuffer];
extern Aimming_Group Aimming1;
extern SystemState_t currentState;
void FreqTask(void *argument) //频率选择
{
		while(1){
			osSemaphoreAcquire(Freq_SemHandle,portMAX_DELAY);
			osSemaphoreAcquire(UartMutexesHandle,portMAX_DELAY);
			HAL_TIM_PWM_Stop(&htim1,TIM_CHANNEL_1);
			HAL_TIM_PWM_Stop(&htim1,TIM_CHANNEL_2);
			HAL_TIM_PWM_Stop(&htim1,TIM_CHANNEL_3);
			HAL_TIM_PWM_Stop(&htim1,TIM_CHANNEL_4);
			Aimming1.Freq = RxBuffer[1];
			switch(Aimming1.Freq){
				case 0x01: MX_TIM1_Init_Again(3,64000);Aimming1.aimming_maxvalue = 64000;printf("频率配置成功:当前频率1KHZ\r\n");break;
				case 0x02: MX_TIM1_Init_Again(1,64000);Aimming1.aimming_maxvalue = 64000;printf("频率配置成功:当前频率2KHZ\r\n");break;
				case 0x04: MX_TIM1_Init_Again(0,64000);Aimming1.aimming_maxvalue = 64000;printf("频率配置成功:当前频率4KHZ\r\n");break;
				case 0x16: MX_TIM1_Init_Again(0,16000);Aimming1.aimming_maxvalue = 16000;printf("频率配置成功:当前频率16KHZ\r\n");break;
				case 0x20: MX_TIM1_Init_Again(0,12800);Aimming1.aimming_maxvalue = 12800;printf("频率配置成功:当前频率20KHZ\r\n");break;
				case 0x25: MX_TIM1_Init_Again(0,10240);Aimming1.aimming_maxvalue = 10240;printf("频率配置成功:当前频率25KHZ\r\n");break;
				default: break;
			}

			HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_1);
			HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_2);
			HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_3);
			HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_4);
			RxBuffer[0] = 0;RxBuffer[1] = 0;
			currentState = SYS_IDLE;
			osSemaphoreRelease(UartMutexesHandle);
			osDelay(100);
		}
}
