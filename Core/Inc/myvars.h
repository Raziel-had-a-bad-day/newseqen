

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
//uint16_t pwmVel;
//uint16_t pwmVelB;
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





uint16_t enc2_lut[180];   // lut to skip cells ,filled in on main
uint16_t feedback_line[17]={'F','o','o','d','b','a','c','k','g','_' ,'k',64, 408, 409, 410,64}; // this is variable , last display line for info needs fix
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

uint16_t menu_locA=0;
char menulist_hold[20]=" not working at all" ;
const char menuList[129] = "_Notes_1_NotePtchNotes_2_AttkDcayLFOspeedLFOdepthLFO_gainkeepgoinNext page_LFOSSWITCHESREPEAT__KEY#____Time____NTE2_____________";   // top menu
const char menuList2 [129] = "_LPF_1___LPF_2___PITCH_1_PITCH_2_PITCH_3_EMPTY___NotePtchNotes_2_AttkDcayLFOspeedLFOdepthLFO_gainkeepgoinNext page_LFOSWI_______";   // lfo target list, etc
const char mainNote[29] = "_CDEFGABCDEF>_1234567890+>_x" ; //note string values 7 notes and flat and sharp which are double length ? 11=1  20=0 21-
uint8_t menulength;
uint8_t noteTiming;  // set timing shift
uint8_t potValues [383];  //low res values mostly for display

uint8_t potSource[383]; // high res version of potValues used when needed 40-0, gonna change to 160 just o break things, need more res for lfo

// const char mainNoteLow[] = "cdefgab_^: " ; // note string for off values
//const uint8_t noteSwitch[] = { 255,0,204,51,170,85,240,15,17,252,0 };  // note switching using bitread , alternating pattern on, off, 11001100, 00110011,10101010,01010101,11110000,00001111,00010001,11101110
//const uint16_t lfoGainValues[8] = {}; // gain adder values
//const uint8_t  waveSaw[17] = {  10,31,47,63,79,95,111,127,0,31,47,63,79,95,111,127}; //lfo saw values lut ,not used

//const uint8_t waveTri[17] = { 0,15,31,47,63,79,95,111,127,111,95,79,63,47,32,15 }; //lfo triangle values lut ,not used


// lfos speed=150-160 ,lfo_depth=160-170,lfo_gain=170-180;lfo_target=180-190;  80-95=notes,

//uint8_t notePos2;
//static unsigned short lfoNext;


//static unsigned short noteTimeFlag;
//uint8_t noteTimebit[]= {0,170,85,240,15,204,51,153,102,165,90,0}   ;   // bit switch for adjustimg rythm
//uint16_t noteAccu; //adder for left over timing
//uint16_t tempAccuA;  // accu for pots
//volatile uint8_t promValue; //eprom
//volatile uint8_t promValues[64]; // hold original potvalues here
//uint8_t noteLength=16; // lenght of note 16 ticks default
//uint8_t noteDelay; // initial delay for trigger

//uint8_t potPosition=0; // potValues pointer
//uint8_t modSwitches [9] ={0,0,0,0,0,0,0,0 }; // function switch storage ,all values are 0-2
// uint8_t ccValues[6]={}; //cc memories storage
//uint8_t currentValues [10] = {  };  //note switches on,stop,off  storage 15=on 0=off 1=loop
//uint8_t currentValuesB [10] = {  };  // note switch  storage for lcd
//uint8_t pitchValues[9] = {  }; // remember pitch values of key pot
//uint8_t noteDuration[9] = {  }; // note length array
//uint8_t displayMem[9] = { }; // past stored values for lcd
//uint8_t displayMemVelocity[9] = {};
//uint8_t lcdBufferString; // string storage
//uint8_t lcdBufferCounter;  // counter in lcd buffer
//uint8_t lcdBufferCounterB;  // counter in lcd buffer
//char lcdBufferMem[33] = {};  // string memory for lcdbuffering
//char lcdBufferMemNew[33] = {};  // string memory for lcdbuffering incoming
//uint8_t memNote[10] = { 0,0,0,0,0,0,0,0,0 };  // current note values for lcd
//uint8_t tempValue[127]={ }; // last value is menu control
//uint8_t tempValueB[127]={ };
//uint16_t tempValueA;
//uint8_t potRange=54; // range of potvalues 8/14
// uint8_t modMatrix[65]={};  // storage for all modulation options, new
//const char modTop[] = "Nt_WvRtWdTyDp"; // modulation top lcd row : Note(1-8),LFO wave type(0-3) 0 is off , Rate(1-8), Pulse width(1-8) , velocity pitch or time modulation(1-3) , depth goes +- for inversion (+5 -5)
//unsigned short modEnable=0;   // true when menu pot above 0

