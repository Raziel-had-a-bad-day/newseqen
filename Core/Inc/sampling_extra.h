
void mask_calc(uint8_t mask_select,uint8_t mask_speed){    //calculate mask output from lfos
uint8_t mask_temp;
uint8_t mask_tempB;

//(lfo_mask[mask_select]>>((seq.pos>>mask_speed) &7)& 1) ;
mask_temp=(seq.pos>>mask_speed) &7 ; // 0-8
mask_tempB=	lfo_mask[mask_select]>>mask_temp;


mask_result=mask_tempB &1;


}

int32_t sine_count(void) {      // sine_out is the output 9  bit  , works
	uint8_t sine_zero;
	int32_t sine_tempA;
	int32_t sine_tempB;
	uint16_t sine_tuned=sine_counter[ 2 ];
	uint16_t sine_counter_temp=sine_counter[ 0 ];
	uint16_t sine_counterB_temp=sine_counter[ 1];

	if (sine_counterB_temp==0) 	sine_tuned=note[3].tuned;

			sine_counterB_temp=sine_counterB_temp+sine_tuned ;  // sine up counter per cycle , however sine adder needs to wait
			if (sine_counterB_temp>>7) sine_zero=0; else sine_zero=1;

		if (sine_counterB_temp>(sine_length<<5)) sine_counterB_temp=0; //fixed for now, last is 19200 then
////////////////////////////////////////////
	sine_counter_temp=(sine_counterB_temp>>5);  // up countr controlled by counter
		if (sine_counter_temp>sine_length) sine_counter_temp = sine_length;		// seems to be faster than using a for loop to calculate both values
		sine_tempA = sine_block[ sine_counter_temp];  // 0- 40000
	//	sine_tempA=sine_out; // grab first value , needs to be always plus
		sine_tempA=sine_tempA-20000; //convert to signed

		sine_counter_temp++;
		if (sine_counter_temp>=sine_length)  sine_counter_temp=0; // limit sin_counter to sample size  set to sample length
				sine_tempB=sine_block[sine_counter_temp]-20000;  // convert to signed and +256 to -256
				sine_tempB=	((sine_tempB-sine_tempA)>>5)*(sine_counterB_temp & 31);   // calculate fraction then add

				 sine_counter[0]=sine_counter_temp;
				 sine_counter[1]=sine_counterB_temp;
				 sine_counter[2]=sine_tuned;
				 return  (sine_tempA+sine_tempB)<<4;   // add back to start value -20k-20k  or about 16bit
	}


int32_t sine_count2(uint8_t note_selected, uint32_t *input_array,
	uint32_t *return_array)
    {      // sine_out is the output 9  bit  , works

    //note_selected=(note_selected*3)+3;
    //uint8_t sine_zero;
    int32_t sine_tempA;
    int32_t sine_tempB;
    uint16_t RAM_size_temp = sampler.length - 2;

    uint32_t sine_counter_temp = *input_array;
    uint32_t sine_counterB_temp = *(input_array + 1);
    uint32_t sine_tuned = *(input_array + 2);

    if (sine_counterB_temp == 0)
	sine_tuned = note[note_selected].tuned; // this needs to be fractional 20-30
    sine_counterB_temp = sine_counterB_temp + sine_tuned; // sine up counter per cycle , however sine adder needs to wait
    if (sine_counterB_temp > ((RAM_size_temp) << 13))
	sine_counterB_temp = 0; //fixed for now, last is 19200 then
////////////////////////////////////////////
    sine_counter_temp = (sine_counterB_temp >> 13); // up countr controlled by counter
    sine_tempA = ((*(sampler.start_ptr + sine_counter_temp)) - 2040) << 10;
    sine_counter_temp++;
    //	if (sine_counter_temp>=RAM_size)  sine_counter_temp=0; // limit sin_counter to sample size  set to sample length
    sine_tempB = ((*(sampler.start_ptr + sine_counter_temp)) - 2040) << 10; // convert to signed and +256 to -256

    sine_tempB = ((sine_tempB - sine_tempA) >> 5) * (sine_counterB_temp & 31); // calculate fraction then add (x/32 )  * (0-31)
    //note_selected=(note_selected*3)+3;
    *return_array = sine_counter_temp;  // this needs fixing
    *(return_array + 1) = sine_counterB_temp;
    *(return_array + 2) = sine_tuned;
    return (sine_tempA + sine_tempB) << 1; // add back to start value -20k-20k  or about 16bit

    }
