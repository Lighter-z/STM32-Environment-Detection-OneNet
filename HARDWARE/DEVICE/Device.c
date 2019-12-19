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

//����Э���
#include "onenet.h"

//�����豸
#include "esp8266.h"

//NTPʱ��
#include "gettime.h"

unsigned short timeCountOneNet = 0;	//���ͼ������
	
unsigned char *dataPtr = NULL;


extern Environment MyEnvironment;

static void ConnectionNtpOneNet(void)
{
	//NTP ��ȡ����ʱ��
	Esp8266NtpTimeInit();
	GetBeijingTime();
	while(set_time < 2)				//GetBeijingTime 2��ʱ��ԭ��  ��һ�ο��ܸ������� ��Ϣ��ȡ����ȫ ����û�л�ȡ����Ϣ Ҳ���ܻ�ȡ����ȫ��Ϣ   get���εĻ� ����һ�β�׼�����
	{
		GetBeijingTime();
		printf("Geting Time \r\n");
	}
	//OneNet����
	if(set_time == 2)
	{
		ESP8266_Init();
		while(OneNet_DevLink())			//����OneNET
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
	Usart2_Init(115200);							//����2������ESP8266��
	
	usmart_dev.init(SystemCoreClock/1000000);	//��ʼ��USMART	
	RTC_Init();	  			//RTC��ʼ��      RTCҪ��get ntp time֮ǰ  ӦΪ��GetBeijingTime�е�����rtc�ж�
	ConnectionNtpOneNet();
	
	DHT11_Init();       //DHT11��ʼ��	
	Smog_Init();				//��������

	//oled
	I2C_Configuration();
	OLED_Init();

	OLED_Fill(0x00);//ȫ����
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
		OLED_ShowStr(0,3,hour,2);				//��ʾСʱ
		
		sprintf(min, " %d" , calendar.min);
		OLED_ShowStr(42,3,min,2);				//��ʾ����
			
		sprintf(sec, " %d" , calendar.sec);
		OLED_ShowStr(85,3,sec,2);				//��ʾ��
	
	}	
	delay_ms(10);	
}

static void Dht11Data(void)
{
	u8 dht11_cp=0;	
	if(dht11_cp%10==0)			//ÿ100ms��ȡһ��
	{									  
		DHT11_Read_Data(&MyEnvironment.temperature,&MyEnvironment.humidity);	//��ȡ��ʪ��ֵ					     	   
	}				   
	delay_ms(10);
	dht11_cp++;
	if(dht11_cp==20)
	{
		dht11_cp=0;
	}
}
/*
250��������

*/
static u32 SmokeProportion(void)
{
	u32 temp;
	float v = 0;
	temp = Smog_Get_Vol();
	//�õ��ٷֱ�
	v = (float)temp * (3.3 / 4096);
	v = v * 30;
	if(v >= 100) v = 100;      
	
	return (u8)v;
}

void OneNetSend(void)
{
	if(++timeCountOneNet >= 20)									//���ͼ��  
		{
			OneNet_SendData();									//��������
		
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
