#include"stm32f4xx_hal.h"
#include"main.h"
#include<stdio.h>
#include"stm32f4xx_hal_tim.h"
#include<string.h>

void systemclock_config(uint8_t clock_freq);
void GPIO_Init(void);
void Timer2_Init();
void lse_configuration(void);
void Error_handler(void);
void UART2_Init(void);
UART_HandleTypeDef huart2;
TIM_HandleTypeDef htimer2;
uint32_t input_captures[2]={0};
uint8_t count=1;
uint8_t is_capture_done = FALSE;
int main(void)
{
 double capture_difference;
 double timer2_cnt_freq;
 double timer2_cnt_res;
 double user_signal_time_period=0;
 double user_signal_freq=0;
 char user_msg[100];
	HAL_Init();
	systemclock_config(SYS_CLK_FREQ_50_MHZ);
	GPIO_Init();
	UART2_Init();
	Timer2_Init();
	lse_configuration();
	HAL_TIM_IC_Start(&htimer2, TIM_CHANNEL_1);
	{
 		if(is_capture_done)
		{

			if(input_captures[1]>input_captures[0])
			{
				capture_difference=input_captures[1]-input_captures[0];

			}
			else
			{
				capture_difference=(0XFFFFFFFF-input_captures[0])+input_captures[1];
			}

			timer2_cnt_freq=(HAL_RCC_GetPCLK1Freq()*2)/(htimer2.Init.Prescaler+1);
			timer2_cnt_res=1/timer2_cnt_freq;
			user_signal_time_period=capture_difference*timer2_cnt_res;
			user_signal_freq=1/user_signal_time_period;
			sprintf(user_msg,"frequency of the signal applied =%f\r\n",user_signal_freq);
			HAL_UART_Transmit(&huart2,(uint8_t*)user_msg,strlen(user_msg),HAL_MAX_DELAY);
			is_capture_done=FALSE;
		}
	}

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
	   		clk_init.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK |RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
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
	TIM_IC_InitTypeDef timer2IC_config;
	htimer2.Instance=TIM2;
	htimer2.Init.CounterMode = TIM_COUNTERMODE_UP ;
	htimer2.Init.Period=0xFFFFFFFF;
	htimer2.Init.Prescaler=1;
	if(HAL_TIM_IC_Init(&htimer2)!= HAL_OK)
	{
		Error_handler();
	}
	timer2IC_config.ICFilter = 0;
	timer2IC_config.ICPolarity=TIM_ICPOLARITY_RISING ;
	timer2IC_config.ICPrescaler=TIM_ICPSC_DIV1 ;
	timer2IC_config.ICSelection=TIM_ICSELECTION_DIRECTTI  ;
	if(HAL_TIM_IC_ConfigChannel(&htimer2, &timer2IC_config, TIM_CHANNEL_1)!=HAL_OK)
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
void lse_configuration(void)
{
#if 0
	RCC_OscInitTypeDef osc_init;
	osc_init.OscillatorType = RCC_OSCILLATORTYPE_LSE;
	osc_init.LSEState = RCC_LSE_ON  ;
	if(HAL_RCC_OscConfig(&osc_init) != HAL_OK)
		{
			Error_handler();
		}
#endif
	HAL_RCC_MCOConfig(RCC_MCO1, RCC_MCO1SOURCE_LSE, RCC_MCODIV_1);
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{

if(!is_capture_done){
	if(count == 1)
	{
		input_captures[0]=__HAL_TIM_GET_COMPARE(htim,TIM_CHANNEL_1);
		count++;
	}
	else if(count ==   2)
	{
		input_captures[1]=__HAL_TIM_GET_COMPARE(htim,TIM_CHANNEL_1);
		count=1;
		is_capture_done=TRUE;
	}

}
}
void Error_handler(void)
{
while(1);
}
