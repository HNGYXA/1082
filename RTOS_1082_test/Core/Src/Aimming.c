#include "Aimming.h"
#include "VAS1082.h"
#include "Uart.h"
Aimming_Group Aimming1 = {1,1,0,0,10000,0,0,0,0,64000,0,0,0};
extern uint8_t RxBuffer[Uart_Rxbuffer];
extern SystemState_t currentState;
void AimmingTask(void *argument)	//调光验证
{
	while(1){
			osSemaphoreAcquire(Aimming_SemHandle,portMAX_DELAY);
			osSemaphoreAcquire(UartMutexesHandle,portMAX_DELAY);
			//xSemaphoreTake(Aimming_SemHandle,portMAX_DELAY);
			//xSemaphoreTake(UartMutexesHandle,portMAX_DELAY);
			float precent = 0;	//显示百分比
			uint32_t count_precent =  0; //显示百分比
			float aimming_start_temp = RxBuffer[2],aimming_end_temp = RxBuffer[4]; //开始，结束中间变量
			uint8_t channel1_en = RxBuffer[6],channel2_en = RxBuffer[7],channel3_en = RxBuffer[8],channel4_en = RxBuffer[9];//通道使能
			uint8_t aimming_curve = RxBuffer[10];//选择曲线
			uint8_t aimming_circular = RxBuffer[11];//选择循环
			uint32_t maxminstoptime = RxBuffer[12] * 1000;//maxmin停留时间
			if(aimming_curve == 0x01){	//选择曲线1
				aimming_start_temp = (RxBuffer[1] + aimming_start_temp/100)/100;
				aimming_end_temp = (RxBuffer[3] + aimming_end_temp/100)/100;
				aimming_start_temp = Aimming1.Aimming_Speed * pow(aimming_start_temp, 1.0 / 2.2);
				aimming_end_temp = Aimming1.Aimming_Speed * pow(aimming_end_temp, 1.0 / 2.2);
				Aimming1.aimming_start = aimming_start_temp;
				Aimming1.aimming_end = aimming_end_temp;
				Aimming1.aimming = Aimming1.aimming_count = Aimming1.aimming_start;	
				Aimming1.aimming_time = RxBuffer[5];
				uint32_t diff = Aimming1.aimming_start > Aimming1.aimming_end ? Aimming1.aimming_start - Aimming1.aimming_end:Aimming1.aimming_end - Aimming1.aimming_start ;
				Aimming1.aimming_time = Aimming1.aimming_time * 1000000 / diff;	
				osMutexRelease(UartMutexesHandle);
				//xSemaphoreGive(UartMutexesHandle);
				printf("开始调光\r\n");osDelay(100); //delay是因为网页版串口短时间内用太多printf，显示容易错误
				while(Aimming1.aimming_stop != 0xBB){
					while(Aimming1.aimming_it == 0xAA){
						if(Aimming1.aimming_stop == 0xBB) {
							osMutexAcquire(UartMutexesHandle,portMAX_DELAY);
							//xSemaphoreTake(UartMutexesHandle,portMAX_DELAY);
							Aimming1.aimming_it = 0;
							osMutexRelease(UartMutexesHandle);
							//xSemaphoreGive(UartMutexesHandle);
						}
						osDelay(100);
					}//触发暂停按钮
					count_precent += Aimming1.aimming_time;
					if(count_precent >= 100000){
						count_precent = 0;
						precent = Aimming1.aimming_data;
						precent = precent/Aimming1.aimming_maxvalue*100;
						if(precent<10) printf("Q0%.2f",precent);
						else printf("Q%.2f",precent);	
					}
					if(Aimming1.aimming_time) 
						delay_us(Aimming1.aimming_time); // 调整速度
					Aimming1.aimming_count += Aimming1.jump;
					Aimming1.aimming = Aimming1.aimming_count;	
					Aimming1.pow_result = pow(Aimming1.aimming / Aimming1.Aimming_Speed, 2.2);	
					Aimming1.aimming_data = Aimming1.aimming_maxvalue * Aimming1.pow_result;	
					if(Aimming1.aimming_start < Aimming1.aimming_end){
						if(Aimming1.aimming_count >= Aimming1.aimming_end){
							osDelay(50);//串口控制台显示问题
							Aimming1.aimming = Aimming1.aimming_count = Aimming1.aimming_end;
							Aimming1.jump = -Aimming1.jump;
							count_precent = 0;
							precent = Aimming1.aimming_data;
							precent = precent/Aimming1.aimming_maxvalue*100;
							if(precent<10)
								printf("Q0%.2f",precent);
							else printf("Q%.2f",precent);		
							if(maxminstoptime) osDelay(maxminstoptime - 50);
							if(!aimming_circular){
								var_init();
								return;
							}
						}
						else if(Aimming1.aimming_count <= Aimming1.aimming_start){
							osDelay(50);
							Aimming1.aimming = Aimming1.aimming_count = Aimming1.aimming_start;
							Aimming1.jump = -Aimming1.jump;
							count_precent = 0;
							precent = Aimming1.aimming_data;
							precent = precent/Aimming1.aimming_maxvalue*100;
							if(precent<10)
								printf("Q0%.2f",precent);
							else printf("Q%.2f",precent);	
							if(maxminstoptime) osDelay(maxminstoptime - 50);			
						}
						else{;}
					}
					else{
						if(Aimming1.aimming_count <= Aimming1.aimming_end){
							osDelay(50);
							Aimming1.aimming = Aimming1.aimming_count = Aimming1.aimming_end;
							Aimming1.jump = -Aimming1.jump;
							count_precent = 0;
							precent = Aimming1.aimming_data;;
							precent = precent/Aimming1.aimming_maxvalue*100;
							if(precent<10)
								printf("Q0%.2f",precent);
							else printf("Q%.2f",precent);		
							if(maxminstoptime) osDelay(maxminstoptime - 50);
							if(!aimming_circular){
								var_init();
								return;
							}
						}
						else if(Aimming1.aimming_count >= Aimming1.aimming_start){
							osDelay(50);
							Aimming1.aimming = Aimming1.aimming_count = Aimming1.aimming_start;
							Aimming1.jump = -Aimming1.jump;
							count_precent = 0;
							precent = Aimming1.aimming_data;;
							precent = precent/Aimming1.aimming_maxvalue*100;
							if(precent<10)
								printf("Q0%.2f",precent);
							else printf("Q%.2f",precent);	
							if(maxminstoptime) osDelay(maxminstoptime - 50);		
						}
				 }		
				if(channel1_en)
					__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, Aimming1.aimming_data);
				if(channel2_en)
					__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, Aimming1.aimming_data);
				if(channel3_en)
					__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, Aimming1.aimming_data);
				if(channel4_en)
					__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_4, Aimming1.aimming_data);			
				}				
			}
			else if(aimming_curve == 0x02){
			aimming_start_temp = (RxBuffer[1] + aimming_start_temp/100)/100*Aimming1.aimming_maxvalue;
			aimming_end_temp = (RxBuffer[3] + aimming_end_temp/100)/100*Aimming1.aimming_maxvalue;
			Aimming1.aimming_start = aimming_start_temp;
			Aimming1.aimming_end = aimming_end_temp;
			Aimming1.aimming_count = Aimming1.aimming_start;	
			Aimming1.aimming_time = RxBuffer[5]; 			
			uint32_t diff = Aimming1.aimming_start > Aimming1.aimming_end ? Aimming1.aimming_start - Aimming1.aimming_end:Aimming1.aimming_end - Aimming1.aimming_start ;
			Aimming1.aimming_time = Aimming1.aimming_time * 1000000 / diff;	
			osMutexRelease(UartMutexesHandle);
			//xSemaphoreGive(UartMutexesHandle);
			printf("开始调光\r\n");osDelay(100); //delay是因为网页版串口短时间内用太多printf，显示容易错误
			while(Aimming1.aimming_stop != 0xBB){	
					while(Aimming1.aimming_it == 0xAA){
						if(Aimming1.aimming_stop == 0xBB) {
							osMutexAcquire(UartMutexesHandle,portMAX_DELAY);
							//xSemaphoreTake(UartMutexesHandle,portMAX_DELAY);
							Aimming1.aimming_it = 0;
							osMutexRelease(UartMutexesHandle);
							//xSemaphoreGive(UartMutexesHandle);
						}
						osDelay(100);
					}
				count_precent += Aimming1.aimming_time;
				if(count_precent >= 100000){
					count_precent = 0;
					precent = Aimming1.aimming_count;
					precent = precent/Aimming1.aimming_maxvalue*100;
					if(precent<10) printf("Q0%.2f",precent);
					else printf("Q%.2f",precent);	
				}

				if(Aimming1.aimming_time) 
					delay_us(Aimming1.aimming_time); 
					
				Aimming1.aimming_count += Aimming1.jump;
				Aimming1.aimming_data = Aimming1.aimming_count;	
				
				if(Aimming1.aimming_start < Aimming1.aimming_end){
					if(Aimming1.aimming_count >= Aimming1.aimming_end){
						osDelay(50);
						Aimming1.aimming_data = Aimming1.aimming_count = Aimming1.aimming_end;
						Aimming1.jump = -Aimming1.jump;
						count_precent = 0;
						precent = Aimming1.aimming_end;
						precent = precent/Aimming1.aimming_maxvalue*100;
						if(precent<10)
							printf("Q0%.2f",precent);
						else printf("Q%.2f",precent);		
						if(maxminstoptime) osDelay(maxminstoptime - 50);
						if(!aimming_circular){
							var_init();
							return;
						}
					}
					if(Aimming1.aimming_count <= Aimming1.aimming_start){				
						osDelay(50);
						Aimming1.aimming_data = Aimming1.aimming_count = Aimming1.aimming_start;
						Aimming1.jump = -Aimming1.jump;
						count_precent = 0;
						precent = Aimming1.aimming_count;
						precent = precent/Aimming1.aimming_maxvalue*100;
						if(precent<10)
							printf("Q0%.2f",precent);
						else printf("Q%.2f",precent);	
						if(maxminstoptime) osDelay(maxminstoptime - 50);
					}
				}
				else{
					if(Aimming1.aimming_count <= Aimming1.aimming_end){	
						osDelay(50);					
						Aimming1.aimming_data = Aimming1.aimming_count = Aimming1.aimming_end;
						Aimming1.jump = -Aimming1.jump;
						count_precent = 0;
						precent = Aimming1.aimming_end;
						precent = precent/Aimming1.aimming_maxvalue*100;
						if(precent<10)
							printf("Q0%.2f",precent);
						else printf("Q%.2f",precent);	
						if(maxminstoptime) osDelay(maxminstoptime-50);
						if(!aimming_circular){
							var_init();
							return;
						}
					}
					if(Aimming1.aimming_count >= Aimming1.aimming_start){
						osDelay(50);
						Aimming1.aimming_data = Aimming1.aimming_count = Aimming1.aimming_start;
						Aimming1.jump = -Aimming1.jump;
						count_precent = 0;
						precent = Aimming1.aimming_start;
						precent = precent/Aimming1.aimming_maxvalue*100;
						if(precent<10)
							printf("Q0%.2f",precent);
						else printf("Q%.2f",precent);	
						if(maxminstoptime) osDelay(maxminstoptime - 50);
					}
				}

				if(channel1_en)
					__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, Aimming1.aimming_data);
				if(channel2_en)
					__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, Aimming1.aimming_data);
				if(channel3_en)
					__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, Aimming1.aimming_data);
				if(channel4_en)
					__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_4, Aimming1.aimming_data);
			 }	
			}				
			/*else if(aimming_curve == 0x03){

			}*/
			else{
				printf("CURVE_ERROR\r\n");
			}
			__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 0);
			__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, 0);
			__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, 0);
			__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_4, 0);
			var_init();
			printf("Q00.00");
			osDelay(10);
	}
	
}

void var_init(void){ 
		osMutexAcquire(UartMutexesHandle,portMAX_DELAY);
		//xSemaphoreTake(UartMutexesHandle,portMAX_DELAY);
		for(uint8_t i = 0;i <= 12;i++)
		{
			RxBuffer[i] = 0;
		}
		Aimming1.aimming_count = 0;
		Aimming1.aimming = 0;
		Aimming1.aimming_stop = 0;
		Aimming1.aimming_start = 0;
		Aimming1.aimming_time = 0;
		Aimming1.aimming_end = 0;
		Aimming1.aimming_it = 0;
		Aimming1.aimming_data = 0;
		Aimming1.jump = 1;
		currentState = SYS_IDLE;
		osMutexRelease(UartMutexesHandle);
		//xSemaphoreGive(UartMutexesHandle);
		osDelay(100);
		printf("调光完成\r\n");	
		osDelay(100);	
}
