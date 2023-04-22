float filter_pole(float input,float old_input, float freq1,float freq2) {

	float output;

	output=(input*freq1)+(old_input*freq2);


return  output;

}

void sampling(void){						// 330 atm or 8.5ms

//	if (time_proc>580) time_final=time_proc;

	  time_proc=0;
    static 	int16_t* sample_ptr2= &flash_read_block2[4];   // DMA load
 	int16_t* sample_ptr3= &flash_read_block3;   // DMA load
    int16_t* signed_ptr=&flash_read_block;

    int16_t  flash_read_a=0;




//time_proc=0;


	seq.swing=0;

static	int32_t sample_accus[10] ={0,0,0,0,0,0,0,0,0,0}; // all these should be static but ok otherwise
uint8_t mask_i;
 int32_t sample_Accu[10] ={0,0,0,0,0,0,0,0,0,0};   // accu for sample output or lookup , not static !

int32_t sample_adc=0;
int32_t ram_temp=0;
uint16_t sample_counts_temp[40]={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
//static  float  filter_accus[36]={0};   // make sure its static here

static  int32_t  filter_accus[36]={0};
static int32_t play_holder1[512];    // data banks
static int32_t play_holder2[512];
static int32_t play_holder3[512];    // data alignment > type
static  int32_t play_holder4[512];
static int32_t play_holder0[512];
static int32_t phase_bank0[32];
static int32_t phase_bank1[32];
static int32_t phase_bank2[32];
static int32_t phase_bank3[32];
int32_t  sample_temp7=0;
int32_t sample_input=0;
tempo_lut[131]=1023;   // force to an even count on samples

int32_t ram_synced=0;   // ram synced to sampler.seq_pos
uint8_t soft_mute=0;
//if ((sampler.record_enable==0) && (flash_flag))   {  flash_page_read ((sampler.ram_seq)<<1 ); flash_flag=0;}

if (bank_write==2)  {sample_pointD=512;}  else sample_pointD=0;

bank_write=0;
memcpy(notes_joined,seq.notes1,16);
memcpy(notes_joined+16,seq.notes1,16);  //  only use notes 1
sample_pointB=sample_pointD;
unsigned short tempo_start=0;  // enabled when i=isrMask;

LCD_Info_notes(seq.loop[4]);

uint8_t counter;
for (counter=0;counter<16;counter++){				// notes for sampler
seq.notes2[counter]= (sampler.Snotes1[counter]&15)+ (sampler.Snotes2[counter]<<4);

}
uint8_t sound_enable[8]={0,0,0,0,0,0,0,0};  // better here

uint16_t i_total;
uint16_t tempo_mod=tempo_lut[seq.tempo];  // set tempo,speed from lut 40-200bpm  ,changed to 4x for note lenght

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


//for (i=0;i<16;i++) {  note_toggler[i]=0; }
	//potSource[150]=(freq_point[0])*100; //0-2


patch_lists();
static int32_t* osc_pointers[10];   // sample outs
uint8_t pars_counter;

//if (sampler.ram_seq>15871)  sampler.ram_seq=0;  //else sampler.ram_seq=sampler.ram_seq+1024;
//setup pointers for osc options
 for (pars_counter=0;pars_counter<5;pars_counter++)	{   // fill up pointers and reload vars


		sample_accus[pars_counter]=sample_accus_hold[pars_counter];
	 if (note[pars_counter].osc==0) {osc_pointers[pars_counter]=&sample_Accu[5];  sound_enable[pars_counter]=0;} // zero
	 if (note[4].osc==0) osc_pointers[5]=&sample_Accu[5];
	 if (note[pars_counter].osc==1) {osc_pointers[pars_counter]=&sample_accus[pars_counter];  sound_enable[pars_counter]=1;} 						// saw
	 			if (note[pars_counter].osc==2){ osc_pointers[pars_counter]=&sample_Accu[pars_counter];sound_enable[pars_counter]=2;} 		// tri
	 			if (note[pars_counter].osc==3) { osc_pointers[pars_counter]=&sample_Accu[pars_counter+6];sound_enable[3]=3;}   // sample_input
	 			if (note[pars_counter].osc==4) { osc_pointers[pars_counter]=&sample_input;osc_pointers[5]=&sample_temp7;sound_enable[4]=4;}  // sampleinput
	 			if (note[pars_counter].osc==5) {osc_pointers[pars_counter]=&sample_adc;sound_enable[5]=5;}    // sine
	 			if (note[pars_counter].osc==6) {osc_pointers[pars_counter]=&ram_temp ; sound_enable[6]=6;}   // non muted RAM , just loop
	 			if (note[pars_counter].osc==7) {osc_pointers[pars_counter]=&ram_synced; sound_enable[7]=7;}   // trigger 0-16
	 			if (note[pars_counter].osc>7) {osc_pointers[pars_counter]=&sample_Accu[5]; sound_enable[pars_counter]=0;}  // zero


 }

note_holdA=0;

// some good phasin and delays here
uint8_t cross_fade[2]={0,0};
uint8_t fader[17]={0,1,5,11,19,28,39,51,64,76,88,99,108,116,122,126,127}; // sine curve for cross fade
//adc_values[2]= 15; //force for now
//if(adc_values[2]&16)     	{cross_fade[1]=127-fader[adc_values[2]&15]; cross_fade[2]=127;}  else {cross_fade[2]=fader[adc_values[2]&15]; cross_fade[1]=127;} //calculate crossfader

// doing lfo calc here as it is slow only for now
////////////////////adsr/////////////////////////////////////////

isr_change=0;
uint8_t seq_dat=(1<<(((adc_values[0]>>3))+1))-1;
///////////////////////////////////////////////////////////////



for (i=0;i<512;i++) {    // this should write 512 bytes , or about 15ms buffer ,works fine, too much scope

	i_total=i+sample_pointB;
	sampling_position=i>>6;   //   0-8 steps
	sampling_position_b=(sampling_position+7)&7;

//potValues[i&255]=potSource[i&255]>>4; //just to update values

if (tempo_count>=tempo_mod) { next_isr=(next_isr+1)& 4095;tempo_count=0;isr_change=next_isr+1;  }  else {tempo_count++;  }  //trigger next note , actual next step for isrCount(future)  8ms,trying to fix slow down here  8000 too  much, adsr clears note info
// tempo_count is about 1000-400
	tempo_start=0;


	if ((next_isr>>4) != (seq.pos)) { 					//     min 6400 cycles per seq.pos ,         next note step 140ms
		seq.pos=(next_isr>>4); // seq pos =256 max , isr = 1/16 of a note, note lenght is 1-4
		 tempo_start=1;
		    }


if (seq.tempo<60) {tempo_start=0; seq.pos=0;sampler.acurrent_sample=1;} // loop on single sample
 	if(tempo_start  )    // Calculates only on note change, gotta change seq.pos somehow  , only activates when change in seq pos ie once in ten bankwrites ,rare
	{
		serial_tosend=1;
		//note_reset();
		//note[1].timeshift=(adc_values[1])&31; //assigned pots to start of loopers 0-16,works
		seqpos_i=i+1;   //record postion


	//	note[2].timeshift=note[1].timeshift=note[3].timeshift=note[4].timeshift;
		uint8_t   loop_temp1[4];
		loop_temp1[0]=seq.pos&7;

		seq.loop[0]=(note[0].timeshift+(seq.pos&note[0].slide_length))&31;
		seq.loop[1]=(note[1].timeshift+(seq.pos&note[1].slide_length))&31;
		seq.loop[2]=(note[2].timeshift+(seq.pos&note[2].slide_length))&31; // this is really handy with sync to notes
		seq.loop[3]=(note[3].timeshift+(seq.pos&note[3].slide_length))&31;
		seq.loop[4]=(note[4].timeshift+(seq.pos&note[4].slide_length))&15;  // use this for sample


		sampler.acurrent_sample=sampler.Snotes1 [seq.loop[4]]; // 0 is none
		sampler.acurrent_sample2=sampler.Snotes2 [seq.loop[4]]; // 0 is none

		soft_mute=1;  // fade out  start on note change
			note[0].pitch=(notes_joined[seq.loop[0]])+(note[0].transpose);    // maybe join 1 and 2
			//note[0].pitch=note[1].pitch+(note[0].transpose); // just double
			note[1].pitch=(notes_joined[seq.loop[1]])+(note[1].transpose);  //loop 8 notes from pos and x times
			note[2].pitch=(notes_joined[seq.loop[2]])+(note[2].transpose);  //loop 8 notes from pos and x times

		note[3].pitch=(notes_joined[seq.loop[3]])+(note[3].transpose); ;  //loop 8 notes from pos and x times ,might disable normal adsr completely
		note[4].position=1;    // should trigger always
		// --------------------- sampler
		uint8_t pos_divide=15;
		if (sampler.repeat) pos_divide=15>>sampler.repeat;

		if (sampler.trigger_1==((seq.pos&pos_divide)+1))  sampler.trigger_position=1;
		if (sampler.trigger_2==((seq.pos&pos_divide)+1))  sampler.trigger_position=1;
		if (sampler.trigger_3==((seq.pos&pos_divide)+1))  sampler.trigger_position=1;
		if (sampler.trigger_4==((seq.pos&pos_divide)+1))  sampler.trigger_position=1;
	//	if(sampler.trigger_position) note[3].position=sampling_position+1;

		patch_target_replace();
		uint8_t detune_temp=0;
	//	detune_temp=(note[3].pitch+(note[3].detune))&31    ;
//		if(detune_temp>27) detune_temp=27;
		   //this is for sine skip mask

//	note[3].tuned=sine_lut[MajorNote[detune_temp]];	//sets freq ,1.0594  * 16536 =17518  ,
//	note[3].tuned= (note[3].tuned*1200)>>10;  // modify different sample size , just need single cycle length and thats it
		mask_result =0;
		uint8_t note_number;
		for (mask_i=0;mask_i<4;mask_i++)	{							// calc detune , slow ,also creates notes

			if  (note[mask_i].osc<3)  { if (seq.loop[mask_i]==0)  {note[mask_i].position=0;   } else  note[mask_i].position=sampling_position+1;    }   // disables trigger ?

			// disable note on lowest , optional

		    detune_temp=note[mask_i].pitch+note[mask_i].detune;


		    if (detune_temp>pitch_limit ) detune_temp=pitch_limit;
		    if (detune_temp>30 )   note_number=MinorNote[detune_temp-31];    // for now
		    else note_number=MajorNote[detune_temp];
		    float tune_temp=sample_Noteadd[note_number];
		    if ((note[mask_i].velocity)&&(note[mask_i].osc))	    LCD_Info[15+mask_i]=major_notes[note_number]; else LCD_Info[15+mask_i]=95;
		    //note[mask_i].tuned=sample_Noteadd[MajorNote[detune_temp]];   // put unit tune here


		    if (notes_joined[seq.loop[mask_i]] )		note[mask_i].tuned=tune_temp*0.99;	// this works
		    else note[mask_i].position=0;  // no retrigger on off note


		    //    6 HZ JUMP REGARDLESS OF FREQ


//	} // relies on note channel clear , not good , clear not channel straight after

	}
//sine_counter[4]=0;    // one shot
		//note_tuned[3]=2751;
	} // end of note calcualte



	if ((i&63)==0)
	{
	    LFO_square_one_pulse();
	    ADSR_loop();   // change to sound instead of channel  , use 4 for sample
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
int32_t  sample_temp6;


memcpy(&sample_counts_temp,sample_counts_holder,80);

sample_counts_temp[30]=note[0].tuned;
sample_counts_temp[31]=note[1].tuned;
sample_counts_temp[32]=note[2].tuned;
sample_counts_temp[33]=note[3].tuned;


static uint32_t*  sine_ptr_temp[5];


//tuned_list[0]=6397;
uint16_t accu_count[4];
memcpy(&accu_count,sample_accu_counter,8);  // copy counters

memcpy(&sample_accus,sample_accus_hold,16);    // this is good





	//if((sampler.ram_seq&255)==0)	  sampler_1k_load(sample_flash_address);

	counter_16=counter_16+2;   // jump 512 bytes
	//    if (sampler.ram_seq>14000) memcpy(&error_data,flash_read_block2,127);

	uint16_t*   ram_ptr=  &RAM	;  // pointer goes to LSB !!!!
	//uint16_t* sample_ptr= &flash_read_block;



	if(RAM_looper>=16383) RAM_looper=0;  // with 256 we have every 64 points along the sample for position
	int32_t   sample_adc_temp;
//	if ((sampler.ram_seq&255)==0)  ram_ptr=  &flash_read_block[128];
	uint16_t looper_point=0;
//	float pitch_counter=1;

//	ram_float=(1<<20) /(1.059463*ram_float);

  flash_read_a=0;

	for (i=0;i<512;i++){    // convert to signed , also use this for double buffer (no bank switch) 512 *16 is enough

	 //   flash_read_a= *(sample_ptr2+i);
	    flash_read_a= *(sample_ptr3+i) - 32767;
					if ((flash_read_a==32767)   ||  (flash_read_a==-32767))   flash_read_a=0;    // stop clicks from FF bytes
					memcpy(&flash_read_block3[i*2],&flash_read_a,2);   // load second set ,hopefully

	}
	for (i=0;i<512;i++){    // convert to signed , also use this for double buffer (no bank switch) 512 *16 is enough

			    flash_read_a= *(signed_ptr+i) - 32767;
			    if ((flash_read_a==32767)   ||  (flash_read_a==-32767))   flash_read_a=0;    // stop clicks from FF bytes
			    memcpy(&flash_read_block[i*2],&flash_read_a,2);   // load first set
			}


///    downsample  is a no go without playback from RAM , leave it , work on loops etc
//	sine_count_sample();   //change playback rate

	 if (LFO_square[0].out[0]>10) LCD_Info[60]=94  ;  else LCD_Info[60]=95;
	 if (LFO_square[2].out[0]>10) LCD_Info[60]=94  ;  else LCD_Info[62]=95;
	 uint8_t sampler_adder_enable=1;

		// TIME FINAL IS 68 UP TO HERE ATM
for (i=0;i<512;i++) {    // this should write 512 bytes , or about 15ms buffer ,oscillators
	i_total=i+sample_pointB;
	sampling_position=(i>>6);
	sampler_adder_enable=1;
	if (sound_enable[5]) 		 {
	sample_adc_temp=input_holder[((adc_playback_position++)&1023)>>1];
	sample_adc_temp=(sample_adc_temp-32767)<<4;
	sample_adc=(sample_adc_temp+sample_adc)>>1;}

	if (sound_enable[6]) 		 {
	//looper_point=(RAM_looper+(sampler.RAM_offset<<10))&16383;  // will use different data here

	looper_point=RAM_looper;
	ram_temp=*(ram_ptr+looper_point);    // works

	ram_temp=(ram_temp-32767)<<4;
	}
	//ram_synced= *(ram_ptr+sampler.ram_seq);



	if (sampler.ram_seq<sampler.start_current) {sample_input=0;  ram_synced=32767;}
	else 	{sample_input=*(signed_ptr+(sampler.ram_seq&255))<<3;ram_synced= *(ram_ptr+sampler.ram_seq);}
	ram_synced=(ram_synced-32767)<<4;

	if (sampler.ram_seq2<sampler.start_current2)  sample_temp7=0; else
	sample_temp7=*(sample_ptr3+(sampler.ram_seq2&255));


//	if (ram_sync_swap) sample_input=ram_synced;  //  good idea but no
	if  (sampler_mute[0])  sample_input=0;    // mute   , stops click on start
	if  (sampler_mute[1])  sample_temp7=0;

	//sample_input=sample_input<<3;
//	sample_temp7=sample_temp7<<3;
		if	((i&1)==1)   {  RAM_looper++;sampler.ram_seq++; sampler.ram_seq2++; }


	if (i==seqpos_i) {sample_counts_temp[20]=sample_counts_temp[30]; 		//delay tuned changed till correct time
	sample_counts_temp[21]=sample_counts_temp[31];
	sample_counts_temp[22]=sample_counts_temp[32];
	sample_counts_temp[23]=sample_counts_temp[33];
	}

	if ((i&63)==0)	{

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

	if(ADSR[0].release)       note[0].velocity=ADSR[0].buffer[sampling_position]>>2;   // disables LFO on release set to 0
	if(ADSR[1].release)       note[1].velocity=ADSR[1].buffer[sampling_position]>>2;
	if(ADSR[2].release)       note[2].velocity=ADSR[2].buffer[sampling_position]>>2;
	if(ADSR[3].release)       note[3].velocity=ADSR[3].buffer[sampling_position]>>2;
	//if(ADSR[4].release)       note[4].velocity=ADSR[4].buffer[sampling_position]>>2;


	{	  sample_accus[0] = sample_accus[0] + sample_counts_temp[20];}  // normal adder full volume   ,11 inst
	{	sample_accus[1] = sample_accus[1] + sample_counts_temp[21];}
	{	sample_accus[2] = sample_accus[2] + sample_counts_temp[22]; }// bouncing somewhere
	{	sample_accus[3] = sample_accus[3] + sample_counts_temp[23]; } // 6 cycles

			if (sample_accus[0]>524287) {sample_accus[0] =-0-sample_accus[0] ; } // faster >  than &  ,strange,  or is even worse
			if (sample_accus[1]>524287) {sample_accus[1] =-0-sample_accus[1] ;}// faster >  than &  ,conditional statement has no ' & '  in assy
			if (sample_accus[2]>524287) {sample_accus[2] =-0-sample_accus[2] ;} //
			if (sample_accus[3]>524287) {sample_accus[3] =-0-sample_accus[3] ;} //  6 cycles


			if (sound_enable[0]==2)    	{sample_Accu[0]=(abs(sample_accus[0])-262144)<<1;	}  // not much help at all
			if (sound_enable[1]==2)		 	{	sample_Accu[1]=(abs(sample_accus[1])-262144)<<1;}
			if (sound_enable[2]==2)		 	{sample_Accu[2]=(abs(sample_accus[2])-262144)<<1;}
			if (sound_enable[3]==2) 				{sample_Accu[3]=(abs(sample_accus[3])-262144)<<1;}


			sample_temp1=(*osc_pointers[0])*note[0].velocity;

			sample_temp2=(sample_temp1)>>12;

			sample_temp1=(*osc_pointers[1])*note[1].velocity; // needs cut a bit  ,default 20bit
			sample_temp3=sample_temp1>>12; // 20+8


			sample_temp1=(*osc_pointers[2])*note[2].velocity;      // needs some gain fine tune !
			sample_temp4 =sample_temp1>>12;    // 64 default 20+8

		//	{sample_Accu[9]=sample_input;sample_Accu[8]=sample_input;sample_Accu[7]=sample_input;sample_Accu[6]=sample_input;}
					//if (note[3].position) sample_Accu[9]=(ram_temp+sample_Accu[9])>>1; // bit hot , also  add avr

					sample_temp1=(*osc_pointers[3])*	note[3].velocity;  // sine out is 16bit, add 4 then 16+8
					sample_temp5 =sample_temp1>>13;
				//	if (soft_mute)  sample_temp6=(*osc_pointers[4])* (256-(i>>1));  else     sample_temp6=(*osc_pointers[4])*note[4].velocity;   // not heavy
					sample_temp6=((*osc_pointers[4])*note[4].velocity)+((*osc_pointers[5])*note[5].velocity);



	play_holder0[i]=sample_temp2;  // write to bank
	play_holder1[i]=sample_temp3;
	play_holder2[i]=sample_temp4;
	play_holder3[i]=sample_temp5;  // easy overload
	play_holder4[i]=(sample_temp6)>>10;  // clean output , samples
	//play_holder4[i]=sample_temp7;


} // end of osc , doing some sound

				//		    if(note[3].position)		sine_counter[9]=sine_counter[9]+256;  // count up when on

    memcpy(&sample_accus_hold,sample_accus,16);
    memcpy(&sample_accu_counter,accu_count,8);   // copy back
    memcpy(&sample_counts_holder,sample_counts_temp,80);
    int32_t filter_Accu;
    int32_t filter_Accu2;
    uint8_t i_31;
    int16_t sample_16[10]={0};    // sample accu 16 signed
    uint32_t    frq_pkt=0;
    uint32_t   smp_pkt=0;
    uint32_t in1_pkt;
    uint32_t in2_pkt;



   static uint8_t phaser[4];
 // memcpy(&filter_accus,filter_accus_hold,60);    doesnt help here


   //uint16_t* click=&input_holder[0];

			//   TIME FINAL IS 188 UP TO HERE ATM
			for (i=0;i<512;i++) {    //  SLOW USES UP ABOUT 140
				i_total=i+sample_pointB;
				i_31=i&31;
			sampling_position=(i>>6);


			if ((i&63)==0){
						phaser[0]=31-(filter[0].resonance);
						phaser[1]=-31-(filter[1].resonance);
						phaser[2]=31-(filter[2].resonance);
						phaser[3]=31-(filter[3].resonance);
						frq_point();
			}


			phase_bank0[i_31]= sample_Accu[0]=play_holder0[i];
			phase_bank1[i_31]=sample_Accu[1]=play_holder1[i] ; // saw
			phase_bank2[i_31]=sample_Accu[2]=play_holder2[i];  // sine input
			phase_bank3[i_31]=sample_Accu[3]=play_holder3[i] ;

			sample_Accu[4]=play_holder4[i];

	if (phaser[0])	sample_Accu[0]=(sample_Accu[0]+phase_bank0[ ( i_31+phaser[0])&31	])>>1; // PHASER
	if (phaser[1])	sample_Accu[1]=(sample_Accu[1]+phase_bank1[( i_31+phaser[1])&31	])>>1;
	if (phaser[2])	sample_Accu[2]=(sample_Accu[2]+phase_bank2[  ( i_31+phaser[2])&31	])>>1;
	if (phaser[3])	sample_Accu[3]=(sample_Accu[3]+phase_bank3[  ( i_31+phaser[3])&31	])>>1;


	//sample_Accu[0]= (sample_Accu[0]+delay_buffer[ (delay_cntr+(delay[0].time<<7))&2047])>>1;
//	delay_buffer[delay_cntr]=(delay_buffer[delay_cntr]+(sample_Accu[0]>>1))>>1;
	//delay_accu=delay_accu+(sample_Accu[0]);
	delay_accu=delay_accu+play_holder0[i];

	//if((i&3)==3)	{	delay_cntr=(delay_cntr+1)&2047;  		delay_buffer[delay_cntr]=(delay_accu>>2);	 delay_accu=0;delay_accu2=((delay_accu2*0.5)+	(delay_buffer[ (delay_cntr+(delay[0].time<<6))&2047]*0.5));		} // about 0.5 sec


	if((i&3)==3)    {delay_cntr=(delay_cntr+1)&2047;

	    delay_accu2=(delay_buffer[ (delay_cntr+(2047-(delay[0].time<<6)))&2047] * delay[0].mix)>>4;}
	delay_accu3=(delay_accu2+ delay_accu3)>>1;
	delay_accu4=(delay_accu3+ delay_accu4)>>1;

	sample_Accu[0]= ((sample_Accu[0])+delay_accu4)>>1;
	//sample_Accu[0]=delay_accu4;
	if((i&3)==3) {delay_buffer[delay_cntr]=((delay_accu<<1)+(delay[0].feedback*delay_accu4)		)>>4;								delay_accu=0;	}


		    filter_accus[2]=((sample_Accu[0]*freq_point[0])+(filter_accus[2]*freq_point[1]))>>16;
		    filter_accus[3]=(((filter_accus[2]*freq_point[0])+(filter_accus[3]*freq_point[1]))>>16); //1
		//filter_accus[3]=(((filter_accus[2]-filter_accus[3])*freq_point[1])+filter_accus[3])>>16; //1
		//filter_accus[3]=(((  ((sample_Accu[0]*freq_point[0])+(filter_accus[2]*freq_point[1]))>>16  )*freq_point[0])+(filter_accus[3]*freq_point[1]))>>16; //1
		    sample_Accu[0]=filter_accus[3];

		    filter_accus[5]=((sample_Accu[1]*freq_point[2])+(filter_accus[5]*freq_point[3]))>>16; // down to 12 cycles per 2 filters
		filter_accus[6]=((filter_accus[5]*freq_point[2])+(filter_accus[6]*freq_point[3]))>>16;// 13 instructions in assy
		sample_Accu[1]=filter_accus[6];

		filter_accus[8]=((sample_Accu[2]*freq_point[4])+(filter_accus[8]*freq_point[5]))>>16;		// external loop is slower
		filter_accus[9]=((filter_accus[8]*freq_point[4])+(filter_accus[9]*freq_point[5]))>>16;
		sample_Accu[2]=filter_accus[9];
/*

		in1_pkt=sample_Accu[3];
		//sample_16[0]=sample_Accu[3];  // input
		in2_pkt=filter_accus[11];
		smp_pkt=  __PKHBT	(	in1_pkt,             // pack 16 bit values into 32 ,Halfword packing instruction. Halfword packing instruction. Combines bits[15:0] of val1 with bits[31:16] of val2 levitated with the val3.
		in2_pkt,
		16
		);

		in1_pkt=freq_point[6];
		in2_pkt=freq_point[7];

		frq_pkt=__PKHBT	(	in1_pkt,             // pack freq_point
			in2_pkt,
			16
			);



		filter_Accu=   __SMUAD	(	smp_pkt,				// Q setting dual 16-bit signed multiply with single 32-bit accumulator.
						    frq_pkt
						);

		filter_accus[11]=filter_Accu>>16;

*/


		filter_accus[11]=((sample_Accu[3]*freq_point[6])+(filter_accus[11]*freq_point[7]))>>16;

		filter_accus[12]=((filter_accus[11]*freq_point[6])+(filter_accus[12]*freq_point[7]))>>16;
		sample_Accu[3]=filter_accus[12];







		  debug_value=filter_accus[9];



	//		if (sound_enable[3]  ||  sound_enable[4] ) {			//filter_accus[10]=sample_Accu[3];
				//filter_accus[10]=filter_accus[10]-(filter_accus[12]*filter_res[3]);
					 // 30 cyles for 2 poles


		filter_Accu=filter_Accu2=0;

		filter_Accu = (sample_Accu[0]+sample_Accu[1]+sample_Accu[2]+sample_Accu[3] +sample_Accu[4])>>10 ;
		filter_Accu =filter_Accu+32767;


		output_mix[(i&510)+1]=(filter_Accu>>8) &255;
		output_mix[(i&510)]=filter_Accu&255;

		filter_Accu=(sample_Accu[0]+sample_Accu[1])>>6;

		filter_Accu2=(sample_Accu[2]+sample_Accu[3]+sample_Accu[4])>>6; //filter + drum out
		//filter_Accu=play_holder0[i];


	//	 if (one_shot!=199)   one_shot++;  //play one attack then stop

		 //if (filter_Accu>0xFFFF) {filter_Accu=0xFFFF;clipping++;} else if (filter_Accu<-65535) filter_Accu=-65535;  // limiter to 16 bits
		 if (filter_Accu>0x3FF) {filter_Accu=0x3FF;clipping++;} else if (filter_Accu<-1023) filter_Accu=-1023;  // limiter to 11 bits
		 if (filter_Accu2>0x3FF) {filter_Accu2=0x3FF;clipping++;} else if (filter_Accu2<-1023) filter_Accu2=-1023;  // limiter to 11 bits



		 play_sample[(i_total<<1)]=(filter_Accu)+1023;   // final output disable for now 2544
		 play_sample[(i_total<<1)+1]=(filter_Accu2)+1023;
		 //play_sample[i_total]=(input_holder[i]);  // works good

		} //  TIME FINAL IS 330 ATM  ... down to 312  ish now

		//	  memcpy(&filter_accus_hold,filter_accus,60);

		//time_final=time_proc;   // in samples

		if (bank_write)   error_count++;  // if bank write is high it means too much stall here

		if  (record_output)  record_output_to_RAM();

		  ram_sync_swap=0;

		if     (sampler.acurrent_sample)  {sampler.start=(sampler.offset[sampler.acurrent_sample-1])<<10;  sampler.start_current=sampler.start;}		 // this works , start is always off by 256 from note start

		if     (sampler.acurrent_sample2)  {sampler.start2=(sampler.offset[sampler.acurrent_sample2-1])<<10;  sampler.start_current2=sampler.start2;}

		    if (note[4].position) {sampler.ram_seq=sampler.start-(seq_pos>>1);sampler.ram_seq2=sampler.start2-(seq_pos>>1); note[4].position=0;	}  // start from note offset


		    if     (sampler.acurrent_sample==0)  sampler_mute[0]=1;    else sampler_mute[0]=0;
		    if     (sampler.acurrent_sample2==0)  sampler_mute[1]=1;    else sampler_mute[1]=0;
		if ((sampler.ram_seq)>=16128)     {sampler.ram_seq=sampler.start; }      // just for playback counter , maybe finishing early will cause it to stop
		if ((sampler.ram_seq2)>=16128)     {sampler.ram_seq2=sampler.start2; }


		//	sampler.length=sampler.end-sampler.start;
		if  ((sampler.acurrent_sample)    &&  (sampler.sample_select[sampler.acurrent_sample-1]==0)   )     ram_sync_swap=1;   // swap to RAM  from sample on 0 select  ,this should be started after last
	//	if(sampler.trigger_position )  { sampler.ram_seq=sampler.start;note[3].position=1; }   // starts here not perfect , figure out better



		if(counter_16>127) counter_16=0;

//	sample_flash_address=((sampler.sample_select[sampler.acurrent_sample] *128)<<8)+((counter_16) <<8);  // change this soemthing mroe friendly , also keep last 64k for menus ,wavs maybe just before as well


			  if  (sampler.acurrent_sample)    sample_flash_address[0]=((sampler.sample_select[sampler.acurrent_sample-1] *128)<<8)+((sampler.ram_seq>>8)<<9);  //let see , limited now with a 1k jumps or 31
				if (sampler.acurrent_sample2)     sample_flash_address[1]=((sampler.sample_select[sampler.acurrent_sample2-1] *128)<<8)+((sampler.ram_seq2>>8)<<9);

			  sampler_1k_load(sample_flash_address[0]);   // about 12 cycles  from send to receive  , total 25 cycles for 2 sets finished
			  sample_dma_counter=1;

			  time_final[0]=time_proc;
			//	sampler_1k_load(sample_flash_address);










		}  // TOTAL IS ABOUT 330 ATM  , 260 with no filters
