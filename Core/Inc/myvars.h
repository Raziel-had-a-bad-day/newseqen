

// STM32F411

// if folding problem , space after // 
/*int _write(int file, char *ptr, int len) {   	// code sw viewer
	 int DataIdx;
	 for (DataIdx = 0; DataIdx < len; DataIdx++){
		 ITM_SendChar(*ptr++);
	 }
	 return len;
 }
*/

// flash start


// flash start 0x08000000    , prog size  59B0  22,960 bytes  end   0x080059B0 , 64k ends at 0x08010000 , maybe after or D2F0 which is 54k so 0x0800D2F0
// source , select mask type , speed of playback

const uint32_t flash_start=0x0800D2F0; // flash start address , 2048 page saize or 0x800
const uint32_t flash_page=0x800;



uint32_t x;  // calc timer note
uint32_t y;
uint16_t pwmVel;
uint16_t pwmVelB;
//const uint8_t tempoLUT[];  // lookup table for tempo instead of calculate
const uint8_t MajorNote[]= { 0,2,4,6,7,9,11,13,14,16,18,19,21,23,25,26,28,30,31,33,35,37,38,40,42,43,45} ;  // major
const uint8_t MinorNote[]={ 0,2,3,5,7,8,10,12,14,15,17,19,20,22,24,26,27,29,31,32,34,36,38,39,41,43,44,46}; // minor
const uint8_t ChromNote[]={0,2,3,5,6,8,9,11,12,14,15,17,18,20,21}; //chromatic, diminished
//const uint8_t noteReference[] = {10,10,10,10,0,10,0,8,1,10,1,8,2,10,3,10,3,8,4,10,4,8,5,10,5,8,6,10,0,11,0,8,1,11,1,8,2,11,3,11,3,8,4,11,4,8,5,9,5,8,6,9,10,11,10,20,10,11,10,12,10,13,10,14,10,15,10,16,10,17,10,18,10,19,11,20,11,11 };// cant read last
const uint16_t timerValues[]= {34400,32469,30647,28927,27303,25771,24324,22959,21670,20454,19306,18222,17200,16234,15323,14463,13651,12885,12162,11479,10835,10227,9653,9111,8600,8117,7661
,7231,6825,6442,6081,5739,5417,5113,4826,4555,4300,4058,3830,3615,3412,3221,3040,2869,2708,2556,2413,2277} ;   // timer values C2-C6
const uint16_t sample_Noteadd[50]= { 2059, 2181, 2311, 2448, 2594, 2748, 2912, 3085, 3268, 3462, 3668, 3886, 4117, 4362, 4622, 4896, 5188, 5496, 5823, 6169, 6536, 6925, 7336, 7773, 8235, 8725, 9243, 9793,
		10375, 10992, 11646, 12338, 13072, 13850,
		14673, 15546, 16470, 17449, 18487, 19586, 20751, 21985, 23292, 24677, 26145, 27699, 29346};  // 35khz add c2-c6  >>12 for correct value , 0-255 samples sawtooth , /2 for extra notes ,prefer circular , replaced first 2059 with 0

//static unsigned short playWave;
const uint16_t freq_lut[]={4186,4434,4698,4978,5274,5587,5919,6271,6644,7039,7458,7902,8371,8869,9397,9955,10547,11175,11839,12543,13289,14079,14916,15803,16743,17739,
		18794,19911,21095,22350,23679,25087,26578,28160,29834,31608,33488,35479,37589,39824,42192,44701,47359,50175,53158,56319,59668,63216};  // freq lookup x64  64hz_>987hz , C2-C6 Note 1-48
//const uint8_t disp_lut [18] [16]= {                    // menu look up




const uint16_t disp_lut [32] [16]= {							 // menu look up using char , this is default but will be dynamic


		{208,209,210,211,212,213,214,215,216,217,218,219,220,221,222,223}, // 16 notes for drums and other tones 80 - 95

		{128,129,130,131,132,133,134,135,136,137,138,139,140,141,142,143},  // 16 notes 0-15 
		{'A'  ,64 ,148 ,149 ,64,64,64,64,64,64,64,64,'T',237,64,64},	// 20,21    attack sustain  maybe follow a diff system ,attach+decay then sustain+release
		{'P',64,200,64,201,64,202,64,64,64,64,64,64,64,64,64},  // pitch settings and loop start

		{'S',64,258,64,259,64,260,64,261 ,64,262,64,263,64,64,64},	// lfo speed 1-7

		{'D',64,268,64,269,64,270,64,271,64,272,64,273,64,64,64},   // lfo depth 1-7
		{'G',64,64,64,64,64,64,64,264,64,64,64,64,64,64,64},	// lfo gain


		{64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64},   // feedback



	//	{84 ,121, 112, 101, 78, 111, 116, 64,240,241,242,243,244,245,246,247},	//feedback

		//		{176,177,178,64,179,180,181,64,182,183,184,64,185,186,187,64},						//p4 tone mods

			//	{98 ,97, 114, 64, 80, 105, 116, 99, 104,64,200,201,202,203,64,64}, //p5  bug with 66?

		//{192,64,193,64,194,64,195,64,196,64,197,64,198,64,199,64},					// keychange
				{'L','F','O','1','=',330,64,64,64,64,64,64,64,64,64,64},
				{'L','F','O','2','=',331,64,64,64,64,64,64,64,64,64,64},
				{'L','F','O','3','=',332,64,64,64,64,64,64,64,64,64,64},
				{'L','F','O','4','=',333,64,64,64,64,64,64,64,64,64,64},
				{'L','F','O','5','=',334,64,64,64,64,64,64,64,64,64,64},
				{'L','F','O','6','=',335,64,64,64,64,64,64,64,64,64,64},
				//{'L','F','O','6','=',552,553,554,555,556,557,558,559,64,64,64},
				{64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64},					//p6 empty
				{64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64},
				{'s','t','a','r','t',64,64,64,64,64,64,64,64,64,64,64},
				{64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64},
				{64,64,274,64,275,64,64,64,64,64,64,64,64,64,64,64},
		{64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64},
		{64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64},					//p6
		{64,64,64,64,64,64,64,64,64,64,64,64,'E','n','d',64},
		{64,64,64,64,64,64,64,64,64,64,64,64,'E','n','d',64},			//p8
		{64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64},
		{64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64},
		{64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64},
				{64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64},
				{64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64},
				{64,64,64,64,64,64,64,64,64,64,64,64,'E','n','d',64},
						{64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64},
						{64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64},






};

uint16_t enc2_lut[180];   // lut to skip cells ,filled in on main
uint16_t feedback_line[17]={'F','o','o','d','b','a','c','k','g','_' ,'k',64, 278, 279, 280,64}; // this is variable , last display line for info
uint16_t feedback_pointer;  // pointer for feedback item start position , maybe bit search for less menu data ,dunno

