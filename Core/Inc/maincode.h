uint8_t*  menu_vars(char* menu_string,  uint8_t var_index   ){ // in comes name and index , out goes variable reference pointer


	char menu_string2[8];
	var_index=var_index&15;    //set var index ie  LFO[1].rate
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
	case 5:     menu_vars_var1= &patch[var_index].target    ; break;
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
	case 35:     menu_vars_var1= &patch[var_index].target_index    ; break;
	case 36: 	menu_vars_var1=&patch[var_index].input1 ;break;
	case 37: 	menu_vars_var1=&patch[var_index].input2 ;break;
	case 38: 	menu_vars_var1=&patch[var_index].in_mix ;break;
	case 39: 	menu_vars_var1=&patch[var_index].in_offset ;break;
	case 40:     menu_vars_var1= &LFO_slave1[var_index].rate   ; break;
	case 41:     menu_vars_var1= &LFO_slave1[var_index].depth    ; break;
	case 42:     menu_vars_var1= &LFO_slave1[var_index].delay    ; break;
	case 43:     menu_vars_var1= &LFO_slave1[var_index].offset    ; break;
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

			if ((menu_counter>110 )&&(menu_counter<128 )) menu_counter=menu_counter+16;   // skip to second page
			if((menu_counter>237)&&(menu_counter<256 )) menu_counter=menu_counter+16; // skip
			if((menu_counter>365)&&(menu_counter<384 )) menu_counter=menu_counter+16; // skip
			if((menu_counter>493)&&(menu_counter<512 )) menu_counter=menu_counter+16; // skip
			if((menu_counter>621)&&(menu_counter<640 )) menu_counter=menu_counter+16; // skip
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
		for (n=0;n<10;n++){

			uint16_t* output_hold;
			uint8_t input_hold=patch[n].input1;

			if (patch[n].input1>=menu_lookup_count) patch[n].input1=0;    // limit

			switch(input_hold&3){     // lfo now , can add adsr later

						case 0:   output_hold=&LFO[input_hold>>2].out[0];break;
						case 1:   output_hold=&LFO[input_hold>>2].out_saw[0];break;
						case 2:   output_hold=&LFO[input_hold>>2].out_tri[0];break;
						case 3:   output_hold=&LFO[input_hold>>2].out_tri[0];break;
						}

			patch[n].in1_ptr=output_hold;   // sets input pointer to first sample , default is lfo[0].out [0]


			if (patch[n].target) {  // test if above zero


				uint8_t target_input=patch[n].target; // copy to avoid messed up pointer
			for(skip=target_input ;skip<menu_lookup_count;skip++){
					if (patch_skip_list[target_input]==1)  target_input++;

				}  // test against list
				if (target_input>(menu_lookup_count-1)) target_input=menu_lookup_count-1;
				if (target_input!=35)  {     // make target index is not selected


			patch[n].target=target_input; // write back corrected value

			uint8_t target_index=patch[n].target_index;

			if (target_index>menu_vars_index_limit[target_index]  )   // test limit
			{	target_index=menu_vars_index_limit[target_index]; }
			patch[n].target_index=target_index;
			uint8_t*  target_out_ptr= menu_vars(menu_titles_final[target_input] , target_index    );

			if (target_out_ptr)           {patch[n].out_ptr =target_out_ptr;     // write ptr
			patch[n].limiter=menu_vars_limiter[menu_vars_ref]; }
				}
				else patch[n].target=0;  // write back 0 if failed



			}
		}

	}

void patch_target_modify(void){					// modify original value  careful position ,not using it now  ,ok

	uint8_t loop_position=sampling_position&7;    // 0-7 , this comes usually from 0-512 loop / 64
	for (n=0;n<10;n++){


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


	}

}
void patch_target_replace(void){					// sttaight value replace  ,ok
	uint8_t loop_position=sampling_position&7;    // 0-7 , this comes usually from 0-512 loop / 64

	for (n=0;n<10;n++){



		if (patch[n].target) {         // check first for enable

			patch[n].output[loop_position]=*(patch[n].in1_ptr+(loop_position));   //write output here


			uint8_t  *ptr_to_modify =patch[n].out_ptr;       // select address , not always 8 bit ,ok
			uint16_t lfo_out_temp=  (patch[n].output [loop_position])>>8;  // 0-256,
			uint8_t lfo_mod1=ptr_to_modify; //ok

			uint8_t  var_replaced =  lfo_out_temp &255 ;   // grab lfo out *    data to be modfied

			if (var_replaced>patch[n].limiter) var_replaced=patch[n].limiter;  // limit lfo output

			*ptr_to_modify =var_replaced;   // replace original value,ok

		}


	}

}



