// disableplay related data

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

	}	//encoder section


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
	//-------------------------------------------------
	if ((disp_stepper==0) || (disp_stepper==1))   // repeat first character
	{
		uint16_t init_holder=init_b;
		init_b= menu_title_lut[enc_out1];




	// this only grabs menu_title_count (&255)      , problem maybe here
		//	if (init_b>(feedback_loc+3) ) init_b=init_b+16;// skip feedback line
		if ((init_b&896)!=(init_holder&896)) {display_fill(); gfx_clear_flag=1;last_pos_hold=0; } // detect x over , not perfect

		lcd_out3=*menu_vars_var;
		div_limit=lcd_out3;
		if (lcd_out3>10)							div_limit= lcd_out3>>2;
		if (lcd_out3>40)							div_limit= lcd_out3>>5;

		default_menu3[init_b]=div_limit+48; lcd_temp=lcd_out3; enc_dir=lcd_temp;       } // force enc_dir

	//--------------------------------------------------------

	if (disp_stepper>4) {default_menu3[feedback_loc+5]=menu_index_list[enc_out1<<1];   	default_menu3[feedback_loc+6]=menu_index_list[(enc_out1<<1)+1];}   // index display
	//-------------------------------------------------------------
	if ((target_display) &&   (disp_stepper>4))      // write LFO.target display , might use it for other things too
	{
		uint8_t target_tmp1=*menu_vars_var ;
		if (target_tmp1>=menu_lookup_count) target_tmp1=0;    // check in case
		if (target_display==1) memcpy(default_menu3+feedback_loc+8, *(menu_titles_final+target_tmp1),8);  // copy info for LFO
		if (target_display==2) memcpy(default_menu3+feedback_loc+8, *(patch_inputs+target_tmp1),8);  // Limited atm
		 		 	}
	//--------------------------------------------------------
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

				gfx_ram[init_y+(d_count*18) ]= gfx_char[d_count+store_x]^31; //write character to ram ,should be elsewhere , blank is correct
			}

		else for (d_count=0;d_count<8;d_count++){
			gfx_ram[init_y+(d_count*18) ]= gfx_char[d_count+store_x]; //write character to ram ,should be elsewhere , seems affected by later stufff
		}

if (disp_stepper==17) {disp_stepper=0;enc2_add=0;  }     else disp_stepper++;				// count to 16 also make sure full loop before skip lines


}    // displayBuffer2

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

void gfx_reverse(uint8_t cursor_pos,uint8_t cursor_partial){					 // does what it supposed to

   uint16_t cursor_loc= (cursor_pos&15)+2;

   uint16_t start_line=(cursor_pos>>4)*144;
    uint16_t end_line=start_line+144;
    uint8_t partial_counter=0;

    for (n=start_line;n<end_line ;n=n+18)    {
    				//for (i=2;i<18;i++)
    		if (cursor_partial&(1<<partial_counter))		     gfx_ram[n+cursor_loc] =gfx_ram[n+cursor_loc]^192;
    		partial_counter++;
    				 }

}