//uint8_t menuSelectX2; // old menu
// unsigned short firstRound=1;
//static unsigned short modMenu=0;
//uint8_t modRate; // lfo rate
//uint8_t modPhase; // lfo phase
//uint8_t modDelay; // lfo initialdelay
//uint8_t modPosition; // like notePosition but freewheel
//uint8_t modPositionB; // like modPosition but shifted
//uint16_t modGain;// moddepth setting ,not sure yet can be -
//uint16_t modPitch;// pitch modifier
//uint16_t modBarPitch; // changes bar pitch , 8 bars
//uint8_t modPointer; // mod pointer
//uint8_t modOut; // mod output
//static unsigned short modStep;// mod up count
//static unsigned short centreLed;  // locate positions with led
//uint8_t oldNote; //store previous note
//uint8_t oldNote2;
//uint8_t eventCounter=0; // event timer
//static unsigned short writeCycle=1; // switch write and read
//uint8_t notePosition; // seq position for noteplayback 8x8 for now 0-63
//uint8_t lfoWave[4]={};
//uint8_t lfolastccA[4]={};
//uint8_t lfoPhase;
//static unsigned short displayFirst=1; // display loop firs round
//uint16_t isrDivide; // counter for isrcount since notimer for now
//uint8_t  Note=0;
//uint8_t NoteB;
//static unsigned short loopMode=0;
//uint8_t octKey = 0;
//static unsigned short NoteSlide=0;
//uint8_t NoteBuffer=0;
//uint8_t patternType=0;
//uint8_t keyPot=0; // make it real time only from now
//uint8_t keyPotB=5;
//uint8_t keyPotC=0; // storage for analogeread
 //uint8_t counterVar=0;

//uint8_t switchOne=0;
//static unsigned short outNote;  // detect note process
//uint8_t patternEditPos=1; // bar position for pattern edit
//uint16_t valueOriginal; // stored value  needs signed
//uint16_t valueCurrent; //current setting
//uint8_t valueReturn; // final value out
//uint8_t valueChange[9]={1,1,1,1,1,1,1,1}; // enable writing
//uint8_t barCount=0;  // counts +1 per 8 bars ie
//uint8_t barCountB=1;// dec 1-8
//uint8_t barCountC=1;
//uint16_t barCountTotal=0;  // total note position in 32 bars 95 * 8 , 16bit
//uint16_t lastbarCountB=0; // ( dec signed barcountB-2)  0-7 pointer
//uint8_t nextbarCountB=0; // next barcounter
//static unsigned short firstbarWrite=1;  // write during first 8 bars

//uint8_t firstbarLoopB;// bit shifted
//volatile int  midiTimer =0; // timing from midiclock to notes

//volatile uint16_t isrTemp;
//uint8_t isrCountC;

//uint16_t isrPoint;
//uint16_t isrPointB;  // for noteBar jump by 4
//uint16_t isrCount; // ISR counter tcik 32 bit
//volatile uint16_t isrCountB; //  512-820 per tick 16bit

//uint8_t lcdCounter=0;
//uint8_t electPitch=64; // Electribe pitch
// uint8_t tempo=20; //delay timer
//uint8_t tempoTemp;
//uint32_t timerVar ;  // multipler for timer1 lenght for freq , too big needs to be 16bit
//uint32_t timerVar2; // timer 2
//uint8_t noteMem=0;
//uint8_t midiMemory[70]={}; //midi storage for incoming
//uint8_t midSettings[]={1,1,0,0}; // midi channel and lfo cc setting
//uint8_t midiBarcount=0; // note off var rollover on 8
//volatile int  t =0;
//uint32_t timExe=0;
//uint8_t timeCount=0; //time execute schedule once per note
//uint8_t noteTraffic[4]={1,48,0,0}; // remember if note was sent from midi or sequencer : switchone , outNote, midi stored value ,0

uint16_t sine_counter;  // up counter for sine reading
uint16_t sine_counterB;  // up counter for sine reading ,fractional * 8
int32_t sine_out;     // generated sine output 9 bit
uint16_t sine_temp2;
uint8_t menuSelect;     // menu selector pot
uint8_t menuSelectB;  // old menu data
uint8_t menuSelectX; // menu horizontal
volatile uint16_t isrMask;  // tempo mask for tick  16 bit
uint8_t firstbarLoop;
uint16_t counterVarB;

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
static void MX_SPI1_Init(void);
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
void menu_parser(void);
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



uint16_t sine_length=600; //holds sample size

uint16_t	sine_lut[]= { 33,33, 36, 38, 41, 43, 46, 48, 51, 54, 57, 61, 65, 68, 72, 77, 81, 86, 91, 97, 102, 108, 115, 122, 129, 137, 145, 153, 163, 172, 182, 193, 205, 217, 230, 244,
		258, 273, 290, 307, 325, 344, 365, 387, 410, 434, 460, 487, 516};  // sine + step *32   ,current