int _write(int file, char *ptr, int len)
{
	int DataIdx;

	for (DataIdx = 0; DataIdx < len; DataIdx++)
	{
	   ITM_SendChar( *ptr++ );
	}

	return len;
}
const char* menu_items[]  =  {"menu_edit","yes","no,square","sine","triangle,saw",				// menu_loc has reference potSource locations
		"OSC1_Notes","OSC2_Notes","OSC3_Notes",
		"OSC1_pitch","OSC2_pitch","OSC3_pitch",
		"OSC1_attack","OSC2_attack","OSC3_attack",
		"OSC1_decay","OSC2_decay","OSC3_decay",
		"LFO1_wave","LFO2_wave","LFO3_wave","LFO4_wave","LFO5_wave","LFO6_wave",
		"LFO1_speed"," LFO2_speed","LFO3_speed","LFO4_speed","LFO5_speed","LFO6_speed",
		"LFO1_depth","LFO2_depth","LFO3_depth","LFO4_depth","LFO5_depth","LFO6_depth",
		"LFO1_gain","LFO2_gain","LFO3_gain","LFO4_gain","LFO5_gain","LFO6_gain",
		"LFO1_offset","LFO2_offset","LFO3_offset","LFO4_offset","LFO5_offset","LFO6_offset",
		"LFO1_out","LFO2_out","LFO3_out","LFO4_out","LFO5_out","LFO6_out"

};
const uint16_t menu_loc[]  ={511,511,511,511,511,511,511,511,  // potSource ref for menu_items[]  ,  maybe above 512 its text reference(not yet) , also 0-128 is just characters ,128-143 notes 1 , 208-223 notes 2
		200,201,202,	//pitch
		148,148,148,  // no new ref yet
		149,149,149,	 // no new ref yet
		300,301,302,303,304,305,   // lfo wave not used yet
		258,259,260,261,262,263,   //lfo speed
		268,269,270,271,272,273,  // lfo depth
		310,311,312,313,314,315, // lfo gain not used
		320,321,322,323,324,325,  // lfo offset not used
		330,331,332,333,334,335  // lfo target not used yet

}; //56
uint16_t menu_locA=0;
char menulist_hold[20]=" not working at all" ;
const char menuList[129] = "_Notes_1_NotePtchNotes_2_AttkDcayLFOspeedLFOdepthLFO_gainkeepgoinNext page_LFOSSWITCHESREPEAT__KEY#____Time____NTE2_____________";   // top menu
const char menuList2 [129] = "_LPF_1___LPF_2___PITCH_1_PITCH_2_PITCH_3_EMPTY___NotePtchNotes_2_AttkDcayLFOspeedLFOdepthLFO_gainkeepgoinNext page_LFOSWI_______";   // lfo target list, etc
const char mainNote[29] = "_CDEFGABCDEF>_1234567890+>_x" ; //note string values 7 notes and flat and sharp which are double length ? 11=1  20=0 21-
uint8_t menulength;



// const char mainNoteLow[] = "cdefgab_^: " ; // note string for off values
const uint8_t noteSwitch[] = { 255,0,204,51,170,85,240,15,17,252,0 };  // note switching using bitread , alternating pattern on, off, 11001100, 00110011,10101010,01010101,11110000,00001111,00010001,11101110
const uint16_t lfoGainValues[8] = {}; // gain adder values
const uint8_t  waveSaw[17] = {  10,31,47,63,79,95,111,127,0,31,47,63,79,95,111,127}; //lfo saw values lut ,not used

const uint8_t waveTri[17] = { 0,15,31,47,63,79,95,111,127,111,95,79,63,47,32,15 }; //lfo triangle values lut ,not used

uint8_t potValues [383];  //low res values mostly for display

uint8_t potSource[383]; // high res version of potValues used when needed 40-0, gonna change to 160 just o break things, need more res for lfo
// lfos speed=150-160 ,lfo_depth=160-170,lfo_gain=170-180;lfo_target=180-190;  80-95=notes,

uint8_t notePos2;
//static unsigned short lfoNext;

uint8_t noteTiming;  // set timing shift
static unsigned short noteTimeFlag;
uint8_t noteTimebit[]= {0,170,85,240,15,204,51,153,102,165,90,0}   ;   // bit switch for adjustimg rythm
uint16_t noteAccu; //adder for left over timing
uint16_t tempAccuA;  // accu for pots
volatile uint8_t promValue; //eprom
volatile uint8_t promValues[64]; // hold original potvalues here
uint8_t noteLength=16; // lenght of note 16 ticks default
uint8_t noteDelay; // initial delay for trigger

uint8_t potPosition=0; // potValues pointer
uint8_t modSwitches [9] ={0,0,0,0,0,0,0,0 }; // function switch storage ,all values are 0-2
 uint8_t ccValues[6]={}; //cc memories storage
uint8_t currentValues [10] = {  };  //note switches on,stop,off  storage 15=on 0=off 1=loop
uint8_t currentValuesB [10] = {  };  // note switch  storage for lcd
uint8_t pitchValues[9] = {  }; // remember pitch values of key pot
uint8_t noteDuration[9] = {  }; // note length array
uint8_t displayMem[9] = { }; // past stored values for lcd
uint8_t displayMemVelocity[9] = {};
uint8_t lcdBufferString; // string storage
uint8_t lcdBufferCounter;  // counter in lcd buffer
uint8_t lcdBufferCounterB;  // counter in lcd buffer
char lcdBufferMem[33] = {};  // string memory for lcdbuffering
char lcdBufferMemNew[33] = {};  // string memory for lcdbuffering incoming
uint8_t memNote[10] = { 0,0,0,0,0,0,0,0,0 };  // current note values for lcd
uint8_t tempValue[127]={ }; // last value is menu control
uint8_t tempValueB[127]={ };
uint16_t tempValueA;
uint8_t potRange=54; // range of potvalues 8/14
// uint8_t modMatrix[65]={};  // storage for all modulation options, new
const char modTop[] = "Nt_WvRtWdTyDp"; // modulation top lcd row : Note(1-8),LFO wave type(0-3) 0 is off , Rate(1-8), Pulse width(1-8) , velocity pitch or time modulation(1-3) , depth goes +- for inversion (+5 -5)
unsigned short modEnable=0;   // true when menu pot above 0
uint8_t menuSelect;     // menu selector pot
uint8_t menuSelectB;  // old menu data
uint8_t menuSelectX; // menu horizontal
uint8_t menuSelectX2; // old menu
 unsigned short firstRound=1;
//static unsigned short modMenu=0;
uint8_t modRate; // lfo rate
uint8_t modPhase; // lfo phase
uint8_t modDelay; // lfo initialdelay
uint8_t modPosition; // like notePosition but freewheel
uint8_t modPositionB; // like modPosition but shifted
uint16_t modGain;// moddepth setting ,not sure yet can be -
uint16_t modPitch;// pitch modifier
uint16_t modBarPitch; // changes bar pitch , 8 bars
uint8_t modPointer; // mod pointer
uint8_t modOut; // mod output
//static unsigned short modStep;// mod up count
//static unsigned short centreLed;  // locate positions with led
uint8_t oldNote; //store previous note
uint8_t oldNote2;
uint8_t eventCounter=0; // event timer
//static unsigned short writeCycle=1; // switch write and read
uint8_t notePosition; // seq position for noteplayback 8x8 for now 0-63
uint8_t lfoWave[4]={};
uint8_t lfolastccA[4]={};
uint8_t lfoPhase;
//static unsigned short displayFirst=1; // display loop firs round
uint16_t isrDivide; // counter for isrcount since notimer for now
uint8_t  Note=0;
uint8_t NoteB;
//static unsigned short loopMode=0;
uint8_t octKey = 0;
//static unsigned short NoteSlide=0;
uint8_t NoteBuffer=0;
uint8_t patternType=0;
uint8_t keyPot=0; // make it real time only from now
uint8_t keyPotB=5;
uint8_t keyPotC=0; // storage for analogeread
 uint8_t counterVar=0;
uint16_t counterVarB;
uint8_t switchOne=0;
//static unsigned short outNote;  // detect note process
uint8_t patternEditPos=1; // bar position for pattern edit
uint16_t valueOriginal; // stored value  needs signed
uint16_t valueCurrent; //current setting
uint8_t valueReturn; // final value out
uint8_t valueChange[9]={1,1,1,1,1,1,1,1}; // enable writing
uint8_t barCount=0;  // counts +1 per 8 bars ie
uint8_t barCountB=1;// dec 1-8
uint8_t barCountC=1;
uint16_t barCountTotal=0;  // total note position in 32 bars 95 * 8 , 16bit
uint16_t lastbarCountB=0; // ( dec signed barcountB-2)  0-7 pointer
uint8_t nextbarCountB=0; // next barcounter
//static unsigned short firstbarWrite=1;  // write during first 8 bars
uint8_t firstbarLoop;
uint8_t firstbarLoopB;// bit shifted
volatile int  midiTimer =0; // timing from midiclock to notes

