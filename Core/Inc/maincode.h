uint8_t*  menu_vars(char* menu_string,  uint8_t var_index   ){ // in comes name and index , out goes variable reference pointer


	char menu_string2[8];
	//var_index=var_index&15;    //set var index ie  LFO[1].rate ,false
	uint8_t menu_countr=0; //  menu vars
	uint8_t *menu_vars_var1=NULL;

	for (i = 0; i <menu_lookup_count; i++) {      // find menu location

		memcpy(menu_string2, menu_titles_final[i], 8);  // copy title list
		if ((strncmp(menu_string, menu_string2, 8)) == 0) {
			menu_countr = i;

		}
	}
	menu_vars_ref=menu_countr ;
	if (var_index>menu_vars_index_limit[menu_countr]) var_index=menu_vars_index_limit[menu_countr];   // make sure it stays right

	switch(menu_countr){
	case 0:     menu_vars_var1= NULL; break;
	case 1:     menu_vars_var1= &LFO[var_index].rate   ; break;
	case 2:     menu_vars_var1= &LFO[var_index].depth    ; break;
	case 3:     menu_vars_var1= &LFO[var_index].delay    ; break;
	case 4:     menu_vars_var1= &LFO[var_index].offset    ; break;
	case 5:     menu_vars_var1= &patch[var_index].target    ; break;  // velocity etc
	case 6:     menu_vars_var1= NULL   ; break;
	case 7:     menu_vars_var1= &ADSR[var_index].attack    ; break;
	case 8:     menu_vars_var1= &ADSR[var_index].decay    ; break;
	case 9:     menu_vars_var1= &ADSR[var_index].sustain    ; break;
	case 10:     menu_vars_var1= &ADSR[var_index].release    ; break;
	case 11:     menu_vars_var1= NULL  ; break;
	case 12:     menu_vars_var1= &note[var_index].osc    ; break;
	case 13:     menu_vars_var1= &note[var_index].osc2    ; break;
	case 14:     menu_vars_var1= &note[var_index].pitch    ; break;
	case 15:     menu_vars_var1= &note[var_index].duration    ; break;
	case 16:     menu_vars_var1= &note[var_index].position    ; break;
	case 17:     menu_vars_var1= &note[var_index].transpose    ; break;
	case 18:     menu_vars_var1= &note[var_index].timeshift    ; break;
	case 19:     menu_vars_var1= &note[var_index].velocity   ; break; // 16 bit
	case 20:     menu_vars_var1= &note[var_index].detune    ; break;
	case 21:     menu_vars_var1= NULL   ; break;
	case 22:     menu_vars_var1= &seq.pos    ; break;
	case 23:     menu_vars_var1= &seq.tempo    ; break;
	case 24:     menu_vars_var1= &seq.notes1[var_index]   ; break;
	case 25:     menu_vars_var1= &seq.notes2[var_index]   ; break;
	case 26:     menu_vars_var1= &seq.loop [var_index]    ; break;
	case 27: 	menu_vars_var1=&filter[var_index].cutoff_1 ;break;
	case 28: 	menu_vars_var1=&filter[var_index].cutoff_2 ;break;  // fine tune
	case 29: 	menu_vars_var1=&filter[var_index].resonance ;break;
	case 30: 	menu_vars_var1=&filter[var_index].q_level  ;break;
	case 31: 	menu_vars_var1=&filter[var_index].level;break;
	case 32: 	menu_vars_var1=&filter[var_index].feedback ;break;
	case 33: 	menu_vars_var1=&filter[var_index].out_mix ;break;
	case 34: 	menu_vars_var1=&filter[var_index].poles ;break;
	case 35:     menu_vars_var1= &patch[var_index].target_index    ; break;     // select note
	case 36: 	menu_vars_var1=&patch[var_index].input1 ;break;
	case 37: 	menu_vars_var1=&patch[var_index].input2 ;break;
	case 38: 	menu_vars_var1=&patch[var_index].in_mix ;break;
	case 39: 	menu_vars_var1=&patch[var_index].in_offset ;break;
	case 40:     menu_vars_var1= &LFO_slave1[var_index].rate   ; break;
	case 41:     menu_vars_var1= &LFO_slave1[var_index].depth    ; break;
	case 42:     menu_vars_var1= &LFO_slave1[var_index].delay    ; break;
	case 43:     menu_vars_var1= &LFO_slave1[var_index].offset    ; break;
	case 44: 	menu_vars_var1=&sampler.offset2[var_index] ;break;
	case 45: 	menu_vars_var1=&sampler.RAM_offset ;break;
	case 46: 	menu_vars_var1=&sampler.end_MSB ;break;
	case 47: 	menu_vars_var1=&sampler.end_LSB ;break;
	case 48: 	menu_vars_var1=&sampler.offset[var_index] ;break;
	case 49:     menu_vars_var1= &LFO_square[var_index].rate   ; break;
	case 50:     menu_vars_var1= &LFO_square[var_index].depth    ; break;
	case 51:     menu_vars_var1= &LFO_square[var_index].offset    ; break;
	case 52:     menu_vars_var1= &LFO_square[var_index].delay    ; break;
	case 53:     menu_vars_var1= &LCD_Info[var_index]    ; break; // send as char
	case 54: 	menu_vars_var1=&sampler.trigger_1 ;break;
	case 55: 	menu_vars_var1=&sampler.trigger_2 ;break;
	case 56: 	menu_vars_var1=&sampler.trigger_3 ;break;
	case 57: 	menu_vars_var1=&sampler.trigger_4 ;break;
	case 58: 	menu_vars_var1=&sampler.repeat  ;break;
	case 59: 	menu_vars_var1=&sampler.sample_select[var_index]  ;break;
	case 60: 	menu_vars_var1=&sampler.sample_save  ;break;
	case 61: 	menu_vars_var1=&sampler.sample_save_enable  ;break;
	case 62:     menu_vars_var1= &note[var_index].slide_length    ; break;
	case 63: 	menu_vars_var1=&sampler.Snotes1[var_index]  ;break;
	case 64: 	menu_vars_var1=&sampler.Snotes2[var_index]  ;break;
	case 65:     menu_vars_var1= &delay[var_index].time    ; break;
	case 66:     menu_vars_var1= &delay[var_index].mix    ; break;
	case 67:     menu_vars_var1= &delay[var_index].feedback    ; break;
	case 68:     menu_vars_var1= &delay[var_index].mod_speed   ; break;
	case 69:     menu_vars_var1= &delay[var_index].mod_depth   ; break;

	default :		menu_vars_var1= NULL   ; break;

	}
    // copy back address  ,ok
	// return the result number for later reference
	//menu_vars_var= menu_vars_var1;
	return menu_vars_var1;
}






