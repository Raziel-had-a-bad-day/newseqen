void menu_vars(void){

	char menu_string[8];   // incoming string holder (single) , dont forget to get the
	char menu_string2[8];
	uint8_t var_index=menu_index_in&15;    //set var index ie  LFO[1].rate
	//uint8_t menu_count; // point to search result for var
	uint8_t *menu_vars_var1=NULL;
	menu_countr =0;
	memcpy(menu_string, menu_vars_in, 8);    //copy 8 strings from incoing


	for (i = 0; i < 27; i++) {      // find menu location

		memcpy(menu_string2, menu_titles_final[i], 8);  // copy title list
		if ((strncmp(menu_string, menu_string2, 8)) == 0) {
			menu_countr = i;
			menu_vars_menu=menu_titles_final+menu_countr; // copy pointer, ok


		}
	}
	switch(menu_countr){
	case 0:     menu_vars_var1= NULL; break;
	case 1:     menu_vars_var1= &LFO[var_index].rate   ; break;
	case 2:     menu_vars_var1= &LFO[var_index].depth    ; break;
	case 3:     menu_vars_var1= &LFO[var_index].gain    ; break;
	case 4:     menu_vars_var1= &LFO[var_index].offset    ; break;
	case 5:     menu_vars_var1= &LFO[var_index].target    ; break;
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
	case 19:     menu_vars_var1= &note[var_index].velocity   ; break;
	case 20:     menu_vars_var1= &note[var_index].detune    ; break;
	case 21:     menu_vars_var1= NULL   ; break;
	case 22:     menu_vars_var1= &seq.pos    ; break;
	case 23:     menu_vars_var1= &seq.tempo    ; break;
	case 24:     menu_vars_var1= &seq.notes1[var_index]   ; break;
	case 25:     menu_vars_var1= &seq.notes2[var_index]   ; break;
	case 26:     menu_vars_var1= &seq.loop    ; break;
	default :		menu_vars_var1= NULL   ; break;

	}

	menu_vars_var=menu_vars_var1;    // copy back address  ,ok

	//menu_vars_var= menu_vars_var1;

}



void menu_parser(void){          // parse out menus , shouldn't have to run (in theory) once filled ,only for feedback pointer maybe


	char menu_string[8]="xxxxxxxx";   // incoming string holder (single)
	char menu_string2[8]="hhhhhhhh";



	// strcpy(menu_string2,menu_out1[i]);
	uint16_t menu_searchsize=sizeof(default_menu)-8;   // this should fairly big always , leave gap at the end , atm 480


	if (string_search>menu_searchsize) {                    //this is ok
		return;    }    // check if bigger than search area


//	if ((!string_search) && menu_counter) {string_search=0; menu_title_count=0;
//	return;} // hold counter until menu writing was reset ,ok

	memcpy(menu_string,default_menu+string_search,8);    //copy 8 strings created menu array
	////////////////////////////
	for (i=0;i<27;i++){    	// test a single menu entry  , for now only the first record

		memcpy(menu_string2,menu_titles_final[i],8);
		if  ((strncmp(menu_string,menu_string2,8))==0) 								// compare and if true pass var,seq
		{
			menu_title_lut[menu_title_count]=(i<<8)+menu_counter;   // search result  and disp lcd position counter

			memcpy(menu_index_list+(menu_title_count*2),default_menu+string_search-2,2); // get array  index under ,LFO[1]  etc ,ok


			menu_title_count++;
			menu_counter++;
			space_check=0;
			string_search=string_search+8;     // advance search position

			return;}

	}

	//string_value=255;    // no result use 255 for now
	//menu_title_lut[menu_title_count]=255;   // record for feedback line
	//menu_title_count++;
	if (space_check>1)  menu_counter++;   //this is ok
	space_check++;  // count empty spaces or fill characters
	string_search++;
	return;


}








void SPI_command(void){
// pins are all good , SPI is always the problem!!!
if ((spi_enable==0)&& (spi_send==1)){				//if data sent and next byte is ready ,creates the actual bytes sent

uint8_t spi_store[5];

	//clk_pin=(i&1)^1;
		//bsrr_long=0;
		if (spi_hold>>8) spi_byte=248; else {spi_byte=250;}  //start with msb ,dont forget flip around at end, 250 =data ,248= command if below 8bit
/*		bsrr_long=spi_byte<<16; // top byte for command 24-16, then 2 bytes of data 16-0
		spi_hold=(spi_hold&255);
		bsrr_long=bsrr_long+((spi_hold>>4)<<12); // 4 bits to top
		bsrr_long=bsrr_long+((spi_hold&15)<<4);	// 4 bits to top
*/
	spi_store[0]=spi_byte&255;
	spi_store[1]=((spi_hold>>4)<<4)&255;
	spi_store[2]=((spi_hold&15)<<4)&255;

// send this to spi for now
		HAL_SPI_Transmit(&hspi2, (uint8_t *)spi_store, 3, 5);  // working good


//HAL_Delay(10);
	spi_enable=1; }
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)    // unreliable

