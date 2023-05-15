#include"main.h"
#include"stm32f4xx_hal_tim.h"
void HAL_MspInit(void)
{
	HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);
	SCB->SHCSR |= 0x7 << 16;
	HAL_NVIC_SetPriority(MemoryManagement_IRQn,0,0);
	HAL_NVIC_SetPriority( BusFault_IRQn,0,0);
	HAL_NVIC_SetPriority( UsageFault_IRQn,0,0);

}
void HAL_TIM_OC_MspInit(TIM_HandleTypeDef *htim)
{
	GPIO_InitTypeDef tim2oc_ch_gpios;
	__HAL_RCC_TIM2_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	/*pa0->channel1
	 * pa1->channel2
	 * pb10->channel3
	 * pb2->channel4
	 */
	tim2oc_ch_gpios.Pin=GPIO_PIN_0|GPIO_PIN_1;
	tim2oc_ch_gpios.Mode= GPIO_MODE_AF_PP ;
	tim2oc_ch_gpios.Pull=GPIO_NOPULL;
	tim2oc_ch_gpios.Speed=GPIO_SPEED_FREQ_LOW;
	tim2oc_ch_gpios.Alternate=GPIO_AF1_TIM2;
	HAL_GPIO_Init(GPIOA,&tim2oc_ch_gpios);

	tim2oc_ch_gpios.Pin=GPIO_PIN_2|GPIO_PIN_10;
		tim2oc_ch_gpios.Mode= GPIO_MODE_AF_PP ;
		tim2oc_ch_gpios.Pull=GPIO_NOPULL;
		tim2oc_ch_gpios.Speed=GPIO_SPEED_FREQ_LOW;
		tim2oc_ch_gpios.Alternate=GPIO_AF1_TIM2;
		HAL_GPIO_Init(GPIOB,&tim2oc_ch_gpios);

	HAL_NVIC_SetPriority(TIM2_IRQn,15,0);
	HAL_NVIC_EnableIRQ(TIM2_IRQn);
}

void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
	GPIO_InitTypeDef gpio_uart;
	__HAL_RCC_USART2_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	gpio_uart.Pin = GPIO_PIN_2;
	gpio_uart.Mode = GPIO_MODE_AF_PP ;
	gpio_uart.Pull = GPIO_PULLUP ;
	gpio_uart.Speed = GPIO_SPEED_FREQ_LOW;
	gpio_uart.Alternate = GPIO_AF7_USART2 ;
	HAL_GPIO_Init(GPIOA,&gpio_uart);
	gpio_uart.Pin = GPIO_PIN_3;
	HAL_GPIO_Init(GPIOA,&gpio_uart);

	HAL_NVIC_EnableIRQ(USART2_IRQn);
	HAL_NVIC_SetPriority(  USART2_IRQn,15,0);

}

