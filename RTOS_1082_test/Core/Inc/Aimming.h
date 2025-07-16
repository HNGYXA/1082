#ifndef  __Aimming_H__
#define  __Aimming_H__

#include "stdint.h"
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

void AimmingTask(void *argument);
void var_init(void);
#endif
