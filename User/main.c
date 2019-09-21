#include "common.h"
#include "tm1638.h"
#include "ds18b20.h"

uc8 GOOD[] = {0x6F, 0x5C, 0x5E};

void setGood()
{
	TM1638_Set(0, GOOD[0]);
	TM1638_Set(1, GOOD[1]);
	TM1638_Set(2, GOOD[1]);
	TM1638_Set(3, GOOD[2]);
}

void resetGood()
{
	TM1638_Set(0, 0);
	TM1638_Set(1, 0);
	TM1638_Set(2, 0);
	TM1638_Set(3, 0);
}

int main (void) {
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	TM1638_Init();
	TM1638_SetNum(0, 0xE);
	while (DS18B20_Init())
	{
	}
	TM1638_Set(0, 0);

	TM1638_SetNum(7, 0xC);
	while(1)
	{
		short temp = DS18B20_Get_Temp();
		TM1638_SetNum(4, temp / 100);
		TM1638_SetNumAndDot(5, temp % 100 / 10);
		TM1638_SetNum(6, temp % 10);
		if (temp < 300)
			setGood();
		else 
			resetGood();
		Delay_ms(1000);
	}
}
