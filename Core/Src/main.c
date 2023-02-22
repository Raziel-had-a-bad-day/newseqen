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

/* USER CODE BEGIN PV */
#include "maincode.h"			// void
#include "sampling.h"     // audio process
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
  MX_SPI1_Init();
  /* USER CODE BEGIN 2 */

  const volatile uint8_t *userConfig=(const volatile uint8_t *)0x0800D2F0;





//LL_SPI_Enable(SPI2);

  HAL_SPI_Init(&hspi2); // write to register hspi2
  HAL_SPI_Init(&hspi1); // write to register hspi2
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
HAL_ADC_Start_DMA(&hadc1, adc_source, 1024); //dma start ,needs this and adc start ,set sampling time to very long or it will fail
//HAL_DMA_Init(&hdma_spi2_tx);

HAL_I2C_MspInit(&hi2c2);
uint8_t send_spi1[5]={0x90,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,};
/*
HAL_SPI_Transmit(&hspi1, send_spi1, 4, 1000); // send dummy,dummy , then whatever command for manuf
HAL_SPI_Receive(&hspi1, return_spi1, 2, 1000);   // manuf return sif correct , 0xEF,0x17 which is correct, then repeats when more request

*/







HAL_Delay(5);


send_spi1[0]=0x06; //enable write  , only lasts for single operation
HAL_GPIO_WritePin(CS1_GPIO_Port, CS1_Pin, 0);
HAL_SPI_Transmit(&hspi1, send_spi1, 1, 1000);
HAL_GPIO_WritePin(CS1_GPIO_Port, CS1_Pin, 1);
HAL_Delay(5);
send_spi1[0]=0x20; //sector erase
send_spi1[1]=0; //24bit address msb
send_spi1[2]=0; //24bit address
send_spi1[3]=1; //24bit address lsb
HAL_GPIO_WritePin(CS1_GPIO_Port, CS1_Pin, 0);         // enable for sector erase   , stays empty when enabled
HAL_SPI_Transmit(&hspi1, send_spi1, 4, 1000);   //erase sector ,works
HAL_GPIO_WritePin(CS1_GPIO_Port, CS1_Pin, 1);


send_spi1[0]=0x05; //read status register  if writing
send_spi1[1]=0; //24bit address msb
status_reg[1]=1; // set busy on

while (status_reg[1]&1){								// check if write busy
HAL_GPIO_WritePin(CS1_GPIO_Port, CS1_Pin, 0);
	HAL_SPI_TransmitReceive(&hspi1, send_spi1, status_reg,2, 200);
	HAL_GPIO_WritePin(CS1_GPIO_Port, CS1_Pin, 1);
}

send_spi1[0]=0x06; //enable write again
HAL_GPIO_WritePin(CS1_GPIO_Port, CS1_Pin, 0);
HAL_SPI_Transmit(&hspi1, send_spi1, 1, 1000);
HAL_GPIO_WritePin(CS1_GPIO_Port, CS1_Pin, 1);
HAL_Delay(5);




//uint8_t temp_spi1[]={0x02,0,0,1,"H","E","L","L","O"," ","W","O","R","L","D",250,0,0} ; //page progrram ,24bit(address)  +1-255 byte data  (page)
uint8_t temp_spi1[]={0x02,0,0,1,128,129,130,131,132,133,134,135,136,137,138,250,0,0} ; //page progrram ,24bit(address)  +1-255 byte data  (page)
memcpy  (send_spi1,temp_spi1, 14);   // copy new array over old

HAL_GPIO_WritePin(CS1_GPIO_Port, CS1_Pin, 0);
HAL_SPI_Transmit(&hspi1, send_spi1, 14, 1000);  //address,page program

HAL_GPIO_WritePin(CS1_GPIO_Port, CS1_Pin, 1);




HAL_Delay(25);
send_spi1[0]=0x04; //disable write
HAL_GPIO_WritePin(CS1_GPIO_Port, CS1_Pin, 0);
HAL_SPI_Transmit(&hspi1, send_spi1, 1, 1000);
HAL_GPIO_WritePin(CS1_GPIO_Port, CS1_Pin, 1);

HAL_Delay(5);
memcpy  (send_spi1,return_spi1, 14);   // clear out
send_spi1[0]=0x03; //read page 1
send_spi1[1]=0; //24bit address msb
send_spi1[2]=0; //24bit address
send_spi1[3]=1; //24bit address lsb

HAL_GPIO_WritePin(CS1_GPIO_Port, CS1_Pin, 0);  // when readin low till the end