int32_t sampler_oneshot(uint8_t note_selected, uint32_t *input_array,
	uint32_t *return_array)
    {      // sine_out is the output 9  bit  , works

    //note_selected=(note_selected*3)+3;
    //uint8_t sine_zero;
    int32_t sine_tempA;
    int32_t sine_tempB;
    uint16_t RAM_size_temp = sampler.length - 2;

//	uint32_t sine_counter_temp=*input_array;
    uint32_t sine_counterB_temp = *(input_array + 1);
    uint32_t sine_tuned = *(input_array + 2);

//	if (sine_counterB_temp==0) 	sine_tuned=note[note_selected].tuned;  // this needs to be fractional 20-30
    if (note[note_selected].position)
	sine_counterB_temp = sine_counterB_temp + 1; // sine up counter per cycle , however sine adder needs to wait
    if (sine_counterB_temp >= (RAM_size_temp))
	sine_counterB_temp = 0; //fixed for now, last is 19200 then
//	if (!sine_counterB_temp)   sine_counterB_temp=0;

    ////////////////////////////////////////////
//	sine_counter_temp=sine_counterB_temp;  // up countr controlled by counter
    sine_tempA = ((*(sampler.start_ptr + (sine_counterB_temp))) - 2040) << 11;
    //	sine_counter_temp++;
    //	if (sine_counter_temp>=RAM_size)  sine_counter_temp=0; // limit sin_counter to sample size  set to sample length
    //			sine_tempB=((*(sampler.start_ptr+sine_counter_temp))-2040)<<10;  // convert to signed and +256 to -256

    //			sine_tempB=	((sine_tempB-sine_tempA)>>5)*(sine_counterB_temp & 31);   // calculate fraction then add (x/32 )  * (0-31)
    //note_selected=(note_selected*3)+3;
    //		*return_array=sine_counter_temp;  // this needs fixing
    *(return_array + 1) = sine_counterB_temp;
    *(return_array + 2) = sine_tuned;
    return (sine_tempA);   // add back to start value -20k-20k  or about 16bit

    }

void LFO_square_one_pulse(void){   // sends one pulse synced to seq.pos , rate= pulse width in notes , delay = start position
       // uint16_t  temp_pulse=0;
	uint8_t seqpos = seq.pos>>3 ;
     //   uint16_t counter_up; // starts at 0+delay
	//uint8_t pulse_high;
	uint8_t pulse_low;
	uint8_t seq_div=7;
	uint8_t lfo_c ;
	//uint16_t pulse_length;     // 1 note -32 ? with fractions  0-8 then 0-16 then 0-32 then 0-64 ,repeat
	uint8_t pulse_high;

	for (lfo_c=0;lfo_c<10;lfo_c++){

	    pulse_low=LFO_square[lfo_c].rate;
	 //   if (LFO_square[lfo_c].delay<8) delay=LFO_square[lfo_c].delay;  else delay=7;
	    pulse_high=LFO_square[lfo_c].delay;

	    if (pulse_low<64) 						 {seq_div=63;}  // This is the end
	    if  (pulse_low<32) 								 {seq_div=31;}
	    if  (pulse_low<16) 								{ seq_div=15;}
	    if  (pulse_low<8) 									 {seq_div=7;}
	    if  (pulse_low<4) 									 {seq_div=3;}
	    //pulse_high=(pulse_high*seq_div)>>3;   // this is the start


	    if (((seqpos&seq_div)<=pulse_low)  &&	((seqpos&seq_div)>=pulse_high))	LFO_square[lfo_c].out[sampling_position]=((LFO_square[lfo_c].depth))<<8;  // high
	    else LFO_square[lfo_c].out[sampling_position]=(LFO_square[lfo_c].offset)<<8;		//low

	}
    }



