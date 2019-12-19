#ifndef __SMOG_H__
#define __SMOG_H__
#include "sys.h"

//R2-1K.
#define SMOG_PIN46_R	1000			//烟雾传感器管脚4、6接出到地的电阻值
#define SMOG_READ_TIMES	10				//定义烟雾传感器读取次数,读这么多次,然后取平均值
//#define SMOG_ADC_CHX	ADC_Channel_10	//定义烟雾传感器所在的ADC通道编号
#define SMOG_ADC_CH1	ADC_Channel_1	//定义烟雾传感器所在的ADC通道编号

void Smog_Init(void);//初始化烟雾传感器
u16 Smog_Get_Vol(void);//读取烟雾传感器的电压值
u16 Smog_Trans_Concentration(void);//分析从烟雾传感器获取的电压值,通过公式计算出可燃气体的浓度值,该函数仅作为参考.

#endif

