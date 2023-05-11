#include"stm32f4xx_hal.h"
#include"main.h"
#include<string.h>
#include"stm32f4xx_hal_UART.h"

void systemclock_config(uint8_t clock_freq);
void GPIO_Init(void);
void Timer2_Init();
void Error_handler(void);
void UART2_Init(void);
UART_HandleTypeDef huart2;
TIM_HandleTypeDef htimer2;
uint32_t pulse1_value=25000;
uint32_t pulse2_value=12500;
uint32_t pulse3_value=6250;
uint32_t pulse4_value=3125;
uint32_t ccr_content;
int main(void)
{

	HAL_Init();
	systemclock_config(SYS_CLK_FREQ_50_MHZ);
	GPIO_Init();
	UART2_Init();
	Timer2_Init();
if(HAL_TIM_OC_Start_IT(&htimer2,TIM_CHANNEL_1)!=HAL_OK)
{
	Error_handler();
}
if(HAL_TIM_OC_Start_IT(&htimer2,TIM_CHANNEL_2)!=HAL_OK)
{
	Error_handler();
}
if(HAL_TIM_OC_Start_IT(&htimer2,TIM_CHANNEL_3)!=HAL_OK)
{
	Error_handler();
}
if(HAL_TIM_OC_Start_IT(&htimer2,TIM_CHANNEL_4)!=HAL_OK)
{
	Error_handler();
}

	while(1);

	return 0;
}
void systemclock_config(uint8_t clock_freq)
{
	RCC_OscInitTypeDef osc_init;
	RCC_ClkInitTypeDef clk_init;
	osc_init.OscillatorType = RCC_OSCILLATORTYPE_HSI |RCC_OSCILLATORTYPE_LSE;
	osc_init.HSIState = RCC_HSI_ON  ;
	osc_init.LSEState = RCC_LSE_ON  ;
	osc_init.HSICalibrationValue = 16;
	osc_init.PLL.PLLState =  RCC_PLL_ON ;
	osc_init.PLL.PLLSource = RCC_PLLSOURCE_HSI;
	switch(clock_freq)
	{
	   case  SYS_CLK_FREQ_50_MHZ:
	   	  {
	   		osc_init.PLL.PLLM = 8;
	   		osc_init.PLL.PLLN = 50;
	   		osc_init.PLL.PLLP = RCC_PLLP_DIV2;
	   		osc_init.PLL.PLLQ = 2;
	   		clk_init.ClockType = RCC_CLOCKTYPE_SYSCLK    |  RCC_CLOCKTYPE_HCLK  | RCC_CLOCKTYPE_PCLK1   | RCC_CLOCKTYPE_PCLK2;
			clk_init.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK ;
			clk_init.AHBCLKDivider = RCC_SYSCLK_DIV1;
			clk_init.APB1CLKDivider = RCC_HCLK_DIV2 ;
			clk_init.APB2CLKDivider = RCC_HCLK_DIV2 ;

	break;
	}
	case SYS_CLK_FREQ_84_MHZ:
	{

   		osc_init.PLL.PLLM = 16;
   		osc_init.PLL.PLLN = 168;
   		osc_init.PLL.PLLP = 2;
   		osc_init.PLL.PLLQ = 2;
		clk_init.ClockType = RCC_CLOCKTYPE_SYSCLK    |  RCC_CLOCKTYPE_HCLK  | RCC_CLOCKTYPE_PCLK1   | RCC_CLOCKTYPE_PCLK2;
		clk_init.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK ;
		clk_init.AHBCLKDivider = RCC_SYSCLK_DIV1;
		clk_init.APB1CLKDivider = RCC_HCLK_DIV2 ;
		clk_init.APB2CLKDivider = RCC_HCLK_DIV2 ;

   		break;
	}
	case SYS_CLK_FREQ_168_MHZ:
	{
		__HAL_RCC_PWR_CLK_ENABLE();
		__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
		osc_init.PLL.PLLM = 16;
		osc_init.PLL.PLLN = 336;
		osc_init.PLL.PLLP = 2;
		osc_init.PLL.PLLQ = 2;
		clk_init.ClockType = RCC_CLOCKTYPE_SYSCLK    |  RCC_CLOCKTYPE_HCLK  | RCC_CLOCKTYPE_PCLK1   | RCC_CLOCKTYPE_PCLK2;
		clk_init.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK ;
		clk_init.AHBCLKDivider = RCC_SYSCLK_DIV1;
		clk_init.APB1CLKDivider = RCC_HCLK_DIV4;
		clk_init.APB2CLKDivider = RCC_HCLK_DIV2;

		break;
	}
	default:
		return;
	}
	if(HAL_RCC_OscConfig(&osc_init) != HAL_OK)
	{
		Error_handler();
	}
	if(HAL_RCC_ClockConfig(&clk_init,FLASH_LATENCY_2)!= HAL_OK)
	{
	Error_handler();
     }
	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);
	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);
	HAL_NVIC_SetPriority(SysTick_IRQn,0,0);
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

