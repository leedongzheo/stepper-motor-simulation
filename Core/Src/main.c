/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "delay.h"
#include "timer_pwm.h"
#include "lcd_16x2.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim2;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM2_Init(void);
void one_Pha_On(uint16_t sovong);
void two_Pha_On(uint16_t sovong);
void half_Step(uint16_t sovong);
void hienthiLCD_chomicrostep(uint16_t count);
void micro_Step(uint16_t goc);
//uint16_t delay_Full_Step = 240;
//uint16_t delay_Full_Step = 240;
uint16_t delay_Full_Step = 50;
//uint16_t delay_Half_Step = 240;
uint16_t delay_Half_Step = 50;
uint16_t delay_Micro_Step = 1000;
uint8_t half_Step_Set_Pin[8] = {0b0001, 0b0101, 0b0100, 0b0110, 0b0010, 0b1010, 0b1000, 0b1001};
uint8_t microstep[8] = {0b000100, 0b010100, 0b010000, 0b011000, 0b001000, 0b101000, 0b100000, 0b100100};

uint16_t mang1[16] = {5880, 12000, 17400, 22800, 28200, 33600, 37800, 42600, 46200, 49800, 52800, 55800, 57360, 58800, 59700, 60000};

uint16_t mang2[16] = {59700, 58800, 57360, 55800, 49800, 46200, 42600, 37800, 33600, 28200, 22800, 17400, 12000, 5880, 0};
void set_PWM_ENA(uint16_t pwm) {
  TIM2->CCR1 = pwm;
}
void set_PWM_ENB(uint16_t pwm) {
	TIM2->CCR2 = pwm;
}
void hienthiLCD_chomicrostep(uint16_t count){
	Lcd_gotoxy(0,1);
	Lcd_write_string("Goc buoc: ");
	Lcd_gotoxy(10,1);
	Lcd_write_int((count*360)/6400+1);
}
void one_Pha_On(uint16_t sovong) {
	set_PWM_ENA(59999);
	set_PWM_ENB(59999);
	uint16_t dem=0;
	uint16_t n=0;
	uint16_t count=0;
	Lcd_gotoxy(0,1);
	Lcd_write_string("So vong: ");
	Lcd_gotoxy(10,1);
	Lcd_write_int(n);
	uint8_t button2;
	uint8_t button3;
	uint8_t button4;
	while(1) {
		button2=HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_10);
		button3=HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_11);
		button4=HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_12);
		if(button2==0||button3==0||button4==0){
			return;
		}
		uint8_t i = 0;
		while(i<8){
		count+=1;
		GPIOA->ODR |= half_Step_Set_Pin[i]<<2; 
	  delayMs(delay_Full_Step);
		GPIOA->ODR &= ~(half_Step_Set_Pin[i]<<2); 
		Lcd_gotoxy(0,0);
		Lcd_write_string("So xung: ");
    Lcd_gotoxy(10,0);
				
		Lcd_write_int(dem);
			dem+=1;
//		prin_Speed_Mode();
		
		i = i + 2;
						
		if(count==200){
		n+=1;
			count=0;
			Lcd_gotoxy(0,1);
		  Lcd_write_string("So vong: ");
			Lcd_gotoxy(10,1);
		  Lcd_write_int(n);
		}
		if(dem>sovong*200){
		return;
			}
		}
 	}
}
void two_Pha_On(uint16_t sovong) {
	set_PWM_ENA(59999);
	set_PWM_ENB(59999);
	uint16_t dem=0;
	uint16_t n=0;
	uint16_t count=0;
	Lcd_gotoxy(0,1);
	Lcd_write_string("So vong: ");
	Lcd_gotoxy(10,1);
	Lcd_write_int(n);
	uint8_t button1;
	uint8_t button3;
	uint8_t button4;
	while(1) {
		button1=HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_9);
		button3=HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_11);
		button4=HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_12);
		if(button1==0||button3==0||button4==0){
			return;
		}
		uint8_t i = 1;
		while(i<9){
		count+=1;
		GPIOA->ODR |= half_Step_Set_Pin[i]<<2;
		delayMs(delay_Full_Step);
		GPIOA->ODR &= ~(half_Step_Set_Pin[i]<<2);	
	//	prin_Speed_Mode();
		Lcd_gotoxy(0,0);
		Lcd_write_string("So xung: ");
    Lcd_gotoxy(10,0);
		Lcd_write_int(dem);
		dem+=1;
		
		i = i + 2;
	//	if(i == 9) i = 1;
			if(count==200){
				n+=1;
				count=0;
				Lcd_gotoxy(0,1);
				Lcd_write_string("So vong: ");
				Lcd_gotoxy(10,1);
				Lcd_write_int(n);
		}
			if(dem>sovong*200){
		return;
			}
		}
	}
}
void half_Step(uint16_t sovong) {
	set_PWM_ENA(59999);
	set_PWM_ENB(59999);
	uint16_t dem=0;
	uint16_t count=0;
	uint16_t n=0;
	Lcd_gotoxy(0,1);
	Lcd_write_string("So vong: ");
	Lcd_gotoxy(10,1);
	Lcd_write_int(n);
	uint8_t button1;
	uint8_t button2;
	uint8_t button4;
	while(1) {
	button1=HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_9);
	button2=HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_10);
	button4=HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_12);
	if(button1==0||button2==0||button4==0){
			return;
	}
	uint8_t i = 0;
	while(i<8) {
		count+=1;
		GPIOA->ODR |= half_Step_Set_Pin[i]<<2;
		delayMs(delay_Half_Step);
		GPIOA->ODR &= ~(half_Step_Set_Pin[i]<<2);
		Lcd_gotoxy(0,0);
		Lcd_write_string("So xung: ");
    Lcd_gotoxy(10,0);
		Lcd_write_int(dem);
		dem+=1;
		i++;
	//	if(i == 8) i = 0;
			if(count==400){
				n+=1;
				count=0;
				Lcd_gotoxy(0,1);
				Lcd_write_string("So vong: ");
				Lcd_gotoxy(10,1);
				Lcd_write_int(n);
		}
		if(dem>sovong*400){
		return;
			}
		}
	}
}
void micro_Step(uint16_t goc) {
	
  set_PWM_ENA(60000);
  set_PWM_ENB(0);
	GPIOA->ODR=microstep[0];
	uint8_t i = 0;
	uint32_t count = 0;
	uint16_t n=0;
	uint8_t button1;
	uint8_t button2;
	uint8_t button3;
	while(1) {
	button1=HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_9);
	button2=HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_10);
	button3=HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_11);
	if(button1==0||button2==0||button3==0){
			return;
	}
	for(i=0;i<=15;i++){
		GPIOA->ODR=microstep[1];
		set_PWM_ENB(mang1[i]);
		delayUs(delay_Micro_Step);
		count++;
		if(count == (goc*6400)/360) return;
		}
	hienthiLCD_chomicrostep(count);
	for(i=0;i<=15;i++){
		GPIOA->ODR=microstep[2];
		set_PWM_ENA(mang2[i]);
		delayUs(delay_Micro_Step);
		count++;
		if(count == (goc*6400)/360) return;
		}
	hienthiLCD_chomicrostep(count);
	for(i=0;i<=15;i++){
		GPIOA->ODR=microstep[3];
		set_PWM_ENA(mang1[i]);
		delayUs(delay_Micro_Step);
		count++;
		if(count == (goc*6400)/360) return;
		}
	hienthiLCD_chomicrostep(count);
	for(i=0;i<=15;i++){
		GPIOA->ODR=microstep[4];
		set_PWM_ENB(mang2[i]);
		delayUs(delay_Micro_Step);
		count++;
		if(count == (goc*6400)/360) return;
		}
	hienthiLCD_chomicrostep(count);
	for(i=0;i<=15;i++){
		GPIOA->ODR=microstep[5];
		set_PWM_ENB(mang1[i]);
		delayUs(delay_Micro_Step);
		count++;
		if(count == (goc*6400)/360) return;
		}
	hienthiLCD_chomicrostep(count);
	for(i=0;i<=15;i++){
		GPIOA->ODR=microstep[6];
		set_PWM_ENA(mang2[i]);
		delayUs(delay_Micro_Step);
		count++;
		if(count == (goc*6400)/360) return;
		}
	hienthiLCD_chomicrostep(count);
	for(i=0;i<=15;i++){
		GPIOA->ODR=microstep[7];
		set_PWM_ENA(mang1[i]);
		delayUs(delay_Micro_Step);
		count++;
		if(count == (goc*6400)/360) return;
		}
	hienthiLCD_chomicrostep(count);
	for(i=0;i<=15;i++){
		GPIOA->ODR=microstep[0];
		set_PWM_ENB(mang2[i]);
		delayUs(delay_Micro_Step);
		count++;
		if(count == (goc*6400)/360) return;
		}
	hienthiLCD_chomicrostep(count);
	}
}

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
void ledtat(){
HAL_GPIO_WritePin(GPIOB,GPIO_PIN_11,0);
};
void ledsang(){
HAL_GPIO_WritePin(GPIOB,GPIO_PIN_11,1);
}
void ledchoptat(){
	uint8_t button1;
	uint8_t button2;
	while(1){
		button1=HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_9);
		button2=HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_10);
		if(button1==0||button2==0){
			return;
		}
		
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_11,0);
	delayUs(500000);
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_11,1);
	delayUs(500000);
}
	}
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();
	Lcd_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM2_Init();
	PWM_Start(&htim2,CH1);
	PWM_Start(&htim2,CH2);

  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	uint8_t button1;
	uint8_t button2;
	uint8_t button3;
	uint8_t button4;
   while (1)
  {
 
	button1=HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_9);
	button2=HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_10);
	button3=HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_11);
	button4=HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_12);
  if(button1==0){			
			one_Pha_On(1);
			//delayMs(1000);
			//one_Pha_On(2);
			//delayMs(1000);
	}
	else if(button2==0){
		two_Pha_On(1);
		//delayMs(1000);
		//two_Pha_On(2);
		//delayMs(1000);
	}
	else if(button3==0){
		half_Step(1);
	}
	else if(button4==0){
		micro_Step(30);
		delayMs(2000);
		micro_Step(60);
		delayMs(2000);
		micro_Step(45);
		delayMs(2000);
		micro_Step(225);
		delayMs(2000);
		micro_Step(360);
		delayMs(2000);
		micro_Step(90);
		delayMs(2000);
	}
		//delayMs(1000);
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
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
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI_DIV2;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 0;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 59999;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 2000;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */
  HAL_TIM_MspPostInit(&htim2);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5, GPIO_PIN_RESET);

  /*Configure GPIO pins : PA2 PA3 PA4 PA5 */
  GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	  /*Configure GPIO pins : PA9 PA10 PA11 PA12 */
  GPIO_InitStruct.Pin = GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	/*Configure GPIO pin : PB11 */
  GPIO_InitStruct.Pin = GPIO_PIN_11;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

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

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
