#include "VAS1082.h"
#include "tim.h"
//串口var
extern uint8_t END_STA;
uint8_t RxBuffer[max_buff_len] = {0};
//调光曲线var
uint16_t Aimming_Speed = 10000;//调光分辨率
int8_t jump = 1;
double pow_result = 0, aimming = 0;
/*
aimming_data --> 调光写入DATA
aimming_count --> 介于aimming_start和aimming_end之间的var
aimming_start --> PWM开始的DATA
aimming_end --> PWM结束的DATA
aimming_maxvalue --> PWM最大value
aimming_time --> 设置调光时间
aimming_it --> 调光暂停标志位
aimming_stop --> 用于中断循环调光
*/
uint16_t aimming_data = 0,aimming_count = 0,aimming_start,aimming_end,aimming_maxvalue = 64000;
uint32_t aimming_time;
uint8_t aimming_it = 1,aimming_stop = 0;

float precent;
void Freq_Sel(void) //频率选择
{
		if(RxBuffer[0] != 0xFE) return; //验证错误
		
		HAL_TIM_PWM_Stop(&htim1,TIM_CHANNEL_1);
		HAL_TIM_PWM_Stop(&htim1,TIM_CHANNEL_2);
		HAL_TIM_PWM_Stop(&htim1,TIM_CHANNEL_3);
		HAL_TIM_PWM_Stop(&htim1,TIM_CHANNEL_4);
		switch(RxBuffer[1]){
			case 0x01: MX_TIM1_Init_Again(3,64000);aimming_maxvalue = 64000;printf("频率配置成功:当前频率1KHZ\r\n");break;
			case 0x02: MX_TIM1_Init_Again(1,64000);aimming_maxvalue = 64000;printf("频率配置成功:当前频率2KHZ\r\n");break;
			case 0x04: MX_TIM1_Init_Again(0,64000);aimming_maxvalue = 64000;printf("频率配置成功:当前频率4KHZ\r\n");break;
			case 0x16: MX_TIM1_Init_Again(0,16000);aimming_maxvalue = 16000;printf("频率配置成功:当前频率16KHZ\r\n");break;
			case 0x20: MX_TIM1_Init_Again(0,12800);aimming_maxvalue = 12800;printf("频率配置成功:当前频率20KHZ\r\n");break;
			case 0x25: MX_TIM1_Init_Again(0,10240);aimming_maxvalue = 10240;printf("频率配置成功:当前频率25KHZ\r\n");break;
			default: break;
		}
		HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_1);
		HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_2);
		HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_3);
		HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_4);
		
		HAL_Delay(100);
		RxBuffer[0] = 0;RxBuffer[1] = 0;
}

