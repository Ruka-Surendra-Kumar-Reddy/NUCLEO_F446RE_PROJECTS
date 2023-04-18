#include"main.h"
void HAL_MspInit(void)
{
	HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);
	SCB->SHCSR |= 0x7 << 16;
	HAL_NVIC_SetPriority(MemoryManagement_IRQn,0,0);
	HAL_NVIC_SetPriority( BusFault_IRQn,0,0);
	HAL_NVIC_SetPriority( UsageFault_IRQn,0,0);

}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htimer)
{
	// enable the clock for tim6 clock
	__HAL_RCC_TIM6_CLK_ENABLE();
	//enable the irq of tim6
	HAL_NVIC_EnableIRQ(TIM6_DAC_IRQn);
	//Setup the priority
	HAL_NVIC_SetPriority(TIM6_DAC_IRQn,15,0);
}
