/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
#include "myvars.h"			// variables


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
ADC_HandleTypeDef hadc1;
DMA_HandleTypeDef hdma_adc1;

I2C_HandleTypeDef hi2c2;

SPI_HandleTypeDef hspi2;

TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim4;

/* USER CODE BEGIN PV */
#include "maincode.h"			// void
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_ADC1_Init(void);
static void MX_SPI2_Init(void);
static void MX_TIM3_Init(void);
static void MX_TIM4_Init(void);
static void MX_I2C2_Init(void);
static void MX_TIM2_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_ADC1_Init();
  MX_SPI2_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  MX_I2C2_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */

  const volatile uint8_t *userConfig=(const volatile uint8_t *)0x0800D2F0;





//LL_SPI_Enable(SPI2);

  HAL_SPI_Init(&hspi2); // write to register hspi2
  // lcd_init(); // keep this late or have issues
//HAL_TIM_Base_Start_IT(&htim1);  // This needs to work for irq   ,disbling tim1 made loop a lot faster
//TIM1->CCER=0;
HAL_TIM_Base_Start_IT(&htim3);  // This needs to work for irq
TIM3->CCER=0;
HAL_TIM_Base_Start(&htim2);
HAL_TIM_Base_Start(&htim4);
//HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_1);  // they both work fine together
HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_3);
//HAL_TIM_PWM_Start_DMA(&htim3, TIM_CHANNEL_4,pData, 63);
TIM2->CNT=32000;
HAL_ADC_Start(&hadc1);
HAL_ADC_Start_DMA(&hadc1, adc_source, 512); //dma start ,needs this and adc start ,set sampling time to very long or it will fail


HAL_I2C_MspInit(&hi2c2);

/*
for(i=0;i<128;i++){			// read values from stored
mem_buf=0;
HAL_I2C_Mem_Read(&hi2c1, 160, ((1+(i>>6))<<6)+(i&63), 2,  &mem_buf, 1, 1000); // add 160 is correct  // only single byte works for now
potSource[i]=mem_buf;
potValues[i]=mem_buf>>2;
} // reads stored values for potvalues
*/

uint8_t potSource2[65];
uint8_t potSource3[65];
HAL_I2C_Mem_Read(&hi2c2, 160, (1<<6), 2,&potSource, 64,1000);		// all good
HAL_I2C_Mem_Read(&hi2c2, 160, (2<<6), 2,&potSource2,64,1000);
HAL_I2C_Mem_Read(&hi2c2, 160, (3<<6), 2,&potSource3,64,1000);

for(i=0;i<1024;i++){
//	gfx_ram[i]=gfx_char[((i>>5)&7)+((i>>8)<<3)]; // test input fill  8*128 v+h just normal characters
	gfx_ram[i&63] [i>>6]  =gfx_char[8+(i&7)+(((i>>3)*8)&63)];
//gfx_ram[i&63] [i>>6]  =250;
}

for(i=0;i<64;i++){
	potSource[i+64]=potSource2[i];  //load up from eeprom
}

for(i=0;i<64;i++){
	potSource[i+128]=potSource3[i];
}

for(i=0;i<180;i++){
	potValues[i]=potSource[i]>>4;
}

float tempo_hold;  // calculate tempo look up
for (i=0;i<161;i++) {

tempo_hold=(i+180)*0.0166666666;

tempo_hold=	1/tempo_hold;
//tempo_hold=	tempo_hold*2187.6*4;      // change for the sake of note length
tempo_hold=	tempo_hold*2187.6*1;      // change for the sake of note length
tempo_lut[i]=tempo_hold;
}
isrMask=571; // def tempo 571=180bpm , 20 ms /isrcount

noteTiming=24;
for (i=0;i<320;i++)	{	// write C into whole section,useful ornot
	spell[i]=67;

}

display_fill();
// build display, enc_lut2, works good

