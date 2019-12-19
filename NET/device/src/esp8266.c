//��Ƭ��ͷ�ļ�
#include "stm32f10x.h"

//�����豸����
#include "esp8266.h"

//Ӳ������
#include "delay.h"
#include "usartOneNet.h"
//#include "gettime.h"

//C��
#include <string.h>
#include <stdio.h>





#define Espsize   512

//WIFI ���� ����
//#define ESP8266_WIFI_INFO		"AT+CWJAP=\"30-702\",\"csj666666\"\r\n"		//���浽flash��
#define ESP8266_WIFI_INFO		"AT+CWJAP=\"STM32\",\"stm32stm32\"\r\n"		//���浽flash��
#define ESP8266_ONENET_INFO		"AT+CIPSTART=\"TCP\",\"183.230.40.39\",6002\r\n"
//AT+CIPSTART="TCP","183.230.40.39",6002


//ʱ��˿ں�
#define TIME_PORTNUM			"80"
//ʱ�������IP
#define TIME_SERVERIP			"www.beijing-time.org"
#define ESP8266_TIME_INFO		"AT+CIPSTART=\"TCP\",\"api.seniverse.com\",80\r\n"

unsigned char esp8266_buf[Espsize];
unsigned short esp8266_cnt = 0, esp8266_cntPre = 0;



//==========================================================
//	�������ƣ�	ESP8266_Clear
//
//	�������ܣ�	��ջ���
//
//	��ڲ�����	��
//
//	���ز�����	��
//
//	˵����		
//==========================================================
void ESP8266_Clear(void)
{

	memset(esp8266_buf, 0, sizeof(esp8266_buf));
	esp8266_cnt = 0;

}

//==========================================================
//	�������ƣ�	ESP8266_WaitRecive
//
//	�������ܣ�	�ȴ��������
//
//	��ڲ�����	��
//
//	���ز�����	REV_OK-�������		REV_WAIT-���ճ�ʱδ���
//
//	˵����		ѭ�����ü���Ƿ�������
//==========================================================
_Bool ESP8266_WaitRecive(void)
{

	if(esp8266_cnt == 0) 							//������ռ���Ϊ0 ��˵��û�д��ڽ��������У�����ֱ����������������
		return REV_WAIT;
		
	if(esp8266_cnt == esp8266_cntPre)				//�����һ�ε�ֵ�������ͬ����˵���������
	{
		esp8266_cnt = 0;							//��0���ռ���
			
		return REV_OK;								//���ؽ�����ɱ�־
	}
		
	esp8266_cntPre = esp8266_cnt;					//��Ϊ��ͬ
	
	return REV_WAIT;								//���ؽ���δ��ɱ�־

}

//==========================================================
//	�������ƣ�	ESP8266_SendCmd
//
//	�������ܣ�	��������
//
//	��ڲ�����	cmd������
//				res����Ҫ���ķ���ָ��
//
//	���ز�����	0-�ɹ�	1-ʧ��
//
//	˵����		
//==========================================================
_Bool ESP8266_SendCmd(char *cmd, char *res)
{
	
	unsigned char timeOut = 200;

	Usart_SendString(USART2, (unsigned char *)cmd, strlen((const char *)cmd));
	
	while(timeOut--)
	{
		if(ESP8266_WaitRecive() == REV_OK)							//����յ�����
		{
			if(strstr((const char *)esp8266_buf, res) != NULL)		//����������ؼ���
			{
				ESP8266_Clear();									//��ջ���
				
				return 0;
			}
		}
		
		delay_ms(10);
	}
	
	return 1;

}

//==========================================================
//	�������ƣ�	ESP8266_SendData
//
//	�������ܣ�	��������
//
//	��ڲ�����	data������
//				len������
//
//	���ز�����	��
//
//	˵����		
//==========================================================
void ESP8266_SendData(unsigned char *data, unsigned short len)
{

	char cmdBuf[32];
	
	ESP8266_Clear();								//��ս��ջ���
	sprintf(cmdBuf, "AT+CIPSEND=%d\r\n", len);		//��������
	if(!ESP8266_SendCmd(cmdBuf, ">"))				//�յ���>��ʱ���Է�������
	{
		Usart_SendString(USART2, data, len);		//�����豸������������
	}

}

