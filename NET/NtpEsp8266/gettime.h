#ifndef __GETTIME_H
#define __GETTIME_H

#include "stm32f10x.h"

typedef struct   //�ṹ�塣
{
    u16  year;   
    u8   month;
    u8   date;
    u8   hour;
    u8   min;
    u8   sec;	 
}nt_calendar_obj;	   

extern nt_calendar_obj nwt_time;  //����ṹ�����
extern nt_calendar_obj last_time;
extern u8 set_time;

//GET time->internet
u8 GetBeijingTime(void);

#endif