volatile uint16_t isrTemp;
uint8_t isrCountC;

uint16_t isrPoint;
uint16_t isrPointB;  // for noteBar jump by 4
uint16_t isrCount; // ISR counter tcik 32 bit
volatile uint16_t isrCountB; //  512-820 per tick 16bit
volatile uint16_t isrMask;  // tempo mask for tick  16 bit
uint8_t lcdCounter=0;
uint8_t electPitch=64; // Electribe pitch
 uint8_t tempo=20; //delay timer
uint8_t tempoTemp;
uint32_t timerVar ;  // multipler for timer1 lenght for freq , too big needs to be 16bit
uint32_t timerVar2; // timer 2
uint8_t noteMem=0;
uint8_t midiMemory[70]={}; //midi storage for incoming
uint8_t midSettings[]={1,1,0,0}; // midi channel and lfo cc setting
uint8_t midiBarcount=0; // note off var rollover on 8
volatile int  t =0;
uint32_t timExe=0;
uint8_t timeCount=0; //time execute schedule once per note
uint8_t noteTraffic[4]={1,48,0,0}; // remember if note was sent from midi or sequencer : switchone , outNote, midi stored value ,0
uint8_t changeNote=2; // note on off flags
uint16_t sine_counter;  // up counter for sine reading
uint16_t sine_counterB;  // up counter for sine reading ,fractional * 8
int32_t sine_out;     // generated sine output 9 bit
uint16_t sine_temp2;
volatile uint8_t lcdstep_toggle;
uint8_t disp_bit;
uint32_t lcd_holder;
uint8_t lcd_return[16];
uint8_t feedback_temp;
uint16_t note_tuned[11];
	uint32_t tune_Accu;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_ADC1_Init(void);
static void MX_SPI2_Init(void);
static void MX_TIM3_Init(void);
static void MX_TIM4_Init(void);
static void MX_I2C2_Init(void);
static void MX_TIM2_Init(void);
/* USER CODE BEGIN PFP */
void display_init(void);

//void adc_read(void);
void analogInputloop(void);
void displayLoop(void);
void displayBuffer(void);
void timerNote(void);
void timerNote2(void);
void sine_count(void);
void sampling(void);
void noteCalc(void);

void lfo(void);
void print_pot(void);
void analoginputloopb(void);
void adsr(void);
void display_gfx(void);
void SPI_command(void);
void display_update(void);
void display_fill(void);
//void velocityLFO(void);
uint8_t seq_pos; // sequencer position linked to isrCount for now but maybe change

void  mask_calc(uint8_t mask_select,uint8_t mask_speed);

 uint16_t noteBar[257]={0,12,12,12,12,12,12,12,12,12,1,22,1};  //   8 bar data , start , end ,vel,wave * 8  3*wave note length cant be uint32_ter than next start
uint8_t NoteC; // second channel note
 uint8_t spell[322] = { 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78,
		79, 254,192, 65, 66, 67, 68, 65, 70, 71, 72, 70, 74, 75, 76, 77, 78,79,79,254,128,0 }; //16x2 chars
uint8_t spellB[42]; // temp store
const uint16_t sine_block2[514]={20000,20245,20491,20736,20981,21226,21471,21716,21960,22204,22448,22692,22935,23177,23419,23661,23902,24142,24382,24621,24860,25097,25334,25570,25806,26040,26274,26506,26738,26968,27198,27426,27654,27880,28105,28329,28551,28772,28992,29211,29428,29644,29858,30071,30282,30492,30700,30906,31111,31315,31516,31716,31914,32110,32305,32497,32688,32877,33063,33248,33431,33612,33791,33968,34142,34315,34485,34653,34819,34983,35144,35303,35460,35615,35767,35917,36064,36209,36352,36492,36629,36764,36897,37027,37155,37279,37402,37521,37638,37753,37864,37973,38080,38183,38284,38382,
		38478,38570,38660,38747,38831,38912,38991,39066,39139,39209,39276,39340,39401,39459,39514,39566,39616,39662,39706,39746,39784,39818,39850,39878,39904,39926,39946,39962,39976,39986,39994,39998,40000,39998,39994,39986,39976,39962,39946,39926,39904,39878,39850,39818,39784,39746,39706,39662,39616,39566,39514,39459,39401,39340,39276,39209,39139,39066,38991,38912,38831,38747,38660,38570,38478,38382,38284,38183,38080,37973,37864,37753,37638,37521,37402,37279,37155,37027,36897,36764,36629,36492,36352,36209,36064,35917,35767,35615,35460,35303,35144,34983,34819,34653,34485,34315,
		34142,33968,33791,33612,33431,33248,33063,32877,32688,32497,32305,32110,31914,31716,31516,31315,31111,30906,30700,30492,30282,30071,29858,29644,29428,29211,28992,28772,28551,28329,28105,27880,27654,27426,27198,26968,26738,26506,26274,26040,25806,25570,25334,25097,24860,24621,24382,24142,23902,23661,23419,23177,22935,22692,22448,22204,21960,21716,21471,21226,20981,20736,20491,20245,20000,19755,19509,19264,19019,18774,18529,18284,18040,17796,17552,17308,17065,16823,16581,16339,16098,15858,15618,15379,15140,14903,14666,14430,14194,13960,13726,13494,13262,13032,12802,12574,
		12346,12120,11895,11671,11449,11228,11008,10789,10572,10356,10142,9929,9718,9508,9300,9094,8889,8685,8484,8284,8086,7890,7695,7503,7312,7123,6937,6752,6569,6388,6209,6032,5858,5685,5515,5347,5181,5017,4856,4697,4540,4385,4233,4083,3936,3791,3648,3508,3371,3236,3103,2973,2845,2721,2598,2479,2362,2247,2136,2027,1920,1817,1716,1618,1522,1430,1340,1253,1169,1088,1009,934,861,791,724,660,599,541,486,434,384,338,294,254,216,182,150,122,96,74,54,38,24,14,6,2,
		0,2,6,14,24,38,54,74,96,122,150,182,216,254,294,338,384,434,486,541,599,660,724,791,861,934,1009,1088,1169,1253,1340,1430,1522,1618,1716,1817,1920,2027,2136,2247,2362,2479,2598,2721,2845,2973,3103,3236,3371,3508,3648,3791,3936,4083,4233,4385,4540,4697,4856,5017,5181,5347,5515,5685,5858,6032,6209,6388,6569,6752,6937,7123,7312,7503,7695,7890,8086,8284,8484,8685,8889,9094,9300,9508,9718,9929,10142,10356,10572,10789,11008,11228,11449,11671,11895,12120,
		12346,12574,12802,13032,13262,13494,13726,13960,14194,14430,14666,14903,15140,15379,15618,15858,16098,16339,16581,16823,17065,17308,17552,17796,18040,18284,18529,18774,19019,19264,19509,19755,20000,20000}; //sine 16bit , 512 samples 68.66HZ  /note sample_noteadd >>12
