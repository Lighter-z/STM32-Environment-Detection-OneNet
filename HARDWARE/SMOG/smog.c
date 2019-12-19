#include "smog.h"
#include "delay.h"
#include "usart.h"
#include "adc.h"


//初始化烟雾传感器PA1
void Smog_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//使能PORTF时钟	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;			//PA1 anolog输入
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//模拟输入引脚
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
//	Adc3_Init();
	Adc_Init();
}

//读取烟雾传感器的电压值
u16 Smog_Get_Vol(void)
{
	u32 temp_val;
	u8 t;
	float read_v;
	read_v = 0;
	temp_val = 0;
	for(t = 0; t < SMOG_READ_TIMES; t++)
	{
//		temp_val+=Get_Adc3(SMOG_ADC_CHX);	//读取ADC值
		temp_val += Get_Adc_Average(SMOG_ADC_CH1,10);	//读取ADC值
		delay_ms(5);
	}
	temp_val /= SMOG_READ_TIMES;//得到平均值
	read_v = (float)temp_val * (3.3 / 4096);//得到电压值   3.3根据模块参考电压     内部adc是12位  2的12次方
//	printf("Smog_ADC_Val:%d\r\n", temp_val);
    return (u16)temp_val;
}
//分析从烟雾传感器获取的电压值，通过公式计算出可燃气体的浓度值
//设Rs为传感器的体电阻，其中气体浓度上升，将导致Rs下降。而Rs的下降则会导致，MQ-2的4脚、6脚对地输出的电压增大。
//所以气体浓度增大，其输出的电压也会增大。因Rs在不同气体中有不同的浓度值，所以该函数仅作为参考.
u16 Smog_Trans_Concentration(void)
{
	u16 ulVal = 0;
	u16 temp_val = Smog_Get_Vol();
	u16 Rs;
	Rs = SMOG_PIN46_R*(4096.0/temp_val - 1);
//	printf("Smog_Rs_Val:%d\r\n", Rs);
	
	ulVal = Rs;//这里填写公式根据Rs计算气体浓度
	
	return ulVal;
}

