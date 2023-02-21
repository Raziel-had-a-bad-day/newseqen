void sampling(void){						// 330 atm or 8.5ms

//	if (time_proc>580) time_final=time_proc;


//time_proc=0;

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

tempo_sync=16384/((tempo_mod*16)/512) ; // 8000 at slowest 15.625 updates to lfo at 1 note 16384/15.625=1048.576+ per update  at setting 80
tempo_sync=tempo_sync/80;  // bit weird her , this is adsr !

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


		note[2].timeshift=(adc_values[0]>>2)&15; //assigned pots to start of loopers 0-16,works
		note[3].timeshift=(adc_values[1]>>2)&15;


		seq.loop[2]=((note[2].timeshift+(seq.pos&7))&15); // calc  8 note loop positions sets looping point in sequence

		//seq.loop[3]=(note[3].timeshift+(( seq.pos&31 ) >>2)) & 15;  // quater speed
			seq.loop[3]=((note[3].timeshift+(seq.pos&7))&15); //sets looping point in sequence this is full 16 note

			seq.loop[4]=((note[2].timeshift+(seq.pos&7))&15);

		//seq.loop[4]=((note[2].timeshift+((seq.pos&15)>>1))&15); // half speed

		note[2].pitch=(seq.notes2[seq.loop[2]]>>4)+(note[2].transpose>>4);  //loop 8 notes from pos and x times
		note[3].pitch=(seq.notes1[seq.loop[3]]>>4);  //loop 8 notes from pos and x times ,might disable normal adsr completely
	if (note[3].pitch) 		{note[3].pitch=note[3].pitch+(note[3].transpose>>4);	adsr_retrigger[3]=1; note_toggler[i>>5]=1<<(i&31   )   ; } // stay at zero for off

	note[5].pitch=(seq.notes2[seq.loop[2]]>>4)+(note[5].transpose>>4);  //
		lfo_target_replace();

	note[5].pitch=MajorNote[note[5].pitch];    //this is for sine skip mask
	//note[5].pitch=(note[5].pitch*(note[5].detune))>>7 ; // works ok with single note @24 but   fails on other
	note[5].tuned=sine_lut[note[5].pitch];	//sets freq ,1.0594  * 16536 =17518  ,
	note[5].tuned= (note[5].tuned*1200)>>10;  // modify different sample size , just need single cycle length and thats it
		mask_result =0;

		for (mask_i=0;mask_i<5;mask_i++)	{							// calc detune , slow ,also creates notes

	if (note[mask_i].pitch) {

	//	note[mask_i].pitch=(note[mask_i].pitch ;
		tune_Accu=sample_Noteadd[MajorNote[note[mask_i].pitch&15]];


	note[mask_i].tuned=(tune_Accu);       } // relies on note channel clear , not good , clear not channel straight after

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
int32_t play_holder1[512];    // data banks
int32_t play_holder2[512];
uint8_t sine_zero;
int32_t  sample_temp1;
int32_t  sample_temp2;
for (i=0;i<512;i++) {    // this should write 512 bytes , or about 15ms buffer ,oscillators
	i_total=i+sample_pointB;
	sampling_position=(i>>6);

	if ((i&63)==0)	lfo_target_replace();    // update values , not too bad

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
									sample_temp1=sample_accus[2]>>10; // needs cut a bit  ,default 20bit

									sample_temp2=sample_temp1*note[2].velocity; // 20+8
									sample_Accu[0]=sample_temp2;

									sample_temp1=sine_out*	note[5].velocity;  // sine out is 16bit, add 4 then 16+8
									sine_out=sample_temp1>>4;

									sample_Accu[0] = ((sine_out+sample_Accu[0])*cross_fade[1]);   // sine input plus other

									sample_temp1=sample_accus[3]>>8;
									sample_temp2=sample_temp1*note[3].velocity;    // 64 default 20+8
									sample_Accu[3] =sample_temp2;



if (sine_counterB==0) 	sine_temp2=note[5].tuned;

	sine_counterB=sine_counterB+sine_temp2 ;  // sine up counter per cycle , however sine adder needs to wait
	if (sine_counterB>>7) sine_zero=0; else sine_zero=1;

if (sine_counterB>(sine_length<<5)) sine_counterB=0; //fixed for now
sine_count(); // calc sine   distortion out when hcagning note
play_holder1[i]=sample_Accu[0];  // write to bank
play_holder2[i]=sample_Accu[3];


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

//if (feedback_out>0xFFFF) feedback_out=0xFFFF; else if (feedback_out<-65535) feedback_out=-65535;  // limiter to 16 bits
//sample_Accu[1]=input_holder[i];

//sample_Accu[1]=(sample_Accu[1]-2020)<<14; // shift to correct level

//sample_Accu[1]=sample_Accu[1]-60000;
sample_Accu[1]=play_holder1[i];  // sine input
sample_Accu[3]=play_holder2[i] ; // saw

// this section is about 100 tmr cycles

freq_point[0]=freq_pointer[0] [sampling_position];; // load up coeffs
//freq_point[1]=freq_pointer[1] [sampling_position];
freq_point[2]=freq_pointer[2] [sampling_position];  // ok , array was too short
//freq_point[3]=freq_pointer[3] [sampling_position];
//freq_point[0]=0.5;  //was ok with this
	//	freq_point[2]=0.5;


adsr_level[3] = adsr_lut	[i>>1];


if (freq_point[0]>1) freq_point[0]=1; else if (freq_point[0]<0) freq_point[0]=0;// just in case
		//freq_point[0]=0.50;
		freq_point[1]=1-freq_point[0];
		//sample_Accu[1] = sample_Accu[1]>>13;

		//int16_t  ADSR[0].buffer_temp2=lfo_out [1] [i>>6];

		//ADSR[0].buffer_temp2=ADSR[0].buffer_temp2-8195;
		//sample_Accu[1] = sample_Accu[1] *lfo_out [0] [i>>6];     // vol lfo
		sample_Accu[1] = sample_Accu[1] ;

		filter_accus[1]=sample_Accu[1]; // saw
	//	filter_accus[1]=	filter_accus[1]*adsr_level[3][sampling_position];
		//filter_accus[1]=	filter_accus[1]*adsr_level[3];

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
filter_Accu=(sample_Accu[0]+sample_Accu[2])>>8; //filter + drum out

//filter_Accu=(sample_Accu[1]+sample_Accu[3])>>8; //filter + drum out ,clean out
 //filter_Accu=sample_Accu[1]>>7;

// filter_Accu=sample_Accu[2]>>11;
//filter_Accu=(sample_Accu[1]>>7)+(sample_Accu[3]>>8); //filter + drum out
 if (one_shot!=199)   one_shot++;  //play one attack then stop

 if (filter_Accu>0xFFFF) filter_Accu=0xFFFF; else if (filter_Accu<-65535) filter_Accu=-65535;  // limiter to 16 bits


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

			sine_out=(sine_tempA+sine_tempB);   // add back to start value -20k-20k  or about 16bit

			//sine_tempA=sine_tempA; //needs

			//sine_out=sine_tempA;


}


void LFO_source(void){     // lfo


float	freq_temp=0;
float 	freq2_temp=0;
uint8_t l ;
float offset=0;
uint32_t lfo_accu_temp;
	uint32_t freq3_temp;
	uint32_t freq4_temp;

for (l=0;l<10;l++){   //current lfo setup , needs sampling position 0-8  and tempo_sync



	lfo_accu_temp=	lfo_accu[l][sampling_position];  // hold

	freq3_temp=LFO[l].rate +1;  // rate. this needs a little log
		freq4_temp=freq3_temp*freq3_temp;  // multiply the rate +32 000 max


			freq3_temp=lfo_accu_temp+ freq4_temp;// get lfo value plus rate , will try to get related to tempo for easier sync , at potS 80?/8192/8notes/ 1 bar


///  counts from -16k to +16 k   @ + 0.03125      to   +400    *10    or 13 ms *8 (10 hz ? fastest  )
		if (freq3_temp>62831) lfo_accu_temp=0; else lfo_accu_temp=freq3_temp; // write back value  counts -16000 to +16000

		lfo_accu[l][sampling_position]=lfo_accu_temp;
		freq_temp=lfo_accu_temp;
		freq2_temp=(freq_temp*0.0001);  //0-360
		freq_temp =arm_sin_f32(freq2_temp); // seems ok   , cmsis is ok  RADIANS !!!!!
		freq2_temp=freq_temp*LFO[l].depth*51;

		if (!LFO[l].offset)    LFO[l].offset=0;   // set to 64 if its at 0
 		offset=(LFO[l].offset<<5)-2048; //  limit now for finetuning
			freq2_temp=(freq2_temp+offset);

		if (freq2_temp>8195)  freq2_temp=8195;
		if (freq2_temp<-8195)  freq2_temp=-8195;   // clip to 13bit -/+ 8000


				   LFO[l].out[sampling_position]=freq2_temp+8196;


		} // lfo gen : 0=f1 , 1=tempo,2=pitch


			freq_pointer[0] [sampling_position]=0.99; // problem was selecting accu instead of out , good now
	freq_pointer[2] [sampling_position] =0.99; // filter lfos

	}








