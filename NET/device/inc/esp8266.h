#ifndef _ESP8266_H_
#define _ESP8266_H_


/*
ESP8266����

3v3  --->  3v3         RX   --->  PA2
RST  --->  PA4         
EN   --->  3v3         
TX   --->  PA3         GND  --->  GND

*/


#define REV_OK		0	//������ɱ�־
#define REV_WAIT	1	//����δ��ɱ�־




void ESP8266_Init(void);

void ESP8266_Clear(void);

void ESP8266_SendData(unsigned char *data, unsigned short len);

unsigned char *ESP8266_GetIPD(unsigned short timeOut);

/********************/
_Bool ESP8266_SendCmd(char *cmd, char *res);

unsigned char *ESP8266_GetTIME(unsigned short timeOut);

void Esp8266NtpTimeInit(void);


#endif

