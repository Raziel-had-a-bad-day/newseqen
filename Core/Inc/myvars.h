

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

uint8_t noteTiming;  // set timing shift
uint8_t potValues [383];  //low res values mostly for display

uint8_t potSource[383]; // high res version of potValues used when needed 40-0, gonna change to 160 just o break things, need more res for lfo

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
void lfo_target_replace(void);
void display_clear (void);
void lfo_target_modify(void);
void gfx_clear(void);
void lfo_target_parse(void);
void gfx_send(void);
float filter_pole(float input,float old_ouput,float freq1,float freq2) ;
uint8_t*  menu_vars(char* menu_string,  uint8_t var_index   ); // string +index , returns pointer to struct
void display_process(void);
void displayBuffer2 (void);  // new version
void analogInputloop(void);
void displayLoop(void);
;void timerNote(void);
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
void display_generate(void);
void gfx_send_page(void);
void LFO_source(void);
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

// seems to send odd data with no audio output for lcd ????????
uint8_t one_shot; // up counter for attack sample
uint8_t cust_char[64];  // holder for custom characters 8x5bit x8 ,Send command: 0x40 (Set pointer to the first CGRAM address).

uint16_t enc_temp;  // hold in for tim2
uint16_t enc_tempB;  // hold old in
uint16_t enc2_temp;  // hold in for tim4
uint16_t enc2_tempB;  // hold old in2
int8_t enc2_tempC=10;
uint16_t enc2_dir;    // enc direction2
int16_t enc_dir;    // enc direction
int8_t enc2_add=2;   // simple adder 1 or 0 or -1 ,starts on 2    , int !
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
uint16_t mem_count; //just count mem address
uint32_t loop_counter2; //long counter
uint8_t mem_buf;
uint16_t menu_page[3]; // switch between pages, keep track when flipping
uint8_t seq_loop[7]; //loop positions

//new stuff///////////////////////////////////////////////////////////////////////////////////////////////////////
float  filter_accus[15];  // hold floats for filter
float filter_hold[5];  //holds some feedback related stuff
float freq_point[8]={0,0,0,0,0,0,0,0} ; // multiplier coeff holder temp
float freq_pointer[4] [9];  // multiplier coeff holder
uint8_t i_frac;  // divide i/64
uint8_t seq_store;  // just an seq_pos holder for adsr
uint16_t trigger_counter;
uint32_t  lfo_accu[10]  [10]; //holds last lfo value , simple 0-255 upcount for now,will change; 10x8
uint16_t  lfo_out[10] [10];   //8x10 values for lfo
uint16_t tempo_lut[162]; // tempo look up 40-200bpm
////////////////////////////////////////////////////////////////////////////////////////////////////////////GFX
//uint8_t gfx_ram[64][18] ; //holds data for lcd 64*128bit +2 bytes ver/hor command for lcd
uint8_t  gfx_ram[1153];

uint16_t  gfx_send_counter=0;   //counts up to 31x18x2
uint8_t gfx_send_counter2=0;			// horizontal pos 0-17
uint8_t gfx_send_counter3=0;
uint8_t gfx_send_cursor=0;  // send cursor line 0-7
uint8_t gfx_send_swap=0;  //swap between lines
uint8_t gfx_send_lines=0; // just count 144   lines
float tempo_sync;

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

struct LFO_settings{      // use first 5*10 , leave the rest  , no bueno  32 each

	uint8_t rate; // (p130)
	uint8_t depth;	//(p140)
	uint8_t gain;
	uint8_t offset;
	uint8_t target;      // lfo modulation target  , number for now , 0-16(index) + (0-27)<<4, variable ptr
	uint8_t target_index;
	uint16_t out[10];       // actual output , needs multiple for loop,,calculated
	uint8_t* out_ptr;   // address from target (mostly 8 bit,mostly )
	};

struct LFO_settings LFO[10]={0};       // create lfo settings

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
struct ADSR_settings ADSR[5]={0};   // adsr data