unsigned char kick[202]={0x7f,0x82,0x97,0xae,0xc1,0xd3,0xe2,0xef,0xf2,0xf7,0xf7,0xf7,0xf5,0xef,0xea,0xe5,0xe0,0xd8,0xd0,0xcb,0xc3,0xbb,0xb6,0xae,0xa9,0xa4,0x9f,0x99,0x94,0x92,0x8f,0x8d,0x8a,0x87,0x87,0x87,0x85,0x85,0x85,0x87,0x82,0x7d,0x75,0x70,0x6b,0x66,0x60,0x5e,0x5b,0x58,0x59,0x56,0x56,0x56,0x59,0x58,0x58,0x5b,0x5e,0x5e,0x60,0x63,0x63,0x65,0x68,0x68,0x6b,0x6b,0x6d,0x6d,0x6d,0x6d,0x6d,0x6d,0x6d,0x6d,0x6d,0x6d,0x6b,0x6b,0x6b,0x68,0x68,0x65,0x63,0x63,0x60,0x60,0x5e,0x5b,0x5b,0x58,0x56,0x56,0x53,0x51,0x51,0x4e,
		0x4b,0x4c,0x49,0x49,0x46,0x44,0x44,0x41,0x41,0x3e,0x3f,0x3e,0x3c,0x3c,0x39,0x39,0x39,0x37,0x37,0x37,0x34,0x34,0x34,0x34,0x32,0x31,0x32,0x31,0x31,0x32,0x2f,0x2f,0x2f,0x2f,0x2f,0x2f,0x2f,0x2f,0x2f,0x2f,0x2f,0x2f,0x2f,0x2f,0x2f,0x2f,0x2f,0x2f,0x32,0x31,0x32,0x31,0x32,0x34,0x34,0x34,0x34,0x37,0x37,0x37,0x39,0x39,0x3c,0x3c,0x3c,0x3e,0x3e,0x41,0x41,0x44,0x44,0x46,0x46,0x49,0x49,0x4b,0x4e,0x4e,0x51,0x53,0x53,0x56,0x59,0x58,0x5b,0x5e,0x5e,0x60,0x63,
		0x66,0x65,0x68,0x6b,0x6d,0x70,0x70,0x72,0x75,0x78,0x7a,0x7d,0x7f,0x7f,0x7f};

const uint16_t sine_block3[601]={20059,20270,20479,20686,20891,21093,21297,21503,21711,21917,22124,22330,22534,22736,22937,23135,23333,23527,23721,23912,24102,24290,24476,24660,24841,25021,25200,25376,25550,25722,25892,26060,26227,26391,26553,26714,26873,27029,27183,27336,27487,27635,27782,27927,28069,28210,28350,28486,28621,28754,28885,29014,29142,29268,29392,29514,29634,29754,29871,29986,30100,30212,30323,30432,30540,30646,30751,30854,30958,31058,31158,31257,31354,31451,31546,31641,31735,31827,31919,32009,32100,32190,32278,32367,32454,32541,32628,32713,32798,32883,32968,33052,33136,33220,33302,33384,33467,33548,
		33630,33711,33792,33872,33951,34031,34110,34189,34267,34346,34423,34501,34577,34653,34728,34804,34879,34952,35026,35098,35170,35242,35313,35382,35452,35521,35588,35654,35720,35785,35849,35912,35973,36034,36093,36152,36208,36264,36319,36372,36424,36474,36525,36576,36629,36683,36740,36799,36863,36931,37004,37082,37166,37254,37348,37447,37551,37659,37772,37888,38007,38128,38250,38375,38497,38621,38741,38860,38975,39088,39193,39296,39391,39481,39565,39641,39711,39772,39828,39874,39913,39946,39970,39987,39996,39999,39996,39987,39970,39948,39919,39886,39847,39803,39755,39702,39644,39583,39517,39448,39374,
		39298,39216,39133,39044,38953,38858,38759,38657,38550,38441,38328,38210,38089,37964,37834,37702,37565,37424,37278,37129,36976,36818,36656,36490,36320,36146,35968,35785,35599,35409,35215,35018,34816,34611,34402,34192,33977,33759,33539,33316,33090,32862,32632,32399,32166,31930,31693,31453,31214,30973,30731,30489,30245,30003,29759,29515,29272,29029,28786,28544,28303,28061,27821,27582,27343,27106,26870,26635,26401,26168,25938,25708,25480,25253,25029,24805,24583,24362,24144,23926,23712,23498,23286,23075,22867,22660,22455,22252,22050,21849,21650,21454,21259,21065,20872,20683,20494,20306,20119,19928,19734,
		19541,19348,19155,18962,18772,18582,18394,18205,18019,17832,17647,17463,17281,17100,16919,16740,16560,16382,16206,16030,15854,15679,15506,15333,15160,14989,14817,14647,14476,14307,14136,13967,13799,13630,13463,13294,13126,12959,12791,12623,12456,12289,12121,11954,11787,11619,11452,11285,11117,10949,10782,10614,10447,10279,10111,9943,9775,9607,9438,9271,9103,8936,8768,8600,8433,8267,8099,7933,7769,7603,7438,7274,7111,6948,6786,6625,6465,6305,6147,5991,5834,5680,5526,5374,5223,5075,4927,4781,4638,4495,4356,4218,4081,3947,3815,3686,3558,3433,3311,3191,3074,2960,2849,2740,2634,2531,2431,2335,2241,2151,
		2064,1980,1899,1823,1749,1680,1613,1549,1490,1434,1382,1334,1290,1249,1211,1177,1148,1122,1099,1081,1066,1054,1046,1043,1043,1046,1053,1064,1078,1096,1118,1143,1171,1203,1238,1276,1318,1363,1411,1462,1515,1572,1631,1694,1759,1827,1898,1971,2047,2123,2204,2286,2370,2457,2545,2635,2728,2821,2916,3014,3112,3211,3313,3415,3519,3624,3730,3837,3945,4053,4163,4272,4384,4495,4609,4720,4833,4941,5048,5150,5248,5340,5426,5505,5576,5640,5695,5743,5782,5814,5839,5856,5867,5875,5875,5873,5868,5862,5854,5847,5840,5837,5836,5837,5845,5858,5875,5900,5931,5969,6014,6067,6126,6193,6268,6349,6437,6531,6633,6739,6852,
		6969,7091,7217,7349,7484,7621,7761,7904,8049,8197,8347,8499,8652,8806,8963,9120,9280,9441,9601,9766,9930,10098,10266,10436,10607,10781,10956,11133,11313,11495,11679,11865,12054,12244,12438,12633,12832,13032,13235,13440,13647,13856,14068,14281,14496,14714,14932,15153,15374,15597,15821,16046,16271,16498,16724,16951,17178,17404,17632,17858,18083,18307,18531,18754,18976,19196,19413,19631,19847
}; // next wave form, organ


