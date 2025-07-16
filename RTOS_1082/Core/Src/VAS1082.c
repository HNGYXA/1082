#include "VAS1082.h"
#include "tim.h"
#include "cmsis_os2.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "queue.h"
extern osMessageQueueId_t UartQueueHandle;
extern osSemaphoreId_t Uart_SemHandle;
extern osSemaphoreId_t Freq_SemHandle;
extern osSemaphoreId_t defaultTaskHandle;
extern osSemaphoreId_t Aimming_SemHandle;
extern osMutexId_t UartMutexesHandle;
extern osThreadId_t Task_OtherHandle;
static uint8_t RxBuffer[128] = {0};
/*
jump --> ��������var
aimming_it --> ������ͣ��־λ
aimming_stop --> �����ж�ѭ������
aimming_start --> PWM��ʼ��DATA
aimming_end --> PWM������DATA
aimming_data --> ����д��DATA
aimming_count --> ����aimming_start��aimming_end֮���var
Aimming_Speed --> ��������var
aimming_maxvalue --> PWM���value
aimming_time --> ���õ���ʱ��
pow_result --> ��������1var
aimming --> ��������1var
*/
typedef struct {
	uint8_t Freq;
	int8_t jump;
	uint8_t aimming_it;
	uint8_t aimming_stop;
	uint16_t Aimming_Speed;
	uint32_t aimming_start;
	uint32_t aimming_end;
	uint32_t aimming_data;
	uint32_t aimming_count;
	uint32_t aimming_maxvalue;
	uint32_t aimming_time;
	double pow_result;
	double aimming;
}Aimming_Group;
static Aimming_Group Aimming1 = {1,1,0,0,10000,0,0,0,0,64000,0,0,0};
/* ϵͳ״̬���� */
typedef enum {
    SYS_IDLE,           // ����״̬
    SYS_FREQ_CONFIG,    // Ƶ��������
    SYS_AIMMING,        // ���������
    SYS_ERROR           // ����״̬
} SystemState_t;
SystemState_t currentState = SYS_IDLE;

void Freq_Sel(void) //Ƶ��ѡ��
{	
		osSemaphoreAcquire(Freq_SemHandle,portMAX_DELAY);
		osSemaphoreAcquire(UartMutexesHandle,portMAX_DELAY);
		//xSemaphoreTake(Freq_SemHandle,portMAX_DELAY);
		//xSemaphoreTake(UartMutexesHandle,portMAX_DELAY);
		HAL_TIM_PWM_Stop(&htim1,TIM_CHANNEL_1);
		HAL_TIM_PWM_Stop(&htim1,TIM_CHANNEL_2);
		HAL_TIM_PWM_Stop(&htim1,TIM_CHANNEL_3);
		HAL_TIM_PWM_Stop(&htim1,TIM_CHANNEL_4);
		Aimming1.Freq = RxBuffer[1];
		switch(Aimming1.Freq){
			case 0x01: MX_TIM1_Init_Again(3,64000);Aimming1.aimming_maxvalue = 64000;printf("Ƶ�����óɹ�:��ǰƵ��1KHZ\r\n");break;
			case 0x02: MX_TIM1_Init_Again(1,64000);Aimming1.aimming_maxvalue = 64000;printf("Ƶ�����óɹ�:��ǰƵ��2KHZ\r\n");break;
			case 0x04: MX_TIM1_Init_Again(0,64000);Aimming1.aimming_maxvalue = 64000;printf("Ƶ�����óɹ�:��ǰƵ��4KHZ\r\n");break;
			case 0x16: MX_TIM1_Init_Again(0,16000);Aimming1.aimming_maxvalue = 16000;printf("Ƶ�����óɹ�:��ǰƵ��16KHZ\r\n");break;
			case 0x20: MX_TIM1_Init_Again(0,12800);Aimming1.aimming_maxvalue = 12800;printf("Ƶ�����óɹ�:��ǰƵ��20KHZ\r\n");break;
			case 0x25: MX_TIM1_Init_Again(0,10240);Aimming1.aimming_maxvalue = 10240;printf("Ƶ�����óɹ�:��ǰƵ��25KHZ\r\n");break;
			default: break;
		}

		HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_1);
		HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_2);
		HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_3);
		HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_4);
		RxBuffer[0] = 0;RxBuffer[1] = 0;
		currentState = SYS_IDLE;
		osSemaphoreRelease(UartMutexesHandle);
		//xSemaphoreGive(UartMutexesHandle);
}