void menu_parser(void){          // parse out menus , shouldn't have to run (in theory) once filled ,only for feedback pointer maybe

	    char menu_string[8]="xxxxxxxx";   // incoming string holder (single)
	    char menu_string2[8]="hhhhhhhh";
	    uint8_t string_counter=0;
    	    uint16_t menu_searchsize=sizeof(default_menu)-8;   // this should fairly big always , leave gap at the end , atm 480

	    if (string_search>menu_searchsize) {                    //this is ok
		    return;    }    // check if bigger than search area
	    //if (menu_counter>240)  return;
	    memcpy(menu_string,default_menu+string_search,8);    //copy 8 strings created menu array
	    ////////////////////////////
	    for (string_counter=0;string_counter<menu_lookup_count;string_counter++){    	// test a single menu entry  , for now only the first record

		    memcpy(menu_string2,menu_titles_final[string_counter],8);
		    if  ((strncmp(menu_string,menu_string2,8))==0) 								// compare and if true pass var,seq
		    {

			    if ((menu_counter>111 )&&(menu_counter<128 )) menu_counter=menu_counter+16;   // skip to second page
			    if((menu_counter>239)&&(menu_counter<256 )) menu_counter=menu_counter+16; // skip
			    if((menu_counter>367)&&(menu_counter<384 )) menu_counter=menu_counter+16; // skip
			    if((menu_counter>495)&&(menu_counter<512 )) menu_counter=menu_counter+16; // skip
			    if((menu_counter>623)&&(menu_counter<640 )) menu_counter=menu_counter+16; // skip

			    menu_title_lut[menu_title_count]=  (string_counter <<16)+(menu_counter&1023);   // search result  and disp lcd position counter
    			    memcpy(menu_index_list+(menu_title_count*2),default_menu+string_search-2,2); // get array  index under ,LFO[1]  etc ,ok
        			    menu_title_count++;
			    menu_counter++;
			    space_check=0;
			    string_search=string_search+8;     // advance search position
    			    return;}
    	    }
        if (space_check>1)  menu_counter++;   //this is ok
	    space_check++;  // count empty spaces or fill characters
	    string_search++;
	    return;

    }