{
	time_proc++;
	//if (((sample_point==511) || (sample_point==1022)) && (bank_write)  ) error_count++;

	if (sample_point==511) {bank_write=1; sample_pointD=0;  }
	if (sample_point==1022) {bank_write=1; sample_pointD=512; }
	sample_point=sample_point & 1023;// this is 1
	play_hold=play_sample[sample_point]; // this is 2





	if(TIM3==htim->Instance)			// nothing here is consistent
{


TIM3->CCR3=play_hold ;  // keep readin sample storage

/* if ((sample_point&7)==6) {
	if (spi_send==2){ GPIOB->BSRR =bsrr_seq[bsrr_counter];bsrr_counter++;}  // send spi when loaded up , this works , too fast however will fail
	if ((bsrr_counter==49) && (spi_send==2)) {bsrr_counter=0; spi_send=0;}
}
*/





//if (sample_point&1) {  // this is needed   , slow down also read only when not written to

	//bsr_out=bsrr_hold[bs_count+ (menu_store*80)]; // send port data to holder

	// if (lcd_send) GPIOB->BSRR =(bsr_out<<10) + (~(bsr_out) <<26);  // write to port but not too early

//if (spi_enable) {GPIOB->BSRR =(bsrr_hold[samp_temp]<<13)+(~(bsrr_hold[samp_temp])<<29); }  // send spi data when enabled

			//	if (spi_enable){ HAL_GPIO_WritePin(MOSI_GPIO_Port, MOSI_Pin,((bsrr_hold[samp_temp]>>1))); HAL_GPIO_WritePin(CLK_GPIO_Port, CLK_Pin,((bsrr_hold[samp_temp])&1));}


//samp_temp++;
//if ((samp_temp)==48) {spi_enable=0;spi_send=0;samp_temp=0;}  // finish , enable next char
			//	 HAL_GPIO_WritePin(RS1_GPIO_Port, RS1_Pin,1);
		 	 	//  HAL_Delay(1);




//if (bs_count==255
	//	)  { bs_count=0; menu_store=(menuSelect>>2); } else bs_count++; // menu select, higher count just spills over  159 chamge to 128 2*64
//}
sample_point++; //this needs to be here or too fast and wrong sample rate

}

}


 /*void adc_read(void){		HAL_ADC_Start(&hadc1);				//disabled
		if (HAL_ADC_PollForConversion(&hadc1, 1) == HAL_OK)      // 1ms time out
				{
		adc_values[0] = HAL_ADC_GetValue(&hadc1);
					}  // get value
		 HAL_ADC_Start(&hadc1); // need for every conversion
		if (HAL_ADC_PollForConversion(&hadc1, 1) == HAL_OK)      // 1ms time out
				{
		adc_values[1] = HAL_ADC_GetValue(&hadc1);
					}  // get value
	 	HAL_ADC_Start(&hadc1);
		if (HAL_ADC_PollForConversion(&hadc1, 1) == HAL_OK)      // 1ms time out
				{
			adc_values[2] = HAL_ADC_GetValue(&hadc1);
				}  // get value this works
		HAL_ADC_Start(&hadc1);
		if (HAL_ADC_PollForConversion(&hadc1, 1) == HAL_OK)      // 1ms time out
						{
					adc_values[4] = HAL_ADC_GetValue(&hadc1);
						}  // get value this this on e is blank
		//HAL_ADC_Stop(&hadc1);
//for (i=0;i<3;i++){ adc_values[i]= (adc_values[i]>>8) &15; } // reduce value to 4 bit
}
*/
void analoginputloopb(void){  //works fine still
uint16_t menu_holder;



	menuSelect = cursor_menu[2]>>4;		//x *7  main menu select
		menuSelectX=cursor_menu[2]&15;  // Y select inside page

	//	counterVarB = menuSelectX + menuSelect; // select mem page 10*8  and location pointer  77
//		if (adc_values[1]>15)	menu_holder=disp_lut [(menuSelect)	+1] [31-adc_values[1]]; // change back to 0-15
//	else menu_holder=disp_lut [(menuSelect)	] [15-adc_values[1]];  // grab disp lut value for pointer if valid then write for now

		//menu_holder=disp_lut [(menuSelect)	] [(15-adc_values[1])];   // value from disp lut
		//menu_holder=disp_lut [menuSelect] [menuSelectX];   // value from disp lut

		cursor_menu[1]=0;
		//cursor_menu[2]=cursor_lookup[enc2_dir];
		// cursor_menu[2]=cursor_lookup[enc2_dir]; //needed for correct line sequence ,obsolete
		cursor_menu[2]=enc2_dir;

		//cursor_menu[2]=(16-adc_values[1]);



		//if (menu_holder>127)	counterVarB=menu_holder-128; //  points to actual potvalues location from dsip_lut when value is higher than 127 , works ok problem with menu display
		//if (menu_holder>511)	counterVarB=menu_holder-384;		// text pointer , max potvalue address is 142 , fix second page issue later


		//enc_dir=potSource[counterVarB];


	enc_temp=(TIM2->CNT)>>1;  // read counter tim2 ,divider ok
	enc2_store[enc2_store_count]=(TIM4->CNT)&255;  // read counter tim4, noisy
	uint16_t enc2_store2=0;
	uint16_t enc2_store3=0;
	if (enc2_store_count==3) enc2_store_count=0; else enc2_store_count++;

	enc2_store2=enc2_store[0]+enc2_store[1]+enc2_store[2]+enc2_store[3];     // average filter hopefully
	enc2_store3=enc2_store2>>3;
	enc2_temp=enc2_store3;
	//enc2_temp=(TIM4->CNT) ;   //force
	//enc2_temp=enc2_temp&127; // fix overflow ? , dont need a lot because of skip


	//if  (enc_temp>enc_tempB)	 enc_dir=enc_dir-(disp_multi[enc2_dir>>4]);   // start settle timer , will do 2 times per turn always, wire opposite
	//if  (enc_temp<enc_tempB)	 enc_dir=enc_dir+(disp_multi[enc2_dir>>4]);   // start settle timer , will do 2 times per turn always, wire opposite , step multiplier setting
	if  (enc_temp>enc_tempB)	 enc_dir=enc_dir-1;
	if  (enc_temp<enc_tempB)	 enc_dir=enc_dir+1;





  if (enc2_temp>383) enc2_temp=383;  //mem overflow somewhere
	if (enc_dir>160) enc_dir=160;
			if (enc_dir<0) enc_dir=0;
			//if (enc2_temp<16) enc_dir=(enc_dir<<2) & 127; // faster for notes on first row

			//potSource[counterVarB]=enc_dir;       // enter data into potsource
				//	enc_dir=0;
			enc_tempB=enc_temp;
			//if (enc2_dir>383) enc2_dir=383;






			//if (enc2_temp<enc2_tempB)	 enc2_dir--;
			enc2_dir=enc2_temp; //temp to try source data
		//	if (enc2_dir>127) menu_page[1]=127; else if (enc2_dir>255)   menu_page[1]=255;				else menu_page[1]=0;
		//	menu_page[1]= (enc2_dir >>7) <<7;

			menu_page[1]=enc2_dir&384;  // single  for now
			menu_page[1]=0; // force a single page for now

			//if (enc2_dir>255) {menu_page[1]=0;display_fill();}

				//	if ((enc2_temp>127) && (enc2_tempB<=127)) display_fill();    // need to optimize
				//	if ((enc2_temp<=127) && (enc2_tempB>127)) display_fill();
				//	if ((enc2_temp>255) && (enc2_tempB<=255)) display_fill();
				//	if ((enc2_temp<=255) && (enc2_tempB>255)) display_fill();
				//	if ((enc2_temp>383) && (enc2_tempB<=383)) display_fill();
					//if ((enc2_temp<=383) && (enc2_tempB>383)) display_fill();
					//if ((enc2_temp<127) && (enc2_tempB>384)) display_fill();
					//if ((enc2_temp<=511) && (enc2_tempB<127)) display_fill();


					enc2_tempB=enc2_temp; // to effective as counter not getting reset


potValues[counterVarB]=(potSource[counterVarB]>>4) & 15 ;  // reduce values for now ,use original for others , slow count

//potValues[counterVarB]=potSource[counterVarB] & 15 ;  // reduce values for now ,use original for others



}
void display_init(void){
	//uint8_t sp2_command=0xf8+(0<<1);


		 // uint16_t disp[]={  304,304,304,270,257,268,384,258,51,51,52,53,54,55,56,57,58,0};  // normal characters
		 uint16_t disp[]={  304,268,257,262,308,310,310,51,51,52,53,54,55,56,57,58,0};  // this should work for gd 310 is extended instruction
		  if (spi_send==0){         // sets data byte


switch(init){     //Remember every line advances +char on display ,,all this is mostly unneeded

default : init_b=init-6;displayBuffer2 ();spi_hold=spell[init_b];break; //initial menu write either page ,skip after
}


if (init<6)	{HAL_Delay(3);spi_hold=disp[init]  ; } //delay needs to go

init++;   // after 6 it quits the rest is not needed
spi_send=1;SPI_command();spi_send=0;spi_enable=0;
		  }


}