void Aimming_Sel(void)	//������֤
{
		osSemaphoreAcquire(Aimming_SemHandle,portMAX_DELAY);
		osSemaphoreAcquire(UartMutexesHandle,portMAX_DELAY);
		//xSemaphoreTake(Aimming_SemHandle,portMAX_DELAY);
		//xSemaphoreTake(UartMutexesHandle,portMAX_DELAY);
		float precent = 0;	//��ʾ�ٷֱ�
		uint32_t count_precent =  0; //��ʾ�ٷֱ�
		float aimming_start_temp = RxBuffer[2],aimming_end_temp = RxBuffer[4]; //��ʼ�������м����
		uint8_t channel1_en = RxBuffer[6],channel2_en = RxBuffer[7],channel3_en = RxBuffer[8],channel4_en = RxBuffer[9];//ͨ��ʹ��
		uint8_t aimming_curve = RxBuffer[10];//ѡ������
		uint8_t aimming_circular = RxBuffer[11];//ѡ��ѭ��
		uint32_t maxminstoptime = RxBuffer[12] * 1000;//maxminͣ��ʱ��
		if(aimming_curve == 0x01){	//ѡ������1
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
			printf("��ʼ����\r\n");osDelay(100); //delay����Ϊ��ҳ�洮�ڶ�ʱ������̫��printf����ʾ���״���
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
				}//������ͣ��ť
				count_precent += Aimming1.aimming_time;
				if(count_precent >= 100000){
					count_precent = 0;
					precent = Aimming1.aimming_data;
					precent = precent/Aimming1.aimming_maxvalue*100;
					if(precent<10) printf("Q0%.2f",precent);
					else printf("Q%.2f",precent);	
				}
				if(Aimming1.aimming_time) 
					delay_us(Aimming1.aimming_time); // �����ٶ�
				Aimming1.aimming_count += Aimming1.jump;
				Aimming1.aimming = Aimming1.aimming_count;	
				Aimming1.pow_result = pow(Aimming1.aimming / Aimming1.Aimming_Speed, 2.2);	
				Aimming1.aimming_data = Aimming1.aimming_maxvalue * Aimming1.pow_result;	
				if(Aimming1.aimming_start < Aimming1.aimming_end){
					if(Aimming1.aimming_count >= Aimming1.aimming_end){
						osDelay(50);//���ڿ���̨��ʾ����
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
		printf("��ʼ����\r\n");osDelay(100); //delay����Ϊ��ҳ�洮�ڶ�ʱ������̫��printf����ʾ���״���
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
}


void Other_Function(void){
	uint8_t channel1 = RxBuffer[1],channel2 = RxBuffer[2],channel3 = RxBuffer[3],channel4 = RxBuffer[4];
	if(channel1)
		{__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 0);}
	if(channel2)
		{__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, 0);}
	if(channel3)
		{__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, 0);}
	if(channel4)
		{__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_4, 0);}
		
	RxBuffer[0] = 0;RxBuffer[1] = 0;RxBuffer[2] = 0;RxBuffer[3] = 0;RxBuffer[4] = 0;RxBuffer[5] = 0;
	osThreadSuspend(Task_OtherHandle);
}

int32_t a1 = 0,a2 = 0,a3 = 0,a4 = 0;
void Uart_Receive(void){
		uint8_t i = 0;
		osSemaphoreAcquire(Uart_SemHandle,portMAX_DELAY);
		osMutexAcquire(UartMutexesHandle,portMAX_DELAY);
		while(osMessageQueueGet(UartQueueHandle,&RxBuffer[i],NULL,100) != osErrorTimeout){
			i++;
		}
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
					osThreadResume(Task_OtherHandle);
				}
				else if(RxBuffer[0] == 0x80){
					uint8_t channel1 = RxBuffer[1],channel2 = RxBuffer[2],channel3 = RxBuffer[3],channel4 = RxBuffer[4];
					uint16_t count = RxBuffer[5];
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
				else if(RxBuffer[0] == 0x81){
					uint8_t channel1 = RxBuffer[1],channel2 = RxBuffer[2],channel3 = RxBuffer[3],channel4 = RxBuffer[4];
					uint16_t count = RxBuffer[5];
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
		osMutexRelease(UartMutexesHandle);
		//xSemaphoreGive(UartMutexesHandle);
		osDelay(100);
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
		printf("�������\r\n");	
		osDelay(100);	
}