void patch_target_parse(void){    // records ptr for target options , works ok
uint8_t skip=0;
		uint8_t counter;

for (counter=0;counter<20;counter++){

			uint16_t* output_hold;
			uint16_t* output_hold2;


			uint8_t input_hold=patch[counter].input1;
			uint8_t input_hold2=patch[counter].input2;
			if (patch[counter].input1>=(menu_lookup_count))    patch[counter].input1=0;    // limit

			switch(input_hold&3){     // lfo now , can add adsr later

						case 0:   output_hold=&LFO[input_hold>>2].out[0];break;
						case 1:   output_hold=&LFO[input_hold>>2].out_saw[0];break;
						case 2:   output_hold=&LFO[input_hold>>2].out_tri[0];break;
						//case 3:   output_hold=&LFO[input_hold>>2].out_tri[0];break;
						case 3:   output_hold=&LFO_square[input_hold>>2].out[0];break;
			}
			if (patch[counter].input2>=(menu_lookup_count))    patch[counter].input2=0;    // limit

			switch(input_hold2&3){     // lfo now , can add adsr later

			case 0:   output_hold2=&LFO[input_hold2>>2].out[0];break;
			case 1:   output_hold2=&LFO[input_hold2>>2].out_saw[0];break;
			case 2:   output_hold2=&LFO[input_hold2>>2].out_tri[0];break;
			//case 3:   output_hold=&LFO[input_hold>>2].out_tri[0];break;
			case 3:   output_hold2=&LFO_square[input_hold2>>2].out[0];break;
			}

			if (patch[counter].input1==41) output_hold=&adc_values[1];   // same format as lfo
			if (patch[counter].input1==42) output_hold=&adc_values[2]; // do this for other single values
			if (patch[counter].input1==43) output_hold=&adc_values[0];
			if (patch[counter].input2==41) output_hold2=&adc_values[1];   // waiting on 8 values
			if (patch[counter].input2==42) output_hold2=&adc_values[2]; // do this for other single values
			if (patch[counter].input2==43) output_hold2=&adc_values[0];



			patch[counter].in1_ptr=output_hold;   // sets input pointer to first sample , default is lfo[0].out [0]
			patch[counter].in2_ptr=output_hold2;


			if (patch[counter].target) {  // test if above zero
				uint8_t target_input=patch[counter].target; // copy to avoid messed up pointer
			for(skip=target_input ;skip<menu_lookup_count;skip++){
					if (patch_skip_list[target_input]==1)  target_input++;
				}  // test against list and skip option if on 1
				if (target_input>(menu_lookup_count-1)) target_input=menu_lookup_count-1;
				if (target_input!=35)  {     // make target index is not selected
			patch[counter].target=target_input; // write back corrected value

			uint8_t target_index=patch[counter].target_index;
			if (target_index>menu_vars_index_limit[target_index]  )   // test limit
			{	target_index=menu_vars_index_limit[target_index]; }
			patch[counter].target_index=target_index;
			uint8_t*  target_out_ptr= menu_vars(menu_titles_final[target_input] , target_index    );
			if (target_out_ptr)           {patch[counter].out_ptr =target_out_ptr;     // write ptr
			patch[counter].divider=LFO_vars_divider[menu_vars_ref];
			patch[counter].limiter=menu_vars_limiter[menu_vars_ref]; }      // writes limiter value for particular patch , deosnt save
				}
				else patch[counter].target=0;  // write back 0 if failed
			}
		}
	}