void LFO_source(void){     // lfo


float	freq_temp=0;
float 	freq2_temp=0;
uint32_t 	freq_saw=0;
uint8_t lfo_c ;
float offset=0;
uint32_t lfo_accu_temp;
	uint32_t freq3_temp;
	uint32_t freq4_temp;
	int16_t  output_hold;
	int32_t freq_tri;
	uint32_t offset2;
	uint32_t freq_temp_saw;
	uint32_t delay_value;

	for (lfo_c=0;lfo_c<10;lfo_c++){   //current lfo setup , needs sampling position 0-8  and tempo_sync

	lfo_accu_temp=	lfo_accu[lfo_c][sampling_position_b];  // hold

	freq3_temp=lfo_tempo_lut[LFO[lfo_c].rate] ;  // rate. this needs a little log
		//freq4_temp=freq3_temp*freq3_temp;  // multiply  , dont more then 8000 reallys
	delay_value=(freq3_temp*8)*(LFO[lfo_c].delay&15);

			freq3_temp=lfo_accu_temp+ freq3_temp;// get lfo value plus rate , will try to get related to tempo for easier sync , at potS 80?/8192/8notes/ 1 bar

//    128 STEPS AT 64   ,OR 16 *8    OR    16 * NEXT_ISR     =   1 SEQ.POS SO 4 = 1 ISR
///  counts from -16k to +16 k   @ + 0.03125      to   +400    *10    or 13 ms *8 (10 hz ? fastest  )
		if (freq3_temp>(1647099+delay_value)) lfo_accu_temp=0+delay_value ; else lfo_accu_temp=freq3_temp; // write back value  counts -16000 to +16000

		if    ( (     seqpos_i ) && (!sampling_position))  lfo_accu_temp=1647099-(seqpos_i* 3216);   // tempo sync,ok

		lfo_accu[lfo_c][sampling_position]=lfo_accu_temp;
		freq_temp=lfo_accu_temp-delay_value;
		freq2_temp=(freq_temp*0.000003814697);  //0-360  , <<18
		freq_temp_saw=freq2_temp*10430;
		freq_temp =arm_sin_f32(freq2_temp); // seems ok   , cmsis is ok  RADIANS !!!!!
		freq2_temp=freq_temp*LFO[lfo_c].depth*204;

	//	if (!LFO[lfo_c].offset)    LFO[lfo_c].offset=0;   // se
 		offset=(LFO[lfo_c].offset<<8); //  limit now for finetuning
			offset2=offset;
 		freq2_temp=(freq2_temp+offset);

		if (freq2_temp>32766)  freq2_temp=32767;
		if (freq2_temp<-32766) {  freq2_temp=-32767; }  // clip to 13bit -/+ 8000

						output_hold=freq2_temp;
				if (freq_temp_saw>65534)  freq_temp_saw=65535;
					//	lfo_accu_temp=lfo_accu_temp/6;
					freq_saw=((freq_temp_saw*LFO[lfo_c].depth)>>8)+offset2;   // 51 000  + offset

					if (freq_saw>65534)  freq_saw=65535;   // clip
				//	freq_saw=65535-freq_saw;
					LFO[lfo_c].out[sampling_position]=output_hold+32767; // sine out
				   LFO[lfo_c].out_saw[sampling_position]=freq_saw;    // saw 16 bit


				   if(freq_temp_saw<32768)   freq_tri=freq_temp_saw; else freq_tri=65536-freq_temp_saw;    // 0-32767
				   freq_tri=((freq_tri*LFO[lfo_c].depth)>>7)+offset2 ; // tri 16 bit
				//   freq_tri=((freq_tri*255)>>7)+offset2 ; // tri 16 bit
				   if (freq_tri>65534)  freq_tri=65535;   // clip
				  LFO[lfo_c].out_tri[sampling_position]=freq_tri;

		} // lfo gen : 0=f1 , 1=tempo,2=pitch



	}

void LFO_source_synced(void){     // lfo , ok , half phase


	float	freq_temp=0;
	float 	freq2_temp=0;
	uint32_t 	freq_saw=0;
	uint8_t lfo_c ;
	float offset=0;
	uint32_t lfo_accu_temp;
	int16_t  output_hold;
	int32_t freq_tri;
	uint32_t offset2;
	uint32_t freq_temp_saw;
	uint32_t delay_value=0;
	uint16_t rate;

	for (lfo_c=0;lfo_c<10;lfo_c++){   //current lfo setup , needs sampling position 0-8  and tempo_sync

	rate= LFO[lfo_c].rate;
	if (rate>10) rate=10;

	lfo_accu_temp=(next_isr&((1<<rate)-1))*(65535>>rate);   // calculate from next_isr

	// if (lfo_c==0)     debug_value=lfo_accu_temp;

	 delay_value=8192*(LFO[lfo_c].delay&7);  //not ok

	 freq_temp=lfo_accu_temp*0.0000479376;

	 freq2_temp=arm_sin_f32(freq_temp);   // +10 cycles
	freq_temp=LFO[lfo_c].depth;
	 freq_temp= (freq2_temp*freq_temp)*256;

	 freq_temp_saw=(lfo_accu_temp+delay_value)&65535        ;// get lfo value plus rate , will try to get related to tempo for easier sync , at potS 80?/8192/8notes/ 1 bar

 		offset=(LFO[lfo_c].offset<<8); //  limit now for finetuning
			offset2=offset;
 		freq2_temp=(freq_temp+offset);

		if (freq2_temp>65534)  freq2_temp=65535;
	//	if (freq2_temp<-32766) {  freq2_temp=-32767; }  // clip to 13bit -/+ 8000

						output_hold=freq2_temp;
				if (freq_temp_saw>65534)  freq_temp_saw=65535;
					//	lfo_accu_temp=lfo_accu_temp/6;
					freq_saw=((freq_temp_saw*LFO[lfo_c].depth)>>8)+offset2;   // 51 000  + offset

					if (freq_saw>65534)  freq_saw=65535;   // clip
				//	freq_saw=65535-freq_saw;
					LFO[lfo_c].out[sampling_position]=output_hold; // sine out
				   LFO[lfo_c].out_saw[sampling_position]=freq_saw;    // saw 16 bit


				   if(freq_temp_saw<32768)   freq_tri=freq_temp_saw; else freq_tri=65536-freq_temp_saw;    // 0-32767
				   freq_tri=((freq_tri*LFO[lfo_c].depth)>>7)+offset2 ; // tri 16 bit
				//   freq_tri=((freq_tri*255)>>7)+offset2 ; // tri 16 bit
				   if (freq_tri>65534)  freq_tri=65535;   // clip
				  LFO[lfo_c].out_tri[sampling_position]=freq_tri;

		} // lfo gen : 0=f1 , 1=tempo,2=pitch



	}


