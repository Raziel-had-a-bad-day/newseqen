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
		HAL_SPI_Transmit(&hspi2, (uint8_t *)spi_store, 3, 100);  // working good


//HAL_Delay(10);
	spi_enable=1; }
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)    // unreliable

{

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


if (init<6)	{HAL_Delay(30);spi_hold=disp[init] ;  init_b=cursor_menu[2]; } //delay needs to go

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
		  /*
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
			HAL_SPI_Transmit(&hspi2, (uint8_t *)spi_store, 3, 100);  // working good ,blocking


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
uint8_t lfotarget_menu[20]={0,40,16,8,40,40,48,56,64,72,0,0,0,0,0,0,0,0,0,0};  // keep lfo target list here for now *8
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

if (disp_stepper>10) store_c=feedback_line[disp_stepper];  /// just the lcd out  values at end of line 8


	// all this can be simplified
	 if (store_c==64) store_c=47;  //EMPTY SPACE
	if ((store_c>127)&& (store_c<255))  {lcd_out3=potSource[store_c-128] ;     store_c= potValues[store_c&127]+48;															}		// NORMAL POTVALUES 0-127
	if ((store_c>254)	&& (store_c<384)) 											{lcd_out3=potSource[store_c-128] ;store_c= potValues[store_c-128]+48;}  // POTVALUES 128+
//	if ((store_c>127)	&& (store_c<384)) 	{store_c= potValues[store_c&127]+48;}		// NORMAL POTVALUES
	if (store_c>511)		{store_c=store_c&127;			store_c= menuList2		[     (lfotarget_menu[   ((store_c>>3))  ])	+(store_c&7)]						; } // VARIABLE MENU ITEMS CHAR LUT

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


uint8_t t_ac;

uint8_t mask_i;
uint8_t mask_k;
uint8_t adsr_mult[5];
//adc_read();
//uint16_t isr_tempo=isrMask; // get tempo value
sample_pointB=sample_pointD;
unsigned short tempo_start=0;  // enabled when i=isrMask;
static unsigned short tempo_end=0;   // last count before note start
uint16_t i_total;
uint16_t tempo_mod=tempo_lut[potSource[109]];  // set tempo,speed from lut 40-200bpm  ,changed to 4x for note lenght

uint8_t l;			// 35.002khz(0.02857ms) sample, 1 sample is temp count (16x=0.00045712) , *16=1 note ,at 300 (437bpm),(1/(0.00002857*tempo count*16)=1beat in s
float freq_temp;	// (1/(bpm/60)) /0.00045712=tempo count ie 1093.8 for 120bpm
float freq2_temp;
float freq_adder;
float tempo_sync=16384/((tempo_mod*16)/512) ; // 8000 at slowest 15.625 updates to lfo at 1 note 16384/15.625=1048.576+ per update  at setting 80
tempo_sync=tempo_sync/80;  // bit weird her , this is adsr !

uint32_t  note_toggler[17]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};  //records note position on 0-512   using a bit
for (i=0;i<16;i++) {  note_toggler[i]=0; }
	//potSource[150]=(freq_point[0])*100; //0-2

//float lcd_out2;

//lcd_out3=menu_page[1]; // still goes to 15

//lcd_out3=adc_values[0]+adc_values[1]+adc_values[2]; // 3 digit read out , works ok,, [2] works but thats it
//lcd_out3=lcd_out3+180;
potSource[150]=(lcd_out3/100)*16;  // still works
potSource[151]=((lcd_out3 %100)/10)*16;		 // 0-160 to 0-10
potSource[152]=(lcd_out3%10)*16;


unsigned short  sine_zero;  // zero cross
note_holdA=0;
//printf ("crap \n");
//printf("%d|",adc_values[0] );printf("%d|",adc_values[1] );printf("%d|",adc_values[2] );printf("%d|\n",adc_values[3] );
uint8_t note_patterns[8]={1,4,2,2,1,4,2,1,4,2,1,4,4};   // creating beats
uint8_t note_lenght=5-note_patterns[seq_pos&7] ; // note length modifier , higher faster
//tempo_mod=tempo_mod-63+(lfo_out[1]>>7);
//if (tempo_mod<450) tempo_mod=((tempo_mod-200)>>1) +200; // more res lower
uint8_t note_plain;
int8_t ring_mod=0;
//sample_Accu2=0;
//printf (adc_values[0]," ",adc_values[1]," ",adc_values[2]," ","crap \n");
// some good phasin and delays here
uint8_t cross_fade[2];
uint8_t fader[17]={0,1,5,11,19,28,39,51,64,76,88,99,108,116,122,126,127}; // sine curve for cross fade

if(adc_values[2]&16)     	{cross_fade[1]=127-fader[adc_values[2]&15]; cross_fade[2]=127;}  else {cross_fade[2]=fader[adc_values[2]&15]; cross_fade[1]=127;} //calculate crossfader

// doing lfo calc here as it is slow only for now
////////////////////adsr/////////////////////////////////////////
if	 (adsr_temp==0) {		adsr_att=(161-potSource[20] ) *0.02 ; // for now all of them from this only , speed , 0-16  // rarely read
as_sustain=((161-potSource[21])*0.01);  // length and level this is ok is running 1/16 ish				as_attack=as_attack-as_sustain;
adsr_att=adsr_att*adsr_att;
as_sustain=as_sustain*as_sustain;

adsr_temp=1;
as_attack=0;
}

float lut_temp;

for (i=0;i<256;i++) {

	if     (as_attack<1000)    																		{as_attack=as_attack+adsr_att;					lut_temp=as_attack; } //0-1000
	 if  ((as_attack<1500)  && (as_attack>999))  										{as_attack=as_attack+adsr_att;					lut_temp=1500-(as_attack-500);  }  // 1000-500
	 if ((as_attack>1499)   && 		(as_attack<2000)) 																{		lut_temp=500; 	as_attack=as_attack+as_sustain;}
	if ((as_attack>1999)  &&  (as_attack<2500)	)																							{as_attack=as_attack+as_sustain	;	lut_temp=2500-as_attack; } //500-0;
	if (as_attack>3000)   																																			{lut_temp=1; as_attack=4000; }     // THE END

adsr_lut[i]= lut_temp*0.001;
}


///////////////////////////////////////////////////////////////

for (i=0;i<512;i++) {    // this should write 512 bytes , or about 15ms buffer ,works fine, too much scope

	i_total=i+sample_pointB;
	i_frac=i>>6;
	note_plain=potValues[seq_pos & 7 ];
potValues[i&255]=potSource[i&255]>>4; //just to update values
	if (tempo_count>=tempo_mod) { next_isr=(next_isr+1)& 4095;tempo_count=0;  }  else {tempo_count++; }  //trigger next note , actual next step for isrCount(future)  8ms,trying to fix slow down here  8000 too  much, adsr clears note info
// tempo_count is about 1000-400
	tempo_start=0;
	if ((next_isr>>4) != (seq_pos)) { 					// next note step 140ms
		seq_pos=(next_isr>>4); // seq pos =256 max , isr = 1/16 of a note, note lenght is 1-4
		tempo_start=1;
// record note triggers or seq_changes position ,NEEDS TO BE OFF FOR NOTE 0
}


	if(tempo_start  )    // Calculates only on note change, gotta change seq_pos somehow  , only activates when change in seq pos
	{
	//printf("\n");//	ITM_SendChar( 65 );   //  Send ASCII code 65 = ’A’
	//printf("%d" ,note_channel[10]);


		potValues[32]=(adc_values[0]>>2)&15; //assigned pots to start of loopers 0-16,works
		potValues[33]=(adc_values[1]>>2)&15;


		seq_loop[2]=((potValues[32]+(seq_pos&7))&15); // calc  8 note loop positions sets looping point in sequence

		//seq_loop[3]=(potValues[33]+(( seq_pos&31 ) >>2)) & 15;  // quater speed
			seq_loop[3]=((potValues[33]+(seq_pos&15))&15); //sets looping point in sequence this is full 16 note

			seq_loop[4]=((potValues[32]+(seq_pos&7))&15);

		//seq_loop[4]=((potValues[32]+((seq_pos&15)>>1))&15); // half speed

		note_channel[2]=potValues[80+seq_loop[2]]+potValues[72];  //loop 8 notes from pos and x times
		note_channel[3]=potValues[seq_loop[3]];  //loop 8 notes from pos and x times ,might disable normal adsr completely
	if (note_channel[3]) 		{note_channel[3]=note_channel[3]+potValues[73];	adsr_retrigger[3]=1; note_toggler[i>>5]=1<<(i&31   )   ; } // stay at zero for off
//	if ((note_channel[3]) && (adsr_retrigger[3]==1))		adsr_retrigger[3]=0;   // while note on , turn of trigger
//	if ((note_channel[3]) && (adsr_retrigger[3]==0))	  adsr_retrigger[3]=1;

//	if (!note_channel[3]) 	adsr_retrigger[3]=0;  // end note


	//note_channel[3]=(note_channel[3]-4)+(lfo_out[2]>>11);

	//if (((seq_pos&7)==0) && (adsr_toggle[6]==2))		{adsr_retrigger[6]=1; } else adsr_retrigger[6]=0; // delete


	note_channel[5]=potValues[80+(seq_pos&15)];  // sample


	if ((note_channel[5]) && (adsr_toggle[5]==2)) {note_holdB=note_channel[5]; one_shot=0;}  // grab note when on ,one shot also , also delete


	note_holdB=potValues[80+seq_loop[2]]+(potValues[74]);  //

	note_holdB=(note_holdB-4)+(lfo_out[2][i_frac]>>11);  //no go with float



	note_holdB=MajorNote[note_holdB];

	sine_adder=sine_lut[note_holdB];	//sets freq ,1.0594  * 16536 =17518  ,
	sine_adder= (sine_adder*1200)>>10;  // modify different sample size , just need single cycle length and thats it
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

		if (note_channel[mask_i]) {tune_Accu=sample_Noteadd[MajorNote[note_channel[mask_i]]];   note_tuned[mask_i]=(tune_Accu);       } // relies on note channel clear , not good , clear not channel straight after

	}


	} // end of note calcualte

  // calc freq 1/isr or 1/16 per note ,need for pitch bend and so on , change depending on decay

// lfo section

	if ((i&63)==0) {   // calculate lfo maybe 8 times for now , seems to fill up
freq_temp=0;
freq2_temp=0;

//uint8_t i_frac2=(i_frac+7)&7;  //previous value can change shape  , not  bad effect

		for (l=0;l<10;l++){   //current lfo setup , messy

			freq_temp=potSource[130+l];
			freq2_temp=freq_temp*tempo_sync ; //correction to one note per cycle ,fixed , maybe loose this

			freq_temp=lfo_accu[l][i_frac]+ freq2_temp;// get lfo value plus rate , will try to get related to tempo for easier sync , at potS 80?/8192/8notes/ 1 bar


//			if ((tempo_count<50) && ((next_isr&15)==15)) freq_temp=0;   // trying retrigger
		if (freq_temp>16384) lfo_accu[l][i_frac]=freq_temp-16384; else lfo_accu[l][i_frac]=freq_temp; // write back value
		freq_temp=lfo_accu[l][i_frac]; // 0-255 limit + above zero
		freq_temp=freq_temp*0.000383495;  // 0-255 , chang this for depth
		freq2_temp =arm_sin_f32(freq_temp); // seems to be working ok till here , not doing right here now
		freq_temp=freq2_temp*potSource[140+l]*51;   // not working  proper something with angles i think

		lfo_out[l] [i_frac]=freq_temp+8195; // ok now

		} // lfo gen : 0=f1 , 1=tempo,2=pitch


			freq_pointer[0] [i_frac]=lfo_out [0][i_frac]*0.00006435; // problem was selecting accu instead of out , good now
		freq_pointer[2] [i_frac] =lfo_out [3][i_frac]*0.00006435;; // filter lfos

	}



}  // end of note  i loop
 //make sure it's finished

// filter loop
int32_t play_holder1[512];    // data banks
int32_t play_holder2[512];


for (i=0;i<512;i++) {    // this should write 512 bytes , or about 15ms buffer ,oscillators
	i_total=i+sample_pointB;
	i_frac=(i>>6);
// every step   1,110,928   >>20  ,per note
// New oscillators , sync, trigger input , waveshape ,zero cross
	sample_accus[0] = sample_accus[0] + note_tuned[0]; //careful with signed bit shift,better compare

	if (sample_accus[0]>524287) sample_accus[0] =-sample_accus[0] ; // faster >  than &  ,strange



	sample_accus[1] = sample_accus[1] + note_tuned[1];  // normal adder full volume
		//	if (!(note_channel[0]))   sample_accus[1] =0;  // turn off with vel now , maybe use mask
			if (sample_accus[1]>524287) sample_accus[1] =-sample_accus[1] ; // faster >  than &  ,strange

			sample_accus[2] = sample_accus[2] + note_tuned[2];
			//		if (!(note_channel[0]))   sample_accus[2] =0;  // turn off with vel now , maybe use mask
					if (sample_accus[2]>524287) sample_accus[2] =-sample_accus[2] ; // faster >  than &  ,strange

					sample_accus[3] = sample_accus[3] + note_tuned[3]; // bouncing somewhere
					//sample_accus[3] = sample_accus[3] +4000;
					//	if (!(note_channel[0]))   sample_accus[3] =0;  // turn off with vel now , maybe use mask
							if (sample_accus[3]>524287) sample_accus[3] =-sample_accus[3] ; // faster >  than &  ,strange

							sample_accus[4] = sample_accus[4] + note_tuned[4];
								//	if (!(note_channel[4]))   sample_accus[4] =0;  // turn off with vel now , maybe use mask
									if (sample_accus[4]>524287) sample_accus[4] =-sample_accus[4] ; // faster >  than &  ,strange

									sample_Accu[2] = 0;sample_Accu[0] =0;sample_Accu[3] =0; //all zeroed
									//if (sample_accus[2]<0) sample_Accu[2]=+sample_accus[2]; else sample_Accu[2]=sample_accus[2]; // convert to triangle ?
									sample_Accu[0]=sample_accus[2]>>7; // needs cut a bit

							sample_Accu[0] = ((sine_out+sample_Accu[0])*cross_fade[1]);   // sine input

									//if (sample_accus[3]<0) sample_Accu[3]=+sample_accus[3]; else sample_Accu[3]=sample_accus[3]; // convert to triangle
									sample_Accu[3]=sample_accus[3];
									sample_Accu[2] = (sample_Accu[3]*cross_fade[2]);			//27b, 2 out f2  might do a crossfade here using pot 3

									//	sample_Accu[5] = sample_Accu[4]+ (sample_accus[4]*4);			// drum and envelope

	//	sample_Accu=sample_Accu-(1<<21);


if (sine_counterB==0) 	sine_temp2=sine_adder;

	sine_counterB=sine_counterB+sine_temp2 ;  // sine up counter per cycle , however sine adder nees to wait
	if (sine_counterB>>7) sine_zero=0; else sine_zero=1;

if (sine_counterB>(sine_length<<5)) sine_counterB=0; //fixed for now
sine_count(); // calc sine
play_holder1[i]=sample_Accu[0];  // write to bank
play_holder2[i]=sample_Accu[2];


} // end of osc , doing some sound



int32_t filter_Accu;

uint16_t temp3_hold[3]={2000,2000,2000};

//uint16_t* click=&input_holder[0];

uint16_t crap_hold=2000;

				uint16_t crap_hold1=2000;
				uint16_t crap_hold2=2000;




for (i=0;i<512;i++)
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


int32_t feedback_out=filter_out[3];
for (i=0;i<512;i++) {    // this should write 512 bytes , or about 15ms buffer ,oscillators , filters and final out
	i_total=i+sample_pointB;
i_frac=(i>>6);

// filter 1
if (		(note_toggler[i>>5]	)==(1<<(i&31)	)) 				{adsr_temp =0;  trigger_counter++; trigger_counter=trigger_counter&1023  ;}

//if (feedback_out>0xFFFF) feedback_out=0xFFFF; else if (feedback_out<-65535) feedback_out=-65535;  // limiter to 16 bits
sample_Accu[1]=input_holder[i];



sample_Accu[1]=(sample_Accu[1]-2020)<<14; // shift to correct level



//sample_Accu[1]=sample_Accu[1]-60000;
//sample_Accu[1]=play_holder1[i];  // sine input

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
		filter_accus[1]=sample_Accu[1];
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
		//sample_Accu[3]=play_holder2[i] >>5; // sine


				if (freq_point[2]>1) freq_point[2]=1;

				freq_point[3]=1-freq_point[2];
				filter_accus[6]=sample_Accu[3];
				filter_accus[6]= filter_accus[6]*adsr_level[3]; // add adsr envelope

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
 filter_Accu=sample_Accu[1]>>8;
//filter_Accu=(sample_Accu[1]>>7)+(sample_Accu[3]>>8); //filter + drum out
 if (one_shot!=199)   one_shot++;  //play one attack then stop

 if (filter_Accu>0xFFFF) filter_Accu=0xFFFF; else if (filter_Accu<-65535) filter_Accu=-65535;  // limiter to 16 bits


 play_sample[i_total]=(filter_Accu>>6)+1272;   // final output disable for now 2544

 //play_sample[i_total]=(input_holder[i]);  // works good

} // end of filer


bank_write=0;
}




void lfo(void){ // save this for another page

	uint8_t n_count;
	uint8_t lfo_speed[5]; // lfo rate , pos exp value
	uint8_t lfo_gain[5]; // gain inc negative for phase on output , maybe allow some clipping as well at the end ,dunno
	uint8_t lfo_offset[5]; // different shapes , triangle for now
	uint8_t lfo_target[5]; // potSource target , various options with limits, maybe some feedback
	//uint16_t lfo_value[5]; // actual lfo value out
	uint8_t lfo_table[7]={0,0,0,0,0,0}; // lookup table for target
	//uint16_t lfo_output[5];

for (n_count=0;n_count<5;n_count++){			//grab variables from potSource , dont use i

lfo_speed[n_count]=potSource[130+n_count];
lfo_gain[n_count]	=potSource[135+n_count];
lfo_offset[n_count]=potSource[140+n_count];
lfo_target[n_count]=potSource[145+n_count];
	}
for (n_count=0;n_count<5;n_count++){

	 lfo_value[n_count]= (lfo_value[n_count]+adsr_lut[lfo_speed[n_count]]);


	if (lfo_value[n_count]>4095) lfo_value[n_count]=0; // limit to 12 bit
	//if (lfo_value[n_count]<-4095) lfo_value[n_count]=0; // limit to 12 bit
	if (lfo_value[n_count]>=2047) lfo_output[n_count]=lfo_value[n_count]-2047; else  lfo_output[n_count]=2047-(lfo_value[n_count]); // up down 0-2047
lfo_output[n_count]=((lfo_output[n_count]*lfo_gain[n_count])>>5)+(lfo_offset[n_count]*51);
if (lfo_output[n_count]>2047) lfo_output[n_count]=2047; else if (lfo_output[n_count]<0) lfo_output[n_count]=0;  // limit output 0-2047
}

potSource[28]=lfo_output[0]>>6;

}
void adsr(void){						// pretty terrible now .better just write a lut
	float note_attack;  //attack length 50/50 , just a default shape that is maybe interpolated , store values for both and position, time in isr
	float note_sustain; //sustaing lenght and height 80/20 / 0-160 0-1-sustain-0  160 is 160 steps(10 notes) 80+80  0+(1/(attack/2))*(attack/2) 1-(1/(attack/2))*(attack/2)+sustain level 1/160*sustain at (1/sustain)*time
	//uint16_t adsr_countup[11];  //holds isr count on notes ,
	//float adsr_level[11]; //float for vol envelope  ,ps 20 21
	uint8_t ad;//counter    0-160-160-160 maybe change 1/10 dunno


	//as_attack=(as_attack*as_attack)*0.01;    //  0-250 log

	// as_attack=adsr_level[3][i_frac] ;


//	 if 	(adsr_level[3][i_frac]>1) adsr_level[3][i_frac]=1;
	//	if 	(adsr_level[3][i_frac]<0) adsr_level[3][i_frac]=0.01;





}





void mask_calc(uint8_t mask_select,uint8_t mask_speed){    //calculate mask output from lfos
uint8_t mask_temp;
uint8_t mask_tempB;

//(lfo_mask[mask_select]>>((seq_pos>>mask_speed) &7)& 1) ;
mask_temp=(seq_pos>>mask_speed) &7 ; // 0-8
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

	uint8_t n_t;
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
//	printf("%d",note_channel[11]);

}
