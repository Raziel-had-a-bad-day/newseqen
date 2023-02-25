float filter_pole(float input,float old_input, float freq1,float freq2) {

	float output;

	output=(input*freq1)+(old_input*freq2);


return  output;

}
void sampling(void){						// 330 atm or 8.5ms

//	if (time_proc>580) time_final=time_proc;

//time_proc=0;

	time_proc=0;

uint8_t mask_i;


bank_write=0;
memcpy(notes_joined,seq.notes2,16);
memcpy(notes_joined+16,seq.notes1,16);
sample_pointB=sample_pointD;
unsigned short tempo_start=0;  // enabled when i=isrMask;

uint16_t i_total;
uint16_t tempo_mod=tempo_lut[seq.tempo];  // set tempo,speed from lut 40-200bpm  ,changed to 4x for note lenght

uint8_t l;			// 35.002khz(0.02857ms) sample, 1 sample is temp count (16x=0.00045712) , *16=1 note ,at 300 (437bpm),(1/(0.00002857*tempo count*16)=1beat in s

tempo_sync=16384/((tempo_mod*16)/512) ; // 8000 at slowest 15.625 updates to lfo at 1 note 16384/15.625=1048.576+ per update  at setting 80
tempo_sync=tempo_sync/80;  // bit weird her , this is adsr !


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



note_holdA=0;

// some good phasin and delays here
uint8_t cross_fade[2]={0,0};
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

		note_reset();
		note[1].timeshift=(adc_values[0])&31; //assigned pots to start of loopers 0-16,works
		note[2].timeshift=note[1].timeshift;
		note[3].timeshift=(adc_values[1])&31;
		note[4].timeshift=note[3].timeshift;

		seq.loop[1]=((note[1].timeshift+(seq.pos&7))&31);
		seq.loop[2]=((note[2].timeshift+(seq.pos&7))&31); // calc  8 note loop positions sets looping point in sequence


			seq.loop[3]=((note[1].timeshift+(seq.pos&7))&15); //sets looping point in sequence this is full 16 note

			seq.loop[4]=((note[2].timeshift+(seq.pos&7))&15);

		//seq.loop[4]=((note[2].timeshift+((seq.pos&15)>>1))&15); // half speed
			note[1].pitch=(notes_joined[seq.loop[1]]>>4)+(note[1].transpose>>4);    // maybe join 1 and 2
			note[0].pitch=note[1].pitch+(note[0].transpose>>4); // just double

			note[2].pitch=(notes_joined[seq.loop[1]]>>4)+(note[2].transpose>>4);  //loop 8 notes from pos and x times

		note[3].pitch=(notes_joined[seq.loop[1]]>>4)+(note[3].transpose>>4); ;  //loop 8 notes from pos and x times ,might disable normal adsr completely
//	if (note[3].pitch) 		{note[3].pitch=note[3].pitch+(note[3].transpose>>4);	adsr_retrigger[3]=1; note_toggler[i>>5]=1<<(i&31   )   ; } // stay at zero for off

	note[5].pitch=(notes_joined[seq.loop[1]]>>4)+(note[5].transpose>>4);  //
		patch_target_replace();
		uint8_t detune_temp=0;
		detune_temp=(note[5].pitch+(note[5].detune>>2))&31    ;
		if(detune_temp>27) detune_temp=27;
		   //this is for sine skip mask

	note[5].tuned=sine_lut[MajorNote[detune_temp]];	//sets freq ,1.0594  * 16536 =17518  ,
	note[5].tuned= (note[5].tuned*1200)>>10;  // modify different sample size , just need single cycle length and thats it
		mask_result =0;

		for (mask_i=0;mask_i<5;mask_i++)	{							// calc detune , slow ,also creates notes

	if (note[mask_i].pitch) {

	//	note[mask_i].pitch=(note[mask_i].pitch ;

		detune_temp=(note[mask_i].pitch+(note[mask_i].detune>>2))&31    ;
		if(detune_temp>27) detune_temp=27;
		note[mask_i].tuned=sample_Noteadd[MajorNote[detune_temp]];


	    } // relies on note channel clear , not good , clear not channel straight after

	}

		//note_tuned[3]=2751;
	} // end of note calcualte



	if ((i&63)==0)
	{

		LFO_source(); // calculate lfo maybe 8 times for now
	}

}  // end of note  i loop
 //make sure it's finished

// filter loop

uint8_t sine_zero;
int32_t  sample_temp1;