void gfx_send(void){         // send spi to lcd from gfx ram


			uint8_t spi_store[3];
			uint8_t spi_store2=0;

			uint8_t spi_store3=gfx_ram[gfx_send_counter];

			if (gfx_send_counter2<2) spi_store2=248; else {spi_store2=250;    }//start with msb ,dont forget flip around at end, 250 =data ,248= command if below 8bit

					spi_store[0]=spi_store2;
					spi_store[1]=((spi_store3>>4)<<4);
					spi_store[2]=((spi_store3&15)<<4);

						HAL_SPI_Transmit(&hspi2, spi_store, 3, 3);    // ca 0.1ms in theory so one line should be about 2ms

					if (gfx_send_counter2==17 ) { gfx_send_counter2=0;    } else gfx_send_counter2++; // check elsewhere if changing gfx_send_counter
			if (gfx_send_counter==1151 ) { gfx_send_counter=0; disp_end=1; gfx_send_counter2=0;    } else gfx_send_counter++;

		}


void display_fill(void)  {     // full update of gfx memory
loop_counter3=1;
uint8_t fill_counter= menu_title_count;
if (!menu_title_count)    fill_counter=127;
//uint16_t store_x;
//uint8_t d_count;

for (n=0;n<fill_counter;n++)	{ //just fills with blank character s

	disp_stepper=1;

	init_b=n;
displayBuffer2();



}
//enc2_dir=menu_page[1];  // end clean
}






