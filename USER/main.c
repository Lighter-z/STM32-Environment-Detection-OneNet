#include "delay.h"
#include "sys.h"
#include "device.h"

int main(void)
{	 
	delay_init();	    	 //��ʱ������ʼ��	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�����ж����ȼ�����Ϊ��2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�

	DeviceInit();       //�豸��ʼ��

	while(1)
	{								    
		GetData();				//�õ�����
	}
}

