#ifndef __PM25_H
#define __PM25_H	 
#include "sys.h"

//PB1做脉冲引脚
//PA6做模拟引脚

#define GP2Y_High GPIO_SetBits(GPIOB,GPIO_Pin_1);
#define GP2Y_Low  GPIO_ResetBits(GPIOB,GPIO_Pin_1);

void PM25Init(void);
float GetGP2Y(void);
u16 Get_GP2Y_Average(u8 times);

		 				    
#endif