void display_process(void){							// keep data processing here


	if ((enc2_tempC==enc2_dir) && (!enc2_add))  {
		if (disp_stepper==5)  {enc2_add=2; 	disp_stepper=13;}   	 // wait till enc2_dir  hasn't changed ,jump and then change feedback
			}

	if (disp_stepper>1)	init_b=123+(disp_stepper);  // write lcd3
	if (disp_stepper>4)	init_b=107+(disp_stepper);  // write feedback line

	if ((lcd_temp!=enc_dir)  && (enc2_tempC==enc2_dir) )   {*menu_vars_var=enc_dir;   // enc2_dir =same enc_dir=changed
	lcd_temp=enc_dir;  }



	if ((disp_stepper==0) && (enc2_tempC!=enc2_dir) && (!enc2_add))  {      // wait till loop end , ok

	if  (enc2_tempC<enc2_dir)	 enc2_add=1;   									// use this to set up or down count for variables , might change
		if  (enc2_tempC>enc2_dir)	 enc2_add=-1;    // slow but 1 step at a time
		//if  (enc2_tempC<enc2_dir)	 enc2_add=+(enc2_dir-enc2_tempC);   	//  fast but skips

		//if  (enc2_tempC>enc2_dir)	 enc2_add=-(enc2_tempC-enc2_dir);


		enc2_tempC=enc2_dir;	   // loop back until change
		if ((enc_out1>=0)  &&  (enc_out1<=menu_title_count))    {enc_out1=enc_out1+enc2_add;}   // count up or down within limits
		if (enc_out1<0)  {  enc_out1=0; }
		if (enc_out1>menu_title_count) enc_out1=menu_title_count;
		enc2_add=2;
	}


	if ((disp_stepper==0) && (enc2_add==2))			// skip if no input for enc2 , also maybe wait till enc2 stopped moving
	{



	uint8_t crap_hold9=menu_title_lut[enc_out1]>>8;   // look up up menu_titles_final
	// fetch values for last line or cursor

	 memcpy(default_menu3+112, *(menu_titles_final+crap_hold9),8);   // copy feedback data for reading,ok
	 memcpy(menu_vars_in,*(menu_titles_final+crap_hold9),8);	// send back for menu vars ok

	 char temp_char[]="  ";
	 memcpy(temp_char,menu_index_list+((enc_out1*2)),2);   // copy char to char,ok
	 menu_index_in=atoi(temp_char)			;   // convert char to int,ok
	 menu_index_in=menu_index_in&15;  //for now , needs another limiter

	 menu_vars();		//test  for vars ok

	    // grab value on ptr address , also write first char , ok

	}

	if ((disp_stepper==0) || (disp_stepper==1))   // repeat first character
	{ init_b= menu_title_lut[enc_out1]&127;    // max 127 for now , max is two pages

	lcd_out3=*menu_vars_var;
	default_menu3[init_b]=((lcd_out3&255)>>4)+48; lcd_temp=lcd_out3&127; enc_dir=lcd_temp;       } // force enc_dir



	if (disp_stepper==11) default_menu3[119]=menu_index_list[enc_out1<<1];   // index char 0-10 normally
	if (disp_stepper==12) default_menu3[120]=menu_index_list[(enc_out1<<1)+1];
	//if (disp_stepper==13) default_menu3[122]=47;

	if (disp_stepper==1)  gfx_send_cursor=(init_b>>4)&7 ;   //send cursor line
	if (disp_stepper==2)  {

		default_menu3[125]=potSource[380]+48; default_menu3[126]=potSource[381]+48; default_menu3[127]=potSource[382]+48; }  // write this straight after start ,ok

	}   // end o void






void displayBuffer2 (void){       // use only writing characters  ,nothing more  , init_b for selecting location
														//when scrolling maybe use this only until  settled


	uint8_t d_count;
uint16_t init_x=(init_b & 15)+2 ;    // +2 important  0-15 hor
uint8_t init_x2=(init_b>>4)<<3 ;  // 0-64  character address in gfx
uint16_t init_y=((init_x2)*18)+init_x;   //   works ok
uint16_t store_x;


store_c= (default_menu3[init_b]-47)&127 ;    // grab char from mem

store_x=(store_c*8);  // i line characters , might shrink it and use extr for other  visuals , old code but keep for now


		if ( (disp_stepper==0))     // blinker for cursor character only  , might just flip the whole last line from prev tables then its x4 faster
			for (d_count=0;d_count<8;d_count++){
				gfx_ram[init_y+(d_count*18) ]= gfx_char[d_count+store_x]^127; //write character to ram ,should be elsewhere , blank is correct
			}
		else for (d_count=0;d_count<8;d_count++){
			gfx_ram[init_y+(d_count*18) ]= gfx_char[d_count+store_x]; //write character to ram ,should be elsewhere , seems affected by later stufff
		}



if (disp_stepper==13) {disp_stepper=0;enc2_add=0;  }     else disp_stepper++;				// count to 16 also make sure full loop before skip lines


}    // displayBuffer2

void sampling(void){						// 18 ms of data

//	if (time_proc>580) time_final=time_proc;


time_proc=0;



uint8_t mask_i;


bank_write=0;
//time_proc=0;   //millis

//adc_read();
//uint16_t isr_tempo=isrMask; // get tempo value
sample_pointB=sample_pointD;
unsigned short tempo_start=0;  // enabled when i=isrMask;

uint16_t i_total;
uint16_t tempo_mod=tempo_lut[seq.tempo];  // set tempo,speed from lut 40-200bpm  ,changed to 4x for note lenght

uint8_t l;			// 35.002khz(0.02857ms) sample, 1 sample is temp count (16x=0.00045712) , *16=1 note ,at 300 (437bpm),(1/(0.00002857*tempo count*16)=1beat in s
float freq_temp;	// (1/(bpm/60)) /0.00045712=tempo count ie 1093.8 for 120bpm
float freq2_temp;

float tempo_sync=16384/((tempo_mod*16)/512) ; // 8000 at slowest 15.625 updates to lfo at 1 note 16384/15.625=1048.576+ per update  at setting 80
tempo_sync=tempo_sync/80;  // bit weird her , this is adsr !

uint32_t  note_toggler[17]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};  //records note position on 0-512   using a bit
for (i=0;i<16;i++) {  note_toggler[i]=0; }
	//potSource[150]=(freq_point[0])*100; //0-2

//float lcd_out2;

//lcd_out3=menu_page[1]; // still goes to 15

//lcd_out3=adc_values[0]+adc_values[1]+adc_values[2]; // 3 digit read out , works ok,, [2] works but thats it
//lcd_out3=lcd_out3+180;
potSource[380]=(lcd_out3/100);  // still works   , potsource ref is located in feedback line var  ,was sendin x16 values
potSource[381]=((lcd_out3 %100)/10);		 // 0-160 to 0-10
potSource[382]=(lcd_out3%10);