void patch_target_modify(void){					// modify original value  careful position ,not using it now  ,ok

/*
	uint8_t loop_position=sampling_position&7;    // 0-7 , this comes usually from 0-512 loop / 64
	for (n=0;n<20;n++){
		if (patch[n].input1>=menu_lookup_count) patch[n].input1=0;    // limit
		if (patch[n].target) {         // check first for enable
			uint8_t  *ptr_to_modify =patch[n].out_ptr;       // select address , not always 8 bit ,ok
			uint16_t lfo_out_temp=  (patch[n].output [loop_position])>>7;  // 0-127, 64 default
			uint8_t lfo_mod1=ptr_to_modify; //ok
			uint32_t  modified_var =  lfo_out_temp*  lfo_mod1  ;   // grab lfo out *    data to be modfied
			//uint8_t  var_replaced= (modified_var>>right_shift)&127;   // scale to 8 bit for now
			uint8_t  var_replaced= (modified_var>>7);   // scale to 8 bit for now
		if (var_replaced>patch[n].limiter) var_replaced=patch[n].limiter;
			*ptr_to_modify =var_replaced;   // replace original value,ok

		}
	}*/
}

	void patch_target_replace(void){					// sttaight value replace  ,ok
	    uint8_t loop_position=sampling_position&7;    // 0-7 , this comes usually from 0-512 loop / 64
	    uint8_t counter;
	   uint8_t input_loop_position;
	   int32_t output_hold;
	   int32_t output_hold2;
	   uint8_t input_mix=0;

	    for (counter=0;counter<20;counter++){    // single phase inc half sine only !!!



		    if (patch[counter].target) {         // check first for enable

			if ((patch[counter].input1>40) ||(patch[counter].input2>40) )input_loop_position=0;  else input_loop_position=loop_position;// hack


				input_mix=patch[counter].in_mix;
				output_hold	      =*(patch[counter].in1_ptr+(input_loop_position));   //write output here
				output_hold2	      =*(patch[counter].in2_ptr+(input_loop_position));   //write output here

				output_hold	=output_hold*(16-input_mix);   // bit messy  might just pre_calc
				output_hold2	=output_hold2*input_mix;



				patch[counter].output[loop_position]	=((output_hold+output_hold2)>>4);


			    uint8_t  *ptr_to_modify =patch[counter].out_ptr;       // select address , not always 8 bit ,ok
			    uint16_t lfo_out_temp=  (patch[counter].output [loop_position])>>8;  // 0-256,
			    uint8_t lfo_mod1=ptr_to_modify; //ok

			    uint8_t  var_replaced =  lfo_out_temp >>patch[counter].divider;   // grab lfo out *    data to be modfied

			    if (var_replaced>patch[counter].limiter) var_replaced=patch[counter].limiter;  // limit lfo output

			    *ptr_to_modify =var_replaced;   // replace original value,ok
		    }
	    }
    }




void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)    // unreliable
	{
		if(TIM10==htim->Instance){     // send spi to display , ok
		    HAL_SPI_Transmit_DMA(&hspi2,spi_tx_block+block_counter,54);
		    			if (block_counter<3456) block_counter=block_counter+54; else block_counter=0;

		}
		if(TIM3==htim->Instance)			//
	{
		    time_proc++;
			play_hold=play_sample[sample_point<<1]; // this is 2
			play_hold2=play_sample[(sample_point<<1)+1];
			if (sample_point>1022)    {bank_write=2;sample_point=0;} else sample_point++;

	TIM3->CCR1=play_hold ;  // keep readin sample storage
	TIM3->CCR2=play_hold2 ;   // load up counter on ch2
	}
	}



    void analoginputloopb(void){  //works ,getting obsolete


	    menuSelect = cursor_menu[2]>>4;		//x *7  main menu select
		//    menuSelectX=cursor_menu[2]&15;  // Y select inside page
        		    cursor_menu[1]=0;
   		    cursor_menu[2]=enc2_dir;

	    enc_temp=(((TIM2->CNT)+32767)>>1)&65535		;  // read counter tim2 ,divider ok
	 //   enc2_store[enc2_store_count]=(((TIM4->CNT)+32767)>>1)&65535;  // read counter tim4, noisy
	    enc2_temp=(((TIM4->CNT)+32767)>>1)&65535;

	 /*   uint16_t enc2_store2=32767;
	    uint16_t enc2_store3=32767;

	    if (enc2_store_count==3) enc2_store_count=0; else enc2_store_count++;

	    enc2_store2=enc2_store[0]+enc2_store[1]+enc2_store[2]+enc2_store[3];     // average filter hopefully
	    enc2_store3=enc2_store2>>2;
	    enc2_temp=enc2_store3;
*/


	    if  (enc_temp>enc_tempB)	 enc_dir=enc_dir-1;   //menu itme value change
	    if  (enc_temp<enc_tempB)	 enc_dir=enc_dir+1;


   //   if (enc2_temp>511) enc2_temp=511;  //mem overflow somewhere

      enc2_dir=65535-enc2_temp; //temp to try source data


      if (enc_dir>32760) enc_dir=0;
			    if (enc_dir<0) enc_dir=16383;

			    enc_tempB=enc_temp;

					    enc2_tempB=enc2_temp; // to effective as counter not getting reset


 //   potValues[counterVarB]=(potSource[counterVarB]>>4) & 15 ;  // reduce values for now ,use original for others , slow count


    }


