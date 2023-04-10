// disableplay related data

void display_process(void){							// keep data processing here




}   // end o void


    void displayBuffer2 (void){       // only for feedback line now
														    //when scrolling maybe use this only until  settled
	uint16_t feedback_loc=(menu_title_lut[enc_out1]&896)+107;														// start 2*8 bit then squeeze in 20*6 bit characters

	uint8_t crap_hold9=(menu_title_lut[enc_out1]>>16)&255;

	memcpy(menu_vars_in,*(menu_titles_final+crap_hold9),8);	// send back for menu vars ok
	 char temp_char[]="  ";
	// char temp_char2[]="________";
				 memcpy(temp_char,menu_index_list+((enc_out1*2)),2);   // copy char to char,ok
				 menu_index_in=atoi(temp_char)			;   // convert char to int,ok

	menu_vars_var=menu_vars(menu_vars_in,menu_index_in);		//test  for vars ok


	     memcpy(default_menu3+feedback_loc+8, *(menu_titles_final+crap_hold9),8);   // copy feedback data for reading,ok
	     memcpy(default_menu3+feedback_loc+17,potSource+380,3);  // lcd out
	     memcpy(default_menu3+feedback_loc+5,temp_char,2);

	     if (menu_vars_ref==5) target_display=1;
	     if (menu_vars_ref==36) target_display=2;
	     if((48<menu_vars_ref)    && (menu_vars_ref<53)) target_display=3;
	 //    if((48<menu_vars_ref)    && (menu_vars_ref<53)) memcpy(LCD_Info+3, *(menu_titles_final+(*menu_vars_var)),8);  // send target for LFO_sqr  to LCD_Info


		 if (target_display)      // write LFO.target display , might use it for other things too
	     	{
	     		uint8_t target_tmp1=*menu_vars_var ;   // this can be put anywhere now
	     		if (target_tmp1>=menu_lookup_count) target_tmp1=0;    // check in case
	     		if (target_display==1) memcpy(default_menu3+feedback_loc+8, *(menu_titles_final+target_tmp1),8);  // copy info for LFO
	     		if (target_display==2) memcpy(default_menu3+feedback_loc+8, *(patch_inputs+target_tmp1),8);  // Limited atm
	     	//	if (target_display==3) memcpy(LCD_Info+3,  *(patch[].target+target_tmp1),8);  // Limited atm
	     		if  (target_display==3)   //  patch feedback works ok
	     		    {    if (LFO_sqr_list[menu_index_in&7])   {memcpy(LCD_Info+3,  menu_titles_final[sqr_target_list[menu_index_in&7]],6);LCD_Info[10]=LFO_sqr_list[menu_index_in&7];

	     		    }  // ok
	     		 else memcpy(LCD_Info+3,"        ",8); //11
	     		    }
	     	}

	     target_display=0;





	     if (disp_stepper==17) {disp_stepper=0;  }     else disp_stepper++;				// count to 16 also make sure full loop before skip lines


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


void gfx_send(void){         // send spi to lcd from gfx ram, not used

    gfx_dma=0;
			uint8_t spi_store[3];
			uint8_t spi_store2=0;
			uint8_t spi_store3=gfx_ram[gfx_send_counter];
	if (gfx_send_counter2>1) {spi_store2=250;

			}

			else {spi_store2=248;    }//start with msb ,dont forget flip around at end, 250 =data ,248= command if below 8bit


					spi_store[0]=spi_store2;
					spi_store[1]=((spi_store3>>4)<<4);
					spi_store[2]=((spi_store3&15)<<4);

					HAL_SPI_Transmit_DMA(&hspi2,spi_store,3);  // ok works in normal transfer mode
					//	HAL_SPI_Transmit(&hspi2,spi_store,3,3);  // ok
					if (gfx_send_counter2==17 ) { gfx_send_counter2=0;    } else gfx_send_counter2++; // check elsewhere if changing gfx_send_counter
			if (gfx_send_counter==1151 ) { gfx_send_counter=0; disp_end=1; gfx_send_counter2=0;    } else gfx_send_counter++; ;

		}
void gfx_send_DMA(void){         // send spi to lcd from gfx ram ,not used now


			uint8_t spi_store[433];
			uint8_t spi_store2=0;
			uint8_t spi_store3=gfx_ram[gfx_send_counter];
			uint16_t  up_counter=0;
			gfx_dma=0;
			for (up_counter=0;up_counter<54;up_counter=up_counter+3){     // no more than one line then needs pause

			    spi_store3=gfx_ram[gfx_send_counter];
			    if (gfx_send_counter2>1) {spi_store2=250;					// 2ms delay between location commands , data is ok full speed

			}

			else {spi_store2=248;    }//start with msb ,dont forget flip around at end, 250 =data ,248= command if below 8bit

					spi_store[up_counter]=spi_store2;
					spi_store[up_counter+1]=((spi_store3>>4)<<4);
					spi_store[up_counter+2]=((spi_store3&15)<<4);

					if (gfx_send_counter2==17 ) { gfx_send_counter2=0;    } else gfx_send_counter2++; // check elsewhere if changing gfx_send_counter
			if (gfx_send_counter==1151 ) { gfx_send_counter=0; disp_end=1; gfx_send_counter2=0;    } else gfx_send_counter++; ;

			} HAL_SPI_Transmit_DMA(&hspi2,spi_store,54);  // ok

}
void gfx_TX_block(void){         // send spi to lcd from gfx ram , full page needed initially as it writes commands as well


			uint8_t spi_store[3];
			uint8_t spi_store2=0;
			uint8_t spi_store3=0;
			uint16_t  up_counter=0;
			uint8_t line_counter=0;

			gfx_send_counter=0;
			for (up_counter=0;up_counter<3456;up_counter=up_counter+3){     // no more than one line then needs pause


			    spi_store3=gfx_ram[gfx_send_counter];
			    if (line_counter>1) {spi_store2=250;					// 2ms delay between location commands , data is ok full speed
			}
			else {spi_store2=248;    }//start with msb ,dont forget flip around at end, 250 =data ,248= command if below 8bit

					spi_store[0]=spi_store2;
					spi_store[1]=((spi_store3>>4)<<4);
					spi_store[2]=((spi_store3&15)<<4);
					memcpy(spi_tx_block+up_counter,spi_store,3);
					if (line_counter==17 ) { line_counter=0;    } else line_counter++; //
			 gfx_send_counter++; ;

			} ;  // ok
			gfx_send_counter=0; disp_end=1;

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
	for (disp_counter=0;disp_counter<1024;disp_counter++){
		default_menu3[disp_counter]=47;
	}


}
void display_generate(void){    // display character generate, 1+1+16 chars
uint8_t counter;
	gfx_clear_flag=0;
//	for (counter=0;counter<18;counter++) {
	display_process();
		  displayBuffer2(); // 5 cycles max for the lot or  0.2ms
	// if(!gfx_clear_flag)   gfx_send_swap=1;   // enable line swapping
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
    		if (cursor_partial&(1<<partial_counter))		     gfx_ram[n+cursor_loc] =gfx_ram[n+cursor_loc]^127;
    		partial_counter++;
    				 }



}

