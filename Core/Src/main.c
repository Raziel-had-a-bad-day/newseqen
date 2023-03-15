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
#include "luts.h"    // big tables
#include <stdlib.h>
#include "string.h"
#include "arm_math.h"

//#define __FPU_PRESENT   1

/* Use ARM MATH for Cortex-M4 */
//#define "arm_math.h"    cmsis
//#include "math.h"   standard
//#include <string.h>
//#include "stdio.h"
//#include <string.h>       for some errors with strings
//#include <stdlib.h>
//#include "myvars.h"

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

SPI_HandleTypeDef hspi1;
SPI_HandleTypeDef hspi2;

TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim4;

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart6;

/* USER CODE BEGIN PV */
#include "maincode.h"			// void
#include "sampling.h"     // audio process
#include "display.h"
// 32khz froom now on


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
static void MX_SPI1_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_USART6_UART_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void main_initial(void);
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
  MX_SPI1_Init();
  MX_USART1_UART_Init();
  MX_USART6_UART_Init();
  /* USER CODE BEGIN 2 */

main_initial();   // initial setup

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)																																																		//   while loop , random 20+ms freeze around 0.5 sec
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  loop_counter++ ;
	  loop_counter2++;// this pretty slow now thanks to gfx , no skips though

	  // if (menu_page<320) lcd_feedback();  //curious no issues with lcd without this  , maybe spell writing

			  if 	((loop_counter2&7)==6)      {analoginputloopb();} // this is ok , plenty quick , no freeze here


		if (loop_counter2==4024) {    //   4096=1min=32bytes so 4mins per 128 bank or 15 writes/hour , no freeze here
			  if (mem_count>512) mem_count=0; else mem_count++; // write to first this was moved for no logical reason ?

			  cursor_partial=240;
			if (last_pos_hold)   gfx_reverse(last_pos_hold,cursor_partial);
			  gfx_reverse(seq.loop[0]+32,cursor_partial);
			  last_pos_hold=seq.loop[0]+32;

			  patch_target_parse(); //
			  uint16_t mem_count2=0;	// read values from stored


			memcpy(potSource,&seq,46); // about 35

			for(i=0;i<10;i++){
					memcpy(potSource+156+(i*14),&note[i],14 );  //grab note settings ,112 total , works

				memcpy(potSource+46+(i*6),&LFO[i],6 );  // + 60  ,ok
				memcpy(potSource+106+(i*5),&ADSR[i],5 );  // +50  ,
				memcpy(potSource+316+(i*6),&patch[i],6 );
				memcpy(potSource+376+(i*6),&LFO_slave1[i],6 ); // ext llof settings

			}	// copy vars into potSource

			//HAL_UART_Transmit(&huart1,serial_send,4, 100);  //send serial again

			mem_count2=0;
			//	mem_buf=0;
					// mem_verify=0;

					// for patch write start at 2048 for now

						 mem_buf=potSource[mem_count];
					//	 if (mem_buf>159) mem_buf=159;
						 mem_count2=((1+(mem_count>>6))<<6)+(mem_count&63);
						 //mem_count2=mem_count2+2048;   // Relocate mem for patch

						 HAL_I2C_Mem_Read(&hi2c2, 160,mem_count2, 2,&mem_verify, 1,100);
						 if (mem_verify!=mem_buf) HAL_I2C_Mem_Write(&hi2c2, 160,mem_count2 , 2, &mem_buf, 1, 100);

			 // "&hi2c2"  actual register address  , write only when needed



		//if (mem_buf!=mem_verify)	 mem_errors++;  // check writes

		loop_counter2=0; //reset

		}


	if (serial_tosend){    // send on seq change
		uint16_t mem_counter=0;
		  memcpy(serial_source,&seq,36); // copy bits

		  memcpy(serial_source+96,potSource+252,64 );  // 4 * 16
		  for(mem_counter=0;mem_counter<10;mem_counter++){
			  if (mem_counter<4)memcpy(serial_source+96+(mem_counter*7),&note[mem_counter+6],7 );
						memcpy(serial_source+36+(mem_counter*6),&LFO_slave1[mem_counter],6 );  // insert lfo settings
				  }

		  while ((serial_source_temp[serial_up]==serial_source[serial_up]) &&(serial_up<161) )   {
			  serial_up++;
		  }

		  serial_send[6]=0;
		  serial_send[7]=seq.pos;  // chase
		  serial_send[2]=serial_up;
		  serial_send[3]=serial_source[serial_up];
		  serial_source_temp[serial_up]=serial_source[serial_up];
		  if (serial_up>161) serial_up=0; else serial_up++;

		  HAL_UART_Transmit(&huart1,serial_send,8, 100);  //send serial
		  serial_tosend=0;
	}

		if (disp_end==1)	  {display_generate();}      // run this after gfx draw page finish

		if (init<6)				// after 6 its done for good   // no freeze here
		{
			for (i=0;i<6;i++) {display_init();}  //1-2ms ?  change length if flickering ,maybe initial data
		}

		if (init > 5) {    //  around 3 cycles per single transmit  , plenty quick as is , spi lcd can really slow things down

/*                       // leave this out for , works fine but needs a better solution

			if (gfx_send_swap==2) gfx_send_lines++;		// this is ok for now
			if (gfx_send_swap==1)  { gfx_send_counter=gfx_send_cursor*144; gfx_send_swap=2; } // jump to cursor pixel line
			if (gfx_send_lines==144)   { gfx_send_lines=0; gfx_send_counter=1008; gfx_send_swap=0;}  // skip to last char line
*/





		    //   fills one char line on current page
		    if(gfx_send_counter==0) gfx_send_counter4=0;
		    if (!gfx_send_counter2){

		    if ((gfx_send_counter4&7)==0)  {gfx_line_fill((gfx_send_counter4>>3)+((menu_title_lut[enc_out1]>>7)*8));  }  // fills current page
		    gfx_send_counter4++;
		    if (gfx_send_counter4>63) gfx_send_counter4=0;
		    }

		    gfx_send();    // don't loop this without using dma  , just makes things really slow
			 menu3_fill();encoder2();
		}


		if (loop_counter2==2024){ // grab adc readings + 3ms , 32 step  // no freeze



			//HAL_ADC_Stop_DMA(&hadc1); // a lot more stable this way , also sampling time no more than /8 +  144 or no go

			HAL_ADCEx_InjectedStart(&hadc1) ;  // start injected mode normal conversion
			uint16_t adc_temp1[4]={0,0,0,0};

			HAL_ADC_PollForConversion(&hadc1,1);  // works but  slow ,blocking very slow, set quick time out

			adc_temp1[0]=HAL_ADCEx_InjectedGetValue(&hadc1, ADC_INJECTED_RANK_1);
			adc_temp1[1] =HAL_ADCEx_InjectedGetValue(&hadc1, ADC_INJECTED_RANK_2);
			adc_temp1[2] =HAL_ADCEx_InjectedGetValue(&hadc1, ADC_INJECTED_RANK_3);
			HAL_ADCEx_InjectedStop(&hadc1) ;
			//  adc_temp1[2] =HAL_ADCEx_InjectedGetValue(&hadc1, ADC_INJECTED_RANK_3);
			adc_values[0]=	31- ( adc_temp1[0]>>7);
			adc_values[1]=	 31-( adc_temp1[1]>>7);
			adc_values[2]=	 31-( adc_temp1[2]>>7);
			//  adc_values[2]=	  adc_temp1[2]>>7;



			//HAL_ADC_Start_DMA(&hadc1, adc_source, 512);


			loop_counter=0;
		}

		if ((seq.pos==7) && (lcd_send==0)) {lcd_send=1;} // runs just once


		HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin,(seq.pos & 1)); // easy skip ?


		//	adc_flag=0;
		if (adc_flag) {     //  only for sending out  , poor quality


			uint16_t    adc_page=0; ;
			if		(adc_flag==1)	  	{	adc_page=0;			}	//dma start ,needs this and adc start ,set sampling time
			if		(adc_flag==2)	  	{	adc_page=1536;  }


			uint16_t    adc_convert_count;
			uint16_t    adc_convert_temp;
			for (i=0;i<512;i++){
				adc_convert_count=(i*3)+adc_page;

				adc_convert_temp=adc_source[adc_convert_count]+adc_source[adc_convert_count+1]+adc_source[adc_convert_count+2];
				input_holder[i]=adc_convert_temp/3;
				//	input_holder[i]=adc_source[(i*3)+adc_page];

			}
			if (sampler.record_enable)  {sampler_ram_record(); sampler.start_MSB=0; sampler.start_LSB=0;sampler.end_MSB=63;}

			adc_flag=0;
		}


		if ((sample_point>512)&&( sample_pointD==512)) bank_write=1;
		while  (bank_write)                         {							// wait for adc , priority

			sampling();

	  	}   // should trigger this after adc reads also reset sample_point here


	  	/*	HAL_SPI_Transmit(&hspi2,(uint8_t *)248,1,1);
	  	HAL_SPI_Transmit(&hspi2,(uint8_t *)64,1,1);
	  	HAL_SPI_Transmit(&hspi2,(uint8_t *)0,1,1);
	  */

	   //works ok, write opp bank



	    }  // while loop , total 250/350  cycles/   5/7ms   , max allowed is 13ms


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
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
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
  sConfig.SamplingTime = ADC_SAMPLETIME_112CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configures for the selected ADC injected channel its corresponding rank in the sequencer and its sample time
  */
  sConfigInjected.InjectedChannel = ADC_CHANNEL_0;
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
  sConfigInjected.InjectedChannel = ADC_CHANNEL_2;
  sConfigInjected.InjectedRank = 2;
  if (HAL_ADCEx_InjectedConfigChannel(&hadc1, &sConfigInjected) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configures for the selected ADC injected channel its corresponding rank in the sequencer and its sample time
  */
  sConfigInjected.InjectedChannel = ADC_CHANNEL_8;
  sConfigInjected.InjectedRank = 3;
  sConfigInjected.InjectedSamplingTime = ADC_SAMPLETIME_3CYCLES;
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
  hi2c2.Init.ClockSpeed = 100000;
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
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

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
  htim3.Init.Period = 3124;
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
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
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
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_HalfDuplex_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief USART6 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART6_UART_Init(void)
{

  /* USER CODE BEGIN USART6_Init 0 */

  /* USER CODE END USART6_Init 0 */

  /* USER CODE BEGIN USART6_Init 1 */

  /* USER CODE END USART6_Init 1 */
  huart6.Instance = USART6;
  huart6.Init.BaudRate = 115200;
  huart6.Init.WordLength = UART_WORDLENGTH_8B;
  huart6.Init.StopBits = UART_STOPBITS_1;
  huart6.Init.Parity = UART_PARITY_NONE;
  huart6.Init.Mode = UART_MODE_TX_RX;
  huart6.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart6.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_HalfDuplex_Init(&huart6) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART6_Init 2 */

  /* USER CODE END USART6_Init 2 */

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

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(CS1_GPIO_Port, CS1_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin : LED_Pin */
  GPIO_InitStruct.Pin = LED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LED_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : record_pin_Pin */
  GPIO_InitStruct.Pin = record_pin_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(record_pin_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : CS1_Pin */
  GPIO_InitStruct.Pin = CS1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(CS1_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

}

/* USER CODE BEGIN 4 */
/* USER CODE BEGIN 4 */


	void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef* hadc){
		adc_flag=1;

	}
	void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)   // get data after conversion
	{
	adc_flag=2;

	HAL_ADC_Stop_DMA(&hadc1); HAL_ADC_Start_DMA(&hadc1,& adc_source, 3072);

	}


void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef * hspi)   // when finished sending
	{
	    spi2_send_enable=1;
	}
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if(GPIO_Pin == record_pin_Pin) {
   sampler.record_enable=1;
  }
}



// if (counterVarB==n_lcd)lcd_blink = !lcd_blink;





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