const uint16_t sine_block[601]={20112,20707,21302,21880,22447,23026,23624,24233,24838,25438,26044,26667,27301,27930,28544,29145,29741,30339,30936,31519,32086,32638,33181,33713,34225,34711,35178,35630,36070,36486,36874,37241,37590,37920,38219,38488,38737,38972,39186,39369,39521,39651,39763,39851,39916,39962,39992,39999,39970,39903,39822,39755,39705,39647,39544,39393,39220,39063,38928,38790,38617,38406,38187,37989,37813,37629,37413,37172,36936,36721,36517,36300,36063,35824,35603,35397,35186,34955,34720,34496,34289,34080,33847,33600,33365,33156,32961,32753,32527,32302,32098,31909,31709,31490,31270,31070,30887,30699,30497
		,30300,30122,29954,29773,29574,29382,29216,29063,28898,28715,28531,28368,28219,28065,27902,27744,27601,27465,27320,27170,27025,26890,26757,26617,26474,26341,26220,26098,25964,25824,25693,25576,25465,25349,25223,25097,24976,24863,24753,24644,24532,24413,24289,24167,24056,23955,23855,23743,23618,23490,23372,23265,23163,23055,22938,22814,22693,22581,22474,22367,22252,22132,22013,21901,21791,21677,21556,21432,21313,21201,21086,20967,20844,20721,20605,20493,20380,20264,20140,20010,19882,19758,19645,19534,19421,19299,19169,19039,18914,18797,18681,18560,18428,18293,18167,18051,17943,17828,17706,17578,17455,17335,
		17214,17089,16965,16849,16736,16622,16501,16380,16263,16149,16028,15900,15773,15657,15549,15436,15312,15182,15058,14947,14841,14731,14610,14485,14364,14249,14137,14019,13893,13766,13644,13528,13411,13291,13170,13049,12932,12812,12691,12572,12455,12335,12205,12070,11942,11830,11724,11604,11467,11328,11205,11096,10983,10854,10717,10589,10474,10359,10236,10106,9978,9855,9733,9608,9483,9362,9243,9119,8989,8862,8745,8633,8514,8385,8251,8126,8016,7910,7795,7667,7537,7422,7322,7225,7118,6999,6880,6775,6683,6592,6497,6395,6293,6195,6104,6019,5936,5850,5758,5668,5588,5521,5459,5389,5314,5242,5183,5137,5096,5053,5010,
		4968,4930,4894,4864,4843,4832,4824,4816,4812,4814,4828,4847,4868,4895,4930,4974,5029,5092,5162,5241,5326,5418,5524,5643,5773,5908,6049,6205,6387,6592,6807,7026,7255,7508,7789,8091,8401,8715,9041,9389,9760,10148,10553,10977,11422,11880,12347,12826,13325,13845,14379,14919,15466,16028,16610,17203,17794,18380,18974,19586,20209,20826,21425,22010,22595,23187,23778,24357,24921,25471,26011,26540,27051,27541,28008,28456,28886,29299,29692,30063,30411,30735,31033,31307,31556,31782,31984,32161,32311,32438,32541,32628,32698,32750,32776,32773,32746,32704,32653,32589,32501,32391,32267,32136,31997,31841,31663,31468,31270,31068,
		30859,30638,30411,30178,29942,29701,29456,29211,28964,28707,28436,28160,27892,27640,27389,27126,26846,26565,26297,26048,25804,25555,25295,25031,24769,24517,24273,24032,23788,23538,23290,23059,22846,22638,22421,22191,21961,21747,21548,21353,21149,20941,20740,20549,20367,20182,19999,19824,19658,19500,19340,19177,19014,18849,18684,18518,18361,18220,18088,17951,17798,17637,17491,17368,17256,17133,16989,16841,16711,16602,16495,16370,16228,16089,15972,15870,15768,15651,15522,15399,15291,15195,15093,14976,14855,14744,14643,14539,14423,14300,14186,14086,13990,13885,13772,13661,13561,13464,13357,13243,13133,13036,12944,12845,
		12735,12620,12513,12416,12328,12241,12147,12045,11941,11846,11763,11686,11605,11517,11426,11341,11260,11183,11107,11033,10963,10897,10835,10780,10731,10680,10629,10581,10544,10516,10491,10466,10447,10441,10449,10462,10479,10499,10527,10562,10606,10659,10726,10807,10903,11010,11124,11247,11376,11519,11677,11849,12030,12226,12443,12683,12941,13204,13473,13762,14078,14414,14758,15107,15476,15872,16291,16720,17156,17606,18073,18556,19058,19590
}; // piano


const uint16_t sine_block5[601]={ 20037, 20533, 20996, 21339, 21610, 21874, 22165, 22508, 22874, 23264, 23679, 24069, 24386, 24592, 24703, 24777, 24859, 24964, 25095, 25244, 25399, 25522, 25586, 25716, 25912, 26134, 26391, 26655, 26929, 27216, 27493, 27768, 28034, 28300, 28546, 28779, 29006, 29208, 29394, 29569, 29718, 29851, 29968, 30062, 30138, 30196, 30235, 30253, 30254, 30236, 30203, 30153, 30082, 30000, 29902, 29788, 29657, 29517, 29365, 29195, 29018, 28831, 28626, 28417, 28198, 27963, 27726, 27484, 27232, 26964, 26698, 26427, 26139, 25857, 25568, 25266, 24969, 24669, 24354, 24047, 23738, 23427, 23100, 22787, 22428, 21713, 20446, 18728, 16764, 15066, 13888,
13278, 13118, 13168, 13074, 12444, 11381, 10228, 9415, 8866, 8480, 8523, 8792, 9173, 9577, 9937, 10257, 10545, 10808, 10918, 10636, 9776, 8463, 6937, 5592, 4855, 4684, 4936, 5409, 5977, 6563, 7095, 7462, 7702, 7891, 8079, 8313, 8620, 8976, 9386, 9849, 10328, 10825, 11337, 11820, 11928, 11394, 10244, 8871, 7658, 6959, 6897, 7295, 7998, 8804, 9645, 10473, 11300, 12070, 12816, 13574, 14292, 15002, 15731, 16422, 17106, 17802, 18457, 19097, 19717, 20340, 20905, 21372, 21706, 21918, 22083, 22258, 22456, 22691, 22965, 23254, 23554, 23857, 24167, 24455, 24730, 24995, 25227, 25434, 25616, 25745, 25814, 25905, 26046, 25968, 25376, 23939, 21463,
18659, 16339, 15211, 15117, 15746, 16696, 17735, 18726, 19647, 20428, 21121, 21768, 22342, 22880, 23404, 23880, 24326, 24757, 25131, 25463, 25739, 25932, 26055, 26226, 26285, 26155, 25756, 25088, 24018, 22543, 21085, 20124, 19778, 19931, 20407, 20995, 21600, 22187, 22700, 23161, 23589, 23957, 24281, 24558, 24723, 24893, 25170, 25489, 25830, 26183, 26563, 26934, 27299, 27613, 27783, 27836, 27824, 27807, 27816, 27871, 27970, 28099, 28250, 28421, 28590, 28758, 28928, 29082, 29226, 29364, 29470, 29372, 28745, 27473, 25904, 24347, 23136, 22569, 22519, 22815, 23256, 23727, 24177, 24551, 24865, 25128, 25363, 25560, 25737, 25900, 26044, 26171,
26289, 26386, 26468, 26538, 26528, 26137, 25091, 23386, 21446, 19645, 18347, 17738, 17607, 17769, 18053, 18369, 18679, 18946, 19183, 19398, 19603, 19793, 19976, 20161, 20338, 20509, 20681, 20840, 20991, 21134, 21270, 21375, 21388, 21258, 21029, 20763, 20500, 20291, 20130, 20014, 19939, 19890, 19860, 19842, 19829, 19822, 19816, 19811, 19804, 19793, 19779, 19762, 19740, 19665, 18989, 16827, 13071, 9465, 5777, 2019, 235, 0, 761, 1977, 3307, 4607, 5719, 6693, 7558, 8376, 9115, 9823, 10539, 11212, 11872, 12539, 13165, 13770, 14370, 14922, 15432, 15806, 15903, 15712, 15346, 14926, 14594, 14393, 14338, 14415, 14595, 14862, 15174, 15524, 15898,
16304, 16704, 17106, 17523, 17916, 18301, 18682, 19029, 19348, 19602, 19618, 19249, 18453, 17370, 16427, 15938, 16004, 16469, 17138, 17880, 18559, 19164, 19710, 20161, 20549, 20873, 21113, 21319, 21596, 21901, 22210, 22519, 22840, 23131, 23245, 22892, 22083, 21156, 20329, 19361, 18746, 18584, 18759, 19102, 19504, 19911, 20268, 20587, 20864, 21111, 21301, 21436, 21621, 21839, 22068, 22320, 22566, 22818, 23076, 23322, 23561, 23752, 23839, 23807, 23709, 23593, 23505, 23456, 23445, 23469, 23517, 23585, 23662, 23744, 23829, 23914, 23996, 24073, 24123, 23896, 23134, 21895, 20606, 19603, 19126, 18917, 18957, 19176, 19406, 19599, 19838, 20078, 20293, 20488, 20677, 20846, 21010, 21173, 21325, 21472, 21613, 21752, 21879, 21997, 22113, 22208, 22237, 22142, 21926, 21368, 20047, 18347, 15857, 12558, 9844, 8454, 8196, 8693, 9542, 10491, 11442, 12271, 13010, 13702, 14318, 14904, 15468, 16041, 16585, 17119, 17662, 18174, 18670, 19171, 19634, 20071, 20422, 20548, 20451, 20220, 19949, 19741, 19623, 19606, 19678, 19817, 20015, 20239, 20485, 20756, 21026, 21294, 21560, 21829, 22078, 22314, 22544, 22749, 22937, 23064, 22921, 22337, 21268, 19995, 18856, 18207, 18130, 18470, 19022, 19655, 20235, 20753, 21218, 21610, 21953, 22258, 22542, 22787, 23005, 23200, 23353, 23460, 23526, 23624, 23618, 23319, 22739, 21638, 20233, 18953, 18220, 17998, 18153, 18501, 18919, 19359, 19757, 20125, 20446, 20463, 19927, 18893, 17570, 16408, 15718, 15635, 16030, 16691, 17482, 18168, 18416, 17937, 16877, 15540, 14355, 13679, 13634, 14041, 14744, 15540, 16356, 17183, 17943, 18665, 19382,

}; // more organ