note_holdA=0;
//printf ("crap \n");
//printf("%d|",adc_values[0] );printf("%d|",adc_values[1] );printf("%d|",adc_values[2] );printf("%d|\n",adc_values[3] );
//uint8_t note_patterns[8]={1,4,2,2,1,4,2,1,4,2,1,4,4};   // creating beats

//tempo_mod=tempo_mod-63+(lfo_out[1]>>7);
//if (tempo_mod<450) tempo_mod=((tempo_mod-200)>>1) +200; // more res lower


//sample_Accu2=0;
//printf (adc_values[0]," ",adc_values[1]," ",adc_values[2]," ","crap \n");
// some good phasin and delays here
uint8_t cross_fade[2];
uint8_t fader[17]={0,1,5,11,19,28,39,51,64,76,88,99,108,116,122,126,127}; // sine curve for cross fade
adc_values[2]= 15; //force for now
if(adc_values[2]&16)     	{cross_fade[1]=127-fader[adc_values[2]&15]; cross_fade[2]=127;}  else {cross_fade[2]=fader[adc_values[2]&15]; cross_fade[1]=127;} //calculate crossfader

// doing lfo calc here as it is slow only for now
////////////////////adsr/////////////////////////////////////////
if	 (ADSR[0].attack_trigger==0) {		adsr_att=(161-ADSR[0].attack ) *0.02 ; // for now all of them from this only , speed , 0-16  // rarely read
ADSR[0].sustain_data=((161-ADSR[0].decay)*0.01);  // length and level this is ok is running 1/16 ish				ADSR[0].attack_data=ADSR[0].attack_data-ADSR[0].sustain_data;
adsr_att=adsr_att*adsr_att;
ADSR[0].sustain_data=ADSR[0].sustain_data*ADSR[0].sustain_data;

ADSR[0].attack_trigger=1;
ADSR[0].attack_data=0;
}

//float ADSR[0].buffer_temp;    // adsr

for (i=0;i<256;i++) {

	if     (ADSR[0].attack_data<1000)    																		{ADSR[0].attack_data=ADSR[0].attack_data+adsr_att;					ADSR[0].buffer_temp=ADSR[0].attack_data; } //0-1000
	 if  ((ADSR[0].attack_data<1500)  && (ADSR[0].attack_data>999))  					{ADSR[0].attack_data=ADSR[0].attack_data+adsr_att;					ADSR[0].buffer_temp=1500-(ADSR[0].attack_data-500);  }  // 1000-500
	 if ((ADSR[0].attack_data>1499)   && 		(ADSR[0].attack_data<2000)) 			{		ADSR[0].buffer_temp=500; 	ADSR[0].attack_data=ADSR[0].attack_data+ADSR[0].sustain_data;}
	if ((ADSR[0].attack_data>1999)  &&  (ADSR[0].attack_data<2500)	)					{ADSR[0].attack_data=ADSR[0].attack_data+ADSR[0].sustain_data	;	ADSR[0].buffer_temp=2500-ADSR[0].attack_data; } //500-0;
	if (ADSR[0].attack_data>3000)   																			{ADSR[0].buffer_temp=1; ADSR[0].attack_data=4000; }     // THE END

adsr_lut[i]= ADSR[0].buffer_temp*0.001;
}


///////////////////////////////////////////////////////////////
uint8_t note_plain;

