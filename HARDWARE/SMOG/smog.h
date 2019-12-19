#ifndef __SMOG_H__
#define __SMOG_H__
#include "sys.h"

//R2-1K.
#define SMOG_PIN46_R	1000			//���������ܽ�4��6�ӳ����صĵ���ֵ
#define SMOG_READ_TIMES	10				//��������������ȡ����,����ô���,Ȼ��ȡƽ��ֵ
//#define SMOG_ADC_CHX	ADC_Channel_10	//���������������ڵ�ADCͨ�����
#define SMOG_ADC_CH1	ADC_Channel_1	//���������������ڵ�ADCͨ�����

void Smog_Init(void);//��ʼ����������
u16 Smog_Get_Vol(void);//��ȡ���������ĵ�ѹֵ
u16 Smog_Trans_Concentration(void);//����������������ȡ�ĵ�ѹֵ,ͨ����ʽ�������ȼ�����Ũ��ֵ,�ú�������Ϊ�ο�.

#endif