void Aimming(void)	//调光验证
{
		if(RxBuffer[0] != 0xFF) return; //验证错误
		float precent = 0;	//显示百分比
		uint32_t count_precent =  0; //显示百分比
		
		float aimming_start_temp = RxBuffer[2],aimming_end_temp = RxBuffer[4]; //开始，结束中间变量
		uint8_t channel1_en = RxBuffer[6],channel2_en = RxBuffer[7],channel3_en = RxBuffer[8],channel4_en = RxBuffer[9];//通道使能
		uint8_t aimming_curve = RxBuffer[10];//选择曲线
		uint8_t aimming_circular = RxBuffer[11];//选择循环
		uint32_t maxminstoptime = RxBuffer[12] * 1000;//maxmin停留时间
		printf("开始调光\r\n");HAL_Delay(100);
		if(aimming_curve == 0x01){	//选择曲线1
			aimming_start_temp = (RxBuffer[1] + aimming_start_temp/100)/100;
			aimming_end_temp = (RxBuffer[3] + aimming_end_temp/100)/100;
			aimming_start_temp = Aimming_Speed * pow(aimming_start_temp, 1.0 / 2.2);
			aimming_end_temp = Aimming_Speed * pow(aimming_end_temp, 1.0 / 2.2);
			aimming_start = aimming_start_temp;
			aimming_end = aimming_end_temp;
			aimming = aimming_count = aimming_start;	
			aimming_time = RxBuffer[5]; 			
			aimming_time = aimming_time * 1000000 / abs((aimming_start - aimming_end));		
			while(aimming_stop != 0xBB){	//触发停止按钮
				aimming_stop = RxBuffer[0];
				aimming_it = RxBuffer[0];
				count_precent += aimming_time;
				if(count_precent >= 50000){
					count_precent = 0;
					precent = aimming_data;
					precent = precent/aimming_maxvalue*100;
					if(precent<10) printf("Q0%.2f\r\n",precent);
					else printf("Q%.2f\r\n",precent);	
				}
				while(!aimming_it){aimming_it = RxBuffer[0];HAL_Delay(100);}//触发暂停按钮
				if(aimming_time) delay_us(aimming_time); // 调整速度
				aimming_count += jump;
				aimming = aimming_count;	
				pow_result = pow(aimming / Aimming_Speed, 2.2);	
				aimming_data = aimming_maxvalue * pow_result;				
				if(aimming_start < aimming_end){
					if(aimming_count >= aimming_end){
						HAL_Delay(50);
						aimming = aimming_count = aimming_end;
						jump = -jump;
						count_precent = 0;
						precent = aimming_data;
						precent = precent/aimming_maxvalue*100;
						if(precent<10)
							printf("Q0%.2f\r\n",precent);
						else printf("Q%.2f\r\n",precent);		
						if(maxminstoptime) HAL_Delay(maxminstoptime-50);
						if(!aimming_circular){
							var_init();
							printf("调光完成\r\n");
							return;
						}
					}
					else if(aimming_count <= aimming_start){
						HAL_Delay(50);
						aimming = aimming_count = aimming_start;
						jump = -jump;
						count_precent = 0;
						precent = aimming_data;
						precent = precent/aimming_maxvalue*100;
						if(precent<10)
							printf("Q0%.2f\r\n",precent);
						else printf("Q%.2f\r\n",precent);	
						if(maxminstoptime) HAL_Delay(maxminstoptime-50);			
					}
					else{;}
				}
				else{
					if(aimming_count <= aimming_end){
						HAL_Delay(50);
						aimming = aimming_count = aimming_end;
						jump = -jump;
						count_precent = 0;
						precent = aimming_data;;
						precent = precent/aimming_maxvalue*100;
						if(precent<10)
							printf("Q0%.2f\r\n",precent);
						else printf("Q%.2f\r\n",precent);		
						if(maxminstoptime) HAL_Delay(maxminstoptime-50);
						if(!aimming_circular){
							var_init();
							printf("调光完成\r\n");
							return;
						}
					}
					else if(aimming_count >= aimming_start){
						HAL_Delay(50);
						aimming = aimming_count = aimming_start;
						jump = -jump;
						count_precent = 0;
						precent = aimming_data;;
						precent = precent/aimming_maxvalue*100;
						if(precent<10)
							printf("Q0%.2f\r\n",precent);
						else printf("Q%.2f\r\n",precent);	
						if(maxminstoptime) HAL_Delay(maxminstoptime-50);		
					}
				}

				if(channel1_en)
					__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, aimming_data);
				if(channel2_en)
					__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, aimming_data);
				if(channel3_en)
					__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, aimming_data);
				if(channel4_en)
					__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_4, aimming_data);
			}				
		}
		else if(aimming_curve == 0x02){
			aimming_start_temp = (RxBuffer[1] + aimming_start_temp/100)/100*aimming_maxvalue;
			aimming_end_temp = (RxBuffer[3] + aimming_end_temp/100)/100*aimming_maxvalue;
			aimming_start = aimming_start_temp;
			aimming_end = aimming_end_temp;
			aimming_count = aimming_start;	
			aimming_time = RxBuffer[5]; 			
			aimming_time = aimming_time * 1000000 / abs((aimming_start - aimming_end));		
			while(aimming_stop != 0xBB){	
				aimming_stop = RxBuffer[0];
				aimming_it = RxBuffer[0];
				
				count_precent += aimming_time;
				if(count_precent >= 50000){
					count_precent = 0;
					precent = aimming_count;
					precent = precent/aimming_maxvalue*100;
					if(precent<10) printf("Q0%.2f\r\n",precent);
					else printf("Q%.2f\r\n",precent);	
				}

				while(!aimming_it){aimming_it = RxBuffer[0];HAL_Delay(100);}
				if(aimming_time) delay_us(aimming_time); 
				aimming_count += jump;
				aimming_data = aimming_count;	
				
				if(aimming_start < aimming_end){
					if(aimming_count >= aimming_end){
						HAL_Delay(50);
						aimming_data = aimming_count = aimming_end;
						jump = -jump;
						count_precent = 0;
						precent = aimming_end;
						precent = precent/aimming_maxvalue*100;
						if(precent<10)
							printf("Q0%.2f\r\n",precent);
						else printf("Q%.2f\r\n",precent);		
						if(maxminstoptime) HAL_Delay(maxminstoptime-50);
						if(!aimming_circular){
							var_init();
							printf("调光完成\r\n");
							return;
						}
					}
					if(aimming_count <= aimming_start){				
						HAL_Delay(50);
						aimming_data = aimming_count = aimming_start;
						jump = -jump;
						count_precent = 0;
						precent = aimming_count;
						precent = precent/aimming_maxvalue*100;
						if(precent<10)
							printf("Q0%.2f\r\n",precent);
						else printf("Q%.2f\r\n",precent);	
						if(maxminstoptime) HAL_Delay(maxminstoptime-50);
					}
				}
				else{
					if(aimming_count <= aimming_end){	
						HAL_Delay(50);					
						aimming_data = aimming_count = aimming_end;
						jump = -jump;
						count_precent = 0;
						precent = aimming_end;
						precent = precent/aimming_maxvalue*100;
						if(precent<10)
							printf("Q0%.2f\r\n",precent);
						else printf("Q100.0%.2f\r\n",precent);	
						if(maxminstoptime) HAL_Delay(maxminstoptime-50);
						if(!aimming_circular){
							var_init();
							printf("调光完成\r\n");
							return;
						}
					}
					if(aimming_count >= aimming_start){
						HAL_Delay(50);
						aimming_data = aimming_count = aimming_start;
						jump = -jump;
						count_precent = 0;
						precent = aimming_start;
						precent = precent/aimming_maxvalue*100;
						if(precent<10)
							printf("Q0%.2f\r\n",precent);
						else printf("Q%.2f\r\n",precent);	
						if(maxminstoptime) HAL_Delay(maxminstoptime-50);
					}
				}

				if(channel1_en)
					__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, aimming_data);
				if(channel2_en)
					__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, aimming_data);
				if(channel3_en)
					__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, aimming_data);
				if(channel4_en)
					__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_4, aimming_data);
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
		printf("Q00.00\r\n");
		HAL_Delay(100);
		printf("调光完成\r\n");
}	

void var_init(void){
		for(uint8_t i = 0;i <= 11;i++)
		{
			RxBuffer[i] = 0;
		}
		aimming_count = 0;
		aimming = 0;
		aimming_stop = 0;
		aimming_start = 0;
		aimming_time = 0;
		aimming_end = 0;
		aimming_it = 1;
		aimming_data = 0;
		jump = 1;
		HAL_Delay(100);
}
