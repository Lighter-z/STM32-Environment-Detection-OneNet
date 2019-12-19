#include "smog.h"
#include "delay.h"
#include "usart.h"
#include "adc.h"


//��ʼ����������PA1
void Smog_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//ʹ��PORTFʱ��	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;			//PA1 anolog����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//ģ����������
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
//	Adc3_Init();
	Adc_Init();
}

//��ȡ���������ĵ�ѹֵ
u16 Smog_Get_Vol(void)
{
	u32 temp_val;
	u8 t;
	float read_v;
	read_v = 0;
	temp_val = 0;
	for(t = 0; t < SMOG_READ_TIMES; t++)
	{
//		temp_val+=Get_Adc3(SMOG_ADC_CHX);	//��ȡADCֵ
		temp_val += Get_Adc_Average(SMOG_ADC_CH1,10);	//��ȡADCֵ
		delay_ms(5);
	}
	temp_val /= SMOG_READ_TIMES;//�õ�ƽ��ֵ
	read_v = (float)temp_val * (3.3 / 4096);//�õ���ѹֵ   3.3����ģ��ο���ѹ     �ڲ�adc��12λ  2��12�η�
//	printf("Smog_ADC_Val:%d\r\n", temp_val);
    return (u16)temp_val;
}
//����������������ȡ�ĵ�ѹֵ��ͨ����ʽ�������ȼ�����Ũ��ֵ
//��RsΪ������������裬��������Ũ��������������Rs�½�����Rs���½���ᵼ�£�MQ-2��4�š�6�ŶԵ�����ĵ�ѹ����
//��������Ũ������������ĵ�ѹҲ��������Rs�ڲ�ͬ�������в�ͬ��Ũ��ֵ�����Ըú�������Ϊ�ο�.
u16 Smog_Trans_Concentration(void)
{
	u16 ulVal = 0;
	u16 temp_val = Smog_Get_Vol();
	u16 Rs;
	Rs = SMOG_PIN46_R*(4096.0/temp_val - 1);
//	printf("Smog_Rs_Val:%d\r\n", Rs);
	
	ulVal = Rs;//������д��ʽ����Rs��������Ũ��
	
	return ulVal;
}