uint16_t sine_length=600; //holds sample size

uint16_t	sine_lut[]= { 33,33, 36, 38, 41, 43, 46, 48, 51, 54, 57, 61, 65, 68, 72, 77, 81, 86, 91, 97, 102, 108, 115, 122, 129, 137, 145, 153, 163, 172, 182, 193, 205, 217, 230, 244,
		258, 273, 290, 307, 325, 344, 365, 387, 410, 434, 460, 487, 516};  // sine + step *32   ,current
uint16_t sine_adder; // sine_lut return
  // sine lookup  ,  multiplier for midi >>14



uint8_t command[5]={254,191,254,128}; // lcd setcursor command
//uint8_t command[5]={168,128,168,128}; // lcd setcursor command

uint8_t command_toggle; // points to lcd command
uint16_t store_c=0;
uint16_t n = 0; // lcd counter
uint8_t rs_toggle = 1; // lcd rs toggle
uint8_t lcddata = 0; // lcd data var
 unsigned short row_toggle=0; // lcd row select
 int32_t sample_Accu[7] ={0,0,0,0,0,0,0};   // accu for sample output or lookup
uint32_t sample_AccuB=0;   // second tone accu
uint16_t sample_Note;  // hold sampleNoteadd value
uint8_t loop_counter; // basic loop schedule

uint16_t sample_pointB; //circular pointer

 uint16_t i;
//static unsigned short lcdit_toggle=1;
 unsigned short lcd_blink; // lcd blinker
uint8_t n_lcd; // lcd var
const char words[10] = "HELLOWORLD";
uint16_t adc_values[5]={20,20,20,20,20} ;  // adc values storage
uint8_t position; // lcd cursor
  uint16_t play_sample[1025]={} ;  //sample storage  , big better
volatile uint16_t sample_point; // pointer for sample array
volatile uint8_t lcd_isr;
 volatile  uint16_t play_hold;
 uint16_t n_calc=0;

 uint16_t sample_pointD;  // global store buffer start

 uint16_t sampling_counter; //  chase counter for sampling
 unsigned short bank_write=1;


uint16_t adsr_time[11];
uint16_t note_channel[31]={ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};  // 10 notes , 10 velocity 20->16bit , 10 mask toggle
 unsigned short mask_result;

static uint16_t adc_source[512] ;  // for soem reason static maybe important , also 16 bit most def
const uint8_t lfo_mask[20]={255,170,85,252,240,192,128,136,238,15,0,0,0,0,0,0,0}; // lfo dif masks
uint16_t sample_lp;  //low pass accu
uint8_t sample_lpbuf[512]; // sample average buff


uint8_t adsr_counter[11]; // hold count from 0-31 max 10+10+10 =ADR,  +1 on posedge then stop when above count value
//const uint16_t adsr_lut[41]= {0,1,2,3,4,5,6,8,9,10,12,14,17,20,24,28,32,37,42,47,53,59,65,72,79,86,96,104,114,128,144,178,228,342,409,455,512,585,682,819,1024}; //count up 1=10 , /9 for output
float adsr_lut[256];   // hold an envelope
uint8_t adsr_temp; // just a pointer for adsr




				  uint8_t adsr_step;
uint8_t adsr_up; //up counter for adsr
uint16_t adsr_isr; // holds adsr isrCount
uint8_t adsr_test=0;   // select if enable upcount
unsigned short adsr_retrigger[11]={0,0,0,0,0,0,0,0,0,0,0}; // adsr auto retrigger or wait for note start , if 0 zero big problem
uint8_t note_enabled=0;  // simple flag for note on seq_pos
uint16_t next_isr; //isrcount ahead
uint8_t adsr_toggle[11]={0,0,0,0,0,0,0,0};   // needs to be 0 or it never starts
uint16_t  note_vol[11]={0,0,0,0,0,0,0,0,0,0};  // note volumes and mask toggle
uint8_t note_holdA;
uint8_t note_holdB;

int32_t sample_accub;
int32_t filter_out[7]={0,0,0,0,0,0,0};	// big out
int32_t filter_in[5]={0,0,0,0,0};  // small values come in

int32_t sample_accus[6]={0,0,0,0,0,0};  // sample array
uint8_t filter_counter=0;
uint8_t flip_list=0;  // line flipping for display
							// 5,6 volume
//uint8_t pot_stored[64] = {114,66,1,50,103,0,160,0,106,106,138,132,122,138,0,0,20,34,20,96,0,0,0,0,22,80,17,2,65,50,0,0,0,0,0,48,49,48,0,0,48,128,9,0,1,0,151,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,16,};
uint8_t pot_stored[130] ={29,15,26,19,12,16,15,0,7,16,1,0,1,1,1,0,40,19,40,17,35,40,40,31,40,24,40,36,29,1,39,24,7,2,4,0,4,17,16,7,0,0,0,0,0,0,0,0,16,23,3,19,19,18,18,22,22,10,25,8,0,0,0,0,5,0,4,0,4,1,4,0,9,0,10,29,0,0,0,0,0,16,0,16,0,0,16,0,0,16,0,0,12,16,0,0,10,23,36,27,1,7,0,4,0,39,28,30,20,14,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0};
// seems to send odd data with no audio output for lcd ????????
uint8_t one_shot; // up counter for attack sample
uint8_t cust_char[64];  // holder for custom characters 8x5bit x8 ,Send command: 0x40 (Set pointer to the first CGRAM address).

uint16_t enc_temp;  // hold in for tim2
uint16_t enc_tempB;  // hold old in
uint16_t enc2_temp;  // hold in for tim4
uint16_t enc2_tempB;  // hold old in2
uint16_t enc2_dir;    // enc direction2
int16_t enc_dir;    // enc direction
uint16_t tempo_count=0; // up counter for tempo reset
 uint8_t bsrr_hold[128]; //store to hold bsrr data ready for transfer in a loop
uint8_t bs_count=0; //count to 40 on bsrrr

