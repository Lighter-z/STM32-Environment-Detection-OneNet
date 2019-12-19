#ifndef _BH1750_H_
#define _BH1750_H_

#include "stm32f10x.h"

/*
*  VCC-->3.3V     
*  GND-->GND
*	 ADDR----->GND
*  SCL ----->pb8
*	 sda ----->pb9
*/


//IO方向设置
 
//#define BH1750_SDA_IN()  {GPIOB->CRL&=0XFF0FFFFF;GPIOB->CRL|=(u32)8<<20;}
//#define BH1750_SDA_OUT() {GPIOB->CRL&=0XFF0FFFFF;GPIOB->CRL|=(u32)3<<20;}
#define BH1750_SDA_IN()  {GPIOB->CRH&=0XFFFFFF0F;GPIOB->CRH|=(u32)8<<4;}
#define BH1750_SDA_OUT() {GPIOB->CRH&=0XFFFFFF0F;GPIOB->CRH|=(u32)3<<4;}

////IO操作函数	 
//#define BH1750_IIC_SCL    PBout(6) //SCL
//#define BH1750_IIC_SDA    PBout(5) //SDA	 
//#define BH1750_READ_SDA   PBin(5)  //输入SDA 

//IO操作函数	 
#define BH1750_IIC_SCL    PBout(8) //SCL
#define BH1750_IIC_SDA    PBout(9) //SDA	 
#define BH1750_READ_SDA   PBin(9)  //输入SDA 

#define	  BH1750_SlaveAddress   0x46 //定义器件在IIC总线中的从地址,根据ALT  ADDRESS地址引脚不同修改
                              //ALT  ADDRESS引脚接地时地址为0x46，接电源时地址为0xB8

void bh1750_iic_init(void);
void bh1750_iic_start(void);
void bh1750_iic_stop(void);
void bh1750_send_ack(char ack);
char bh1750_recv_ack(void);
void bh1750_send_byte(char dat);
char bh1750_recv_byte(unsigned char ack);

//void bh1750_iic_init(void);
void Single_Write_bh1750(unsigned char Reg_Address);
//void bh1750_Init(void);
//u8 bh1750_Init(void);
float bh1750_Init(void);
void bh1750_ReadOneTime(void);
void bh1750_ReadContinuous1(void);




#endif