struct note_settings{								//default note/osc/patch settings  14*8 bytes (112), use all
	uint8_t osc;      // waveforms or patches setting
	uint8_t osc2;		// second wave for decays or stacking
	uint8_t pitch;
	uint8_t duration;   // note length
	uint8_t position;   // note position in sequence loop
	uint8_t transpose;   // pshift note pitch up or down (p 72,73)
	uint8_t timeshift; // shift position left or right in seuqence for 8 note looper   (pvalues 32,33) ,slide
	uint8_t velocity;				// gain level or output mod
	uint8_t detune; 					// finetune maybe for lfos or some default tune
	uint16_t osc_add;   // this is the add value hold for sine/wav/saw etc depends on wave form,calculated
	uint16_t tuned;   // final output after note detune and osc_add calculation , drives the oscillators ,calculated


};
struct note_settings note[7]={[0].velocity=64,[1].velocity=64,[2].velocity=64,[3].velocity=64,[4].velocity=64,[5].velocity=64,[6].velocity=64
															,[0].detune=64,[1].detune=64,[2].detune=64,[3].detune=64,[4].detune=64,[5].detune=64,[6].detune=64};
//struct note_settings note[7];


struct seq_settings {				// 46 bytes need all
	uint8_t pos;    // actual position of the sequencer atm ,calculated
	uint8_t tempo; // (p109)
	uint8_t notes1[17];    // all the notes for loop 1  (pvalues 0)
	uint8_t notes2[17];    // all the notes for loop 2  (pvalues 80)
	uint8_t loop[10];    // looping start point in a sequence   0-15


};

struct seq_settings seq;                       // sequencer data (46 bytes)
struct filter_settings{
	uint8_t cutoff_1;					//basic cutoff  0-159 prolly lut
	uint8_t cutoff_2;   // extra bits for cutoff  159
	uint8_t resonance;			//159
	uint8_t q_level;					//0-16
	uint8_t level;			//default 64
	uint8_t poles;		//1-4
	uint8_t out_mix; 		// default 64
	uint8_t feedback;  // 159

};
struct filter_settings filter[4]={[0].cutoff_1=159,[1].cutoff_1=159,[2].cutoff_1=159,[3].cutoff_1=159,[0].level=64,[1].level=64,[2].level=64,[3].level=64};


uint16_t string_search=0;   // search position on created menu
uint16_t string_value=0;  // holds the variable result from the search result
uint16_t menu_counter=0;  // 127 per page needs plenty
uint8_t space_check=0;   // look for gaps


uint8_t menu_title_count=0;   // holds the counter for menu_title_lut

uint32_t  menu_var_lut[128];    // hold pointers for variables , for now its enougg

uint32_t menu_title_lut[128];  // hold pointer for feedback line , points to default_menu first character(1<<8)   as well the current display loc(0)  , skip empty areas for now

char menu_index_list[256];   //  use along the menu_var_lut uses double the records !! gets weird when using 256
char* menu_vars_menu=0;    // return pointer to menu_titles final
uint8_t * menu_vars_var=0;			// return memory location to var !
char menu_vars_in[8];  // incoming string ,ok
uint8_t menu_index_in=0; // gets the struct index ie LFO[1].rate
uint16_t default_menu3_size=0;    //  just set size of menu

int8_t enc_out1=1;    // menu_title_lut   cursor position
uint8_t  enc2_store[5];
uint8_t enc2_store_count=0;
uint8_t  lcd_temp=0;   //temp hold
uint8_t spi2_send_enable=0;
uint8_t target_display=0;   // enabled when cursor is on LFO.target
uint8_t sampling_position=0; // tracks loop position in sampling for LFO mainly 0-7
uint8_t gfx_clear_flag=0;    // important for clearing screen
uint8_t mem_errors=0;   // eeprom error count
uint8_t mem_verify=0;
int32_t play_holder1[512];    // data banks
int32_t play_holder2[512];
int32_t play_holder3[512];    // data banks
int32_t play_holder0[512];
//  USE THE BREAK WITH SWITCH STATEMENT MORON!!!


/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
