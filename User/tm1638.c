#include "tm1638.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"

uc8 BCD[] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x77,0x7C,0x39,0x5E,0x79,0x71};

void TM1638_Start()
{
	TM1638_STB = 0;
	Delay_us(1);
}

void TM1638_Stop()
{
	TM1638_STB = 1;
	Delay_us(1);
}

//写入一个字节数据
void TM1638_Write(u8 dat)
{
	u8 i;
	for (i = 0; i < 8; i++)
	{
		TM1638_DIO = dat & 1;
		TM1638_CLK = 0;
		Delay400ns();
		TM1638_CLK = 1;
		Delay400ns();
		dat >>= 1;
	}
	Delay_us(1);
}

//设置某一位的数据
void TM1638_Set(u8 addr, u16 dat)
{
	TM1638_Start();
	TM1638_Write(0x44);//普通模式 固定地址 写数据
	TM1638_Stop();
	TM1638_Start();
	TM1638_Write(0xC0 | (addr * 2));//设置地址
	TM1638_Write(dat);//低8位
	TM1638_Stop();
	TM1638_Start();
	TM1638_Write(0xC0 | (addr * 2 + 1));//设置地址
	TM1638_Write((dat >> 8) & 0x03);//高2位
	TM1638_Stop();
}

void TM1638_SetNum(u8 addr, u8 num)
{
	TM1638_Set(addr, BCD[num % 0x10]);
}

void TM1638_SetNumAndDot(u8 addr, u8 num)
{
    TM1638_Set(addr, BCD[num % 0x10] | 0x80);
}

void TM1638_Init()
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
	u8 i;
	TM1638_Start();
	TM1638_Write(0x40);//普通模式 自动增加地址 写数据
	TM1638_Stop();
	TM1638_Start();
	TM1638_Write(0x80 | 0x8 | 0x2);//显示开 1级亮度
	TM1638_Stop();
	TM1638_Start();
	TM1638_Write(0xC0);
	for(i=0;i<16;i++)//清屏
		TM1638_Write(0x00);
	TM1638_Stop();
}