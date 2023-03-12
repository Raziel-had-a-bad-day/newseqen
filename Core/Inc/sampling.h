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
int32_t sample_adc;
int32_t ram_temp;
if (bank_write==2)  {sample_pointD=512;}  else sample_pointD=0;

bank_write=0;
memcpy(notes_joined,seq.notes1,16);
memcpy(notes_joined+16,seq.notes2,16);
sample_pointB=sample_pointD;
unsigned short tempo_start=0;  // enabled when i=isrMask;

uint16_t i_total;
uint16_t tempo_mod=tempo_lut[seq.tempo];  // set tempo,speed from lut 40-200bpm  ,changed to 4x for note lenght

uint8_t l;			// 35.002khz(0.02857ms) sample, 1 sample is temp count (16x=0.00045712) , *16=1 note ,at 300 (437bpm),(1/(0.00002857*tempo count*16)=1beat in s

tempo_sync=16384/((tempo_mod*16)/512) ; // 8000 at slowest 15.625 updates to lfo at 1 note 16384/15.625=1048.576+ per update  at setting 80
tempo_sync=tempo_sync/80;  // bit weird her , this is adsr !

uint8_t looper_dat[8]={1,3,7,15};
uint32_t lfo_tempo_hold;

if (tempo_mod_hold!=tempo_mod){					// lfo lut calculate only when changes tempo

		float lfo_tempo_calc=((62831*4)/(tempo_mod)); // deafualt 1 note


						for (i=0;i<256;i++){								// 128 is 1 note
	lfo_tempo_hold=(((i+1)*lfo_tempo_calc))/64;
	lfo_tempo_lut[i]= lfo_tempo_hold;

}}
tempo_mod_hold=tempo_mod;   // for lfo

uint32_t  note_toggler[17]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};  //records note position on 0-512   using a bit
for (i=0;i<16;i++) {  note_toggler[i]=0; }
	//potSource[150]=(freq_point[0])*100; //0-2

potSource[380]=(lcd_out3/100);  // still works   , potsource ref is located in feedback line var  ,was sendin x16 values
potSource[381]=((lcd_out3 %100)/10);		 // 0-160 to 0-10
potSource[382]=(lcd_out3%10);
int32_t* osc_pointers[10];   // sample outs
uint8_t pars_counter;

if (sampler.ram_seq>15871)  sampler.ram_seq=0;  //else sampler.ram_seq=sampler.ram_seq+1024;
//setup pointers for osc options
 for (pars_counter=0;pars_counter<4;pars_counter++)	{   // fill up pointers and reload vars


		sample_accus[pars_counter]=sample_accus_hold[pars_counter];
	 if (note[pars_counter].osc==0) osc_pointers[pars_counter]=&sample_Accu[5];  // zero
	 	 	 	 if (note[pars_counter].osc==1) osc_pointers[pars_counter]=&sample_accus[pars_counter];   // saw
	 			if (note[pars_counter].osc==2) osc_pointers[pars_counter]=&sample_Accu[pars_counter];		// tri
	 			if (note[pars_counter].osc==3) { osc_pointers[pars_counter]=&sample_Accu[pars_counter+6];sampler.one_shot=sampler.one_shot | (1<<pars_counter);}   // sine
	 			if (note[pars_counter].osc==4) { osc_pointers[pars_counter]=&sample_Accu[pars_counter+6];sampler.one_shot=sampler.one_shot &  ~(1<<pars_counter);}  // looping sample
	 			if (note[pars_counter].osc==5) osc_pointers[pars_counter]=&sample_adc;   // sine
	 			if (note[pars_counter].osc==6) osc_pointers[pars_counter]=&ram_temp ;   // sine
	 			if (note[pars_counter].osc>6) osc_pointers[pars_counter]=&sample_Accu[5];  // zero
	 			tuned_list[pars_counter]=note[pars_counter].tuned;   // this needs to run here as well

 }

note_holdA=0;

// some good phasin and delays here
uint8_t cross_fade[2]={0,0};
uint8_t fader[17]={0,1,5,11,19,28,39,51,64,76,88,99,108,116,122,126,127}; // sine curve for cross fade
//adc_values[2]= 15; //force for now
//if(adc_values[2]&16)     	{cross_fade[1]=127-fader[adc_values[2]&15]; cross_fade[2]=127;}  else {cross_fade[2]=fader[adc_values[2]&15]; cross_fade[1]=127;} //calculate crossfader

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

