#include "VAS1082.h"
#include "tim.h"
#include "cmsis_os2.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "queue.h"
extern osMessageQueueId_t UartQueueHandle;
extern osSemaphoreId_t Uart_SemHandle;
extern osSemaphoreId_t Freq_SemHandle;
extern osSemaphoreId_t Aimming_SemHandle;
extern osMutexId_t UartMutexesHandle;
static uint8_t RxBuffer[128] = {0};
/*
jump --> 调光曲线var
aimming_it --> 调光暂停标志位
aimming_stop --> 用于中断循环调光
aimming_start --> PWM开始的DATA
aimming_end --> PWM结束的DATA
aimming_data --> 调光写入DATA
aimming_count --> 介于aimming_start和aimming_end之间的var
Aimming_Speed --> 调光曲线var
aimming_maxvalue --> PWM最大value
aimming_time --> 设置调光时间
pow_result --> 调光曲线1var
aimming --> 调光曲线1var
*/
typedef struct {
	uint8_t Freq;
	int8_t jump;
	uint8_t aimming_it;
	uint8_t aimming_stop;
	uint16_t aimming_start;
	uint16_t aimming_end;
	uint16_t aimming_data;
	uint16_t aimming_count;
	uint16_t Aimming_Speed;
	uint16_t aimming_maxvalue;
	uint32_t aimming_time;
	double pow_result;
	double aimming;
	
}Aimming_Group;
static Aimming_Group Aimming1 = {1,1,0,0,0,0,0,0,10000,64000,0,0,0};
/* 系统状态定义 */
typedef enum {
    SYS_IDLE,           // 空闲状态
    SYS_FREQ_CONFIG,    // 频率配置中
    SYS_AIMMING,        // 调光控制中
    SYS_ERROR           // 错误状态
} SystemState_t;
SystemState_t currentState = SYS_IDLE;
void Freq_Sel(void) //频率选择
{	
		xSemaphoreTake(Freq_SemHandle,portMAX_DELAY);
		xSemaphoreTake(UartMutexesHandle,portMAX_DELAY);
		HAL_TIM_PWM_Stop(&htim1,TIM_CHANNEL_1);
		HAL_TIM_PWM_Stop(&htim1,TIM_CHANNEL_2);
		HAL_TIM_PWM_Stop(&htim1,TIM_CHANNEL_3);
		HAL_TIM_PWM_Stop(&htim1,TIM_CHANNEL_4);

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
		xSemaphoreGive(UartMutexesHandle);
}

void Aimming_Sel(void)	//调光验证
{
		xSemaphoreTake(Aimming_SemHandle,portMAX_DELAY);
		xSemaphoreTake(UartMutexesHandle,portMAX_DELAY);
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
			Aimming1.aimming_time = Aimming1.aimming_time * 1000000 / abs((Aimming1.aimming_start - Aimming1.aimming_end));	
			xSemaphoreGive(UartMutexesHandle);
			printf("开始调光\r\n");osDelay(100); //delay是因为网页版串口短时间内用太多printf，显示容易错误
			while(Aimming1.aimming_stop != 0xBB){
				while(Aimming1.aimming_it == 0xAA){
					if(Aimming1.aimming_stop == 0xBB) {
						xSemaphoreTake(UartMutexesHandle,portMAX_DELAY);
						Aimming1.aimming_it = 0;
						xSemaphoreGive(UartMutexesHandle);
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
		Aimming1.aimming_time = Aimming1.aimming_time * 1000000 / abs((Aimming1.aimming_start - Aimming1.aimming_end));		
		xSemaphoreGive(UartMutexesHandle);
		printf("开始调光\r\n");osDelay(100); //delay是因为网页版串口短时间内用太多printf，显示容易错误
		while(Aimming1.aimming_stop != 0xBB){	
				while(Aimming1.aimming_it == 0xAA){
					if(Aimming1.aimming_stop == 0xBB) {
						xSemaphoreTake(UartMutexesHandle,portMAX_DELAY);
						Aimming1.aimming_it = 0;
						xSemaphoreGive(UartMutexesHandle);
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
					else printf("Q100.0%.2f",precent);	
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
		else if(aimming_curve == 0x03){

		}
		else{
			printf("CURVE_ERROR\r\n");
		}
		__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 0);
		__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, 0);
		__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, 0);
		__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_4, 0);
		var_init();
		printf("Q00.00");
}

void Uart_Receive(void){
		uint8_t i = 0;

		xSemaphoreTake(Uart_SemHandle,portMAX_DELAY);
		xSemaphoreTake(UartMutexesHandle,portMAX_DELAY);
		while(xQueueReceive(UartQueueHandle, &RxBuffer[i], 100)){
			i++;
		}
		switch(currentState){
			 case SYS_IDLE:
				if(RxBuffer[0] == 0xFE){
					currentState = SYS_FREQ_CONFIG;
					Aimming1.Freq = RxBuffer[1];
					xSemaphoreGive(Freq_SemHandle);
				}
				else if(RxBuffer[0] == 0xFF){
					currentState = SYS_AIMMING;
					xSemaphoreGive(Aimming_SemHandle);
				}
				else{}
				break;
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
				break;
			case SYS_ERROR: break;
			default: break;		
		}
		xSemaphoreGive(UartMutexesHandle);
		osDelay(100);
}

void var_init(void){
		xSemaphoreTake(UartMutexesHandle,portMAX_DELAY);
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
		xSemaphoreGive(UartMutexesHandle);
		osDelay(100);
		printf("调光完成\r\n");	
		osDelay(100);	
}
