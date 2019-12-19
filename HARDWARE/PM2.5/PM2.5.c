#include "PM2.5.h"
#include "sys.h"
#include "delay.h"
#include "usart.h"	
#include "adc.h"


void PM25Init(void)
{ 	
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE );	
	//PB1 作为脉冲输出引脚   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;	                 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;           
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GP2Y_High;

}				  

float GetGP2Y(void)
{
	u32 AD_PM;  
	double pm; 
	GP2Y_Low;
	delay_us(280);
	AD_PM = Get_Adc(ADC_Channel_6);	//PA6
	delay_us(40);
	GP2Y_High;
	delay_us(9680);
	pm = 0.17*AD_PM-0.1; //转换公式
//    printf("%f\r\n",pm);
	return pm;
} 

u16 Get_GP2Y_Average(u8 times)
{
	u32 pm_val=0;
	u8 t;
	for(t=0;t<times;t++)
	{
		pm_val+=GetGP2Y();
		delay_ms(5);
	}
	return pm_val/times;
} 


