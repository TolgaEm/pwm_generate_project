
/*

Pulse width modulation mode allows generating a signal with a frequency determined by the
value of the TIMx_ARR register and a duty cycle determined by the value of the
TIMx_CCRx register.

The PWM mode can be selected independently on each channel (one PWM per OCx
output) by writing 110 (PWM mode 1) or ‘111 (PWM mode 2) in the OCxM bits in the
TIMx_CCMRx register. The user must enable the corresponding preload register by setting
the OCxPE bit in the TIMx_CCMRx register, and eventually the auto-reload preload register
by setting the ARPE bit in the TIMx_CR1 register.

As the preload registers are transferred to the shadow registers only when an update event
occurs, before starting the counter, the user has to initialize all the registers by setting the
UG bit in the TIMx_EGR register.

OCx polarity is software programmable using the CCxP bit in the TIMx_CCER register. It
can be programmed as active high or active low. OCx output is enabled by the CCxE bit in
the TIMx_CCER register. Refer to the TIMx_CCERx register description for more details.

In PWM mode (1 or 2), TIMx_CNT and TIMx_CCRx are always compared to determine
whether TIMx_CCRx? TIMx_CNT or TIMx_CNT? TIMx_CCRx (depending on the direction
of the counter). However, to comply with the ETRF (OCREF can be cleared by an external
event through the ETR signal until the next PWM period), the OCREF signal is asserted
only:
	* When the result of the comparison changes, or
	*	When the output compare mode (OCxM bits in TIMx_CCMRx register) switches from
the “frozen” configuration (no comparison, OCxM=‘000) to one of the PWM modes
(OCxM=‘110 or ‘111).

The timer is able to generate PWM in edge-aligned mode or center-aligned mode
depending on the CMS bits in the TIMx_CR1 register.

*/
#include "main.h"

void gpio_config(void);
void pwm_config(void);
uint16_t pwm_counter;
void delay(uint16_t);
void SystemClock_Config(void);

int main(void)
{



  SystemClock_Config();
  gpio_config();
  pwm_config();


  while (1)
  {

		for (pwm_counter = 0; pwm_counter < 100; pwm_counter++) {
			if (pwm_counter == 99) {
				pwm_counter = 0;
				TIM2->CCR1 = pwm_counter;
				break;
			}

			TIM2->CCR1 = pwm_counter;
			delay(100);

		}

		for (pwm_counter = 100; pwm_counter >= 0; pwm_counter--) {
			if (pwm_counter == 0) {

				pwm_counter = 0;
				TIM2->CCR2 = pwm_counter;
				break;
			}
			TIM2->CCR2 = pwm_counter;
			delay(100);
		}

	  }
  }

void gpio_config(void){

	RCC->APB2ENR |= (1<<2); //set PORTA clock bus
	GPIOA->CRL = 0X00009999; // set PA0,PA1,PA2,PA3 alternate function

}

void pwm_config(void){

	RCC->APB1ENR |= (1<<0); //set TIM2 clock bus enable
	TIM2->PSC = 80 - 1; //timer clock 1 MHz
	TIM2->ARR = 100 - 1 ; // period  1 KHz
	TIM2->CCR1 = 0;
	TIM2->CCR2 = 0;
	TIM2->CCR3 = 0;
	TIM2->CCR4 = 0;
	TIM2->CCMR1 = 0X6060;  // set channel1 and channel2 pwm mode
	TIM2->CCMR2 = 0X6060; // set channel3 and channel4 pwm mode
	TIM2->CR1 |= (1<<7); // ARPE bit set
	TIM2->EGR |= (1<<0); // set UG bit and  reinitialize counter
	TIM2->CCER = 4369; // set compare output enable

}

void delay(uint16_t time){

	int c, d;

   for (c = 1; c <= time; c++)
       for (d = 1; d <= time; d++)
       {}

}

void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

