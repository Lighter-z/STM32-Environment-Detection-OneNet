//网络设备
#include "esp8266.h"
//自定义文件
#include "gettime.h"
//#include "mymalloc.h"
#include "usart.h"
#include "usartOneNet.h"
#include "delay.h"
#include "rtc.h"
//C库
#include <string.h>
#include <stdlib.h>

nt_calendar_obj nwt_time;
nt_calendar_obj last_time;

u8 set_time = 0;

//获取网络时间并且解析
//获取北京时间
u8 GetBeijingTime(void)
{
	u8 len;
	u8 *temp2 = NULL;

	len = strlen((const char *)("AT+CIPSEND\r\n"));
	Usart_SendString(USART2,"AT+CIPSEND\r\n",len);
	len = strlen((const char *)("GET /time15.asp HTTP/1.1\r\nHost:www.beijing-time.org\r\n"));
	Usart_SendString(USART2,"GET /time15.asp HTTP/1.1\r\nHost:www.beijing-time.org\r\n",len);
	temp2 = ESP8266_GetTIME(50);
	
	if(temp2 != NULL)
	{
		nwt_time.hour = atoi((const char *)(temp2+12))+8;
		nwt_time.min = atoi((const char *)(temp2+15));
		nwt_time.sec = atoi((const char *)(temp2+18));
		nwt_time.year = atoi((const char *)(temp2+7));
		nwt_time.date = atoi((const char *)(temp2));
		//获取网络时间
		if((u8*)strstr((char*)temp2,(char*) " Jan")) nwt_time.month=1; 
		else if   ((u8*)strstr((char*)temp2,(char*) "Feb")) nwt_time.month=2; 
		else if   ((u8*)strstr((char*)temp2,(char*) "Mar")) nwt_time.month=3; 
		else if   ((u8*)strstr((char*)temp2,(char*) "Apr")) nwt_time.month=4; 
		else if   ((u8*)strstr((char*)temp2,(char*) "May")) nwt_time.month=5; 
		else if   ((u8*)strstr((char*)temp2,(char*) "Jun")) nwt_time.month=6; 
		else if   ((u8*)strstr((char*)temp2,(char*) "Jul")) nwt_time.month=7; 
		else if   ((u8*)strstr((char*)temp2,(char*) "Aug")) nwt_time.month=8; 
		else if   ((u8*)strstr((char*)temp2,(char*) "Sep")) nwt_time.month=9; 
		else if   ((u8*)strstr((char*)temp2,(char*) "Oct")) nwt_time.month=10; 
		else if   ((u8*)strstr((char*)temp2,(char*) "Nov")) nwt_time.month=11; 
		else if   ((u8*)strstr((char*)temp2,(char*) "Dec")) nwt_time.month=12;
		//比较两次时间一致则说明写入时间无误
		if(set_time == 0)
		{
			if(last_time.year != nwt_time.year )
			{
				last_time.year = nwt_time.year;
				last_time.month = nwt_time.month;
				last_time.date = nwt_time.date;
				last_time.hour = nwt_time.hour;
				last_time.min = nwt_time.min;
				last_time.sec = nwt_time.sec;
				set_time++;
			}
		}
		else if(set_time == 1)
		{	
			if(last_time.year != nwt_time.year )
			{
				set_time = 0;
			}
			else if(last_time.year == nwt_time.year )
			{
				set_time++;
				printf("get time over\r\n");
				//把NTP时间写入RTC
				RTC_Set(nwt_time.year,nwt_time.month,nwt_time.date,nwt_time.hour,nwt_time.min,nwt_time.sec);	
			}				
		}
		printf("nwt_time.year = %d\r\n",nwt_time.year);
		printf("nwt_time.month = %d\r\n",nwt_time.month);
		printf("nwt_time.date = %d\r\n",nwt_time.date);  //获取data 28日
		printf("nwt_time.hour = %d\r\n",nwt_time.hour);
		printf("nwt_time.min = %d\r\n",nwt_time.min);
		printf("nwt_time.sec = %d\r\n",nwt_time.sec);
			
	}
	else
	{
		printf("GET time failed\r\n");
	}
		
	UsartPrintf(USART_DEBUG,"+++\r\n");//退出透传
	delay_ms(2000);

	if(set_time == 2)
	{
			
		UsartPrintf(USART_DEBUG, "AT+CIPCLOSE\r\n");
		ESP8266_SendCmd("AT+CIPCLOSE\r\n", "OK");
	}
	UsartPrintf(USART_DEBUG, "AT\r\n");         //关闭连接	
	printf("CLOSE udp\r\n");
	return 0;
}