void  frq_point(void){
	freq_point[0]=freq_pointer[0] [sampling_position];; // load up coeffs

			freq_point[2]=freq_pointer[1] [sampling_position];  // ok , array was too short
			freq_point[4]=freq_pointer[2] [sampling_position];  // ok , array was too short
			freq_point[6]=freq_pointer[3] [sampling_position];  // ok , array was too short

			if (freq_point[0]>1) freq_point[0]=1; else if (freq_point[0]<0) freq_point[0]=0;// just in case
			if (freq_point[4]>1) freq_point[4]=1; else if (freq_point[4]<0) freq_point[4]=0;// just in case
			if (freq_point[2]>1) freq_point[2]=1; else if (freq_point[2]<0) freq_point[2]=0;// just in case
			if (freq_point[6]>1) freq_point[6]=1; else if (freq_point[6]<0) freq_point[6]=0;// just in case

			freq_point[1]=1-freq_point[0];
			freq_point[3]=1-freq_point[2];
			freq_point[5]=1-freq_point[4];
			freq_point[7]=1-freq_point[6];

			filter_res[0]=freq_point[0]*0.2;
			filter_res[1]=freq_point[1]*0.2;
			filter_res[2]=freq_point[2]*0.2;
			filter_res[3]=freq_point[3]*0.2;

}

void sine_count_sample(void) {      //  signed  , about 10 cycles not bad , only of upsmpling

	int32_t sine_tempA=0;
	int32_t sine_tempB=0;
	int32_t sine_tempC=0;


	uint16_t sine_tuned=270;   // only upsample !
	uint16_t sine_counter_temp=0;    // up counter to read block
	uint32_t sine_counterB_temp=sine_tuned;

	int16_t* block_ptr=   &flash_read_block;
	uint16_t counter;
	int16_t out_signed;

//	sine_tuned=32;

	for (counter=0;counter<256;counter++){

	sine_counter_temp=sine_counterB_temp>>8; // large step

	sine_tempA = *(block_ptr+((sine_counter_temp)&255));
	sine_tempB = *(block_ptr+((sine_counter_temp+1)&255));

	sine_tempC=((sine_tempB-sine_tempA) *(sine_counterB_temp&255))>>8;  // fraction


	sine_tempA=sine_tempB+sine_tempC;

				out_signed=(sine_tempA+out_signed)>>1;

				memcpy(&flash_read_block[counter*2],&out_signed,2);  // write back
				  sine_counterB_temp=(sine_counterB_temp+sine_tuned)&65535 ;  // sine up counter per cycle , however sine adder needs to wait

	}




	}


void ADSR_loop(void){
		uint8_t counter;
		uint8_t ok=1;
		uint8_t ADSR_out=0;
		uint8_t position=sampling_position;
		//position=0;

		for (counter=0;counter<4;counter++){
		    ok=1;
	while(ok){	      // do one thing then exit
	    ADSR[counter].buffer[position]=0;
	    ADSR[counter].sustain_data=ADSR[counter].sustain<<4;
      if ( note[counter].position) {   ADSR[counter].attack_data=1000;     ADSR[counter].decay_data=1000;
      ADSR[counter].release_data= ADSR[counter].sustain_data;       note[counter].position=0; }

      if (ADSR[counter].attack_data>0) {   ADSR[counter].attack_data=ADSR[counter].attack_data-(ADSR[counter].attack<<2);
    ADSR[counter].buffer[position]=1000-ADSR[counter].attack_data;break;
      }

     if (ADSR[counter].decay_data>=ADSR[counter].sustain_data)   { ADSR[counter].decay_data=ADSR[counter].decay_data-(ADSR[counter].decay);
    ADSR[counter].buffer[position]=ADSR[counter].decay_data;break;

      }
      if (ADSR[counter].release_data>0)  {  ADSR[counter].release_data=ADSR[counter].release_data-ADSR[counter].release;
      ADSR[counter].buffer[position]=ADSR[counter].release_data;break;
      }
      ok=0;
	}
	if ( ADSR[counter].buffer[position]<0)    ADSR[counter].buffer[position]=0;     // 0-127
	if ( ADSR[counter].buffer[position]>1000)    ADSR[counter].buffer[position]=1000;

  }




}