HAL_SPI_Transmit (&hspi1, send_spi1, 4, 100);
HAL_SPI_Receive (&hspi1, return_spi1, 10, 100);   // works fine

//HAL_SPI_TransmitReceive(&hspi1, send_spi1, return_spi1,14, 100);  // better in case skip , 4 bytes is null then data , slow
//HAL_Delay(5);

//HAL_SPI_Receive(&hspi1, return_spi1, 12, 1000);  // reverse msb ?
HAL_GPIO_WritePin(CS1_GPIO_Port, CS1_Pin, 1);



HAL_SPI_Transmit(&hspi1, send_spi1, 1, 1000);


uint8_t potSource2[120];    // { [0 ... 112] = 64 };

	for(i=0;i<6;i++){     // 256
	HAL_I2C_Mem_Read(&hi2c2, 160, 64+(i*64), 2,&potSource2, 64,1000);		// all good readin eeprom  values

	memcpy (potSource+(i*64),potSource2,sizeof(potSource2));   //this works  ok now ,leave it alone


	}
	for(i=0;i<260;i++){			// write potvalues ,for display ,also filter bad data IMPORTANT !!!

		if (potSource[i]>159) potSource[i]=159;
		potValues[i]=potSource[i]>>4;

	}

	uint16_t mem_counter=0;
	memcpy(&seq,potSource,46 );  // load from potSource  ,, causes problems with memory ,NEEDS TO BE CONTINUOS OR  WILL  GET CORRUPT
    memcpy(&note,potSource+156,112 );   // this works but keep checking for fragmentation

    for(mem_counter=0;mem_counter<10;mem_counter++){

		memcpy(&LFO[mem_counter],potSource+46+(mem_counter*6),6 );  // + 60 ,ok here

		memcpy(&ADSR[mem_counter],potSource+106+(mem_counter*5),5 );  // +50  ,

	}

	for(i=0;i<64;i++){       //   fill with characters also add lcd command ,ok

	for 	(n=0;n<18;n++){					// this is ok
		if (n==0) gfx_ram[(i*18)+n] = 128+(i&31);   // half page
		if (n==1) gfx_ram[(i*18)+n] = 128+((i>>5)*8);    // change x to 8
		if (n>1)  gfx_ram[(i*18)+n] = 255;

	}

	}
seq.pos=0;


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

	gfx_clear();
uint16_t pars_counter;

for (pars_counter=0;pars_counter<512;pars_counter++)	{   // fill up display data , needs to run a lot more though or wont finish string_search

		menu_parser();  // run it closer to default_menu size ,times, if default_menu is corrupt gfx breaks pretty bad
		default_menu3[pars_counter>>1]=64;
	}
	default_menu3_size = strlen(default_menu3);  // grab menu size , this is needed
	menu_title_count--;  //count back one
	display_clear ();

uint16_t lut_temp2=0;
uint16_t lut_temp3=0;


menuSelect=0;
// fill up sample
firstbarLoop=0;



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
	  if (mem_count>260) mem_count=0; else mem_count++; // write to first this was moved for no logical reason ?
	  lfo_target_parse(); //
	// read values from stored

	memcpy(potSource,&seq,46); // about 35

	for(i=0;i<10;i++){
		if (i<8){    memcpy(potSource+156+(i*14),&note[i],14 );}  //grab note settings ,112 total , works

		memcpy(potSource+46+(i*6),&LFO[i],6 );  // + 60  ,ok
		memcpy(potSource+106+(i*5),&ADSR[i],5 );  // +50  ,

	}	// copy vars into potSource



		uint16_t mem_count2=0;
	//	mem_buf=0;
			// mem_verify=0;


				 mem_buf=potSource[mem_count];
				 if (mem_buf>159) mem_buf=159;
				 mem_count2=((1+(mem_count>>6))<<6)+(mem_count&63);
				 HAL_I2C_Mem_Read(&hi2c2, 160,mem_count2, 2,&mem_verify, 1,100);
				 if (mem_verify!=mem_buf) HAL_I2C_Mem_Write(&hi2c2, 160,mem_count2 , 2, &mem_buf, 1, 100);







	 // "&hi2c2"  actual register address  , write only when needed



//if (mem_buf!=mem_verify)	 mem_errors++;  // check writes