uint8_t menu_store=1;
uint16_t cursor_pos[2];
unsigned short lcd_send=0;
//spi pins
unsigned short cs_pin=1;
uint8_t clk_pin=0;
unsigned short mosi_pin=0;
unsigned short spi_enable=0;
unsigned short spi_state=0; //select data or command 0= command
uint8_t bsr_out;
uint8_t spi_counter=0; // tracks spi progress
uint8_t spi_byte; //incoming data
uint16_t spi_hold; // temp hold for incoming
uint8_t spi_send=0; // keeps track of send process
uint8_t init=0; //init counter
uint8_t samp_temp=0;
uint32_t bsrr_long;
uint32_t bsrr_longB;
uint8_t seq_spi;
uint16_t init_b;  //  needs to above 256 for ore pages
uint32_t bsrr_seq[49]; // holds bits for one byte spi
uint8_t bsrr_counter;
uint16_t cursor_menu[3]; // holds cursor position
uint8_t spi_toggle; // toggle between command a data for writing characters
uint8_t cursor_blink=0; // blink position  blink 1 or 2 or off
uint16_t lfo_base[3]; // new lfo multi
uint32_t lfo_send[10];
uint8_t mem_count; //just count mem address
uint32_t loop_counter2; //long counter
uint8_t buffer[4];
uint8_t mem_buf;
uint16_t menu_page[3]; // switch between pages, keep track when flipping
uint8_t seq_loop[7]; //loop positions

//new stuff///////////////////////////////////////////////////////////////////////////////////////////////////////
float filter_accus[15];  // hold floats for filter
float filter_hold[5];  //holds some feedback related stuff
float freq_point[4] ; // multiplier coeff holder temp
float freq_pointer[4] [9];  // multiplier coeff holder
uint8_t i_frac;  // divide i/64
uint8_t seq_store;  // just an seq_pos holder for adsr

uint16_t lfo_value[5]={0,0,0,0}; //  lfo value hold
int16_t lfo_output[5]; // lfo out 0-2047 normally
uint16_t trigger_counter;

float  lfo_accu[10]  [10]; //holds last lfo value , simple 0-255 upcount for now,will change; 10x8
uint16_t  lfo_out[10] [10];   //8x10 values for lfo
uint16_t tempo_lut[162]; // tempo look up 40-200bpm
////////////////////////////////////////////////////////////////////////////////////////////////////////////GFX
uint8_t gfx_ram[64][16] ; //holds data for lcd 64*256bit , display 16kbyte/sec so 8fps , maybe use dma for memory transfers
uint8_t gfx_char[1024]={   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  0x00,     // 8*5 fonts vert aligned  , maybe play with other fonts too
	     0x0E, 0x11, 0x13, 0x15, 0x19, 0x11, 0x0E,  0x00,
	     0x04, 0x0C, 0x04, 0x04, 0x04, 0x04, 0x0E,  0x00,
	     0x0E, 0x11, 0x01, 0x02, 0x04, 0x08, 0x1F,  0x00,
	     0x1F, 0x02, 0x04, 0x02, 0x01, 0x11, 0x0E,  0x00,
	     0x02, 0x06, 0x0A, 0x12, 0x1F, 0x02, 0x02,  0x00,
	     0x1F, 0x10, 0x1E, 0x01, 0x01, 0x11, 0x0E,  0x00,
	     0x06, 0x08, 0x10, 0x1E, 0x11, 0x11, 0x0E,  0x00,
	     0x1F, 0x11, 0x01, 0x02, 0x04, 0x04, 0x04,  0x00,
	     0x0E, 0x11, 0x11, 0x0E, 0x11, 0x11, 0x0E,  0x00,
	     0x0E, 0x11, 0x11, 0x0F, 0x01, 0x02, 0x0C,  0x00,
	     0x00, 0x0C, 0x0C, 0x00, 0x0C, 0x0C, 0x00,  0x00,
	     0x00, 0x0C, 0x0C, 0x00, 0x0C, 0x04, 0x08,  0x00,
	     0x02, 0x04, 0x08, 0x10, 0x08, 0x04, 0x02,  0x00,
	     0x00, 0x00, 0x1F, 0x00, 0x1F, 0x00, 0x00,  0x00,
	     0x08, 0x04, 0x02, 0x01, 0x02, 0x04, 0x08,  0x00,
	     0x0E, 0x11, 0x01, 0x02, 0x04, 0x00, 0x04,  0x00,
	     0x0E, 0x11, 0x01, 0x0D, 0x15, 0x15, 0x0E,  0x00,
	     0x0E, 0x11, 0x11, 0x11, 0x1F, 0x11, 0x11,  0x00,
	     0x1E, 0x11, 0x11, 0x1E, 0x11, 0x11, 0x1E,  0x00,
	     0x0E, 0x11, 0x10, 0x10, 0x10, 0x11, 0x0E,  0x00,
	     0x1C, 0x12, 0x11, 0x11, 0x11, 0x12, 0x1C,  0x00,
	     0x1F, 0x10, 0x10, 0x1E, 0x10, 0x10, 0x1F,  0x00,
	     0x1F, 0x10, 0x10, 0x1E, 0x10, 0x10, 0x10,  0x00,
	     0x0E, 0x11, 0x10, 0x17, 0x11, 0x11, 0x0F,  0x00,
	     0x11, 0x11, 0x11, 0x1F, 0x11, 0x11, 0x11,  0x00,
	     0x0E, 0x04, 0x04, 0x04, 0x04, 0x04, 0x0E,  0x00,
	     0x07, 0x02, 0x02, 0x02, 0x02, 0x12, 0x0C,  0x00,
	     0x11, 0x12, 0x14, 0x18, 0x14, 0x12, 0x11,  0x00,
	     0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x1F,  0x00,
	     0x11, 0x1B, 0x15, 0x15, 0x11, 0x11, 0x11,  0x00,
	     0x11, 0x11, 0x19, 0x15, 0x13, 0x11, 0x11,  0x00,
	     0x0E, 0x11, 0x11, 0x11, 0x11, 0x11, 0x0E,  0x00,
	     0x1E, 0x11, 0x11, 0x1E, 0x10, 0x10, 0x10,  0x00,
	     0x0E, 0x11, 0x11, 0x11, 0x15, 0x12, 0x0D,  0x00,
	     0x1E, 0x11, 0x11, 0x1E, 0x14, 0x12, 0x11,  0x00,
	     0x0F, 0x10, 0x10, 0x0E, 0x01, 0x01, 0x1E,  0x00,
	     0x1F, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04,  0x00,
	     0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x0E,  0x00,
	     0x11, 0x11, 0x11, 0x11, 0x11, 0x0A, 0x04,  0x00,
	     0x11, 0x11, 0x11, 0x15, 0x15, 0x15, 0x0A,  0x00,
	     0x11, 0x11, 0x0A, 0x04, 0x0A, 0x11, 0x11,  0x00,
	     0x11, 0x11, 0x11, 0x0A, 0x04, 0x04, 0x04,  0x00,
	     0x1F, 0x01, 0x02, 0x04, 0x08, 0x10, 0x1F,  0x00,
	     0x1C, 0x10, 0x10, 0x10, 0x10, 0x10, 0x1C,  0x00,
	     0x11, 0x0A, 0x1F, 0x04, 0x1F, 0x04, 0x04,  0x00,
	     0x0E, 0x02, 0x02, 0x02, 0x02, 0x02, 0x0E,  0x00,
	     0x04, 0x0A, 0x11, 0x00, 0x00, 0x00, 0x00,  0x00,
	     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1F,  0x00,
	     0x08, 0x04, 0x02, 0x00, 0x00, 0x00, 0x00,  0x00,
	     0x00, 0x00, 0x0E, 0x01, 0x0F, 0x11, 0x0F,  0x00,
	     0x10, 0x10, 0x16, 0x19, 0x11, 0x11, 0x1E,  0x00,
	     0x00, 0x00, 0x0E, 0x10, 0x10, 0x11, 0x0E,  0x00,
	     0x01, 0x01, 0x0D, 0x13, 0x11, 0x11, 0x0F,  0x00,
	     0x00, 0x00, 0x0E, 0x11, 0x1F, 0x10, 0x0E,  0x00,
	     0x06, 0x09, 0x08, 0x1C, 0x08, 0x08, 0x08,  0x00,
	     0x00, 0x0F, 0x11, 0x11, 0x0F, 0x01, 0x0E,  0x00,
	     0x10, 0x10, 0x16, 0x19, 0x11, 0x11, 0x11,  0x00,
	     0x04, 0x00, 0x0C, 0x04, 0x04, 0x04, 0x0E,  0x00,
	     0x02, 0x00, 0x06, 0x02, 0x02, 0x12, 0x0C,  0x00,
	     0x10, 0x10, 0x12, 0x14, 0x18, 0x14, 0x12,  0x00,
	     0x0C, 0x04, 0x04, 0x04, 0x04, 0x04, 0x0E,  0x00,
	     0x00, 0x00, 0x1A, 0x15, 0x15, 0x11, 0x11,  0x00,
	     0x00, 0x00, 0x16, 0x19, 0x11, 0x11, 0x11,  0x00,
	     0x00, 0x00, 0x0E, 0x11, 0x11, 0x11, 0x0E,  0x00,
	     0x00, 0x00, 0x1E, 0x11, 0x1E, 0x10, 0x10,  0x00,
	     0x00, 0x00, 0x0D, 0x13, 0x0F, 0x01, 0x01,  0x00,
	     0x00, 0x00, 0x16, 0x19, 0x10, 0x10, 0x10,  0x00,
	     0x00, 0x00, 0x0E, 0x10, 0x0E, 0x01, 0x1E,  0x00,
	     0x08, 0x08, 0x1C, 0x08, 0x08, 0x09, 0x06,  0x00,
	     0x00, 0x00, 0x11, 0x11, 0x11, 0x13, 0x0D,  0x00,
	     0x00, 0x00, 0x11, 0x11, 0x11, 0x0A, 0x04,  0x00,
	     0x00, 0x00, 0x11, 0x15, 0x15, 0x15, 0x0A,  0x00,
	     0x00, 0x00, 0x11, 0x0A, 0x04, 0x0A, 0x11,  0x00,
	     0x00, 0x00, 0x11, 0x11, 0x0F, 0x01, 0x0E,  0x00,
	     0x00, 0x00, 0x1F, 0x02, 0x04, 0x08, 0x1F,  0x00,
	     0x02, 0x04, 0x04, 0x08, 0x04, 0x04, 0x02,  0x00,
	     0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04,  0x00,
	     0x08, 0x04, 0x04, 0x02, 0x04, 0x04, 0x08,  0x00,
	     0x00, 0x04, 0x02, 0x1F, 0x02, 0x04, 0x00,  0x00,
	     0x00, 0x04, 0x08, 0x1F, 0x08, 0x04, 0x00,  0x00
	      }; //various alphabet and other characters,for now 5*8 , 26*8 bytes+others , will update

