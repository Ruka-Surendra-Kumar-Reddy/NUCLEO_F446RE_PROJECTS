#include<string.h>
#include"stm32f4xx_hal.h"
#include"main.h"
#include<stdio.h>
void systemclockconfig();
void timer6_init(void);
void GPIO_Init(void);
void Error_handler(void);
TIM_HandleTypeDef htimer6;

int main(void)
{

	HAL_Init();
	systemclockconfig();
	GPIO_Init();
	timer6_init();
	HAL_TIM_Base_Start(&htimer6);
	while(1)
	{
		// loop untill the update event flag is set
		while(!(TIM6->SR & TIM_SR_UIF));
		TIM6->SR=0;
		HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
	}
	return 0;
}
void timer6_init()
{
	htimer6.Instance = TIM6;
	htimer6.Init.Prescaler=24;
	htimer6.Init.Period=64000-1;
	if(HAL_TIM_Base_Init(&htimer6)!= HAL_OK)
	{
		Error_handler();
	}

}
void GPIO_Init(void)
{
	__HAL_RCC_GPIOA_CLK_ENABLE();
	GPIO_InitTypeDef gpioled;
	gpioled.Pin = GPIO_PIN_5;
	gpioled.Mode= GPIO_MODE_OUTPUT_PP;
	gpioled.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOA,&gpioled);

}
void systemclockconfig(void)
{

}

void Error_handler(void)
{
while(1);
}