void gfx_line_fill (){   // fills a line to gfx_ram from default_menu3 also fills gfx_block
		    // this now is the  emain char to disp
   // uint8_t line_selected=(loop_counter&7)+((menu_title_lut[enc_out1]>>7)*8);

    uint16_t cursor= menu_title_lut[enc_out1];
    uint16_t line_selected=(loop_counter&7)+((cursor>>7)*8);    // thsi needs to be 1024/16
    uint16_t start_line=(line_selected&7)*144;
     uint16_t end_line=start_line+144;
     disp_end=1;
   //  uint8_t  character=default_menu3[(i-2)+(line_selected*16)]-47;
    uint8_t char_skip=0;
     uint16_t character;
     uint8_t spi_store[3];
     uint8_t spi_store3=0;
     uint8_t reverse[16]={0};



     if ((cursor>>4) == line_selected)    reverse [(cursor&15)]=1;

     for (n=start_line;n<end_line ;n=n+18)    {    //  8*18   , 0-1152

	 for (i=0;i<16;i++){
		 character=((default_menu3[i+(line_selected*16)]-47)*8)+char_skip;
		 spi_store3= gfx_char[character&1023 ];     // limited characters
		spi_store3=spi_store3^(reverse[i]*127);
		 gfx_ram[n+i+2]=spi_store3;

						    spi_store[0]=250;
						    spi_store[1]=((spi_store3>>4)<<4);
						    spi_store[2]=((spi_store3&15)<<4);
						    memcpy(spi_tx_block+(((n+i)*3)+6),spi_store,3);  // writes only data no command
		     }
	char_skip++;
    }
    }