for (i=0;i<512;i++) {    // this should write 512 bytes , or about 15ms buffer ,works fine, too much scope

	i_total=i+sample_pointB;
	i_frac=i>>6;   //   0-8 steps
	note_plain=seq.notes1[seq.pos & 7 ];
potValues[i&255]=potSource[i&255]>>4; //just to update values
	if (tempo_count>=tempo_mod) { next_isr=(next_isr+1)& 4095;tempo_count=0;  }  else {tempo_count++; }  //trigger next note , actual next step for isrCount(future)  8ms,trying to fix slow down here  8000 too  much, adsr clears note info
// tempo_count is about 1000-400
	tempo_start=0;
	if ((next_isr>>4) != (seq.pos)) { 					// next note step 140ms
		seq.pos=(next_isr>>4); // seq pos =256 max , isr = 1/16 of a note, note lenght is 1-4
		tempo_start=1;
// record note triggers or seq_changes position ,NEEDS TO BE OFF FOR NOTE 0
}


	if(tempo_start  )    // Calculates only on note change, gotta change seq.pos somehow  , only activates when change in seq pos
	{
	//printf("\n");//	ITM_SendChar( 65 );   //  Send ASCII code 65 = ’A’
	//printf("%d" ,note[].pitch[10]);


		note[2].timeshift=(adc_values[0]>>2)&15; //assigned pots to start of loopers 0-16,works
		note[3].timeshift=(adc_values[1]>>2)&15;


		seq.loop[2]=((note[2].timeshift+(seq.pos&7))&15); // calc  8 note loop positions sets looping point in sequence

		//seq.loop[3]=(note[3].timeshift+(( seq.pos&31 ) >>2)) & 15;  // quater speed
			seq.loop[3]=((note[3].timeshift+(seq.pos&15))&15); //sets looping point in sequence this is full 16 note

			seq.loop[4]=((note[2].timeshift+(seq.pos&7))&15);

		//seq.loop[4]=((note[2].timeshift+((seq.pos&15)>>1))&15); // half speed

		note[2].pitch=(seq.notes2[seq.loop[2]]>>4)+(note[2].transpose>>4);  //loop 8 notes from pos and x times
		note[3].pitch=(seq.notes1[seq.loop[3]]>>4);  //loop 8 notes from pos and x times ,might disable normal adsr completely
	if (note[3].pitch) 		{note[3].pitch=note[3].pitch+(note[3].transpose>>4);	adsr_retrigger[3]=1; note_toggler[i>>5]=1<<(i&31   )   ; } // stay at zero for off
//	if ((note[].pitch[3]) && (adsr_retrigger[3]==1))		adsr_retrigger[3]=0;   // while note on , turn of trigger
//	if ((note[].pitch[3]) && (adsr_retrigger[3]==0))	  adsr_retrigger[3]=1;

//	if (!note[].pitch[3]) 	adsr_retrigger[3]=0;  // end note


	//note[].pitch[3]=(note[].pitch[3]-4)+(lfo_out[2]>>11);

	//if (((seq.pos&7)==0) && (adsr_toggle[6]==2))		{adsr_retrigger[6]=1; } else adsr_retrigger[6]=0; // delete


	//note[5].pitch=potValues[80+(seq.pos&15)];  // sample


	//if ((note[5].pitch) && (adsr_toggle[5]==2)) {note[5].pitch=note[5].pitch; one_shot=0;}  // grab note when on ,one shot also , also delete


	note[5].pitch=(seq.notes2[seq.loop[2]]>>4)+(note[5].transpose>>4);  //





	note[5].pitch=MajorNote[note[5].pitch];    //this is for sine skip mask
	//note[5].pitch=11; // works ok with single note @24 but   fails on other
	note[5].tuned=sine_lut[note[5].pitch];	//sets freq ,1.0594  * 16536 =17518  ,
	note[5].tuned= (note[5].tuned*1200)>>10;  // modify different sample size , just need single cycle length and thats it
		mask_result =0;

/*
		sample_Accu[0]=0; // reset to 0 mani sample hold
		sample_Accu[1]=0; // reset to 0 mani sample hold
		sample_Accu[2]=0; // reset to 0 mani sample hold
		sample_Accu[3]=0; // reset to 0 mani sample hold
		sample_Accu[4]=0; // reset to 0 mani sample hold
		sample_Accu[5]=0; // reset to 0 mani sample hold
*/

		for (mask_i=0;mask_i<5;mask_i++)	{							// calc detune , slow ,also creates notes

	if (note[mask_i].pitch) {tune_Accu=sample_Noteadd[MajorNote[note[mask_i].pitch]];   note[mask_i].tuned=(tune_Accu);       } // relies on note channel clear , not good , clear not channel straight after

	}

		//note_tuned[3]=2751;
	} // end of note calcualte

  // calc freq 1/isr or 1/16 per note ,need for pitch bend and so on , change depending on decay

// lfo section

	if ((i&63)==0) {   // calculate lfo maybe 8 times for now , seems to fill up
freq_temp=0;
freq2_temp=0;

//uint8_t i_frac2=(i_frac+7)&7;  //previous value can change shape  , not  bad effect

		for (l=0;l<10;l++){   //current lfo setup , messy
			//LFO[l].rate=potSource[130+l];   //grab dat for now
			freq_temp=LFO[l].rate +1;  // rate. this needs a little log
			freq2_temp=freq_temp*freq_temp;
			freq_temp=freq2_temp/64;
			freq2_temp=freq_temp*tempo_sync ; //correction to one note per cycle ,fixed , maybe loose this

			freq_temp=lfo_accu[l][i_frac]+ freq2_temp;// get lfo value plus rate , will try to get related to tempo for easier sync , at potS 80?/8192/8notes/ 1 bar


//			if ((tempo_count<50) && ((next_isr&15)==15)) freq_temp=0;   // trying retrigger
		if (freq_temp>16384) lfo_accu[l][i_frac]=freq_temp-16384; else lfo_accu[l][i_frac]=freq_temp; // write back value
		freq_temp=lfo_accu[l][i_frac]; // 0-255 limit + above zero
		freq_temp=freq_temp*0.000383495;  // 0-255 , chang this for depth
		freq2_temp =arm_sin_f32(freq_temp); // seems ok   , cmsis is ok
		freq_temp=freq2_temp*LFO[l].depth*51;   // depth
		if (freq_temp>8195)  freq_temp=8195;
		if (freq_temp<-8195)  freq_temp=-8195;   // clip to 13bit
		lfo_out[l] [i_frac]=freq_temp+8195; // ok now     , 8 steps per i loop , 14 bit
		LFO[l].out[i_frac]=lfo_out[l] [i_frac];  // just copy for now
		} // lfo gen : 0=f1 , 1=tempo,2=pitch


			freq_pointer[0] [i_frac]=lfo_out [0][i_frac]*0.00006435; // problem was selecting accu instead of out , good now
		freq_pointer[2] [i_frac] =lfo_out [3][i_frac]*0.00006435;; // filter lfos

	}



}  // end of note  i loop
 //make sure it's finished

// filter loop
int32_t play_holder1[512];    // data banks
int32_t play_holder2[512];
uint8_t sine_zero;

