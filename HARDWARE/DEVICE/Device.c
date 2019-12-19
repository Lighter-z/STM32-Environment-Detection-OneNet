#include "Device.h"

#include <stdlib.h>

#include "sys.h"
#include "delay.h"
#include "usart.h"	
#include "usartOneNet.h"	
#include "usmart.h"	 


#include "rtc.h" 
#include "dht11.h"
#include "smog.h"
#include "bh1750.h"
#include "OLED_I2C.h"
#include "PM2.5.h"

//网络协议层
#include "onenet.h"

//网络设备
#include "esp8266.h"

//NTP时间
#include "gettime.h"

unsigned short timeCountOneNet = 0;	//发送间隔变量
	
unsigned char *dataPtr = NULL;


extern Environment MyEnvironment;

static void ConnectionNtpOneNet(void)
{
	//NTP 获取网络时间
	Esp8266NtpTimeInit();
	GetBeijingTime();
	while(set_time < 2)				//GetBeijingTime 2次时间原因  第一次可能刚连接上 信息获取不完全 或者没有获取到信息 也可能获取到完全信息   get两次的话 避免一次不准的情况
	{
		GetBeijingTime();
		printf("Geting Time \r\n");
	}
	//OneNet连接
	if(set_time == 2)
	{
		ESP8266_Init();
		while(OneNet_DevLink())			//接入OneNET
		{	
			delay_ms(500);
		}
		printf("ESP8266 OneNet is OK\r\n");
		printf("system_init\r\n");
		set_time++;
	}
}

void DeviceInit(void)
{
	Usart1_Init(115200);
	Usart2_Init(115200);							//串口2，驱动ESP8266用
	
	usmart_dev.init(SystemCoreClock/1000000);	//初始化USMART	
	RTC_Init();	  			//RTC初始化      RTC要在get ntp time之前  应为在GetBeijingTime中调用了rtc中断
	ConnectionNtpOneNet();
	
	DHT11_Init();       //DHT11初始化	
	Smog_Init();				//烟雾传感器

	//oled
	I2C_Configuration();
	OLED_Init();

	OLED_Fill(0x00);//全屏灭
	OLED_ShowStr(30,3,":",2);	
	OLED_ShowStr(75,3,":",2);	
	bh1750_iic_init();
	PM25Init();
}

static void RtcData(void)
{
	u8 rtc_cp=0;	

	char hour[30];
	char min[30];
	char sec[30];
	
	if(rtc_cp!=calendar.sec)
	{
		rtc_cp=calendar.sec;
		
		sprintf(hour, " %d" , calendar.hour);
		OLED_ShowStr(0,3,hour,2);				//显示小时
		
		sprintf(min, " %d" , calendar.min);
		OLED_ShowStr(42,3,min,2);				//显示分钟
			
		sprintf(sec, " %d" , calendar.sec);
		OLED_ShowStr(85,3,sec,2);				//显示秒
	
	}	
	delay_ms(10);	
}

static void Dht11Data(void)
{
	u8 dht11_cp=0;	
	if(dht11_cp%10==0)			//每100ms读取一次
	{									  
		DHT11_Read_Data(&MyEnvironment.temperature,&MyEnvironment.humidity);	//读取温湿度值					     	   
	}				   
	delay_ms(10);
	dht11_cp++;
	if(dht11_cp==20)
	{
		dht11_cp=0;
	}
}
/*
250以下正常

*/
static u32 SmokeProportion(void)
{
	u32 temp;
	float v = 0;
	temp = Smog_Get_Vol();
	//得到百分比
	v = (float)temp * (3.3 / 4096);
	v = v * 30;
	if(v >= 100) v = 100;      
	
	return (u8)v;
}

void OneNetSend(void)
{
	if(++timeCountOneNet >= 20)									//发送间隔  
		{
			OneNet_SendData();									//发送数据
		
			timeCountOneNet = 0;
			ESP8266_Clear();
		}
	printf("                                   \r\n");
	printf("timeCount is %d",timeCountOneNet);
	printf("                                   \r\n");
	dataPtr = ESP8266_GetIPD(0);
	if(dataPtr != NULL)
		OneNet_RevPro(dataPtr);
		
	delay_ms(10);
}


void GetData(void)
{
	RtcData();
	Dht11Data();
	
	MyEnvironment.smoke = SmokeProportion();
	MyEnvironment.light =  bh1750_Init();
	MyEnvironment.pm_25 = GetGP2Y();
	
	OneNetSend();
}