//==========================================================
//	�������ƣ�	ESP8266_GetIPD

//	�������ܣ�	��ȡƽ̨���ص�����

//	��ڲ�����	�ȴ���ʱ��(����10ms)

//	���ز�����	ƽ̨���ص�ԭʼ����

//	˵����		��ͬ�����豸���صĸ�ʽ��ͬ����Ҫȥ����
//				��ESP8266�ķ��ظ�ʽΪ	"+IPD,x:yyy"	x�������ݳ��ȣ�yyy����������
//==========================================================
unsigned char *ESP8266_GetIPD(unsigned short timeOut)
{

	char *ptrIPD = NULL;
	
	do
	{
		if(ESP8266_WaitRecive() == REV_OK)								//����������
		{
			ptrIPD = strstr((char *)esp8266_buf, "IPD,");				//������IPD��ͷ
			if(ptrIPD == NULL)											//���û�ҵ���������IPDͷ���ӳ٣�������Ҫ�ȴ�һ�ᣬ�����ᳬ���趨��ʱ��
			{
				//UsartPrintf(USART_DEBUG, "\"IPD\" not found\r\n");
			}
			else
			{
				ptrIPD = strchr(ptrIPD, ':');							//�ҵ�':'
				if(ptrIPD != NULL)
				{
					ptrIPD++;
					return (unsigned char *)(ptrIPD);
				}
				else
					return NULL;
				
			}
		}
		
		delay_ms(5);													//��ʱ�ȴ�
	} while(timeOut--);
	
	return NULL;														//��ʱ��δ�ҵ������ؿ�ָ��

}

unsigned char *ESP8266_GetTIME(unsigned short timeOut)
{
	u8 Get_timebuff[Espsize];
	u8 buff[64];
	char *ptrIPD = NULL;
	char *ptrtail = NULL;
	u8 num = 0;
	u8 count = 0;

	do
	{
		if(ESP8266_WaitRecive() == REV_OK)								//����������
		{
			if(num == 0)
			{
				num++;
				strcpy((char *)Get_timebuff,(char *)esp8266_buf);
				ESP8266_Clear();
			}
			ptrIPD = strstr((char *)Get_timebuff, "Date: ");				//������IPD��ͷ
			if(ptrIPD == NULL && ptrtail == NULL)											//���û�ҵ���������IPDͷ���ӳ٣�������Ҫ�ȴ�һ�ᣬ�����ᳬ���趨��ʱ��
			{
				//UsartPrintf(USART_DEBUG, "\"IPD\" not found\r\n");
			}
			else
			{
				ptrtail = strstr((char *)Get_timebuff, "GMT");
				while(ptrIPD != ptrtail)
				{		
					buff[count] = *ptrIPD;
					count++;
					ptrIPD++;
				}
						
				printf("buff:%s\r\n",buff);
				ptrIPD = strchr((char*)buff,':'); 
				
				if(ptrIPD!= NULL)
				{
					ptrIPD = ptrIPD + 7;
					printf("ptrIPD:%s\r\n",ptrIPD);
					return (unsigned char *)(ptrIPD);
				}
				else
					return NULL;
				
			}
		}	
		delay_ms(5);													//��ʱ�ȴ�
	} while(timeOut--);
	
	return NULL;														//��ʱ��δ�ҵ������ؿ�ָ��
}

