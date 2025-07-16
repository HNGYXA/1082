#include "other.h"
#include "stdint.h"
#include "tim.h"
#include "Uart.h"
#include "Aimming.h"
#include "stdio.h"
extern uint8_t RxBuffer[Uart_Rxbuffer];
extern Aimming_Group Aimming1;
extern SystemState_t currentState;
void OtherTask(void *argument){
		int32_t a1 = 0,a2 = 0,a3 = 0,a4 = 0;
		uint8_t flag666 = RxBuffer[0],channel1 = RxBuffer[1],channel2 = RxBuffer[2],channel3 = RxBuffer[3],channel4 = RxBuffer[4];
		uint16_t count = RxBuffer[5];
		while(1){
			if(1){
				if(channel1)
					{__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, a1);a1+=count;}
				if(channel2)
					{__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, a2);a2+=count;}
				if(channel3)
					{__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, a3);a3+=count;}
				if(channel4)
					{__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_4, a4);a4+=count;}
				if(a1 >= Aimming1.aimming_maxvalue) a1 = Aimming1.aimming_maxvalue;
				if(a2 >= Aimming1.aimming_maxvalue) a2 = Aimming1.aimming_maxvalue;
				if(a3 >= Aimming1.aimming_maxvalue) a3 = Aimming1.aimming_maxvalue;
				if(a4 >= Aimming1.aimming_maxvalue) a4 = Aimming1.aimming_maxvalue;
				printf("%d %d %d %d\r\n",a1,a2,a3,a4);
			}
			if(0){
				if(channel1)
					{__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, a1);a1-=count;}
				if(channel2)
					{__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, a2);a2-=count;}
				if(channel3)
					{__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, a3);a3-=count;}
				if(channel4)
					{__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_4, a4);a4-=count;}
				if(a1 <= 0) a1 = 0;
				if(a2 <= 0) a2 = 0;
				if(a3 <= 0) a3 = 0;
				if(a4 <= 0) a4 = 0;
				printf("%d %d %d %d\r\n",a1,a2,a3,a4);
			}
		}
}