for (i=0;i<512;i++) {    // this should write 512 bytes , or about 15ms buffer ,oscillators
	i_total=i+sample_pointB;
	i_frac=(i>>6);
// every step   1,110,928   >>20  ,per note
// New oscillators , sync, trigger input , waveshape ,zero cross
	sample_accus[0] = sample_accus[0] + note[0].tuned; //careful with signed bit shift,better compare

	if (sample_accus[0]>524287) sample_accus[0] =-sample_accus[0] ; // faster >  than &  ,strange



	sample_accus[1] = sample_accus[1] + note[1].tuned;  // normal adder full volume
		//	if (!(note[].pitch[0]))   sample_accus[1] =0;  // turn off with vel now , maybe use mask
			if (sample_accus[1]>524287) sample_accus[1] =-sample_accus[1] ; // faster >  than &  ,strange

			sample_accus[2] = sample_accus[2] + note[2].tuned;
			//		if (!(note[].pitch[0]))   sample_accus[2] =0;  // turn off with vel now , maybe use mask
					if (sample_accus[2]>524287) sample_accus[2] =-sample_accus[2] ; // faster >  than &  ,strange

					sample_accus[3] = sample_accus[3] + note[3].tuned; // bouncing somewhere
					//sample_accus[3] = sample_accus[3] +4000;
					//	if (!(note[].pitch[0]))   sample_accus[3] =0;  // turn off with vel now , maybe use mask
							if (sample_accus[3]>524287) sample_accus[3] =-sample_accus[3] ; // faster >  than &  ,strange

							sample_accus[4] = sample_accus[4] + note[4].tuned;
								//	if (!(note[].pitch[4]))   sample_accus[4] =0;  // turn off with vel now , maybe use mask
									if (sample_accus[4]>524287) sample_accus[4] =-sample_accus[4] ; // faster >  than &  ,strange

									sample_Accu[2] = 0;sample_Accu[0] =0;sample_Accu[3] =0; //all zeroed
									//if (sample_accus[2]<0) sample_Accu[2]=+sample_accus[2]; else sample_Accu[2]=sample_accus[2]; // convert to triangle ?
									sample_Accu[0]=sample_accus[2]>>7; // needs cut a bit

							sample_Accu[0] = ((sine_out+sample_Accu[0])*cross_fade[1]);   // sine input plus other
							//sample_Accu[0] = (sine_out*cross_fade[1]);  // sine out only
									//if (sample_accus[3]<0) sample_Accu[3]=+sample_accus[3]; else sample_Accu[3]=sample_accus[3]; // convert to triangle
									sample_Accu[3]=sample_accus[3];
									sample_Accu[2] = (sample_Accu[3]*cross_fade[2]);			//27b, 2 out f2  might do a crossfade here using pot 3

									//	sample_Accu[5] = sample_Accu[4]+ (sample_accus[4]*4);			// drum and envelope

	//	sample_Accu=sample_Accu-(1<<21);


if (sine_counterB==0) 	sine_temp2=note[5].tuned;

	sine_counterB=sine_counterB+sine_temp2 ;  // sine up counter per cycle , however sine adder needs to wait
	if (sine_counterB>>7) sine_zero=0; else sine_zero=1;

if (sine_counterB>(sine_length<<5)) sine_counterB=0; //fixed for now
sine_count(); // calc sine   distortion out when hcagning note
play_holder1[i]=sample_Accu[0];  // write to bank
play_holder2[i]=sample_Accu[2];


} // end of osc , doing some sound



int32_t filter_Accu;



//uint16_t* click=&input_holder[0];

uint16_t crap_hold=2000;

				uint16_t crap_hold1=2000;
				uint16_t crap_hold2=2000;




for (i=0;i<512;i++) // 15-20 tmr cycles (174)
			{




				crap_hold=((input_holder[i]*7)+crap_hold2)>>3;
					crap_hold1=((crap_hold*7)+crap_hold1)>>3;
					input_holder[i] =((crap_hold1*7)+crap_hold2)>>3;


			}

/*
for (i=0;i<510;i++){
	temp3_hold[0]=((temp3_hold[2]*7)+input_holder[i])>>3;
	temp3_hold[1]=((temp3_hold[0]*7)+input_holder[i+1])>>3;
	temp3_hold[2]=((temp3_hold[1]*7)+input_holder[i+2])>>3;

	input_holder[i]=temp3_hold[0];
	input_holder[i+1]=temp3_hold[1];
	input_holder[i+2]=temp3_hold[2];


}
*/