uint8_t seq_dat=(1<<(((adc_values[0]>>3))+1))-1;
///////////////////////////////////////////////////////////////
uint8_t note_plain;

for (i=0;i<512;i++) {    // this should write 512 bytes , or about 15ms buffer ,works fine, too much scope

	i_total=i+sample_pointB;
	sampling_position=i>>6;   //   0-8 steps
	sampling_position_b=(sampling_position+7)&7;
	note_plain=seq.notes1[seq.pos & 7 ];
potValues[i&255]=potSource[i&255]>>4; //just to update values
	if (tempo_count>=tempo_mod) { next_isr=(next_isr+1)& 4095;tempo_count=0;  }  else {tempo_count++; }  //trigger next note , actual next step for isrCount(future)  8ms,trying to fix slow down here  8000 too  much, adsr clears note info
// tempo_count is about 1000-400
	tempo_start=0;
	if ((next_isr>>4) != (seq.pos)) { 					//     min 6400 cycles per seq.pos ,         next note step 140ms
		seq.pos=(next_isr>>4); // seq pos =256 max , isr = 1/16 of a note, note lenght is 1-4
		tempo_start=1;
// record note triggers or seq_changes position ,NEEDS TO BE OFF FOR NOTE 0
}


	if(tempo_start  )    // Calculates only on note change, gotta change seq.pos somehow  , only activates when change in seq pos ie once in ten bankwrites ,rare
	{
		serial_tosend=1;
		note_reset();
		//note[1].timeshift=(adc_values[1])&31; //assigned pots to start of loopers 0-16,works

	//	note[2].timeshift=note[1].timeshift=note[3].timeshift=note[4].timeshift;
		uint8_t   loop_temp1[4];
		loop_temp1[0]=seq.pos&7;
		loop_temp1[1]=adc_values[1]>>2;
		loop_temp1[2]=adc_values[2]>>2;
		loop_temp1[3]=adc_values[0]>>2;

		//loop_temp1=(seq.pos&seq_dat); // loop position and length


		seq.loop[0]=(note[0].timeshift+loop_temp1[0]+loop_temp1[1])&31;
		seq.loop[1]=(note[1].timeshift+loop_temp1[0]+loop_temp1[2])&31;
		seq.loop[2]=(note[2].timeshift+loop_temp1[0])&31; // this is really handy with sync to notes
		seq.loop[3]=(note[3].timeshift+loop_temp1[0]+loop_temp1[3])&31;
		//seq.loop[5]=(note[5].timeshift+loop_temp1[0])&31;


			note[0].pitch=(notes_joined[seq.loop[0]])+(note[0].transpose);    // maybe join 1 and 2
			//note[0].pitch=note[1].pitch+(note[0].transpose); // just double
			note[1].pitch=(notes_joined[seq.loop[1]])+(note[1].transpose);  //loop 8 notes from pos and x times
			note[2].pitch=(notes_joined[seq.loop[2]])+(note[2].transpose);  //loop 8 notes from pos and x times

		note[3].pitch=(notes_joined[seq.loop[3]])+(note[3].transpose); ;  //loop 8 notes from pos and x times ,might disable normal adsr completely
//	if (note[3].pitch) 		{note[3].pitch=note[3].pitch+(note[3].transpose>>4);	adsr_retrigger[3]=1; note_toggler[i>>5]=1<<(i&31   )   ; } // stay at zero for off

//	note[3].pitch=(notes_joined[seq.loop[3]])+(note[3].transpose);  //
		patch_target_replace();
		uint8_t detune_temp=0;
	//	detune_temp=(note[3].pitch+(note[3].detune))&31    ;
//		if(detune_temp>27) detune_temp=27;
		   //this is for sine skip mask

//	note[3].tuned=sine_lut[MajorNote[detune_temp]];	//sets freq ,1.0594  * 16536 =17518  ,
//	note[3].tuned= (note[3].tuned*1200)>>10;  // modify different sample size , just need single cycle length and thats it
		mask_result =0;

		for (mask_i=0;mask_i<5;mask_i++)	{							// calc detune , slow ,also creates notes

if (note[mask_i].pitch) {

	//	note[mask_i].pitch=(note[mask_i].pitch ;

		//detune_temp=(note[mask_i].pitch+(note[mask_i].detune))&31    ;
		//if(detune_temp>27) detune_temp=27;


		note[mask_i].tuned=sample_Noteadd[MajorNote[note[mask_i].pitch]];
		//tuned_list[mask_i]=note[mask_i].tuned;
		if (note[mask_i].osc>=3)  note[mask_i].tuned=note[mask_i].tuned>>1;
		if (sampler.one_shot &(1<<mask_i))  sine_counter[4+(mask_i*3)]=0;  // clear if one shto enabled
		//if (note[mask_i].osc==3)	{	note[mask_i].tuned=sine_lut[MajorNote[detune_temp]];	//sets freq ,1.0594  * 16536 =17518  ,
	//		note[mask_i].tuned= (note[mask_i].tuned*1200)>>10; } //calc sampler



	} // relies on note channel clear , not good , clear not channel straight after

	}
//sine_counter[4]=0;    // one shot
		//note_tuned[3]=2751;
	} // end of note calcualte



	if ((i&63)==0)
	{

		LFO_source(); // calculate lfo maybe 8 times for now
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


uint32_t*  sine_ptr_temp[5];
	sampler.start=(sampler.start_MSB<<8)+sampler.start_LSB;
	sampler.end=(sampler.end_MSB<<8)+sampler.end_LSB;

	if ((sampler.one_shot&1) && (sine_counter[4]>(sampler.end-1000)) && (note[0].osc==3))   note[0].tuned=0;   //stop at the end
	if ((sampler.one_shot&2) && (sine_counter[7]>(sampler.end-1000))&& (note[1].osc==3))    note[1].tuned=0;
	if ((sampler.one_shot&4) && (sine_counter[11]>(sampler.end-1000))&& (note[2].osc==3))    note[2].tuned=0;
	if ((sampler.one_shot&8) && (sine_counter[14]>(sampler.end-1000))&& (note[3].osc==3))   note[3].tuned=0;

	sine_ptr_temp[0]=&sine_counter[3];
	sine_ptr_temp[1]=&sine_counter[6];
	sine_ptr_temp[2]=&sine_counter[9];
	sine_ptr_temp[3]=&sine_counter[12];


	if(sampler.start>sampler.end) sampler.start=0; //flip to 0  for now
	if (sampler.ram_seq<sampler.start)      sampler.ram_seq=sampler.start;
	if ((sampler.ram_seq+512)>sampler.end) sampler.ram_seq=sampler.start;

	sampler.length=sampler.end-sampler.start;
	sampler.start_ptr=&RAM[sampler.start];



for (i=0;i<512;i++) {    // this should write 512 bytes , or about 15ms buffer ,oscillators
	i_total=i+sample_pointB;
	sampling_position=(i>>6);
	sample_adc=input_holder[i];
	sample_adc=(sample_adc-2040)<<12;
	ram_temp=((RAM[sampler.ram_seq])); // grab data
	ram_temp=(ram_temp-2040)<<12;
	sampler.ram_seq++;


	if ((i&63)==0)	{  patch_target_replace();    // update values , not too bad
float freq_temp=arm_sin_f32(filter[0].cutoff_1*0.006159)    ;   // need this for useful filter
	freq_pointer[0] [sampling_position]=1-freq_temp; // problem was selecting accu instead of out , good now
	 freq_temp=arm_sin_f32(filter[1].cutoff_1*0.006)    ;
	freq_pointer[1] [sampling_position] =1-freq_temp ; // filter lfos
	freq_temp=arm_sin_f32(filter[2].cutoff_1*0.006)    ;
	freq_pointer[2] [sampling_position] =1-freq_temp ; // filter lfos
	 freq_temp=arm_sin_f32(filter[3].cutoff_1*0.006)    ;
	freq_pointer[3] [sampling_position] =1-freq_temp ; // filter lfos

	}

		sample_accus[0] = sample_accus[0] + tuned_list[0];  // normal adder full volume
		sample_accus[1] = sample_accus[1] + tuned_list[1];
		sample_accus[2] = sample_accus[2] + tuned_list[2]; // bouncing somewhere
		sample_accus[3] = sample_accus[3] + tuned_list[3];  // 7 cycles

	if (sample_accus[0]>524287) sample_accus[0] =-1048576+sample_accus[0] ; // faster >  than &  ,strange,  or is even worse
	if (sample_accus[1]>524287) sample_accus[1] =-1048576+sample_accus[1] ; // faster >  than &  ,strange
	if (sample_accus[2]>524287) sample_accus[2] =-1048576+sample_accus[2] ; //
	if (sample_accus[3]>524287) sample_accus[3] =-1048576+sample_accus[3] ; //


	sample_Accu[0]=(abs(sample_accus[0])-262144)<<1;
	sample_Accu[1]=(abs(sample_accus[1])-262144)<<1;
	sample_Accu[2]=(abs(sample_accus[2])-262144)<<1;
	sample_Accu[3]=(abs(sample_accus[3])-262144)<<1;


					sample_Accu[6]=sine_count2(0,sine_ptr_temp[0],sine_ptr_temp[0]);  // might unroll later
	 	 	 	 	 sample_temp1=(*osc_pointers[0])*note[0].velocity;// needs cut a bit  maybe some diff vol settings
					sample_temp2=(sample_temp1)>>2;
				    //sample_Accu[0] =(sample_accus[0]+sample_accus[1])>>8;
					sample_Accu[7]=sine_count2(1,sine_ptr_temp[1],sine_ptr_temp[1]);  // might unroll later // might unroll later
					sample_temp1=(*osc_pointers[1])*note[1].velocity; // needs cut a bit  ,default 20bit
					sample_temp3=sample_temp1>>2; // 20+8
					//sample_Accu[1] =sample_accus[2]>>8;

					sample_Accu[8]=sine_count2(2,sine_ptr_temp[2],sine_ptr_temp[2]);  // might unroll later // might unroll later
					sample_temp1=(*osc_pointers[2])*note[2].velocity;      // needs some gain fine tune !
					sample_temp4 =sample_temp1>>2;    // 64 default 20+8


					sample_Accu[9]=sine_count2(3,sine_ptr_temp[3],sine_ptr_temp[3]);  // might unroll later // might unroll later
					sample_temp1=(*osc_pointers[3])*	note[3].velocity;  // sine out is 16bit, add 4 then 16+8
					sample_temp5 =sample_temp1>>2;
					//sample_Accu[3] =sine_out>>4;
	play_holder0[i]=sample_temp2;  // write to bank
	play_holder1[i]=sample_temp3;
	play_holder2[i]=sample_temp4;
	play_holder3[i]=sample_temp5;
	} // end of osc , doing some sound

for (pars_counter=0;pars_counter<4;pars_counter++)	{   // save sample_accu data

	sample_accus_hold[pars_counter]=sample_accus[pars_counter];

}


int32_t filter_Accu;
int32_t filter_Accu2;

//uint16_t* click=&input_holder[0];


for (i=0;i<512;i++) {    // this should write 512 bytes , or about 15ms buffer ,oscillators , filters and final out ,slow 133
	i_total=i+sample_pointB;

sampling_position=(i>>6);
// filter 1
if (		(note_toggler[i>>5]	)==(1<<(i&31)	)) 				{ADSR[0].attack_trigger =0;  trigger_counter++; trigger_counter=trigger_counter&1023  ;}


sample_Accu[0]=play_holder0[i];
sample_Accu[1]=play_holder1[i] ; // saw
sample_Accu[2]=play_holder2[i];  // sine input
sample_Accu[3]=play_holder3[i] ;

// this section is about 100 tmr cycles
if ((i&63)==0){
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

		//freq_point[0]=0.50;

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


filter_Accu=filter_Accu2=0;
filter_Accu=(sample_Accu[0]+sample_Accu[1])>>16;
filter_Accu2=(sample_Accu[2]+sample_Accu[3])>>16; //filter + drum out

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


//bank_write=0;   /// total 320 sample time (39khz)
}

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


int32_t sine_count2(uint8_t note_selected,  uint32_t* input_array, uint32_t* return_array ) {      // sine_out is the output 9  bit  , works


	//note_selected=(note_selected*3)+3;
	//uint8_t sine_zero;
	int32_t sine_tempA;
	int32_t sine_tempB;
	uint16_t RAM_size_temp=sampler.length-2;

	uint32_t sine_counter_temp=*input_array;
	uint32_t sine_counterB_temp=*(input_array+1);
	uint32_t sine_tuned=*(input_array+2);

	if (sine_counterB_temp==0) 	sine_tuned=note[note_selected].tuned;  // this needs to be fractional 20-30
	sine_counterB_temp=sine_counterB_temp+sine_tuned ;  // sine up counter per cycle , however sine adder needs to wait
	if (sine_counterB_temp>((RAM_size_temp)<<13)) sine_counterB_temp=0; //fixed for now, last is 19200 then
////////////////////////////////////////////
	sine_counter_temp=(sine_counterB_temp>>13);  // up countr controlled by counter
		sine_tempA=((*(sampler.start_ptr+sine_counter_temp))-2040)<<10;
		sine_counter_temp++;
	//	if (sine_counter_temp>=RAM_size)  sine_counter_temp=0; // limit sin_counter to sample size  set to sample length
				sine_tempB=((*(sampler.start_ptr+sine_counter_temp))-2040)<<10;  // convert to signed and +256 to -256

				sine_tempB=	((sine_tempB-sine_tempA)>>5)*(sine_counterB_temp & 31);   // calculate fraction then add (x/32 )  * (0-31)
				//note_selected=(note_selected*3)+3;
				*return_array=sine_counter_temp;  // this needs fixing
				*(return_array+1)=sine_counterB_temp;
				*(return_array+2)=sine_tuned;
				 return  (sine_tempA+sine_tempB)<<1;   // add back to start value -20k-20k  or about 16bit

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

	for (lfo_c=0;lfo_c<10;lfo_c++){   //current lfo setup , needs sampling position 0-8  and tempo_sync

	lfo_accu_temp=	lfo_accu[lfo_c][sampling_position_b];  // hold

	freq3_temp=lfo_tempo_lut[LFO[lfo_c].rate] ;  // rate. this needs a little log
		//freq4_temp=freq3_temp*freq3_temp;  // multiply  , dont more then 8000 really


			freq3_temp=lfo_accu_temp+ freq3_temp;// get lfo value plus rate , will try to get related to tempo for easier sync , at potS 80?/8192/8notes/ 1 bar


///  counts from -16k to +16 k   @ + 0.03125      to   +400    *10    or 13 ms *8 (10 hz ? fastest  )
		if (freq3_temp>62831) lfo_accu_temp=0; else lfo_accu_temp=freq3_temp; // write back value  counts -16000 to +16000
		if(!seq.pos)  lfo_accu_temp=0;   // tempo sync, mediocre
		lfo_accu[lfo_c][sampling_position]=lfo_accu_temp;
		freq_temp=lfo_accu_temp;
		freq2_temp=(freq_temp*0.0001);  //0-360

		freq_temp =arm_sin_f32(freq2_temp); // seems ok   , cmsis is ok  RADIANS !!!!!
		freq2_temp=freq_temp*LFO[lfo_c].depth*204;

		if (!LFO[lfo_c].offset)    LFO[lfo_c].offset=0;   // se
 		offset=(LFO[lfo_c].offset<<8); //  limit now for finetuning
			freq2_temp=(freq2_temp+offset);

		if (freq2_temp>32767)  freq2_temp=32767;
		if (freq2_temp<-32767) {  freq2_temp=-32767; }  // clip to 13bit -/+ 8000

						output_hold=freq2_temp;
						if(lfo_accu_temp<32768)   freq_tri=lfo_accu_temp; else freq_tri=65535-lfo_accu_temp;    // 0-32767
					freq_saw=((lfo_accu_temp*LFO[lfo_c].depth)>>8)+offset;
					if (freq_saw>65535)  freq_saw=65535;   // clip

					LFO[lfo_c].out[sampling_position]=output_hold+32767;
				   LFO[lfo_c].out_saw[sampling_position]=freq_saw;

				   freq_tri=((freq_tri*LFO[lfo_c].depth)>>7)+offset;
				   if (freq_tri>65535)  freq_tri=65535;   // clip
				  LFO[lfo_c].out_tri[sampling_position]=freq_tri;

		} // lfo gen : 0=f1 , 1=tempo,2=pitch



	}








