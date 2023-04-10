float filter_pole(float input,float old_input, float freq1,float freq2) {

	float output;

	output=(input*freq1)+(old_input*freq2);


return  output;

}

void sampling(void){						// 330 atm or 8.5ms

//	if (time_proc>580) time_final=time_proc;

//time_proc=0;

	time_proc=0;
	int32_t sample_accus[10] ={0,0,0,0,0,0,0,0,0,0};
uint8_t mask_i;
int32_t sample_Accu[10] ={0,0,0,0,0,0,0,0,0,0};   // accu for sample output or lookup
uint16_t tuned_list[10];
int32_t sample_adc=0;
int32_t ram_temp=0;
uint16_t sample_counts_temp[40]={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
uint8_t flash_result;
int32_t sample_input=0;
tempo_lut[131]=1023;   // force to an even count on samples
int32_t sample_loop_input=0;
//if ((sampler.record_enable==0) && (flash_flag))   {  flash_page_read ((sampler.ram_seq)<<1 ); flash_flag=0;}

if (bank_write==2)  {sample_pointD=512;}  else sample_pointD=0;

bank_write=0;
memcpy(notes_joined,seq.notes1,16);
memcpy(notes_joined+16,seq.notes2,16);
sample_pointB=sample_pointD;
unsigned short tempo_start=0;  // enabled when i=isrMask;

LCD_Info_notes();



uint16_t i_total;
uint16_t tempo_mod=tempo_lut[seq.tempo];  // set tempo,speed from lut 40-200bpm  ,changed to 4x for note lenght
//if (seq.tempo<40)  tempo_mod=511;  // syncs to main loop
uint8_t l;			// 35.002khz(0.02857ms) sample, 1 sample is temp count (16x=0.00045712) , *16=1 note ,at 300 (437bpm),(1/(0.00002857*tempo count*16)=1beat in s



tempo_sync=16384/((tempo_mod*16)/512) ; // 8000 at slowest 15.625 updates to lfo at 1 note 16384/15.625=1048.576+ per update  at setting 80
tempo_sync=tempo_sync/80;  // bit weird her , this is adsr !

if ((tempo_mod==511 )   && (sampler.ram_seq<=256)    && (seq.pos>>4)      )   { next_isr=0; tempo_count=0; }   //  this should sync  rough

seqpos_i=0; //reset
	sampler.trigger_position=0;     // reset sample start here




uint8_t looper_dat[8]={1,3,7,15};
uint32_t lfo_tempo_hold;

if (tempo_mod_hold!=tempo_mod){					// lfo lut calculate only when changes tempo

		float lfo_tempo_calc=((1647099*4)/(tempo_mod)); // deafualt 1 note    12867  at 512


						for (i=0;i<256;i++){								// 128 is 1 note
	lfo_tempo_hold=(((i+1)*lfo_tempo_calc))/16;               //  at 64 its 12867
	lfo_tempo_lut[i]= lfo_tempo_hold;

}}
tempo_mod_hold=tempo_mod;   // for lfo

uint32_t  note_toggler[17]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};  //records note position on 0-512   using a bit
for (i=0;i<16;i++) {  note_toggler[i]=0; }
	//potSource[150]=(freq_point[0])*100; //0-2


patch_lists();
int32_t* osc_pointers[10];   // sample outs
uint8_t pars_counter;

//if (sampler.ram_seq>15871)  sampler.ram_seq=0;  //else sampler.ram_seq=sampler.ram_seq+1024;
//setup pointers for osc options
 for (pars_counter=0;pars_counter<4;pars_counter++)	{   // fill up pointers and reload vars


		sample_accus[pars_counter]=sample_accus_hold[pars_counter];
	 if (note[pars_counter].osc==0) osc_pointers[pars_counter]=&sample_Accu[5];  // zero
	 	 	 	 if (note[pars_counter].osc==1) osc_pointers[pars_counter]=&sample_accus[pars_counter];   // saw
	 			if (note[pars_counter].osc==2) osc_pointers[pars_counter]=&sample_Accu[pars_counter];		// tri
	 			if (note[pars_counter].osc==3) { osc_pointers[pars_counter]=&sample_Accu[pars_counter+6];}   // sample_input
	 			if (note[pars_counter].osc==4) { osc_pointers[pars_counter]=&sample_input;}  // sampleinput
	 			if (note[pars_counter].osc==5) osc_pointers[pars_counter]=&sample_adc;   // sine
	 			if (note[pars_counter].osc==6) osc_pointers[pars_counter]=&ram_temp ;   // non muted RAM , just loop
	 			if (note[pars_counter].osc==7) osc_pointers[pars_counter]=&ram_temp ;   // trigger 0-16
	 			if (note[pars_counter].osc>7) osc_pointers[pars_counter]=&sample_Accu[5];  // zero


 }

note_holdA=0;

// some good phasin and delays here
uint8_t cross_fade[2]={0,0};
uint8_t fader[17]={0,1,5,11,19,28,39,51,64,76,88,99,108,116,122,126,127}; // sine curve for cross fade
//adc_values[2]= 15; //force for now
//if(adc_values[2]&16)     	{cross_fade[1]=127-fader[adc_values[2]&15]; cross_fade[2]=127;}  else {cross_fade[2]=fader[adc_values[2]&15]; cross_fade[1]=127;} //calculate crossfader

// doing lfo calc here as it is slow only for now
////////////////////adsr/////////////////////////////////////////
/*
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
}*/
isr_change=0;
uint8_t seq_dat=(1<<(((adc_values[0]>>3))+1))-1;
///////////////////////////////////////////////////////////////
uint8_t note_plain;


for (i=0;i<512;i++) {    // this should write 512 bytes , or about 15ms buffer ,works fine, too much scope

	i_total=i+sample_pointB;
	sampling_position=i>>6;   //   0-8 steps
	sampling_position_b=(sampling_position+7)&7;
	note_plain=seq.notes1[seq.pos & 7 ];
potValues[i&255]=potSource[i&255]>>4; //just to update values

if (tempo_count>=tempo_mod) { next_isr=(next_isr+1)& 4095;tempo_count=0;isr_change=next_isr+1;  }  else {tempo_count++;  }  //trigger next note , actual next step for isrCount(future)  8ms,trying to fix slow down here  8000 too  much, adsr clears note info
// tempo_count is about 1000-400
	tempo_start=0;
	if ((next_isr>>4) != (seq.pos)) { 					//     min 6400 cycles per seq.pos ,         next note step 140ms
		seq.pos=(next_isr>>4); // seq pos =256 max , isr = 1/16 of a note, note lenght is 1-4
		tempo_start=1;
// record note triggers or seq_changes position ,NEEDS TO BE OFF FOR NOTE 0
}

	//if ((next_isr&7)==0+(sampler.offset&7))	note[3].position=1;  // for sample     run always


	if(tempo_start  )    // Calculates only on note change, gotta change seq.pos somehow  , only activates when change in seq pos ie once in ten bankwrites ,rare
	{
		serial_tosend=1;
		//note_reset();
		//note[1].timeshift=(adc_values[1])&31; //assigned pots to start of loopers 0-16,works
		seqpos_i=i+1;   //record postion


	//	note[2].timeshift=note[1].timeshift=note[3].timeshift=note[4].timeshift;
		uint8_t   loop_temp1[4];
		loop_temp1[0]=seq.pos&7;
	//	loop_temp1[1]=adc_values[1]>>2;
	//	loop_temp1[2]=adc_values[2]>>2;
	//	loop_temp1[3]=adc_values[0]>>2;




		seq.loop[0]=(note[0].timeshift+(seq.pos&note[0].slide_length))&31;
		seq.loop[1]=(note[1].timeshift+(seq.pos&note[1].slide_length))&31;
		seq.loop[2]=(note[2].timeshift+(seq.pos&note[2].slide_length))&31; // this is really handy with sync to notes
		seq.loop[3]=(note[3].timeshift+(seq.pos&note[3].slide_length))&31;



			note[0].pitch=(notes_joined[seq.loop[0]])+(note[0].transpose);    // maybe join 1 and 2
			//note[0].pitch=note[1].pitch+(note[0].transpose); // just double
			note[1].pitch=(notes_joined[seq.loop[1]])+(note[1].transpose);  //loop 8 notes from pos and x times
			note[2].pitch=(notes_joined[seq.loop[2]])+(note[2].transpose);  //loop 8 notes from pos and x times

		note[3].pitch=(notes_joined[seq.loop[3]])+(note[3].transpose); ;  //loop 8 notes from pos and x times ,might disable normal adsr completely

		// --------------------- sampler
		uint8_t pos_divide=15;
		if (sampler.repeat) pos_divide=15>>sampler.repeat;

		if (sampler.trigger_1==((seq.pos&pos_divide)+1))  sampler.trigger_position=1;
		if (sampler.trigger_2==((seq.pos&pos_divide)+1))  sampler.trigger_position=1;
		if (sampler.trigger_3==((seq.pos&pos_divide)+1))  sampler.trigger_position=1;
		if (sampler.trigger_4==((seq.pos&pos_divide)+1))  sampler.trigger_position=1;
		if(sampler.trigger_position) note[3].position=sampling_position+1;

		patch_target_replace();
		uint8_t detune_temp=0;
	//	detune_temp=(note[3].pitch+(note[3].detune))&31    ;
//		if(detune_temp>27) detune_temp=27;
		   //this is for sine skip mask

//	note[3].tuned=sine_lut[MajorNote[detune_temp]];	//sets freq ,1.0594  * 16536 =17518  ,
//	note[3].tuned= (note[3].tuned*1200)>>10;  // modify different sample size , just need single cycle length and thats it
		mask_result =0;
		uint8_t note_number;
		for (mask_i=0;mask_i<5;mask_i++)	{							// calc detune , slow ,also creates notes

			if  (note[mask_i].osc<3)  { if (seq.loop[mask_i]==0)  {note[mask_i].position=0;   zero_cross[mask_i]=0;} else  note[mask_i].position=sampling_position+1;    }


			// disable note on lowest , optional

		    detune_temp=note[mask_i].pitch+note[mask_i].detune;

	//if	   (detune_temp>=30) detune_temp=30;
	//note[mask_i].tuned=(sample_Noteadd[MajorNote[detune_temp]]);   // put unit tune here

		        if (detune_temp>pitch_limit ) detune_temp=pitch_limit;
		    if (detune_temp>30 )   note_number=MinorNote[detune_temp-31];    // for now
	else note_number=MajorNote[detune_temp];
		    note[mask_i].tuned=sample_Noteadd[note_number];
	if ((note[mask_i].velocity)&&(note[mask_i].osc))	    LCD_Info[15+mask_i]=major_notes[note_number]; else LCD_Info[15+mask_i]=95;
	//note[mask_i].tuned=sample_Noteadd[MajorNote[detune_temp]];   // put unit tune here

		float tune_temp=note[mask_i].tuned;
		note[mask_i].tuned=tune_temp*0.99;	// this works


//	sample_counts_holder[mask_i+10]=(sample_counts[MajorNote[note[mask_i].pitch]]);   // add +10 so update only on zero cross


	//note[mask_i].tuned=sample_Noteadd[MajorNote[note[mask_i].pitch]];   // put unit tune here
	//	note[mask_i].tuned=sample_Noteadd[MajorNote[12]];
		//tuned_list[mask_i]=note[mask_i].tuned;
	//	if (note[mask_i].osc>=3)  note[mask_i].tuned=note[mask_i].tuned>>1;



	//	if (sampler.one_shot &(1<<mask_i))  sine_counter[4+(mask_i*3)]=0;  // clear if one shto enabled


		    //    6 HZ JUMP REGARDLESS OF FREQ



		//if (note[mask_i].osc==3)	{	note[mask_i].tuned=sine_lut[MajorNote[detune_temp]];	//sets freq ,1.0594  * 16536 =17518  ,
	//		note[mask_i].tuned= (note[mask_i].tuned*1200)>>10; } //calc sampler
		//


//	} // relies on note channel clear , not good , clear not channel straight after

	}
//sine_counter[4]=0;    // one shot
		//note_tuned[3]=2751;
	} // end of note calcualte



	if ((i&63)==0)
	{
	    LFO_square_one_pulse();

		LFO_source_synced();
	}

}  // end of note  i loop
 //make sure it's finished

// filter loop
int32_t  sample_temp2;
uint8_t sine_zero;
int32_t  sample_temp1;
int32_t  sample_temp3;
int32_t  sample_temp4;
int32_t  sample_temp5;


memcpy(&sample_counts_temp,sample_counts_holder,80);

sample_counts_temp[30]=note[0].tuned;
sample_counts_temp[31]=note[1].tuned;
sample_counts_temp[32]=note[2].tuned;
sample_counts_temp[33]=note[3].tuned;


uint32_t*  sine_ptr_temp[5];
	sampler.start=(sampler.start_MSB<<8)+sampler.start_LSB;
	sampler.end=(sampler.end_MSB<<8)+sampler.end_LSB;

//	sine_ptr_temp[0]=&sine_counter[3];
//	sine_ptr_temp[1]=&sine_counter[6];
//	sine_ptr_temp[2]=&sine_counter[9];
//	sine_ptr_temp[3]=&sine_counter[12];

	    // does go out of time , likely skipping samples
	if(sampler.start_MSB>sampler.end_MSB) sampler.start_MSB=0; //flip to 0  for now
	if (sampler.ram_seq<sampler.start)      sampler.ram_seq=sampler.start;
	//if ((sampler.ram_seq)>=sampler.end)     {sampler.ram_seq=sampler.start; note[3].position=0;}      // just for playback counter
	if ((sampler.ram_seq)>=sampler.end)     {sampler.ram_seq=sampler.start; }      // just for playback counter
	sampler.length=sampler.end-sampler.start;

	if(sampler.trigger_position )  { sampler.ram_seq=sampler.start;note[3].position=1; }   // starts here not perfect , figure out better



//tuned_list[0]=6397;
uint16_t accu_count[4];
memcpy(&accu_count,sample_accu_counter,8);  // copy counters

memcpy(&sample_accus,sample_accus_hold,16);    // this is good


uint8_t*  sample_block_ptr=&flash_read_block ;    // temporary only

	if(counter_16>127) counter_16=0;

	  uint32_t sample_flash_address=((sampler.sample_select*128)<<8)+((counter_16) <<8);  // change this soemthing mroe friendly , also keep last 64k for menus ,wavs maybe just before as well


	 sample_flash_address=((sampler.sample_select*128)<<8)+((sampler.ram_seq&15872)*2);  //let see , limited now with a 1k jumps or 31




	 sampler_1k_load(sample_flash_address);
	//if((sampler.ram_seq&255)==0)	  sampler_1k_load(sample_flash_address);

	counter_16=counter_16+2;   // jump 512 bytes
	//    if (sampler.ram_seq>14000) memcpy(&error_data,flash_read_block2,127);

	uint16_t*   ram_ptr=  &RAM	;  // pointer goes to LSB !!!!
	uint16_t* sample_ptr= &flash_read_block;



	if(RAM_looper>=16383) RAM_looper=0;  // with 256 we have every 64 points along the sample for position
	int32_t   sample_adc_temp;
//	if ((sampler.ram_seq&255)==0)  ram_ptr=  &flash_read_block[128];
	uint16_t looper_point=0;
//	float pitch_counter=1;

//	ram_float=(1<<20) /(1.059463*ram_float);

	int16_t  flash_read_a=0;

	for (i=0;i<256;i++){    // convert to signed

					flash_read_a= *(sample_ptr+i) - 32767;
				//	float_table[i]=flash_read_a;
					memcpy(&flash_read_block[i*2],&flash_read_a,2);   //

	}

///    downsample  is a no go without playback from RAM , leave it , work on loops etc
//	sine_count_sample();   //change playback rate
	int16_t* signed_ptr=&flash_read_block;
	 if (LFO_square[0].out[0]>10) LCD_Info[60]=94  ;  else LCD_Info[60]=95;
	 if (LFO_square[2].out[0]>10) LCD_Info[60]=94  ;  else LCD_Info[62]=95;


for (i=0;i<512;i++) {    // this should write 512 bytes , or about 15ms buffer ,oscillators
	i_total=i+sample_pointB;
	sampling_position=(i>>6);


	sample_adc_temp=input_holder[((adc_playback_position++)&1023)>>1];
	sample_adc_temp=(sample_adc_temp-32767)<<4;
	sample_adc=(sample_adc_temp+sample_adc)>>1;
	looper_point=(RAM_looper+(sampler.offset<<11))&16383;
	   ram_temp=*(ram_ptr+looper_point);    // works

	ram_temp=(ram_temp-32767)<<4;


//	sample_input=*(sample_ptr+(sampler.ram_seq&255));
//	sample_input=(sample_input-32767)<<4;
//	sample_input=(sample_input)<<4;
	//int_float= (i/2);

	//arm_float_to_q31(&ram_float,&inter_float,1);    // q31 is different here
	//sample_input=	arm_linear_interp_q15(signed_ptr,int_float,256.);
	sample_input=*(signed_ptr+(sampler.ram_seq&255));

	    sample_input=sample_input<<4;


	if	((i&1)==1)  {sampler.ram_seq=sampler.ram_seq+1; RAM_looper++; } // half speed



	if (i==seqpos_i) {sample_counts_temp[20]=sample_counts_temp[30]; 		//delay tuned changed till correct time
	sample_counts_temp[21]=sample_counts_temp[31];
	sample_counts_temp[22]=sample_counts_temp[32];
	sample_counts_temp[23]=sample_counts_temp[33];
	}

	if ((i&63)==0)	{
	    ADSR_loop();
	    //  LFO_source_new(); // calculate lfo maybe 8 times for now
	   patch_target_replace();    // update values , not too bad
float freq_temp=arm_sin_f32(filter[0].cutoff_1*0.006159)    ;   // need this for useful filter
	freq_pointer[0] [sampling_position]=1-freq_temp; // problem was selecting accu instead of out , good now
	 freq_temp=arm_sin_f32(filter[1].cutoff_1*0.006)    ;
	freq_pointer[1] [sampling_position] =1-freq_temp ; // filter lfos
	freq_temp=arm_sin_f32(filter[2].cutoff_1*0.006)    ;
	freq_pointer[2] [sampling_position] =1-freq_temp ; // filter lfos
	 freq_temp=arm_sin_f32(filter[3].cutoff_1*0.006)    ;
	freq_pointer[3] [sampling_position] =1-freq_temp ; // filter lfos

	}

	note[0].velocity=ADSR[0].buffer[sampling_position]>>3;
	note[1].velocity=ADSR[1].buffer[sampling_position]>>3;
	note[2].velocity=ADSR[2].buffer[sampling_position]>>3;
	note[3].velocity=ADSR[3].buffer[sampling_position]>>3;

	if (note[0].velocity)   sample_accus[0] = sample_accus[0] + sample_counts_temp[20];  else sample_accus[0]=200000;// normal adder full volume
		sample_accus[1] = sample_accus[1] + sample_counts_temp[21];
		sample_accus[2] = sample_accus[2] + sample_counts_temp[22]; // bouncing somewhere
		sample_accus[3] = sample_accus[3] + sample_counts_temp[23];  // 6 cycles

			if (sample_accus[0]>524287) {sample_accus[0] =-0-sample_accus[0] ; } // faster >  than &  ,strange,  or is even worse
			if (sample_accus[1]>524287) {sample_accus[1] =-0-sample_accus[1] ;}// faster >  than &  ,strange
			if (sample_accus[2]>524287) {sample_accus[2] =-0-sample_accus[2] ;} //
			if (sample_accus[3]>524287) {sample_accus[3] =-0-sample_accus[3] ;} //  6 cycles



		//	if (note[0].velocity==0) sample_accus[0]=200000;



			sample_Accu[0]=(abs(sample_accus[0])-262144)<<1;
			sample_Accu[1]=(abs(sample_accus[1])-262144)<<1;
			sample_Accu[2]=(abs(sample_accus[2])-262144)<<1;
			sample_Accu[3]=(abs(sample_accus[3])-262144)<<1;






			sample_temp1=(*osc_pointers[0])*note[0].velocity;
		//	sample_temp1=(*osc_pointers[0])*note[0].velocity;// needs cut a bit  maybe some diff vol settings
					sample_temp2=(sample_temp1)>>2;

							sample_temp1=(*osc_pointers[1])*note[1].velocity; // needs cut a bit  ,default 20bit
					sample_temp3=sample_temp1>>2; // 20+8


							sample_temp1=(*osc_pointers[2])*note[2].velocity;      // needs some gain fine tune !
					sample_temp4 =sample_temp1>>2;    // 64 default 20+8

					if (note[3].position) {sample_Accu[9]=sample_input;sample_Accu[8]=sample_input;sample_Accu[7]=sample_input;sample_Accu[6]=sample_input;}
					//if (note[3].position) sample_Accu[9]=(ram_temp+sample_Accu[9])>>1; // bit hot , also  add avr

					sample_temp1=(*osc_pointers[3])*	note[3].velocity;  // sine out is 16bit, add 4 then 16+8
					sample_temp5 =sample_temp1>>2;

	play_holder0[i]=sample_temp2;  // write to bank
	play_holder1[i]=sample_temp3;
	play_holder2[i]=sample_temp4;
	play_holder3[i]=sample_temp5;
	} // end of osc , doing some sound

				//		    if(note[3].position)		sine_counter[9]=sine_counter[9]+256;  // count up when on

    memcpy(&sample_accus_hold,sample_accus,16);
    memcpy(&sample_accu_counter,accu_count,8);   // copy back
    memcpy(&sample_counts_holder,sample_counts_temp,80);
    int32_t filter_Accu;
    int32_t filter_Accu2;

    //uint16_t* click=&input_holder[0];


			for (i=0;i<512;i++) {    // this should write 512 bytes , or about 15ms buffer ,oscillators , filters and final out ,slow 133
				i_total=i+sample_pointB;

			sampling_position=(i>>6);
			// filter 1


			phase_bank0[i&31]= sample_Accu[0]=play_holder0[i];
			phase_bank1[i&31]=sample_Accu[1]=play_holder1[i] ; // saw
			phase_bank2[i&31]=sample_Accu[2]=play_holder2[i];  // sine input
			phase_bank3[i&31]=sample_Accu[3]=play_holder3[i] ;


	if (filter[0].resonance)	sample_Accu[0]=(sample_Accu[0]>>1)+(phase_bank0[  ((i&31)+(31-(filter[0].resonance&31)))&31]>>1); // PHASER
	if (filter[1].resonance)	sample_Accu[1]=(sample_Accu[1]>>1)+(phase_bank1[  ((i&31)+(31-(filter[1].resonance&31)))&31]>>1);
	if (filter[2].resonance)	sample_Accu[2]=(sample_Accu[2]>>1)+(phase_bank2[  ((i&31)+(31-(filter[2].resonance&31)))&31]>>1);
	if (filter[3].resonance)	sample_Accu[3]=(sample_Accu[3]>>1)+(phase_bank3[  ((i&31)+(31-(filter[3].resonance&31)))&31]>>1);
	// this section is about 100 tmr cycles
			if ((i&63)==0){frq_point();
		}


   // vol lfo


		filter_accus[1]=sample_Accu[0]; // saw
		filter_accus[1]=filter_accus[1]-(filter_accus[3]*filter_res[0]);

		filter_accus[2]=(filter_accus[1]*freq_point[0])+(filter_accus[2]*freq_point[1]);      //short=fast , adding makes it slower
		filter_accus[3]=(filter_accus[2]*freq_point[0])+(filter_accus[3]*freq_point[1]);			// int32 after conversions is no quicker
		sample_Accu[0]=filter_accus[3];

	//	sample_Accu[3]=play_holder2[i] >>5;
		filter_accus[4]=sample_Accu[1];
		filter_accus[4]=filter_accus[4]-(filter_accus[6]*filter_res[1]);
		filter_accus[5]=(filter_accus[4]*freq_point[2])+(filter_accus[5]*freq_point[3]);   // 30 cyles for 2 poles
		filter_accus[6]=(filter_accus[5]*freq_point[2])+(filter_accus[6]*freq_point[3]);
		sample_Accu[1]=filter_accus[6];

		filter_accus[7]=sample_Accu[2];
		filter_accus[7]=filter_accus[7]-(filter_accus[9]*filter_res[2]);  // resonance
				filter_accus[8]=(filter_accus[7]*freq_point[4])+(filter_accus[8]*freq_point[5]);   // 30 cyles for 2 poles
				filter_accus[9]=(filter_accus[8]*freq_point[4])+(filter_accus[9]*freq_point[5]);
				sample_Accu[2]=filter_accus[9];

				filter_accus[10]=sample_Accu[3];
				filter_accus[10]=filter_accus[10]-(filter_accus[12]*filter_res[3]);
						filter_accus[11]=(filter_accus[10]*freq_point[6])+(filter_accus[11]*freq_point[7]);   // 30 cyles for 2 poles
						filter_accus[12]=(filter_accus[11]*freq_point[6])+(filter_accus[12]*freq_point[7]);
						sample_Accu[3]=filter_accus[12];

	//	filter_hold[0]=(filter_accus[5]+filter_accus[11])*0.5; //half sample , nice
	//	sample_Accu[0] =filter_accus[5]; // out
	//	filter_accus[11]=filter_accus[5]; //write back new value
		//sample_Accu[0] =sample_Accu[1];

				//sample_Accu[0]=(sample_Accu[0]>>2)+(play_holder0[(i+550)&511]>>2);   // just need 7 samples for aphaser at 500 hz

		filter_Accu=filter_Accu2=0;
		//if  (record_output)  {
		filter_Accu = (sample_Accu[0]+sample_Accu[1]+sample_Accu[2]+sample_Accu[3] )>>10 ;
		filter_Accu =filter_Accu+32767;

		  debug_value=filter_Accu;
		output_mix[(i&510)+1]=(filter_Accu>>8) &255;
		output_mix[(i&510)]=filter_Accu&255;


	//	}

		filter_Accu=(sample_Accu[0]+sample_Accu[1])>>16;



		filter_Accu2=(sample_Accu[2]+sample_Accu[3])>>16; //filter + drum out
		//filter_Accu=play_holder0[i];


		 if (one_shot!=199)   one_shot++;  //play one attack then stop

		 //if (filter_Accu>0xFFFF) {filter_Accu=0xFFFF;clipping++;} else if (filter_Accu<-65535) filter_Accu=-65535;  // limiter to 16 bits
		 if (filter_Accu>0x3FF) {filter_Accu=0x3FF;clipping++;} else if (filter_Accu<-1023) filter_Accu=-1023;  // limiter to 11 bits
		 if (filter_Accu2>0x3FF) {filter_Accu2=0x3FF;clipping++;} else if (filter_Accu2<-1023) filter_Accu2=-1023;  // limiter to 11 bits



		 play_sample[(i_total<<1)]=(filter_Accu)+1023;   // final output disable for now 2544
		 play_sample[(i_total<<1)+1]=(filter_Accu2)+1023;
		 //play_sample[i_total]=(input_holder[i]);  // works good

		} // end of filer


		//time_final=time_proc;   // in samples

		if (bank_write)   error_count++;  // if bank write is high it means too much stall here
		time_final[0]=time_proc;
		if  (record_output)  record_output_to_RAM();

		//bank_write=0;   /// total 320 sample time (39khz)
		}