void encoder2(void){  // encoder pos and data input


	uint8_t crap_hold9=(menu_title_lut[enc_out1]>>16)&255;

	    memcpy(menu_vars_in,*(menu_titles_final+crap_hold9),8);	// send back for menu vars ok
	     char temp_char[]="  ";
				     memcpy(temp_char,menu_index_list+((enc_out1*2)),2);   // copy char to char,ok
				     menu_index_in=atoi(temp_char)			;   // convert char to int,ok
	    menu_vars_var=menu_vars(menu_vars_in,menu_index_in);		//test  for vars ok
		if ((*menu_vars_var)>menu_vars_limiter[menu_vars_ref])     *menu_vars_var=menu_vars_limiter[menu_vars_ref];   // limit value sher too

	    // input encode ------------------------
	    if ((enc_dir!=enc_mem_dir)  )    ///write back value if encoder changed ;
	    {
		if (enc_dir>enc_mem_dir) 							*menu_vars_var=((*menu_vars_var)+1)&255;
	    if ((enc_dir<enc_mem_dir)&&(*menu_vars_var))		 *menu_vars_var=((*menu_vars_var)-1)&255;
	    }
	    enc_mem_dir=enc_dir;

	    // location encode ------------------------

	    if ((enc2_dir!=enc2_mem_dir)  )    ///write back value if encoder changed ;
	   	    {
	   		if (enc2_dir>enc2_mem_dir) 							enc_out1++;
	   	    if ((enc2_dir<enc2_mem_dir))		enc_out1--;

	   	    }

	    if (page_skip)  {enc_out1=enc_out1+20; page_skip=0; }
	    if (enc_out1>menu_title_count) enc_out1=0;
	   	 if (enc_out1<0) enc_out1=menu_title_count;

	    enc2_mem_dir=enc2_dir;
	   //-------------------

		    enc2_add=0;

    }


    void menu3_fill(void){   // looping write into default_menu3
	uint16_t   enc_up=disp_up_counter;

	if(enc_up>menu_title_count) {enc_up=0; disp_up_counter=0;}

	uint8_t crap_hold10=(menu_title_lut[enc_up]>>16)&255;   // look up up menu_titles_final

	memcpy(menu_vars_in,*(menu_titles_final+crap_hold10),8);	// send back for menu vars ok
	 char temp_char[]="  ";
				 memcpy(temp_char,menu_index_list+((enc_up*2)),2);   // copy char to char,ok
				 menu_index_in=atoi(temp_char)			;   // convert char to int,ok

	menu_vars_var=menu_vars(menu_vars_in,menu_index_in);		//test  for vars ok

			lcd_out3=*menu_vars_var;

		if (enc_out1==enc_up) 	lcd_temp=lcd_out3;  // this may be a problem

		div_limit=lcd_out3;
			if (div_limit>9) div_limit=9;

		/*    if (lcd_out3>10)							div_limit= lcd_out3>>2;
				if (lcd_out3>40)							div_limit= lcd_out3>>5;*/


				uint16_t crap8=menu_title_lut[enc_up];

				default_menu3[crap8]=div_limit+48;
				if (menu_vars_ref==53)  default_menu3[crap8]=lcd_out3;   // keep in char , ok

				if ((menu_vars_ref==24) | (menu_vars_ref==25))  default_menu3[crap8]=major_notes[lcd_out3&31];
				disp_up_counter++;
				if(disp_up_counter>menu_title_count) disp_up_counter=0;  // just auto scans lcd_data , can be an issue

    }

    void LCD_Info_feedback(void){
	  potSource[380]=(lcd_temp/100) +48;  // still works   , potsource ref is located in feedback line var  ,was sendin x16 values
			    potSource[381]=((lcd_temp %100)/10)+48;		 // 0-160 to 0-10
			    potSource[382]=(lcd_temp%10)+48;

			    LCD_Info[0]  =( seq.pos/100) +48;
			    LCD_Info[1]  =( (seq.pos%100) /10)+48;
			    LCD_Info[2]  =( seq.pos%10)+48 ;

			    LCD_Info[11]  =( seq.pos>>6) +49;   // +1
			    LCD_Info[12]  =( (seq.pos&63)>>4) +49;
			    LCD_Info[13]  =( (((seq.pos&15)+1)%100)/10)+48 ;
			    LCD_Info[14]  =( ((seq.pos&15)+1)%10)+48 ;



    }
void LCD_Info_notes(void){
uint8_t counter=0;

for (counter=0;counter<8;counter++){

  LCD_Info[20+counter]=major_notes[notes_joined[counter+note[0].timeshift]];
  LCD_Info[28+counter]=major_notes[notes_joined[counter+note[1].timeshift]];
  LCD_Info[36+counter]=major_notes[notes_joined[counter+note[2].timeshift]];
  LCD_Info[44+counter]=major_notes[notes_joined[counter+note[3].timeshift]];
}




}