//==========================================================
//	�������ƣ�	ESP8266_Init
//
//	�������ܣ�	��ʼ��ESP8266
//
//	��ڲ�����	��
//
//	���ز�����	��
//
//	˵����		
//==========================================================
void ESP8266_Init(void)
{
	
//	GPIO_InitTypeDef GPIO_Initure;
//	
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

//	//ESP8266��λ����
//	GPIO_Initure.GPIO_Mode = GPIO_Mode_Out_PP;
//	GPIO_Initure.GPIO_Pin = GPIO_Pin_5;					//GPIOB5-��λ
//	GPIO_Initure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(GPIOB, &GPIO_Initure);
	
//	GPIO_WriteBit(GPIOB, GPIO_Pin_5, Bit_RESET);
//	delay_ms(250);
//	GPIO_WriteBit(GPIOB, GPIO_Pin_5, Bit_SET);
//	delay_ms(500);
		GPIO_WriteBit(GPIOA, GPIO_Pin_4, Bit_RESET);
	delay_ms(250);
	GPIO_WriteBit(GPIOA, GPIO_Pin_4, Bit_SET);
	delay_ms(500);
	
	ESP8266_Clear();

	UsartPrintf(USART_DEBUG, "1. AT\r\n");
	while(ESP8266_SendCmd("AT\r\n", "OK"))
		delay_ms(500);
	
	UsartPrintf(USART_DEBUG, "2. CWMODE\r\n");
	while(ESP8266_SendCmd("AT+CWMODE=1\r\n", "OK"))
		delay_ms(500);
	
	UsartPrintf(USART_DEBUG, "3. CWJAP\r\n");
	while(ESP8266_SendCmd(ESP8266_WIFI_INFO, "GOT IP"))
		delay_ms(500);
	
	UsartPrintf(USART_DEBUG, "4. CIPSTART\r\n");
	while(ESP8266_SendCmd(ESP8266_ONENET_INFO, "CONNECT"))
		delay_ms(500);
	
	UsartPrintf(USART_DEBUG, "5. ESP8266 Init OK\r\n");

}

void Esp8266NtpTimeInit(void)
{
	GPIO_InitTypeDef GPIO_Initure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

//	//ESP8266��λ����
//	GPIO_Initure.GPIO_Mode = GPIO_Mode_Out_PP;
//	GPIO_Initure.GPIO_Pin = GPIO_Pin_5;					//GPIOB5-��λ
//	GPIO_Initure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(GPIOB, &GPIO_Initure);
//	
//	GPIO_WriteBit(GPIOB, GPIO_Pin_5, Bit_RESET);
//	delay_ms(250);
//	GPIO_WriteBit(GPIOB, GPIO_Pin_5, Bit_SET);
//	delay_ms(500);
	
		//ESP8266��λ����
	GPIO_Initure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Initure.GPIO_Pin = GPIO_Pin_4;					//GPIOA4-��λ
	GPIO_Initure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_Initure);
	
	GPIO_WriteBit(GPIOA, GPIO_Pin_4, Bit_RESET);
	delay_ms(250);
	GPIO_WriteBit(GPIOA, GPIO_Pin_4, Bit_SET);
	delay_ms(500);
	
	ESP8266_Clear();
	UsartPrintf(USART_DEBUG, "1. AT\r\n");
	while(ESP8266_SendCmd("AT\r\n", "OK"))
		delay_ms(500);
	
	UsartPrintf(USART_DEBUG, "2. CWMODE_CUR\r\n");
	while(ESP8266_SendCmd("AT+CWMODE_CUR=1\r\n", "OK"))
		delay_ms(500);
	
	UsartPrintf(USART_DEBUG, "3. AT+CWDHCP_CUR\r\n");
	while(ESP8266_SendCmd("AT+CWDHCP_CUR=1,1\r\n", "OK"))
		delay_ms(500);
	
	UsartPrintf(USART_DEBUG, "4. AT+CWJAP_DEF\r\n");
	while(ESP8266_SendCmd(ESP8266_WIFI_INFO, "GOT IP"))
		delay_ms(500);
	

		UsartPrintf(USART_DEBUG, "5. CIPSTART Time\r\n");
	while(ESP8266_SendCmd(ESP8266_TIME_INFO,"CONNECT"))
		delay_ms(500);    //����Ŀ��TCP������)

	UsartPrintf(USART_DEBUG, "6. CIPMODE\r\n");
	while(ESP8266_SendCmd("AT+CIPMODE=1\r\n","OK"))	
		delay_ms(500);      //����ģʽΪ��͸��	
	
}

//==========================================================
//	�������ƣ�	USART2_IRQHandler
//
//	�������ܣ�	����2�շ��ж�
//
//	��ڲ�����	��
//
//	���ز�����	��
//
//	˵����		
//==========================================================
void USART2_IRQHandler(void)
{

	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) //�����ж�
	{
		if(esp8266_cnt >= sizeof(esp8266_buf))	esp8266_cnt = 0; //��ֹ���ڱ�ˢ��
		esp8266_buf[esp8266_cnt++] = USART2->DR;
		
		USART_ClearFlag(USART2, USART_FLAG_RXNE);
	}

}