for (i=0;i<512;i++) {    // this should write 512 bytes , or about 15ms buffer ,oscillators , filters and final out
	i_total=i+sample_pointB;
i_frac=(i>>6);

// filter 1
if (		(note_toggler[i>>5]	)==(1<<(i&31)	)) 				{ADSR[0].attack_trigger =0;  trigger_counter++; trigger_counter=trigger_counter&1023  ;}

//if (feedback_out>0xFFFF) feedback_out=0xFFFF; else if (feedback_out<-65535) feedback_out=-65535;  // limiter to 16 bits
//sample_Accu[1]=input_holder[i];



//sample_Accu[1]=(sample_Accu[1]-2020)<<14; // shift to correct level



//sample_Accu[1]=sample_Accu[1]-60000;
sample_Accu[1]=play_holder1[i];  // sine input
sample_Accu[3]=play_holder2[i] >>5; // sine

// this section is about 100 tmr cycles

freq_point[0]=freq_pointer[0] [i_frac];; // load up coeffs
//freq_point[1]=freq_pointer[1] [i_frac];
freq_point[2]=freq_pointer[2] [i_frac];  // ok , array was too short
//freq_point[3]=freq_pointer[3] [i_frac];
//freq_point[0]=0.5;  //was ok with this
	//	freq_point[2]=0.5;


adsr_level[3] = adsr_lut	[i>>1];


if (freq_point[0]>1) freq_point[0]=1; else if (freq_point[0]<0) freq_point[0]=0;// just in case
		//freq_point[0]=0.50;
		freq_point[1]=1-freq_point[0];
		sample_Accu[1] = sample_Accu[1]>>13;



		//int16_t  ADSR[0].buffer_temp2=lfo_out [1] [i>>6];

		//ADSR[0].buffer_temp2=ADSR[0].buffer_temp2-8195;
		//sample_Accu[1] = sample_Accu[1] *lfo_out [0] [i>>6];     // vol lfo
		sample_Accu[1] = sample_Accu[1] *LFO[0].out[i>>6];



		//	filter_accus[1]=sample_Accu[1]+((filter_hold[0])*0.5); // saw
	//	filter_accus[1]=	filter_accus[1]*adsr_level[3][i_frac];
		filter_accus[1]=	filter_accus[1]*adsr_level[3];

		filter_accus[2]=(filter_accus[1]*freq_point[0])+(filter_accus[2]*freq_point[1]);					// maybe allow bandpass insted of lpf
		filter_accus[3]=(filter_accus[2]*freq_point[0])+(filter_accus[3]*freq_point[1]);
		filter_accus[4]=(filter_accus[3]*freq_point[0])+(filter_accus[4]*freq_point[1]);
		filter_accus[5]=(filter_accus[4]*freq_point[0])+(filter_accus[5]*freq_point[1]);
		filter_hold[0]=(filter_accus[5]+filter_accus[11])*0.5; //half sample , nice
		sample_Accu[0] =filter_accus[5]; // out
		filter_accus[11]=filter_accus[5]; //write back new value
		//sample_Accu[0] =sample_Accu[1];

		//filter 2
		sample_Accu[3]=play_holder2[i] >>5; // sine


				if (freq_point[2]>1) freq_point[2]=1;

				freq_point[3]=1-freq_point[2];
				filter_accus[6]=sample_Accu[3];
			//	filter_accus[6]= filter_accus[6]*adsr_level[3]; // add adsr envelope

				filter_accus[7]=(filter_accus[6]*freq_point[2])+(filter_accus[7]*freq_point[3]);
				filter_accus[8]=(filter_accus[7]*freq_point[2])+(filter_accus[8]*freq_point[3]);
				filter_accus[9]=(filter_accus[8]*freq_point[2])+(filter_accus[9]*freq_point[3]);
				filter_accus[10]=(filter_accus[9]*freq_point[2])+(filter_accus[10]*freq_point[3]);
				filter_hold[1]=(filter_accus[10]+filter_accus[12])*0.5; //half sample
				sample_Accu[2] =filter_accus[10]; //out
				filter_accus[12]=filter_accus[10]; //write back new value


filter_Accu=0;
//filter_Accu=(sample_Accu[0]+sample_Accu[2])>>8; //filter + drum out


//filter_Accu=(sample_Accu[1]+sample_Accu[3])>>8; //filter + drum out ,clean out
 filter_Accu=sample_Accu[1]>>7;

// filter_Accu=sample_Accu[2]>>11;
//filter_Accu=(sample_Accu[1]>>7)+(sample_Accu[3]>>8); //filter + drum out
 if (one_shot!=199)   one_shot++;  //play one attack then stop

 if (filter_Accu>0xFFFF) filter_Accu=0xFFFF; else if (filter_Accu<-65535) filter_Accu=-65535;  // limiter to 16 bits


 play_sample[i_total]=(filter_Accu>>6)+1024;   // final output disable for now 2544

 //play_sample[i_total]=(input_holder[i]);  // works good

} // end of filer


//time_final=time_proc;   // in samples

if (bank_write) {time_final[1]=time_proc; error_count++;};  // tick timer in samples ,normal max 20 but some 230
if (time_proc>100)  	time_final[0]=time_proc;


//bank_write=0;   /// total 320 sample time (39khz)
}




void lfo(void){ // save this for another page


}
void adsr(void){						// pretty terrible now .better just write a lut






}





void mask_calc(uint8_t mask_select,uint8_t mask_speed){    //calculate mask output from lfos
uint8_t mask_temp;
uint8_t mask_tempB;

//(lfo_mask[mask_select]>>((seq.pos>>mask_speed) &7)& 1) ;
mask_temp=(seq.pos>>mask_speed) &7 ; // 0-8
mask_tempB=	lfo_mask[mask_select]>>mask_temp;


mask_result=mask_tempB &1;


}

void sine_count(void) {         // sine_out is the output 9  bit  , works

int32_t sine_tempA;
int32_t sine_tempB;
int8_t sine_frac;


sine_counter=(sine_counterB>>5);  // up countr controlled by counter
sine_frac=sine_counterB & 31;  // grab last 5 bits, actual position for linear interpol,fractional

	if (sine_counter>sine_length) sine_counter = sine_length;		// seems to be faster than using a for loop to calculate both values

	sine_out = sine_block[sine_counter];  // 0- 40000
	sine_tempA=sine_out; // grab first value , needs to be always plus
	sine_tempA=sine_tempA-20000; //convert to signed

	sine_counter++;
	if (sine_counter>=sine_length)  sine_counter=0; // set to sample length

			sine_out = sine_block[sine_counter];  // grab second value

		sine_tempB=sine_out; // grab first value
			sine_tempB=sine_tempB-20000;  // convert to signed and +256 to -256

			sine_tempB=	sine_tempB-sine_tempA;   // calculate fraction then add
			sine_tempB=sine_tempB>>5; // div 32 or 32 upsample

			sine_tempB=sine_tempB*sine_frac; // mult by steps , can overshoot !!

			sine_out=(sine_tempA+sine_tempB);   // add back to start value

			//sine_tempA=sine_tempA; //needs

			//sine_out=sine_tempA;


}


void print_pot(void){


/*
	printf("%d",bsr_out);
printf(",");
printf("%d",spi_byte);
printf(",");
printf("%d",clk_pin);
printf(",");
printf("%d",i);
printf("\n");
*/
	//for (n_t=0;n_t<128;n_t+=1) {
//printf("%d",(potValues[n_t] +(potValues[n_t+1]<<4)));
	//	printf("%d",bsr_out);
//printf(",");
//printf("%d",spi_hold);
//printf("\n");
//printf("%d",potSource[n_t]);
//printf(",");
//	}// print all into 64bytes 4bit+4bit
	//printf("%d",bsr_out);
//	printf(",");
//	printf("%d",note[].pitch[11]);

}