loop_counter2=0; //reset

}


	  if (disp_end==1)	  display_generate();      // run this after gfx draw page finish




	  if (init<6)				// after 6 its done for good   // no freeze here
{
	  for (i=0;i<6;i++) {display_init();}  //1-2ms ?  change length if flickering ,maybe initial data
}

	  if (init > 5) {    //  around 3 cycles per single transmit  , plenty quick as is , spi lcd can really slow things down


		  if (gfx_send_swap==2) gfx_send_lines++;		// this is ok for now
		  if (gfx_send_swap==1)  { gfx_send_counter=gfx_send_cursor*144; gfx_send_swap=2; } // jump to cursor pixel line
	      if (gfx_send_lines==144)   { gfx_send_lines=0; gfx_send_counter=1008; gfx_send_swap=0;}  // skip to last char line
				gfx_send();    // don't loop this without using dma  , just makes things really slow

	}


	  if (loop_counter == 255)	{ // grab adc readings + 3ms , 32 step  // no freeze



		  //HAL_ADC_Stop_DMA(&hadc1); // a lot more stable this way , also sampling time no more than /8 +  144 or no go

		  HAL_ADCEx_InjectedStart(&hadc1) ;  // start injected mode normal conversion
		  uint16_t adc_temp1[4]={0,0,0,0};

		  HAL_ADC_PollForConversion(&hadc1,1);  // works but  slow ,blocking very slow, set quick time out

		  adc_temp1[0]=HAL_ADCEx_InjectedGetValue(&hadc1, ADC_INJECTED_RANK_1);
		  adc_temp1[1] =HAL_ADCEx_InjectedGetValue(&hadc1, ADC_INJECTED_RANK_2);
		//  adc_temp1[2] =HAL_ADCEx_InjectedGetValue(&hadc1, ADC_INJECTED_RANK_3);
		  adc_values[0]=	  adc_temp1[0]>>7;
		  adc_values[1]=	  adc_temp1[1]>>7;
		//  adc_values[2]=	  adc_temp1[2]>>7;
		  HAL_ADCEx_InjectedStop(&hadc1) ;


		  //HAL_ADC_Start_DMA(&hadc1, adc_source, 512);


	  	loop_counter=0;
	  }

	  if ((seq.pos==7) && (lcd_send==0)) {lcd_send=1;} // runs just once
	  /*
	   if (promValue<64) promValue=promValue+1 ; else promValue=0;  // fetch eeprom   nogo
	  	  if ((promValues[promValue] ) !=(potValues[promValue]))  EEPROM.write(promValue,(potValues[promValue]));   //  not too happy can totally kill speed  will have to put elsewhere
	  	  promValues[promValue] =potValues[promValue];
	  	   */

	  	     HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin,(seq.pos & 1)); // easy skip ?
	  	    // very inconsistent

	 // if (sample_point>511) sample_pointD=0; // loop when zero cross , good but limited, works ok
	//  if (sample_point<511)  sample_pointD=512;


	//  if (sample_pointB!=sample_pointD) bank_write=1; // set start of buffer ,grab , works ok

	  	//	adc_flag=0;
	  		if (adc_flag) {
	  		HAL_ADC_Stop_DMA(&hadc1); // a lot more stable this way , also sampling time no more than /8 +  144 or no go
	  		HAL_ADC_Start_DMA(&hadc1, adc_source, 1024); //dma start ,needs this and adc start ,set sampling time

	  			uint16_t* click=&adc_source[0];

	  			for (i=0;i<512;i++)
	  			{

	  				uint16_t crap_hold=*click;

	  			uint16_t crap_hold1=*(++click);
	  		click++;

	  	//			uint16_t crap_hold=adc_source[i*2];

	  				 // 				uint16_t crap_hold1=adc_source[(i*2)+1];

	  				input_holder[i] = (crap_hold+crap_hold1 )>>1;
	  				adc_flag=0;
	  			}
	  		}

	while  (bank_write)                         {							// wait for adc , priority



		  //HAL_Delay(4);
	  		//sample_point=sample_point&768 ;
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
  sConfig.SamplingTime = ADC_SAMPLETIME_144CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configures for the selected ADC injected channel its corresponding rank in the sequencer and its sample time
  */
  sConfigInjected.InjectedChannel = ADC_CHANNEL_0;
  sConfigInjected.InjectedRank = 1;
  sConfigInjected.InjectedNbrOfConversion = 2;
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

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(CS1_GPIO_Port, CS1_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin : LED_Pin */
  GPIO_InitStruct.Pin = LED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LED_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : CS1_Pin */
  GPIO_InitStruct.Pin = CS1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(CS1_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
/* USER CODE BEGIN 4 */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)   // get data after conversion
{
adc_flag=1;


}
void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef * hspi)   // when finished sending
	{
	    spi2_send_enable=1;
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