uint16_t sine_adder; // sine_lut return
  // sine lookup  ,  multiplier for midi >>14

uint16_t store_c=0;
uint16_t n = 0; // lcd counter

 int32_t sample_Accu[7] ={0,0,0,0,0,0,0};   // accu for sample output or lookup
uint32_t sample_AccuB=0;   // second tone accu

uint8_t loop_counter; // basic loop schedule

uint16_t sample_pointB; //circular pointer

 uint16_t i;
uint16_t adc_values[5]={20,20,20,20,20} ;  // adc values storage

  uint16_t play_sample[1025]={} ;  //sample storage  , big better
volatile uint16_t sample_point; // pointer for sample array

 volatile  uint16_t play_hold;
 uint16_t sample_pointD;  // global store buffer start

 unsigned short bank_write=1;

uint16_t note_channel[31]={ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};  // 10 notes , 10 velocity 20->16bit , 10 mask toggle
 unsigned short mask_result;

static uint16_t adc_source[1025] ;  // for soem reason static maybe important , also 16 bit most def
const uint8_t lfo_mask[20]={255,170,85,252,240,192,128,136,238,15,0,0,0,0,0,0,0}; // lfo dif masks
//const uint16_t adsr_lut[41]= {0,1,2,3,4,5,6,8,9,10,12,14,17,20,24,28,32,37,42,47,53,59,65,72,79,86,96,104,114,128,144,178,228,342,409,455,512,585,682,819,1024}; //count up 1=10 , /9 for output
float adsr_lut[256];   // hold an envelope

unsigned short adsr_retrigger[11]={0,0,0,0,0,0,0,0,0,0,0}; // adsr auto retrigger or wait for note start , if 0 zero big problem
uint8_t note_enabled=0;  // simple flag for note on seq_pos
uint16_t next_isr; //isrcount ahead
uint8_t adsr_toggle[11]={0,0,0,0,0,0,0,0};   // needs to be 0 or it never starts

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


uint8_t spi_byte; //incoming data
uint16_t spi_hold; // temp hold for incoming
uint8_t spi_send=0; // keeps track of send process
uint8_t init=0; //init counter
uint16_t init_b;  //  needs to above 256 for ore pages
uint16_t cursor_menu[3]; // holds cursor position
uint8_t mem_count; //just count mem address
uint32_t loop_counter2; //long counter
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
uint16_t trigger_counter;
float  lfo_accu[10]  [10]; //holds last lfo value , simple 0-255 upcount for now,will change; 10x8
uint16_t  lfo_out[10] [10];   //8x10 values for lfo
uint16_t tempo_lut[162]; // tempo look up 40-200bpm
////////////////////////////////////////////////////////////////////////////////////////////////////////////GFX
uint8_t gfx_ram[64][16] ; //holds data for lcd 64*256bit , display 16kbyte/sec so 8fps , maybe use dma for memory transfers