for (i=0;i<512;i++) {    // this should write 512 bytes , or about 15ms buffer ,oscillators
	i_total=i+sample_pointB;
	sampling_position=(i>>6);

	if ((i&63)==0)	{  patch_target_replace();    // update values , not too bad
float freq_temp=arm_sin_f32(filter[0].cutoff_1*0.01)    ;   // need this for useful filter
freq_pointer[0] [sampling_position]=1-freq_temp; // problem was selecting accu instead of out , good now
 freq_temp=arm_sin_f32(filter[1].cutoff_1*0.006)    ;
freq_pointer[1] [sampling_position] =1-freq_temp ; // filter lfos
freq_temp=arm_sin_f32(filter[2].cutoff_1*0.006)    ;
freq_pointer[2] [sampling_position] =1-freq_temp ; // filter lfos
 freq_temp=arm_sin_f32(filter[3].cutoff_1*0.006)    ;
freq_pointer[3] [sampling_position] =1-freq_temp ; // filter lfos



	}

// every step   1,110,928   >>20  ,per note
// New oscillators , sync, trigger input , waveshape ,zero cross
	sample_accus[0] = sample_accus[0] + note[0].tuned; //careful with signed bit shift,better compare

	if (sample_accus[0]>524287) sample_accus[0] =-sample_accus[0] ; // faster >  than &  ,strange

	sample_accus[1] = sample_accus[1] + note[1].tuned;  // normal adder full volume

			if (sample_accus[1]>524287) sample_accus[1] =-sample_accus[1] ; // faster >  than &  ,strange

			sample_accus[2] = sample_accus[2] + note[2].tuned;

					if (sample_accus[2]>524287) sample_accus[2] =-sample_accus[2] ; // faster >  than &  ,strange

					sample_accus[3] = sample_accus[3] + note[3].tuned; // bouncing somewhere

							if (sample_accus[3]>524287) sample_accus[3] =-sample_accus[3] ; // faster >  than &  ,strange

							sample_accus[4] = sample_accus[4] + note[4].tuned;

									if (sample_accus[4]>524287) sample_accus[4] =-sample_accus[4] ; // faster >  than &  ,strange

									sample_Accu[0] =sample_Accu[1] =sample_Accu[2]=sample_Accu[3] =0; //all zeroed 20 bits



									sample_temp1=(sample_accus[1]+sample_accus[0])>>9; // needs cut a bit  ,default 20bit
								    sample_Accu[0]=sample_temp1*note[1].velocity; // 20+8

									sample_temp1=sample_accus[2]>>8; // needs cut a bit  ,default 20bit
									sample_Accu[1]=sample_temp1*note[2].velocity; // 20+8


									sample_temp1=sample_accus[3]>>8;
									sample_Accu[2] =sample_temp1*note[3].velocity;    // 64 default 20+8


									sample_temp1=sine_out*	note[5].velocity;  // sine out is 16bit, add 4 then 16+8
									sample_Accu[3] =sample_temp1>>4;


if (sine_counterB==0) 	sine_temp2=note[5].tuned;

	sine_counterB=sine_counterB+sine_temp2 ;  // sine up counter per cycle , however sine adder needs to wait
	if (sine_counterB>>7) sine_zero=0; else sine_zero=1;

if (sine_counterB>(sine_length<<5)) sine_counterB=0; //fixed for now
sine_count(); // calc sine   distortion out when hcagning note
play_holder0[i]=sample_Accu[0];  // write to bank
play_holder1[i]=sample_Accu[1];
play_holder2[i]=sample_Accu[2];
play_holder3[i]=sample_Accu[3];
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



for (i=0;i<512;i++) {    // this should write 512 bytes , or about 15ms buffer ,oscillators , filters and final out ,slow 133
	i_total=i+sample_pointB;

sampling_position=(i>>6);
// filter 1
if (		(note_toggler[i>>5]	)==(1<<(i&31)	)) 				{ADSR[0].attack_trigger =0;  trigger_counter++; trigger_counter=trigger_counter&1023  ;}


sample_Accu[0]=play_holder0[i];  // sine input
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


filter_Accu=0;
filter_Accu=sample_Accu[0]+sample_Accu[1]+sample_Accu[2]+sample_Accu[3]; //filter + drum out
filter_Accu=filter_Accu>>5;

 if (one_shot!=199)   one_shot++;  //play one attack then stop

 if (filter_Accu>0xFFFF) {filter_Accu=0xFFFF;clipping++;} else if (filter_Accu<-65535) filter_Accu=-65535;  // limiter to 16 bits


 play_sample[i_total]=(filter_Accu>>6)+1024;   // final output disable for now 2544

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

void sine_count(void) {         // sine_out is the output 9  bit  , works

	int32_t sine_tempA;
	int32_t sine_tempB;
	int8_t sine_frac;


	sine_counter=(sine_counterB>>5);  // up countr controlled by counter
		if (sine_counter>sine_length) sine_counter = sine_length;		// seems to be faster than using a for loop to calculate both values
		sine_out = sine_block[sine_counter];  // 0- 40000
		sine_tempA=sine_out; // grab first value , needs to be always plus
		sine_tempA=sine_tempA-20000; //convert to signed

		sine_counter++;
		if (sine_counter>=sine_length)  sine_counter=0; // set to sample length
				sine_tempB=sine_block[sine_counter]-20000;  // convert to signed and +256 to -256
				sine_tempB=	((sine_tempB-sine_tempA)>>5)*(sine_counterB & 31);   // calculate fraction then add
				sine_out=(sine_tempA+sine_tempB);   // add back to start value -20k-20k  or about 16bit


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