void Timer2_Init()
{
TIM_OC_InitTypeDef tim2oc;
	htimer2.Instance=TIM2;
	htimer2.Init.Period=0xffffffff;
	htimer2.Init.Prescaler=1;
	if(	HAL_TIM_OC_Init(&htimer2)!=HAL_OK)
	{
		Error_handler();
	}
	tim2oc.OCMode=TIM_OCMODE_TOGGLE ;
	tim2oc.OCPolarity=TIM_OCPOLARITY_HIGH;
	tim2oc.Pulse=pulse1_value;
	if(HAL_TIM_OC_ConfigChannel(&htimer2,&tim2oc,TIM_CHANNEL_1)!=HAL_OK)
	{
		Error_handler();
	}
	tim2oc.Pulse=pulse2_value;
	if(HAL_TIM_OC_ConfigChannel(&htimer2,&tim2oc,TIM_CHANNEL_2)!=HAL_OK)
	{
		Error_handler();
	}
	tim2oc.Pulse=pulse3_value;
	if(HAL_TIM_OC_ConfigChannel(&htimer2,&tim2oc,TIM_CHANNEL_3)!=HAL_OK)
	{
		Error_handler();
	}
	tim2oc.Pulse=pulse4_value;
	if(HAL_TIM_OC_ConfigChannel(&htimer2, &tim2oc,TIM_CHANNEL_4)!=HAL_OK)
	{
		Error_handler();
	}

}
void UART2_Init(void)
{
	huart2.Instance = USART2;
	huart2.Init.BaudRate = 115200;
	huart2.Init.WordLength = UART_WORDLENGTH_8B ;
	huart2.Init.StopBits = UART_STOPBITS_1 ;
	huart2.Init.Parity = UART_PARITY_NONE  ;
	huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE ;
	huart2.Init.Mode = UART_MODE_TX_RX ;
	if (HAL_UART_Init(&huart2) != HAL_OK)
	{
		Error_handler();
	}
}
void  HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Channel==HAL_TIM_ACTIVE_CHANNEL_1)
	{
		ccr_content=HAL_TIM_ReadCapturedValue(htim,TIM_CHANNEL_1);
		__HAL_TIM_SET_COMPARE(htim,TIM_CHANNEL_1,ccr_content+pulse1_value);
	}
	if(htim->Channel==HAL_TIM_ACTIVE_CHANNEL_2)
		{
		ccr_content=HAL_TIM_ReadCapturedValue(htim,TIM_CHANNEL_2);
		__HAL_TIM_SET_COMPARE(htim,TIM_CHANNEL_2,ccr_content+pulse2_value);
		}
	if(htim->Channel==HAL_TIM_ACTIVE_CHANNEL_3)
		{
		ccr_content=HAL_TIM_ReadCapturedValue(htim,TIM_CHANNEL_3);
		__HAL_TIM_SET_COMPARE(htim,TIM_CHANNEL_3,ccr_content+pulse3_value);
		}
	if(htim->Channel==HAL_TIM_ACTIVE_CHANNEL_4)
		{
		ccr_content=HAL_TIM_ReadCapturedValue(htim,TIM_CHANNEL_4);
		__HAL_TIM_SET_COMPARE(htim,TIM_CHANNEL_4,ccr_content+pulse4_value);
		}
}
void Error_handler(void)
{
while(1);
}