//uint8_t gfx_char[2048]={};
uint8_t gfx_char1[512] =  {
		0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x10,0x10,0x10,0x10,0x10,0x20,0x10,0x20,0x6c,0x24,0x48,0x20,0x20,0x20,0x20,0x20,0x20,0x24,0x7e,0x24,0x48,0xc3,0xbc,0x48,0x20,0x08,0x3e,0x48,0x3c,0x12,0x7c,0x10,0x20,0x60,0xc2,0x94,0x68,0x10,0x2c,0x52,0x0c,0x20,0x30,0x48,0x50,0x66,0xc2,0x94,0xc2,0x88,0x76,0x20,0x18,0x08,0x10,0x20,0x20,0x20,0x20,0x20,0x10,0x20,0x20,0x40,0x40,0x20,0x20,0x10,0x40,0x20,0x20,0x10,0x10,0x20,0x20,0x40,0x20,0x10,0x7c,0x38,0x28,0x20,0x20,0x20,0x20,0x10,0x10,0x7c,0x10,0x10,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x30,0x10,0x20,0x20,0x20,0x20,0x7c,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x10,0x10,0x20,0x20,0x02,0x04,0x08,0x10,0x20,0x40,0x20,0x38,0x44,0xc2,0x8a,0xc2,0x92,0xc2,0xa2,0x44,0x38,0x20,0x10,0x30,0x10,0x10,0x10,0x10,0x38,0x20,0x7c,0xc2,0x82,0x02,0x0c,0x30,0x42,0xc3,0xbe,0x20,0xc3,0xbe,0xc2,0x84,0x08,0x1c,0x02,0xc2,0x82,0x7c,0x20,0x18,0x28,0x48,0xc2,0x88,0xc3,0xbe,0x08,0x1c,0x20,0xc3,0xbe,0xc2,0x82,0xc2,0x80,0xc3,0xbc,0x02,0xc2,0x82,0x7c,0x20,0x18,0x20,0x40,0xc3,0xbc,0xc2,0x82,0xc2,0x82,0x7c,0x20,0xc3,0xbe,0xc2,0x82,0x04,0x08,0x10,0x10,0x10,0x20,0x7c,0xc2,0x82,0xc2,0x82,0x7c,0xc2,0x82,0xc2,0x82,0x7c,0x20,0x7c,0xc2,0x82,0xc2,0x82,0x7e,0x04,0x08,0x30,0x20,0x20,0x10,0x10,0x20,0x20,0x10,0x10,0x20,0x20,0x08,0x08,0x20,0x20,0x18,0x08,0x10,0x04,0x08,0x10,0x20,0x10,0x08,0x04,0x20,0x20,0x20,0x7c,0x20,0x7c,0x20,0x20,0x20,0x20,0x10,0x08,0x04,0x08,0x10,0x20,0x20,0x7c,0xc2,0x82,0x02,0x1c,0x20,0x10,0x10,0x20,0x1c,0x22,0x4e,0x52,0x4e,0x20,0x1c,0x20,0x18,0x18,0x24,0x24,0x7e,0x42,0xe7,0xa0,0xbc,0x42,0x42,0x7c,0x42,0x42,0xc3,0xbc,0x20,0x3c,0x42,0xc2,0x80,0xc2,0x80,0xc2,0x80,0x42,0x3c,0x20,0xc3,0xb8,0x44,0x42,0x42,0x42,0x44,0xc3,0xb8,0x20,0xc3,0xbe,0x42,0x48,0x78,0x48,0x42,0xc3,0xbe,0x20,0xc3,0xbe,0x42,0x48,0x78,0x48,0x40

}; //font pack 1
float adsr_level[11]  ; //float for vol envelope  ,ps 20 21
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

uint16_t input_holder[512];    // hold adc incoming data, vref sits around 1020 with a cap

uint8_t adc_flag=0;  // sets flag for to run sampling()
uint8_t return_spi1[16]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
uint8_t status_reg[2]={0,1};
 uint16_t  time_final[2]={0,0};  // tick timer
 uint16_t time_proc=0; // tick start
uint16_t  error_count=0;

struct LFO_settings{      // use first 5*10 , leave the rest

	uint8_t gain;
	uint8_t depth;	//(p140)
	uint8_t rate; // (p130)
	uint8_t offset;
	uint8_t target;      // lfo modulation target  , number for now
	uint16_t out[10];       // actual output , needs multiple for loop
};

struct LFO_settings LFO[10];       // create lfo settings
struct ADSR_settings{   // use initial 5*10  , leave rest
	uint8_t attack;   // presets
	uint8_t decay;
	uint8_t sustain;
	uint8_t release;
	uint8_t attack_trigger;    // start adsr
	float attack_data;        // processing data
	float decay_data;
	float sustain_data;
	float release_data;
    float buffer_temp;  // temp hold
	uint16_t buffer[256];  //this holds the actual envelope  for ADSR for later processing
};
struct ADSR_settings ADSR[5];   // adsr data
struct note_settings{								//default note/osc/patch settings  14*8 bytes (112), use all
	uint8_t osc;      // waveforms or patches setting
	uint8_t osc2;		// second wave for decays or stacking
	uint16_t osc_add;   // this is the add value hold for sine/wav/saw etc depends on wave form
	uint8_t pitch;
	uint8_t duration;
	uint8_t position;   // note position in sequence loop
	uint8_t transpose;   // pshift note pitch up or down (p 72,73)
	uint8_t timeshift; // shift position left or right in seuqence for 8 note looper   (pvalues 32,33)
	uint16_t velocity;				// gain level or output mod
	uint8_t detune; 					// finetune maybe for lfos or some default tune
	uint16_t tuned;   // final output after note detune and osc_add calculation , drives the oscillators
};
struct note_settings note[7];         // for now 0-4 saw and 5 is sine , add more later (112 byte)
struct seq_settings {				// 46 bytes need all
	uint8_t pos;    // actual position of the sequencer atm
	uint8_t notes1[17];    // all the notes for loop 1  (pvalues 0)
	uint8_t notes2[17];    // all the notes for loop 2  (pvalues 80)
	uint8_t loop[10];    // various positions in the loop for notes
	uint8_t tempo; // (p109)
};

struct seq_settings seq;                       // sequencer data (46 bytes)
const char seq_names[] = {'Pos', 'Notes1','Notes2','Loop','Tempo',
				};


//  USE THE BREAK WITH SWITCH STATEMENT MORON!!!


/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
