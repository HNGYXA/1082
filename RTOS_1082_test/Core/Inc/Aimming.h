#ifndef  __Aimming_H__
#define  __Aimming_H__

#include "stdint.h"
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

void AimmingTask(void *argument);
void var_init(void);
#endif
