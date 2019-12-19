#ifndef _ONENET_H_
#define _ONENET_H_

#include "sys.h" 

typedef struct
{
	u8 temperature;  	    
	u8 humidity;
	u8 smoke;
	float light;
	u8 people_num;
	float pm_25;
  
} Environment;

_Bool OneNet_DevLink(void);

void OneNet_SendData(void);

void OneNet_RevPro(unsigned char *cmd);

#endif