for (i=0;i<512;i++)	{gfx_char[i]=gfx_char[i];


}    //font replace



uint16_t lut_temp2=0;
uint16_t lut_temp3=0;
for  (i=0;i<390;i++){					// get a few more pages

	if (disp_lut [i>>4] [i&15] <95) lut_temp2=lut_temp2+1;    // skip space or characters

	else {enc2_lut[lut_temp3] =i;  lut_temp3++;}     // goes to 511
}


menuSelect=0;
// fill up sample
firstbarLoop=0;
printf("Hello everybody");


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  loop_counter++ ;
	  loop_counter2++;//

	  // if (menu_page<320) lcd_feedback();  //curious no issues with lcd without this  , maybe spell writing
if 	((loop_counter2&7)==6)      {analoginputloopb();} // this is ok , plenty quick
if (loop_counter2==9096) {    //   4096=1min=32bytes so 4mins per 128 bank or 15 writes/hour

	mem_buf=potSource[mem_count];
	// read values from stored
	HAL_I2C_Mem_Write(&hi2c2, 160, ((1+(mem_count>>6))<<6)+(mem_count&63), 2, &mem_buf, 1, 1000);  // "&hi2c2"  actual register address
	HAL_Delay(5);
	if (mem_count==255) mem_count=0; else mem_count++;  // write to first
	loop_counter2=0; //reset

}


	  if (disp_end==1)	 {  // displaybuffer after each full screen update on spi

		  { if (loop_counter3)  enc2_tempc=enc2_dir; else enc2_dir=enc2_tempc; }    //hold enc till finished , this to clean up characters for now ,works ok
		  loop_counter3=!loop_counter3;  //blinker flips on each full page refresh

		  for (i=0;i<16;i++) {   displayBuffer();}
		  enc2_dir=enc2_tempc;

	  }


	  if (init<6)				// after 6 its done for good
{
	  for (i=0;i<6;i++) {display_init();}  //1-2ms ?  change length if flickering ,maybe initial data
} else {display_update(); }  // send spi line data every loop cycle , self contained, single 8pixel line 18*256steps

	  ///////////////////////////////////////////////////////////////////////////////

	  if (loop_counter == 255)	{ // grab adc readings + 3ms , 32 step



		  //HAL_ADC_Stop_DMA(&hadc1); // a lot more stable this way , also sampling time no more than /8 +  144 or no go

		  HAL_ADCEx_InjectedStart(&hadc1) ;  // start injected mode normal conversion
		  uint16_t adc_temp1[4]={0,0,0,0};

		  HAL_ADC_PollForConversion(&hadc1,10);  // works but  slow ,blocking

		  adc_temp1[0]=HAL_ADCEx_InjectedGetValue(&hadc1, ADC_INJECTED_RANK_1);
		  adc_temp1[1] =HAL_ADCEx_InjectedGetValue(&hadc1, ADC_INJECTED_RANK_2);
		  adc_temp1[2] =HAL_ADCEx_InjectedGetValue(&hadc1, ADC_INJECTED_RANK_3);
		  adc_values[0]=	  adc_temp1[0]>>7;
		  adc_values[1]=	  adc_temp1[1]>>7;
		  adc_values[2]=	  adc_temp1[2]>>7;
		  HAL_ADCEx_InjectedStop(&hadc1) ;
		  //HAL_ADC_Start_DMA(&hadc1, adc_source, 512);


	  	loop_counter=0;
	  }

	  if ((seq_pos==7) && (lcd_send==0)) {lcd_send=1;} // runs just once
	  /*
	   if (promValue<64) promValue=promValue+1 ; else promValue=0;  // fetch eeprom   nogo
	  	  if ((promValues[promValue] ) !=(potValues[promValue]))  EEPROM.write(promValue,(potValues[promValue]));   //  not too happy can totally kill speed  will have to put elsewhere
	  	  promValues[promValue] =potValues[promValue];
	  	   */

	  	     HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin,(seq_pos & 1)); // easy skip ?
	  	    // very inconsistent

	  if (sample_point>511) sample_pointD=0; // loop when zero cross , good but limited, works ok
	  if (sample_point<511)  sample_pointD=512;


	  if (sample_pointB!=sample_pointD) bank_write=1; // set start of buffer ,grab , works ok
	  if (bank_write){

	  	sampling();
	  /*	HAL_SPI_Transmit(&hspi2,(uint8_t *)248,1,1);
	  	HAL_SPI_Transmit(&hspi2,(uint8_t *)64,1,1);
	  	HAL_SPI_Transmit(&hspi2,(uint8_t *)0,1,1);
	  */

	  } //works ok, write opp bank





	    }  // while loop





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

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 100;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};
  ADC_InjectionConfTypeDef sConfigInjected = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */

  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV8;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.ScanConvMode = ENABLE;
  hadc1.Init.ContinuousConvMode = ENABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 1;
  hadc1.Init.DMAContinuousRequests = ENABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SEQ_CONV;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_9;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_144CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configures for the selected ADC injected channel its corresponding rank in the sequencer and its sample time
  */
  sConfigInjected.InjectedChannel = ADC_CHANNEL_4;
  sConfigInjected.InjectedRank = 1;
  sConfigInjected.InjectedNbrOfConversion = 3;
  sConfigInjected.InjectedSamplingTime = ADC_SAMPLETIME_56CYCLES;
  sConfigInjected.ExternalTrigInjecConvEdge = ADC_EXTERNALTRIGINJECCONVEDGE_NONE;
  sConfigInjected.ExternalTrigInjecConv = ADC_INJECTED_SOFTWARE_START;
  sConfigInjected.AutoInjectedConv = DISABLE;
  sConfigInjected.InjectedDiscontinuousConvMode = DISABLE;
  sConfigInjected.InjectedOffset = 0;
  if (HAL_ADCEx_InjectedConfigChannel(&hadc1, &sConfigInjected) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configures for the selected ADC injected channel its corresponding rank in the sequencer and its sample time
  */
  sConfigInjected.InjectedChannel = ADC_CHANNEL_5;
  sConfigInjected.InjectedRank = 2;
  if (HAL_ADCEx_InjectedConfigChannel(&hadc1, &sConfigInjected) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configures for the selected ADC injected channel its corresponding rank in the sequencer and its sample time
  */
  sConfigInjected.InjectedChannel = ADC_CHANNEL_6;
  sConfigInjected.InjectedRank = 3;
  if (HAL_ADCEx_InjectedConfigChannel(&hadc1, &sConfigInjected) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief I2C2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C2_Init(void)
{

  /* USER CODE BEGIN I2C2_Init 0 */

  /* USER CODE END I2C2_Init 0 */

  /* USER CODE BEGIN I2C2_Init 1 */

  /* USER CODE END I2C2_Init 1 */
  hi2c2.Instance = I2C2;
  hi2c2.Init.ClockSpeed = 400000;
  hi2c2.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c2.Init.OwnAddress1 = 0;
  hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c2.Init.OwnAddress2 = 0;
  hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C2_Init 2 */

  /* USER CODE END I2C2_Init 2 */

}

/**
  * @brief SPI2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI2_Init(void)
{

  /* USER CODE BEGIN SPI2_Init 0 */

  /* USER CODE END SPI2_Init 0 */

  /* USER CODE BEGIN SPI2_Init 1 */

  /* USER CODE END SPI2_Init 1 */
  /* SPI2 parameter configuration*/
  hspi2.Instance = SPI2;
  hspi2.Init.Mode = SPI_MODE_MASTER;
  hspi2.Init.Direction = SPI_DIRECTION_2LINES;
  hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi2.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi2.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi2.Init.NSS = SPI_NSS_SOFT;
  hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_128;
  hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi2.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI2_Init 2 */

  /* USER CODE END SPI2_Init 2 */

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

  TIM_Encoder_InitTypeDef sConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 0;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 65535;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV4;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  sConfig.EncoderMode = TIM_ENCODERMODE_TI1;
  sConfig.IC1Polarity = TIM_ICPOLARITY_FALLING;
  sConfig.IC1Selection = TIM_ICSELECTION_DIRECTTI;
  sConfig.IC1Prescaler = TIM_ICPSC_DIV1;
  sConfig.IC1Filter = 2;
  sConfig.IC2Polarity = TIM_ICPOLARITY_FALLING;
  sConfig.IC2Selection = TIM_ICSELECTION_DIRECTTI;
  sConfig.IC2Prescaler = TIM_ICPSC_DIV1;
  sConfig.IC2Filter = 2;
  if (HAL_TIM_Encoder_Init(&htim2, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 0;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 2544;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM2;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */
  HAL_TIM_MspPostInit(&htim3);

}

/**
  * @brief TIM4 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM4_Init(void)
{

  /* USER CODE BEGIN TIM4_Init 0 */

  /* USER CODE END TIM4_Init 0 */

  TIM_Encoder_InitTypeDef sConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM4_Init 1 */

  /* USER CODE END TIM4_Init 1 */
  htim4.Instance = TIM4;
  htim4.Init.Prescaler = 0;
  htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim4.Init.Period = 65535;
  htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  sConfig.EncoderMode = TIM_ENCODERMODE_TI1;
  sConfig.IC1Polarity = TIM_ICPOLARITY_FALLING;
  sConfig.IC1Selection = TIM_ICSELECTION_DIRECTTI;
  sConfig.IC1Prescaler = TIM_ICPSC_DIV1;
  sConfig.IC1Filter = 2;
  sConfig.IC2Polarity = TIM_ICPOLARITY_FALLING;
  sConfig.IC2Selection = TIM_ICSELECTION_DIRECTTI;
  sConfig.IC2Prescaler = TIM_ICPSC_DIV1;
  sConfig.IC2Filter = 2;
  if (HAL_TIM_Encoder_Init(&htim4, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM4_Init 2 */

  /* USER CODE END TIM4_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA2_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA2_Stream4_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream4_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream4_IRQn);

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
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : LED_Pin */
  GPIO_InitStruct.Pin = LED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LED_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
/* USER CODE BEGIN 4 */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)   // get data after conversion
{
	for (i=0;i<512;i++)
	{input_holder[i] = adc_source[i];

	}
}


/*
void analogInputloop(void) { // might make menuselect a master override for everything  if chnaged then halt writing new values until screen refreshed
		menuSelect = (7 - (adc_values[0] >>1))<<4;
		menuSelectX=15 - (adc_values[1] );
		tempValueA=adc_values[2];
		counterVarB = menuSelectX + menuSelect; // select mem page 10*8  and location pointer  77
	if (counterVarB != tempValue[120]) {
		modEnable = 1;
	//	lcd.setCursor((menuSelectX * 2), 1);
	} else
		modEnable = 0;   //compared to stored value
	tempValue[120] = counterVarB;
 tempValueA=(tempValueA*11) >> 4;
	if (tempValueA>10) tempValueA=10;
 tempValue[counterVarB] = tempValueA; // read and store pots 0-200 ,writemenu to single pos ,
	if ((modEnable) || (firstRound))
		tempValueB[counterVarB] = tempValue[counterVarB]; // needs replicate values after moving menupot to avoidwriting new values needs to move twice not good
	valueCurrent = tempValue[counterVarB]; // incoming value  10*8 blocks
	valueOriginal = tempValueB[counterVarB]; // read original stored value    might just make  all separate regardless
	if (valueCurrent != valueOriginal) {
		tempValueB[counterVarB] = valueCurrent;
		//  potValues[counterVarB] = 10 - (valueCurrent);  // potvalues 0- 63 and 180 up
	}    //perfect now   , change to 0-9 here
	potValues[counterVarB]=potValues[counterVarB]+
	firstRound = 0;
} // loop for reading analog inputs
*/



// if (counterVarB==n_lcd)lcd_blink = !lcd_blink;


/*
	if (n_lcd > 18) {
		row_toggle = !row_toggle;
		n_lcd = 1;
	}
	lcddata = spell[(n_lcd) + (row_toggle * 18)]; // when n=1 changes
	if (command_toggle == 4) {
		command_toggle = 0;
		lcd_blink = !lcd_blink;
	} // top row seems to be slower from potValues 8
	//if (n == 31)   lcddata = spell[(n >> 1)+ (row_toggle*17)] ; // needs to be here or it loops wrong, maybe not
	if (n_lcd > 16) {
		rs_toggle = 0;
		lcddata = command[command_toggle]; // 0-3 command set
		command_toggle++;
	} else
		rs_toggle = 1;  // sending commands after printing
*/


/*
	sine_counter++;
	sine_counter = sine_counter & 511;
		switch (sine_counter >> 7) {
		case 0:
			sine_out = 512 - sine_block[127 - sine_counter];
			break;     	//0-127
		case 1:
			sine_out = 512 - sine_block[sine_counter - 127];
			break;		// 128-255
		case 2:
			sine_out = sine_block[383 - sine_counter];
			break;			//256-383
		case 3:
			sine_out = sine_block[sine_counter - 384];
			break;    //384-511
		default:
			break;
 // future goal = interpolate instead of add
if ((note_holdA) && (adsr[1] ==128)) adsr[1]=1;  // dont use note for trigger ,works
if (adsr[1]==1) {
	adsr_time[1]=(isrCount+17) &1023; note_channel[5]=127; adsr[1]=2;} // set counter , clear trigger, works , 16 per note
if ((isrCount>=adsr_time[1]) && (adsr[1]==2))	 {adsr[1]=128;note_channel[5]=0; } // simple start finish, wwatch isrcount ,works
if (note_channel[0] && (adsr[0] ==128)) adsr[0]=1;  // dont use note for trigger ,works
if (adsr[0]==1) {
	adsr_time[0]=(isrCount+17) &1023; note_channel[4]=127; adsr[0]=2;} // set counter , clear trigger, works , 16 per note
if ((isrCount>=adsr_time[0]) && (adsr[0]==2))	 {adsr[0]=128;note_channel[4]=0; } // simple start finish, wwatch isrcount ,works
*/



/*			HAL_GPIO_WritePin(GPIOB, D4_Pin, ((lcddata) & 1));
HAL_GPIO_WritePin(GPIOB, D5_Pin, ((lcddata >> 1) & 1));
HAL_GPIO_WritePin(GPIOB, D6_Pin, ((lcddata >> 2) & 1));
HAL_GPIO_WritePin(GPIOB, D7_Pin, ((lcddata >> 3) & 1)); // true or 0 return , value not important , works
*/
/*
	 HAL_GPIO_WritePin(GPIOB, D4_Pin, ((lcddata >> 4) & 1));
		HAL_GPIO_WritePin(GPIOB, D5_Pin, ((lcddata >> 5) & 1));
			HAL_GPIO_WritePin(GPIOB, D6_Pin, ((lcddata >> 6) & 1));
			HAL_GPIO_WritePin(GPIOB, D7_Pin, ((lcddata >> 7) & 1)); // true or 0 return , value not important , works
tim example
 if (GPIOE->IDR & 0x01) TIM2->CNT = 0; // reset counter
 if (GPIOE->IDR & 0x02) TIM2->CR1 |= 0x01; // enable counter
 if (GPIOE->IDR & 0x04) TIM2->CR1 &= ~0x01; // disable counter
*/


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
