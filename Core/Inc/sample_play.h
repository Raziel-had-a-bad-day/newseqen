	//   Everything  with  managing sample data

	uint8_t  sampler_ram_clear_test(uint16_t sample_number){    // returns 15   if location is clear    or ok , else  0  , sample number is  0-511 (slow version)



	//  HAL_SPI_Init(&hspi1); // write to register hspi2
	if (sample_number>509) return 0;
	MX_SPI1_Init();

	uint8_t spi_test[4];
	uint8_t test_counter=0;
	uint8_t test_return[24]={0};

	test_return[0]=0x03; //read page 1
	test_return[1]=sample_number>>1; //24bit address msb
	test_return[2]=(sample_number&1)<<7; //24bit address
	test_return[3]=0; //24bit address lsb


	//test_return[1]=0; //24bit address msb
	////	test_return[2]=1; //
	//	test_return[3]=0;



	HAL_GPIO_WritePin(CS1_GPIO_Port, CS1_Pin, 0);  // when readin low till the end
	//HAL_SPI_ (&hspi1,test_return, 20, 1000);   // works fine
	HAL_SPI_TransmitReceive (&hspi1, test_return,test_return,  24, 100); // request data


	HAL_GPIO_WritePin(CS1_GPIO_Port, CS1_Pin, 1);  // high end

	//HAL_SPI_Transmit(&hspi1, send_spi1, 1, 1000);
	for (test_counter=0;test_counter<20;test_counter++){
	    if (test_return[test_counter+20]!=255) return 0;  else return 15;

	}
	return 15;
    }

	//------------------------------------ this is needed for now
	    void byte_swap(uint8_t* to_swap, uint32_t  array_size){   // sample byte  swap
	    uint32_t counter;
	    uint32_t swap_size=array_size*2; // in bytes !


	    for (counter=0;counter<swap_size;counter=counter+2){


		uint8_t* byte1=&to_swap[counter>>1];
		uint8_t* byte2=&to_swap[(counter>>1)+1];
		to_swap[counter>>1]=*byte2;
		to_swap[(counter>>1)+1]=*byte1;


	    }

	}

 void sample_save(uint16_t sample_number, uint8_t* sample_data,  uint16_t  sample_size ){				// writes data to flash (slow version )

		stop_toggle=1;  stop_start();   // halt for slow write

		uint8_t spi_test[270]={0};
		if (sample_number>509) return ;



			spi_test[1]=sample_number>>1; //24bit address msb
			spi_test[2]=(sample_number&1)<<7; //24bit address
			spi_test[3]=0; //24bit address lsb

				//----formAT SECTION
					spi_test[0]=0x06; //enable write  , only lasts for single operation
					HAL_GPIO_WritePin(CS1_GPIO_Port, CS1_Pin, 0); // start
					HAL_SPI_Transmit(&hspi1, spi_test, 1, 1000);       // enable write
					HAL_GPIO_WritePin(CS1_GPIO_Port, CS1_Pin, 1);   // end
					HAL_Delay(5);
					spi_test[0]=0x52; //block

					HAL_GPIO_WritePin(CS1_GPIO_Port, CS1_Pin, 0);         // enable for sector erase   , stays empty when enabled
					HAL_SPI_Transmit(&hspi1, spi_test, 4, 1000);   //erase sector ,works       4kbytes   (block erase=32kbytes)
					HAL_GPIO_WritePin(CS1_GPIO_Port, CS1_Pin, 1);
					HAL_Delay(250);  // SHOULD BE ENOUGH



					if (sampler_ram_clear_test(sample_number)==0) return;    // quit if not empty


		//uint16_t size_count=sample_size;

			spi_test[0]=0x03; //read page 1

			uint16_t write_counter=0;    // write start location
			uint8_t second_byte=(sample_number&1)<<7;
			uint8_t max_count= sample_size>>8;
			if (max_count>127) max_count=127;

					spi_test[0]=0x06; //enable write
					HAL_GPIO_WritePin(CS1_GPIO_Port, CS1_Pin, 0);
					HAL_SPI_Transmit(&hspi1, spi_test, 1, 1000);
					HAL_GPIO_WritePin(CS1_GPIO_Port, CS1_Pin, 1);
					HAL_Delay(5);


					for (write_counter=0;write_counter<=max_count;write_counter++)  {

					    spi_test[0]=0x02; //write
					    spi_test[1]=sample_number>>1; //0-256 this stays
					    spi_test[2]=write_counter+second_byte; //  0->127 or 128->255
					    spi_test[3]=0; //24bit address lsb

					//   spi_test[1]=255; //24bit address msb
					    			//		spi_test[2]=127+write_counter; //24bit address
					    			//		spi_test[3]=0; //24bit address lsb

					    memcpy  (spi_test+4 ,sample_data+(write_counter*256),  256);   // copy data to write buffer  sseems to be rolling over at 256

					    spi_test[0]=0x06; //enable write each time
					    HAL_GPIO_WritePin(CS1_GPIO_Port, CS1_Pin, 0);
					    HAL_SPI_Transmit(&hspi1, spi_test, 1, 1000);
					    HAL_GPIO_WritePin(CS1_GPIO_Port, CS1_Pin, 1);

					    spi_test[0]=0x02; //write
					    HAL_GPIO_WritePin(CS1_GPIO_Port, CS1_Pin, 0);   // low
					    HAL_SPI_Transmit(&hspi1, spi_test ,260, 1000);  //address,then data
					    HAL_GPIO_WritePin(CS1_GPIO_Port, CS1_Pin, 1);  // high end
					    HAL_Delay(10);

					}


					spi_test[0]=0x04; //disable write
										    HAL_GPIO_WritePin(CS1_GPIO_Port, CS1_Pin, 0); // low
										    HAL_SPI_Transmit(&hspi1, spi_test, 1, 1000);
										    HAL_GPIO_WritePin(CS1_GPIO_Port, CS1_Pin, 1);   // high end
										    HAL_Delay(20);  //should be enough in theory


	 stop_toggle=2;  stop_start();  		// restart

	    }




	void  sampler_1k_load(uint32_t load_address){   // 24 bit load address , for audio , 1kbyte   in 512 byte  chunks, last 64k off limits

	    uint8_t send_spi2[1030]={0} ;

	    load_address=load_address& 16777215; // 24 bit
	    if (load_address >0xFEBFFF ) return;  // IMPORTANT !
	    send_spi2[0]=0x03; //read page 1
	    send_spi2[1]=load_address>>16;			// last patch for now
	    send_spi2[2]=(load_address>>8)&255;    //
	    send_spi2[3]=load_address&255;     // can start anywhere*/    // usally 0 padded when written

	    current_spi[0]=send_spi2[0];   // to track  later
	    current_spi[1]=send_spi2[1];
	    current_spi[2]=send_spi2[2];
	    current_spi[3]=send_spi2[3];

	    if ((flash_flag==3)&&(flash_bank_read==0))  {   memcpy( &flash_read_block, flash_read_block2+516,512); flash_flag=4; }  // second half
	    if ((flash_flag==2)&&(flash_bank_read==1))   {   memcpy( &flash_read_block, flash_read_block2+4,512);   flash_flag=3;           }  // second half
	    flash_bank_read=!flash_bank_read;

		    if ((flash_flag==4)&&  (flash_bank_read==1) )    {

			HAL_GPIO_WritePin(CS1_GPIO_Port, CS1_Pin, 0);HAL_SPI_TransmitReceive_DMA(&hspi1, send_spi2,flash_read_block2,1028);
		    //HAL_GPIO_WritePin(CS1_GPIO_Port, CS1_Pin, 0);  //  terrible internet advice , put cs low before not after  !
		    flash_flag=0;  }

	}
