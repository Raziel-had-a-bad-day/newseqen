
void menu_parses(void){

	uint8_t menu_pass=0;   // variable output
	char menu_string[10];   // incoming string holder (single)

	char menu_string2[10];   // output string
	uint8_t menu_pos1=0;    // menu parsing position

	const char *menu_out1[]={"Tempo","Seq pos","Loop pos","Notes 1" , "Notes 2"," "}; // used for test but also  output comments
	strcpy(menu_string,test_menu[menu_pos1]);    // copy from list

for (i=0;i<6;i++){    	// test a single menu entry

	strcpy(menu_string2,menu_out1[i]);
	switch (i) {
	case 0 :			if  ((strncmp(menu_string,menu_string2,sizeof(menu_string2)))==0) { menu_pass=seq.tempo;  }  ;break; // compare and if true pass var
	case 1 :			if  ((strncmp(menu_string,menu_string2,sizeof(menu_string2)))==0) { menu_pass=seq.pos;  }  ;break;
	case 2 :			if  ((strncmp(menu_string,menu_string2,sizeof(menu_string2)))==0) { menu_pass=seq.loop[i&7];  }  ;break;
	case 3 :			if  ((strncmp(menu_string,menu_string2,sizeof(menu_string2)))==0) { menu_pass=seq.notes1[i&15];  }  ;break;
	case 4 :			if  ((strncmp(menu_string,menu_string2,sizeof(menu_string2)))==0) { menu_pass=seq.notes2[i&15];  }  ;break;
	case 5 :			if  ((strncmp(menu_string,menu_string2,sizeof(menu_string2)))==0) { menu_pass=254;  }  ;break;     // space
	default: menu_pass=255;break;       // no match
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


	//menuSelect = (7 - (adc_values[0] >>2))<<1;		//x *16  main menu select
	//menuSelect = (15 - (adc_values[0] >>1));		//x *7  main menu select
	//menuSelectX=(31 - adc_values[1])>>1;  // Y select inside page
	menuSelect = cursor_menu[2]>>4;		//x *7  main menu select
		menuSelectX=cursor_menu[2]&15;  // Y select inside page




	//	counterVarB = menuSelectX + menuSelect; // select mem page 10*8  and location pointer  77
//		if (adc_values[1]>15)	menu_holder=disp_lut [(menuSelect)	+1] [31-adc_values[1]]; // change back to 0-15
//	else menu_holder=disp_lut [(menuSelect)	] [15-adc_values[1]];  // grab disp lut value for pointer if valid then write for now

		//menu_holder=disp_lut [(menuSelect)	] [(15-adc_values[1])];   // value from disp lut
		menu_holder=disp_lut [menuSelect] [menuSelectX];   // value from disp lut
		cursor_menu[1]=0;
		//cursor_menu[2]=cursor_lookup[enc2_dir];
		// cursor_menu[2]=cursor_lookup[enc2_dir]; //needed for correct line sequence ,obsolete
		cursor_menu[2]=enc2_dir;

		//cursor_menu[2]=(16-adc_values[1]);



		if (menu_holder>127)	counterVarB=menu_holder-128; //  points to actual potvalues location from dsip_lut when value is higher than 127 , works ok problem with menu display
		if (menu_holder>511)	counterVarB=menu_holder-384;		// text pointer , max potvalue address is 142 , fix second page issue later


		enc_dir=potSource[counterVarB];


	enc_temp=(TIM2->CNT)>>1;  // read counter tim2 ,divider ok
	enc2_temp=(TIM4->CNT)>>1;  // read counter tim4
	enc2_temp=enc2_temp&127; // fix overflow ? , dont need a lot because of skip

	if  (enc_temp>enc_tempB)	 enc_dir=enc_dir-(disp_multi[enc2_dir>>4]);   // start settle timer , will do 2 times per turn always, wire opposite
	if  (enc_temp<enc_tempB)	 enc_dir=enc_dir+(disp_multi[enc2_dir>>4]);   // start settle timer , will do 2 times per turn always, wire opposite , step multiplier
	//if (enc_temp<enc_tempB)	 enc_dir++;

  enc2_temp=enc2_lut[enc2_temp];  // force alternative values for skip  , seems to work ok , disable temporarily
  if (enc2_temp>383) enc2_temp=383;  //mem overflow somewhere
	if (enc_dir>160) enc_dir=160;
			if (enc_dir<0) enc_dir=0;
			//if (enc2_temp<16) enc_dir=(enc_dir<<2) & 127; // faster for notes on first row

			potSource[counterVarB]=enc_dir;       // enter data into potsource
					enc_dir=0;
			enc_tempB=enc_temp;
			//if (enc2_dir>383) enc2_dir=383;

			//if  (enc2_temp>enc2_tempB)	 enc2_dir++;   // start settle timer , will do 2 times per turn always
			//if (enc2_temp<enc2_tempB)	 enc2_dir--;
			enc2_dir=enc2_temp; //temp to try source data
		//	if (enc2_dir>127) menu_page[1]=127; else if (enc2_dir>255)   menu_page[1]=255;				else menu_page[1]=0;
		//	menu_page[1]= (enc2_dir >>7) <<7;

			menu_page[1]=enc2_dir&384;


			//if (enc2_dir>255) {menu_page[1]=0;display_fill();}

					if ((enc2_temp>127) && (enc2_tempB<=127)) display_fill();    // need to optimize
					if ((enc2_temp<=127) && (enc2_tempB>127)) display_fill();
					if ((enc2_temp>255) && (enc2_tempB<=255)) display_fill();
					if ((enc2_temp<=255) && (enc2_tempB>255)) display_fill();
					if ((enc2_temp>383) && (enc2_tempB<=383)) display_fill();
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
		 uint16_t disp[]={  304,268,257,262,308,310,310,51,51,52,53,54,55,56,57,58,0};  // this should work for gd
		  if (spi_send==0){         // sets data byte

/*if ((enc2_dir>63) && (menu_page[2]==0))  {init=4;menu_page[2]=1;}  //restart page draw for second page , might need home pos init
if  ((init==76) && (menu_page[2]==1)) menu_page[2]=2; //after drawing second page and finish
if ((enc2_dir<63) && (menu_page[2]==2))  {init=4;menu_page[2]=0;}
*/
switch(init){     //Remember every line advances +char on display ,,all this is mostly unneeded

case 77: spi_hold=384+(init_b>>1);init=init+((init_b&1)*2);break; // no delay ,jump to either blink sets

case 78: spi_hold=10;break; //write one set then jump
case 79: spi_hold=spell[(init_b&62)+1];init=81;break;
case 80: spi_hold=spell[init_b&62];break; // or write other set then jump
case 81: spi_hold=10;init=81;break;


case 82: spi_hold=384+(init_b>>1);break;  // finish writes
case 83: spi_hold=spell[init_b&62];break;
case 84 : spi_hold=spell[(init_b&62)+1];init_b=cursor_menu[2]&63;displayBuffer ();init=76; break; //update cursor and displaybuffer
default : init_b=init-6;displayBuffer ();spi_hold=spell[init_b];break; //initial menu write either page ,skip after
}


if (init<6)	{HAL_Delay(3);spi_hold=disp[init] ;  init_b=cursor_menu[2]; } //delay needs to go

init++;   // after 6 it quits the rest is not needed
spi_send=1;SPI_command();spi_send=0;spi_enable=0;
		  }
/////////////////////////////////////////////////////////////////////////////
		  /*		  if (spi_enable)			// load up one byte , works good
		  {
			 for (samp_temp=0;samp_temp<48;samp_temp++){      // convert bytes to bits
			  clk_pin=(samp_temp&1)^1; // start on high
		  bsrr_longB=bsrr_long>>(((48-samp_temp)>>1));   // need to repeat bit for clock
		  bsr_out=bsrr_longB&1;
		  bsr_out=(bsr_out<<2)+clk_pin; //chmaged for other pin
		 // {GPIOB->BSRR =(bsr_out<<13)+(~(bsr_out)<<29); }  // send spi data when enabled
		  {bsrr_seq[samp_temp] =(bsr_out<<13)+(~(bsr_out)<<29); }  // send spi data when enabled ,fills up bsrr hold
		//  samp_temp++;
		  //	if ((samp_temp)==48)
			 }
				 spi_enable=0;spi_send=2;samp_temp=0; // finish , enable next char
  spi_send=0; // bypass the bsrr stuff

		 while((spi_send==2) && ((sample_point & 3)==3))   {    //send bits for a single command , seem to time alright
		  //if (spi_send==2){ GPIOB->BSRR =bsrr_seq[bsrr_counter];bsrr_counter++;}  // send spi when loaded up , this works , too fast however will fail
	// GPIOB->BSRR =bsrr_seq[bsrr_counter];bsrr_counter++;  // send spi when loaded up , this works , too fast however will fail ,disable for now
			 bsrr_counter++; // add this or stuck
		  //if ((bsrr_counter==49) && (spi_send==2)) {bsrr_counter=0; spi_send=0;}
		  if (bsrr_counter==49)  {bsrr_counter=0; spi_send=0;}
		 }
*/

}
void display_gfx(void){   // new display driver , all gfx based



}

void display_update(void){				//spi display updater code , all gfx , works pretty ok n doesn't need to be running always 18x 128 ,constant scanning

	uint8_t spi_store[5];

switch (gfx_skip) { // 1-17  one row


case 1 : {spi_hold=((gfx_counter[0])&31)+384;gfx_counter[4]=(gfx_counter[0]>>5);  gfx_counter[0]=(gfx_counter[0]+1)&63 ;gfx_skip++;break ;}  // count up positions
case 2  : {	spi_hold=384+(gfx_counter[4]*8);gfx_skip++;break ;  }
case 3   : gfx_counter[3]=0; gfx_skip++;break;
//case 18 : gfx_counter[2]=(gfx_counter[2]+1) &63; gfx_counter[3]=0;gfx_skip=1;break;  //not ideal
default :  gfx_counter[3]=(gfx_counter[3]+1)&15;gfx_skip++;break;

}


if ((gfx_skip==2) || (gfx_skip==3) || (gfx_skip==1)) spi_hold=spi_hold; else spi_hold=gfx_ram[gfx_counter[2]] [gfx_counter[3]] ; // write command or data

if (gfx_skip==19)  {gfx_counter[2]=(gfx_counter[2]+1) &63; gfx_counter[3]=0;gfx_skip=1;}   // fixed last doubled char

		if (spi_hold>>8) spi_byte=248; else {spi_byte=250;}  //start with msb ,dont forget flip around at end, 250 =data ,248= command if below 8bit

		spi_store[0]=spi_byte&255;
		spi_store[1]=((spi_hold>>4)<<4)&255;
		spi_store[2]=((spi_hold&15)<<4)&255;

	// send this to spi for now
			HAL_SPI_Transmit(&hspi2, (uint8_t *)spi_store, 3, 3);  // working good ,blocking , never set time too high  or figure other option maybe DMA


disp_end=gfx_skip+gfx_counter[2]+gfx_counter[3];			// vsynch for displaybuffer


}
void display_fill(void)  {     // full update of gfx memory
loop_counter3=1;
for (n=0;n<2048;n++)	{ //fills up gfx ram or not

enc2_dir=(n>>4)+menu_page[1]; 												// 0-128         +      0,127,255,

displayBuffer();
}
//enc2_dir=menu_page[1];  // end clean
}



void displayBuffer (void){        //  in a constant state of flux

	if (disp_stepper==0) init_b=enc2_dir; else init_b=111+disp_stepper;  // fetch values for last line or cursor

	uint8_t d_count;
uint16_t init_x=((init_b>>4)<<3);    // normal x8 , try other 64 x16
uint8_t init_x2=init_x&63;  // 0-64  character address in gfx
uint8_t init_y=init_b&15;
uint16_t store_x;

// use this to set feedback pointer for now
// just to point the lfo number



//if (enc2_dir<128){
	if (disp_stepper==0) { store_c= disp_lut [init_b>>4]  [init_y] ; feedback_pointer=((enc2_dir>>4)<<3)&127; feedback_line[10]=((enc2_dir&15)>>1)+48;  // pointer and data , runs at every character drawn at cursor


	menulength = sizeof(menu_items) / sizeof(menu_items[feedback_pointer]);  // get length of data on location

	for (i = 0; i < 56; i++)
		{ if (menu_loc[i]==store_c){menu_locA=i;break;}    // gets disp_lut number that points in menu_items

		}

	for (i = 0; i < menulength; i++)
	{ menulist_hold[i+1]=menu_items[menu_locA] [i];


	}   // grab item , this works but slower

	}

	if  ((disp_stepper<11) && (disp_stepper))   {      // 0-9 chars of line 8 , overhead is ok

//{	feedback_line[disp_stepper]=menuList[disp_stepper+feedback_pointer];  // feedback data info standard , per character 0-16

		feedback_line[disp_stepper]=menulist_hold[disp_stepper];  // feedback data modified 0-16

store_c=feedback_line[disp_stepper];//gets potvalues pointer from menus or feedback line , per character
	}

//if (disp_stepper>10) store_c=feedback_line[disp_stepper];  /// just the lcd out  values at end of line 8
if ((disp_stepper>11) &&  (disp_stepper<15)) store_c=potSource[368+disp_stepper]+48;  /// just the lcd out  values at end of line 8
if (disp_stepper>14) store_c=feedback_line[disp_stepper];  /// just the lcd out  values at end of line 8
	// all this can be simplified
	 if (store_c==64) store_c=47;  //EMPTY SPACE
	if ((store_c>127)&& (store_c<255))  {lcd_out3=potSource[store_c-128] ;     store_c= potValues[store_c&127]+48;															}		// NORMAL POTVALUES 0-127
	if ((store_c>254)	&& (store_c<384)) 											{lcd_out3=potSource[store_c-128] ;store_c= potValues[store_c-128]+48;}  // POTVALUES 128+
//	if ((store_c>127)	&& (store_c<384)) 	{store_c= potValues[store_c&127]+48;}		// NORMAL POTVALUES
	//if (store_c>511)		{store_c=store_c&127;			store_c= menuList2		[     (lfotarget_menu[   ((store_c>>3))  ])	+(store_c&7)]						; } // VARIABLE MENU ITEMS CHAR LUT
	if (store_c>511)		{store_c=store_c&127;			store_c= menu_items		[potSource[330]]    	[store_c&7]						; } // VARIABLE MENU ITEMS CHAR LUT
	store_c=store_c-47; store_c = store_c &127;	  // spell no longer ?, store_c changes		STARTS FROM 0

store_x=(store_c*8);  // i line characters , might shrink it and use extr for other  visuals


if (( !loop_counter3) && (disp_stepper==0))     // blinker for cursor character only  , might just flip the whole last line from prev tables then its x4 faster
	for (d_count=0;d_count<7;d_count++){
						gfx_ram[d_count+init_x2] [init_y] = gfx_char[d_count+store_x]^127; //write character to ram ,should be elsewhere , blank is correct
	}
else for (d_count=0;d_count<7;d_count++){
	gfx_ram[d_count+init_x2] [init_y] = gfx_char[d_count+store_x]; //write character to ram ,should be elsewhere , seems affected by later stufff
}





gfx_ram[7+init_x2] [init_y] = 0; // last line is blank between rows or whatever



if (disp_stepper==15) disp_stepper=0; else disp_stepper++;				// count to 16

}


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
potSource[380]=(lcd_out3/100)*16;  // still works   , potsource ref is located in feedback line var
potSource[381]=((lcd_out3 %100)/10)*16;		 // 0-160 to 0-10
potSource[382]=(lcd_out3%10)*16;



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

		note[2].pitch=seq.notes2[seq.loop[2]]+note[2].transpose;  //loop 8 notes from pos and x times
		note[3].pitch=seq.notes1[seq.loop[3]];  //loop 8 notes from pos and x times ,might disable normal adsr completely
	if (note[3].pitch) 		{note[3].pitch=note[3].pitch+note[3].transpose;	adsr_retrigger[3]=1; note_toggler[i>>5]=1<<(i&31   )   ; } // stay at zero for off
//	if ((note[].pitch[3]) && (adsr_retrigger[3]==1))		adsr_retrigger[3]=0;   // while note on , turn of trigger
//	if ((note[].pitch[3]) && (adsr_retrigger[3]==0))	  adsr_retrigger[3]=1;

//	if (!note[].pitch[3]) 	adsr_retrigger[3]=0;  // end note


	//note[].pitch[3]=(note[].pitch[3]-4)+(lfo_out[2]>>11);

	//if (((seq.pos&7)==0) && (adsr_toggle[6]==2))		{adsr_retrigger[6]=1; } else adsr_retrigger[6]=0; // delete


	//note[5].pitch=potValues[80+(seq.pos&15)];  // sample


	//if ((note[5].pitch) && (adsr_toggle[5]==2)) {note[5].pitch=note[5].pitch; one_shot=0;}  // grab note when on ,one shot also , also delete


	note[5].pitch=seq.notes2[seq.loop[2]]+(note[5].transpose);  //





	note[5].pitch=MajorNote[note[5].pitch];
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