void note_reset (void){          // reset deafult values before modulation , in case it gets left  with no modulator

	note[0].velocity=255;note[1].velocity=255;note[2].velocity=255;note[3].velocity=255;note[4].velocity=255;note[5].velocity=255;note[6].velocity=255
																;note[0].detune=0;note[1].detune=0;note[2].detune=0;note[3].detune=0;note[4].detune=0;
	note[5].detune=0;note[6].detune=0;
	filter[0].cutoff_1=0;filter[1].cutoff_1=0;filter[2].cutoff_1=0;filter[4].cutoff_1=0;
	filter[0].resonance=0;filter[1].resonance=0;filter[2].resonance=0;filter[3].resonance=0;
	note[0].position=1;note[1].position=1;note[2].position=1;note[3].position=1;
}

void main_initial(void){
	const volatile uint8_t *userConfig=(const volatile uint8_t *)0x0800D2F0;



	HAL_DMA_Init(&hdma_spi2_tx);
	HAL_DMA_Init(&hdma_spi1_rx);
	HAL_DMA_Init(&hdma_spi1_tx);


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
	HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_2);
	//HAL_TIM_PWM_Start_DMA(&htim3, TIM_CHANNEL_4,pData, 63);
	TIM2->CNT=32000;
	HAL_ADC_Start(&hadc1);
	HAL_ADC_Start_DMA(&hadc1, adc_source, 3072); //dma start ,needs this and adc start ,set sampling time to very long or it will fail



	HAL_I2C_MspInit(&hi2c2);
	uint8_t	send_spi1[260]={0x02,0,1,0,128,129,130,131,132,133,134,135,136,137,138,250,0,0} ; //page program ,24bit(address)  +1-255 byte data  (page)


	HAL_Delay(5);




		//uint16_t sample_size=sizeof(RAM);

		uint16_t sample_size=32678;



		uint8_t* ram_ptr=&RAM[0];

				sample_save(2,ram_ptr, sample_size);  // write sample no 255
				send_spi1[0]=sampler_ram_clear_test(2);  // test written ok



	uint8_t potSource2[64]={0};    // { [0 ... 112] = 64 };
	uint16_t mem_count2=0;
	uint16_t mem_counter=0;
	uint8_t flash_test;
	//uint8_t send_spi1[520];