void sampler_ram_record(void) {
	  if (!record_counter) {stop_toggle=1;  stop_start();}   // halt
	    memcpy(	&RAM[record_counter], input_holder,1024);   // transfer
	record_counter =record_counter+1024;
	if (record_counter>=32767) { record_counter=0; sampler.record_enable=0; stop_toggle=2; RAM_normalise();  stop_start(); }  // reset and stop record

	}

void RAM_normalise(void){
	    uint16_t counter=0;
	    uint16_t peak=0;
	    uint16_t*   ram_ptr=  &RAM;
	    float process1=32767 ;
	   int32_t incoming;
	    for (counter=0;counter<16384;counter++){

		incoming= *(ram_ptr+counter);
		if (incoming>peak ) peak=incoming;
	    }
	    process1=(32767/(peak-process1))*0.9;

	    for (counter=0;counter<16384;counter++){
		incoming= (*(ram_ptr+counter))-32767;
		incoming=(incoming*process1)+32767;

		*(ram_ptr+counter)=incoming&65535;

	    }

	}
void record_output_to_RAM(void){
//	  if (sampler.record_enable)  { return;} // skip if recording already
    memcpy(	&RAM[record_out_counter], output_mix,512);   // transfer
    record_out_counter =record_out_counter+512;
if (record_out_counter>=32767) { record_out_counter=0; record_output=0;

						RAM_normalise();   }  // reset and stop record RAM_normalise();

		     }