void SPI_command(void){
// pins are all good , SPI is always the problem!!!
if ((spi_enable==0)&& (spi_send==1)){				//if data sent and next byte is ready ,creates the actual bytes sent

uint8_t spi_store[5];

	//clk_pin=(i&1)^1;
		//bsrr_long=0;
		if (spi_hold>>8) spi_byte=248; else {spi_byte=250;}  //start with msb ,dont forget flip around at end, 250 =data ,248= command if below 8bit

	spi_store[0]=spi_byte&255;
	spi_store[1]=((spi_hold>>4)<<4)&255;
	spi_store[2]=((spi_hold&15)<<4)&255;

// send this to spi for now
	HAL_SPI_Transmit(&hspi2, (uint8_t *)spi_store, 3,3
			);  // working good


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
		play_hold=play_sample[sample_point<<1]; // this is 2
		play_hold2=play_sample[(sample_point<<1)+1];
		if(TIM3==htim->Instance)			// nothing here is consistent ?
	{


	TIM3->CCR1=play_hold ;  // keep readin sample storage
	TIM3->CCR2=play_hold2 ;   // load up counter on ch2

	sample_point++; //this needs to be here or too fast and wrong sample rate

	}

	}



void analoginputloopb(void){  //works ,getting obsolete


	menuSelect = cursor_menu[2]>>4;		//x *7  main menu select
		menuSelectX=cursor_menu[2]&15;  // Y select inside page


		cursor_menu[1]=0;

		cursor_menu[2]=enc2_dir;


	enc_temp=(TIM2->CNT)>>1;  // read counter tim2 ,divider ok
	enc2_store[enc2_store_count]=(TIM4->CNT)&255;  // read counter tim4, noisy
	uint16_t enc2_store2=0;
	uint16_t enc2_store3=0;
	if (enc2_store_count==3) enc2_store_count=0; else enc2_store_count++;

	enc2_store2=enc2_store[0]+enc2_store[1]+enc2_store[2]+enc2_store[3];     // average filter hopefully
	enc2_store3=enc2_store2>>3;
	enc2_temp=enc2_store3;

	if  (enc_temp>enc_tempB)	 enc_dir=enc_dir-1;
	if  (enc_temp<enc_tempB)	 enc_dir=enc_dir+1;


  if (enc2_temp>511) enc2_temp=511;  //mem overflow somewhere
	if (enc_dir>255) enc_dir=255;
			if (enc_dir<0) enc_dir=0;

			enc_tempB=enc_temp;

			enc2_dir=enc2_temp; //temp to try source data


					enc2_tempB=enc2_temp; // to effective as counter not getting reset


potValues[counterVarB]=(potSource[counterVarB]>>4) & 15 ;  // reduce values for now ,use original for others , slow count


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

		//  if (init==5) display_fill();
}


void gfx_send(void){         // send spi to lcd from gfx ram


			uint8_t spi_store[3];
			uint8_t spi_store2=0;
			uint8_t spi_store3=gfx_ram[gfx_send_counter];
	if (gfx_send_counter2>1) {spi_store2=250;

			}

			else {spi_store2=248;    }//start with msb ,dont forget flip around at end, 250 =data ,248= command if below 8bit


					spi_store[0]=spi_store2;
					spi_store[1]=((spi_store3>>4)<<4);
					spi_store[2]=((spi_store3&15)<<4);


						HAL_SPI_Transmit(&hspi2,spi_store,3,3);  // ok
					if (gfx_send_counter2==17 ) { gfx_send_counter2=0;    } else gfx_send_counter2++; // check elsewhere if changing gfx_send_counter
			if (gfx_send_counter==1151 ) { gfx_send_counter=0; disp_end=1; gfx_send_counter2=0;    } else gfx_send_counter++; ;

		}

void gfx_clear(void){     // simple gfx ram clear ,besides feedback

	for (n=0;n<1152 ;n=n+18)    {
				for (i=2;i<18;i++){ gfx_ram[n+i] =0;

				}  }
	 gfx_send_swap=0;  // disable line skip
	 gfx_send_counter=0;    //reset page
	 disp_end=0;     //  should disable display process till one page

}


void gfx_send_page(void){    // send a full page , very slow

	gfx_send_counter=0;
	for (n=0;n<1152 ;n++)    {

		 gfx_send();

	 }  }

void display_clear (void){ //  simple character clear on default menu3

	uint16_t disp_counter=0;
	for (disp_counter=0;disp_counter<default_menu3_size;disp_counter++){
		default_menu3[disp_counter]=47;
	}


}
void display_generate(void){    // display character generate, 1+1+16 chars

	gfx_clear_flag=0;
	for (i=0;i<18;i++) {
	display_process();
		  displayBuffer2();} // 5 cycles max for the lot or  0.2ms
	 if(!gfx_clear_flag)   gfx_send_swap=1;   // enable line swapping
	 disp_end=0;   ///reset till next full page

	//  if (!disp_stepper) break;

}

void display_fill(void)  {     // full update of gfx memory, ok
loop_counter3=1;
uint16_t fill_counter= 0;
uint16_t init_b1=init_b;
uint16_t init_bs=init_b&896;   // get msb
fill_counter=fill_counter+init_bs; // add msb from init b

while ((fill_counter&896)==init_bs)   // skip when over a page something  127+
{
	init_b=fill_counter; // upcount

	disp_stepper=1;   // first character
	displayBuffer2();	//read default menu
	fill_counter++;
}
init_b=init_b1; // write back original value
gfx_send_swap=0;  // disable line skip
	 gfx_send_counter=0;    //reset page
	 disp_end=0;     //  should disable display process till one page


	}



void display_process(void){							// keep data processing here
uint16_t feedback_loc=(init_b&896)+107;

	if ((enc2_tempC==enc2_dir) && (!enc2_add))  {
		if ((disp_stepper==5)&&(!target_display))  {enc2_add=2; 	disp_stepper=17;}   	 // wait till enc2_dir  hasn't changed ,jump and then change feedback
			}

	if (disp_stepper>1)	init_b=feedback_loc+16+(disp_stepper);  // write lcd3
	if (disp_stepper>4)	init_b=feedback_loc+(disp_stepper);  // write feedback line

	if ((lcd_temp!=enc_dir)  && (enc2_tempC==enc2_dir) )   {*menu_vars_var=enc_dir;   // enc2_dir =same enc_dir=changed
	lcd_temp=enc_dir;  }



	if ((disp_stepper==0) && (enc2_tempC!=enc2_dir) && (!enc2_add))  {      // wait till loop end , ok

	if  (enc2_tempC<enc2_dir)	 enc2_add=1;   									// use this to set up or down count for variables , might change
		if  (enc2_tempC>enc2_dir)	 enc2_add=-1;    // slow but 1 step at a time
		if  (enc2_tempC<enc2_dir)	 enc2_add=+(enc2_dir-enc2_tempC);   	//  fast but skips

		if  (enc2_tempC>enc2_dir)	 enc2_add=-(enc2_tempC-enc2_dir);


		enc2_tempC=enc2_dir;	   // loop back until change
		if ((enc_out1>=0)  &&  (enc_out1<=menu_title_count))    {enc_out1=enc_out1+enc2_add;}   // count up or down within limits
		if (enc_out1<0)  {  enc_out1=menu_title_count; }
		if (enc_out1>menu_title_count) enc_out1=0;
		enc2_add=2;

	}


	if ((disp_stepper==0) && (enc2_add==2))			// skip if no input for enc2 , also maybe wait till enc2 stopped moving
	{



	uint8_t crap_hold9=(menu_title_lut[enc_out1]>>16)&255;   // look up up menu_titles_final
	if (crap_hold9==5) target_display=1;   // check if LFO.target is on cursor , may use for other things
	else if (crap_hold9==36) target_display=2;   else target_display=0;

	// fetch values for last line or cursor

	 memcpy(default_menu3+feedback_loc+8, *(menu_titles_final+crap_hold9),8);   // copy feedback data for reading,ok
	 memcpy(menu_vars_in,*(menu_titles_final+crap_hold9),8);	// send back for menu vars ok

	 char temp_char[]="  ";
	 memcpy(temp_char,menu_index_list+((enc_out1*2)),2);   // copy char to char,ok
	 menu_index_in=atoi(temp_char)			;   // convert char to int,ok


	 menu_vars_var=menu_vars(menu_vars_in,menu_index_in);		//test  for vars ok


	    // grab value on ptr address , also write first char , ok

	}

	if ((disp_stepper==0) || (disp_stepper==1))   // repeat first character
	{


		uint16_t init_holder=init_b;

		init_b= menu_title_lut[enc_out1];    // this only grabs menu_title_count (&255)      , problem maybe here
		//	if (init_b>(feedback_loc+3) ) init_b=init_b+16;// skip feedback line
		if ((init_b&896)!=(init_holder&896)) {display_fill(); gfx_clear_flag=1; } // detect x over , not perfect

		lcd_out3=*menu_vars_var;
		div_limit=lcd_out3;
		if (lcd_out3>10)							div_limit= lcd_out3>>2;
		if (lcd_out3>40)							div_limit= lcd_out3>>5;

		default_menu3[init_b]=div_limit+48; lcd_temp=lcd_out3; enc_dir=lcd_temp;       } // force enc_dir

	if (disp_stepper>4) {default_menu3[feedback_loc+5]=menu_index_list[enc_out1<<1];   	default_menu3[feedback_loc+6]=menu_index_list[(enc_out1<<1)+1];}   // index display

	if ((target_display) &&   (disp_stepper>4))      // write LFO.target display , might use it for other things too
	{
		uint8_t target_tmp1=*menu_vars_var ;
		if (target_tmp1>=menu_lookup_count) target_tmp1=0;    // check in case
		if (target_display==1) memcpy(default_menu3+feedback_loc+8, *(menu_titles_final+target_tmp1),8);  // copy info for LFO
		if (target_display==2) memcpy(default_menu3+feedback_loc+8, *(patch_inputs+target_tmp1),8);  // Limited atm
		 		 	}

	if (disp_stepper==1)  gfx_send_cursor=(init_b>>4)&7 ;   //send cursor line
	if (disp_stepper==2)  {

		default_menu3[feedback_loc+18]=potSource[380]+48;
		default_menu3[feedback_loc+19]=potSource[381]+48; default_menu3[feedback_loc+20]=potSource[382]+48; }  // write this straight after start ,ok


}   // end o void


void displayBuffer2 (void){       // use only writing characters  ,nothing more  , init_b for selecting location
														//when scrolling maybe use this only until  settled
														// start 2*8 bit then squeeze in 20*6 bit characters

	store_c= (default_menu3[init_b]-47)&127 ;    // grab char from mem
	uint8_t init_bx=init_b&127;

	uint8_t d_count;
uint8_t init_x=(init_bx & 15)+2 ;    // +2 important  2-17 hor char pos
uint8_t init_x2=(init_bx>>4)<<3 ;  // 0,8,16-64  vertical pos  gfx
uint16_t init_y=((init_x2)*18)+init_x;   //   works ok  8  bit  0-1152
uint16_t store_x;

store_x=(store_c*8);  // i line characters , might shrink it and use extr for other  visuals , old code but keep for now


		if ( (disp_stepper==0))     // blinker for cursor character only  , might just flip the whole last line from prev tables then its x4 faster
			for (d_count=0;d_count<8;d_count++){

				gfx_ram[init_y+(d_count*18) ]= gfx_char[d_count+store_x]^127; //write character to ram ,should be elsewhere , blank is correct
			}

		else for (d_count=0;d_count<8;d_count++){
			gfx_ram[init_y+(d_count*18) ]= gfx_char[d_count+store_x]; //write character to ram ,should be elsewhere , seems affected by later stufff
		}

if (disp_stepper==17) {disp_stepper=0;enc2_add=0;  }     else disp_stepper++;				// count to 16 also make sure full loop before skip lines


}    // displayBuffer2


void note_reset (void){          // reset deafult values before modulation , in case it gets left  with no modulator

	note[0].velocity=255;note[1].velocity=255;note[2].velocity=255;note[3].velocity=255;note[4].velocity=255;note[5].velocity=255;note[6].velocity=255
																;note[0].detune=127;note[1].detune=127;note[2].detune=127;note[3].detune=127;note[4].detune=127;
	note[5].detune=127;note[6].detune=127;


}