//flash_block_erase(0);



	flash_read_block2[0]=0x03; //read page 1
		flash_read_block2[1]=255;			// last patch for now
		flash_read_block2[2]=128;		// 1->127  = 32kbyte,but need 512 per step , only counts to 16383 ,    64*512
		flash_read_block2[3]=0;

		HAL_GPIO_WritePin(CS1_GPIO_Port, CS1_Pin, 1);HAL_SPI_TransmitReceive_DMA(&hspi1,flash_read_block2,flash_read_block2,1028);HAL_GPIO_WritePin(CS1_GPIO_Port, CS1_Pin, 0);



	HAL_I2C_Mem_Read (&hi2c2,160,64, 2 , potSource, EPROM_limit,1000); //ok


		memcpy(&seq,potSource,46 );  // load from potSource  ,, causes problems with memory ,NEEDS TO BE CONTINUOS OR  WILL  GET CORRUPT
	    memcpy(&note,potSource+156,160 );   // this works but keep checking for fragmentation
	    memcpy(&sampler,potSource+476,36 );
	    memcpy(&delay,potSource+572,5 );
	    //    memcpy(&sampler+36,potSource+572,20 );   // not yet



	    for(mem_counter=0;mem_counter<10;mem_counter++){

			memcpy(&LFO[mem_counter],potSource+46+(mem_counter*6),6 );  // + 60 ,ok here
			memcpy(&ADSR[mem_counter],potSource+106+(mem_counter*5),5 );  // +50  ,
			memcpy(&patch[mem_counter],potSource+316+(mem_counter*6),6 );

			memcpy(&LFO_slave1[mem_counter],potSource+376+(mem_counter*6),6 );  // + 60 ,ok here
			memcpy(&LFO_square[mem_counter],potSource+436+(mem_counter*4),4 );
			memcpy(&patch[mem_counter+10],potSource+512+(mem_counter*6),6 );


	    }

	    uint32_t test_eeprom=0;
	    for(i=0;i<16;i++){

	    test_eeprom=seq.notes1[i]+test_eeprom;
	    test_eeprom=seq.notes2[i]+test_eeprom;
		}
	//   if (test_eeprom==0)  disable_eeprom=1;  // if eeprom didn't load , disable writes  , usually zeros when load fails


	    for(i=0;i<64;i++){       //   fill with characters also add lcd command ,ok

		for 	(n=0;n<18;n++){					// this is ok

			if (n==0) gfx_ram[(i*18)+n] = 128+(i&31);   // half page
			if (n==1) gfx_ram[(i*18)+n] = 128+((i>>5)*8);    // change x to 8
			if (n>1)  gfx_ram[(i*18)+n] = 255;

		}

		}
	seq.pos=0;
	uint8_t counter;
	for (counter=0;counter<16;counter++){     // notes for sampler

	sampler.Snotes1[counter]=seq.notes2[counter]&15;
	sampler.Snotes2[counter]=(seq.notes2[counter]>>4)&15;
	}

	uint16_t lfo_list[24]= {1,2,3,4,8,12,16,24,32,48,64,96,128,156,228,256,512,768,1024,2048,4096,9196,16384,32768};

	    for (counter=0;counter<20;counter++){
	//	lfo_table[counter]=    (counter*counter*((counter>>5)+1))<<((counter>>2)+1);
	//	lfo_table[counter]=   (lfo_list[counter]*16) <<(counter>>15);
	//	lfo_table[counter]= (((counter>>2)+1)*16)<<(((counter>>2)+1)<<1);
	//lfo_list[counter	lfo_table[counter-1]=(( (counter-1)&3)+1)*counter*counter;
		lfo_table[counter]=lfo_list[counter]*16;



		if (lfo_table[counter]<17) lfo_table[counter]=16;
	    }



	uint32_t  tempo_hold=1;  // calculate tempo look up


	  	for (i=1;i<256;i++) {     // change for 32khz   1-256 bpm
	  		tempo_hold=i;
	  		tempo_hold=(60*seq_sample_rate)/(tempo_hold*16); //

	  	tempo_lut[i]=tempo_hold;
	  	}

	//isrMask=571; // def tempo 571=180bpm , 20 ms /isrcount

		noteTiming=24;
		for (i=0;i<320;i++)	{	// write C into whole section,useful ornot
			spell[i]=67;

		}

		gfx_clear();
	uint16_t pars_counter;

	for (pars_counter=0;pars_counter<menu_parser_limit;pars_counter++)	{   // fill up display data , needs to run a lot more though or wont finish string_search

			menu_parser();  // run it closer to default_menu size ,times, if default_menu is corrupt gfx breaks pretty bad

		}
	for (i=0;i<16384;i++) {     // change for 32khz   1-256 bpm
//	RAM[i]=0;      disable for now
	}
	for (i=0;i<1024;i++) {     // change for 32khz   1-256 bpm
		//default_menu3[i]=64; not it

	}



		for (pars_counter=0;pars_counter<512;pars_counter++)	  default_menu3[pars_counter>>1]=64;

		default_menu3_size = strlen(default_menu3);  // grab menu size , this is needed
			menu_title_count--;  //count back one
			display_clear ();
			for (pars_counter=0;pars_counter<menu_title_count;pars_counter++)	default_menu3 [menu_title_lut[pars_counter]&1023]=48;
			note_reset();


			uint8_t find_set=0;
			uint8_t var_hold=0;
			for (pars_counter=0;pars_counter<menu_lookup_count;pars_counter++){
			   var_hold=menu_vars_limiter[pars_counter];
			    find_set=0;
			   while ( var_hold >>= 1) {
			       find_set++;
			    }

			    LFO_vars_divider[pars_counter]= 7-find_set;    // high set bit , use it to shift left from 255
			}

			menuSelect=0;
	// fill up sample
	firstbarLoop=0;
	//sample_ram =  &RAM[0];    //  allocate half a second

}

void sampler_save(void){


}

void patch_lists(void){   //   ok
    uint8_t  list_counter;
    uint8_t list_counter2=0;
    for (list_counter=0;list_counter<20;list_counter++){

	if ((patch[list_counter].input1&3)== 3   )    { LFO_sqr_list[list_counter2]   =  (patch[list_counter].target_index&7)+48;   // only writes sqr related  to list ,others left off
	sqr_target_list[list_counter2] =  (patch[list_counter].target);   //just
	list_counter2++;
	}

    }



}














