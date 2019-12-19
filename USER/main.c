#include "delay.h"
#include "sys.h"
#include "device.h"

int main(void)
{	 
	delay_init();	    	 //延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级

	DeviceInit();       //设备初始化

	while(1)
	{								    
		GetData();				//得到数据
	}
}

