#include "Uart.h"
#include "Aimming.h"
#include "Freq.h"
#include "VAS1082.h"
extern Aimming_Group Aimming1;
SystemState_t currentState = SYS_IDLE;
uint8_t RxBuffer[Uart_Rxbuffer] = {0};

void UartTask(void *argument){

		uint8_t i = 0;
		while(1){
			osSemaphoreAcquire(Uart_SemHandle,portMAX_DELAY);
			osMutexAcquire(UartMutexesHandle,portMAX_DELAY);
			while(osMessageQueueGet(UartQueueHandle,&RxBuffer[i],NULL,100) != osErrorTimeout){
				i++;
			}
			i = 0;
			switch(currentState){
				 case SYS_IDLE:
					if(RxBuffer[0] == 0xFE){
						currentState = SYS_FREQ_CONFIG;
						osSemaphoreRelease(Freq_SemHandle);
						//xSemaphoreGive(Freq_SemHandle);
					}
					else if(RxBuffer[0] == 0xFF){
						currentState = SYS_AIMMING;
						osSemaphoreRelease(Aimming_SemHandle);
						//xSemaphoreGive(Aimming_SemHandle);
					}
					else if(RxBuffer[0] == 0xBB){
						
					}					
				case SYS_FREQ_CONFIG: 
					Aimming1.Freq = RxBuffer[1];
					break;
				case SYS_AIMMING: 
					if(RxBuffer[0] == 0xBB){
						Aimming1.aimming_stop = 0xBB;
					}
					else if(RxBuffer[0] == 0xAA){
						Aimming1.aimming_it = 0xAA;
					}
					else if(RxBuffer[0] == 0x00){
						Aimming1.aimming_it = 0x00;
					}
					else if(RxBuffer[0] == 0x80){
						osThreadResume(OtherTaskHandle);
					}
					else if(RxBuffer[0] == 0x81){
						osThreadResume(OtherTaskHandle);
					}
					break;
				case SYS_ERROR: break;
				default: break;		
			}
			osMutexRelease(UartMutexesHandle);
			//xSemaphoreGive(UartMutexesHandle);
			
			osDelay(100);
		}	
}