//uint8_t gfx_char[2048]={};
uint8_t gfx_char1[512] =  {
		0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x10,0x10,0x10,0x10,0x10,0x20,0x10,0x20,0x6c,0x24,0x48,0x20,0x20,0x20,0x20,0x20,0x20,0x24,0x7e,0x24,0x48,0xc3,0xbc,0x48,0x20,0x08,0x3e,0x48,0x3c,0x12,0x7c,0x10,0x20,0x60,0xc2,0x94,0x68,0x10,0x2c,0x52,0x0c,0x20,0x30,0x48,0x50,0x66,0xc2,0x94,0xc2,0x88,0x76,0x20,0x18,0x08,0x10,0x20,0x20,0x20,0x20,0x20,0x10,0x20,0x20,0x40,0x40,0x20,0x20,0x10,0x40,0x20,0x20,0x10,0x10,0x20,0x20,0x40,0x20,0x10,0x7c,0x38,0x28,0x20,0x20,0x20,0x20,0x10,0x10,0x7c,0x10,0x10,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x30,0x10,0x20,0x20,0x20,0x20,0x7c,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x10,0x10,0x20,0x20,0x02,0x04,0x08,0x10,0x20,0x40,0x20,0x38,0x44,0xc2,0x8a,0xc2,0x92,0xc2,0xa2,0x44,0x38,0x20,0x10,0x30,0x10,0x10,0x10,0x10,0x38,0x20,0x7c,0xc2,0x82,0x02,0x0c,0x30,0x42,0xc3,0xbe,0x20,0xc3,0xbe,0xc2,0x84,0x08,0x1c,0x02,0xc2,0x82,0x7c,0x20,0x18,0x28,0x48,0xc2,0x88,0xc3,0xbe,0x08,0x1c,0x20,0xc3,0xbe,0xc2,0x82,0xc2,0x80,0xc3,0xbc,0x02,0xc2,0x82,0x7c,0x20,0x18,0x20,0x40,0xc3,0xbc,0xc2,0x82,0xc2,0x82,0x7c,0x20,0xc3,0xbe,0xc2,0x82,0x04,0x08,0x10,0x10,0x10,0x20,0x7c,0xc2,0x82,0xc2,0x82,0x7c,0xc2,0x82,0xc2,0x82,0x7c,0x20,0x7c,0xc2,0x82,0xc2,0x82,0x7e,0x04,0x08,0x30,0x20,0x20,0x10,0x10,0x20,0x20,0x10,0x10,0x20,0x20,0x08,0x08,0x20,0x20,0x18,0x08,0x10,0x04,0x08,0x10,0x20,0x10,0x08,0x04,0x20,0x20,0x20,0x7c,0x20,0x7c,0x20,0x20,0x20,0x20,0x10,0x08,0x04,0x08,0x10,0x20,0x20,0x7c,0xc2,0x82,0x02,0x1c,0x20,0x10,0x10,0x20,0x1c,0x22,0x4e,0x52,0x4e,0x20,0x1c,0x20,0x18,0x18,0x24,0x24,0x7e,0x42,0xe7,0xa0,0xbc,0x42,0x42,0x7c,0x42,0x42,0xc3,0xbc,0x20,0x3c,0x42,0xc2,0x80,0xc2,0x80,0xc2,0x80,0x42,0x3c,0x20,0xc3,0xb8,0x44,0x42,0x42,0x42,0x44,0xc3,0xb8,0x20,0xc3,0xbe,0x42,0x48,0x78,0x48,0x42,0xc3,0xbe,0x20,0xc3,0xbe,0x42,0x48,0x78,0x48,0x40

}; //font pack 1
float  adsr_countup[11];  //holds isr count on notes ,
	float adsr_level[11]  ; //float for vol envelope  ,ps 20 21
	float as_attack=0; // for now all of them from this only , speed , 0-16  // rarely read
	float as_sustain=0;  // length and level this is ok is running 1/16 ish
	float adsr_att;
uint16_t gfx_counter[6]={0,0,0,0,0}; // just upcounter for gfx ram bytes
uint8_t gfx_skip=1;  // important
uint8_t gfx_blink=0; // blinker counter
uint16_t lcd_out3; //for feedback
uint8_t disp_multi[33]={8,1,8,1,1,1,1,1,8,1,8,1,1,1,1,1,8,1,8,1,1,1,1,1,8,1,8,1,1,1,1,1,1,1,1,1};   // display row potSource multiplier for different resolutions , for faster input
uint8_t disp_stepper=1; // display these chars before moving onto next cursor position
uint8_t disp_end; // end of spi sequence per whole page
uint8_t loop_counter3;
uint16_t enc2_tempc; //enc2 holder duh
uint8_t disp_refresh;  // full screen refresh
uint16_t input_holder[512];    // hold adc incoming data, vref sits around 1020 with a cap
uint16_t input_count; // adc input couinter

//  USE THE BREAK WITH SWITCH STATEMENT MORON!!!


/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
