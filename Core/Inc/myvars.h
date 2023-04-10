

// STM32F411
//   user adjust 0-160    , internal 0-255 or 16bit
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
#define menu_title_counter_limit    512

// flash start 0x08000000    , prog size  59B0  22,960 bytes  end   0x080059B0 , 64k ends at 0x08010000 , maybe after or D2F0 which is 54k so 0x0800D2F0
// source , select mask type , speed of playback
//#define seq_sample_rate 39308
// clock/period

const uint32_t flash_start=0x0800D2F0; // flash start address , 2048 page saize or 0x800
const uint32_t flash_page=0x800;

uint32_t x;  // calc timer note
uint32_t y;
//uint16_t pwmVel;
//uint16_t pwmVelB;
//const uint8_t tempoLUT[];  // lookup table for tempo instead of calculate

const char major_notes[]={"cdefgahCDEFGAHcdefgahCDEFGAHcdefgahCDEFGAHcdefgahCDEFGAHcdefgah"};
const uint8_t MajorNote[30]= { 0,2,4,5,7,9,11,12,14,16,17,19,21,23,24,26,28,29,31,33,35,36,38,40,41,43,45,47,48,50} ;  // major
const uint8_t MinorNote[30]={ 0,2,3,5,7,8,10,12,14,15,17,19,20,22,24,26,27,29,31,32,34,36,38,39,41,43,44,46,48,49}; // minor
const uint8_t ChromNote[]={0,2,3,5,6,8,9,11,12,14,15,17,18,20,21}; //chromatic, diminished
//const uint8_t noteReference[] = {10,10,10,10,0,10,0,8,1,10,1,8,2,10,3,10,3,8,4,10,4,8,5,10,5,8,6,10,0,11,0,8,1,11,1,8,2,11,3,11,3,8,4,11,4,8,5,9,5,8,6,9,10,11,10,20,10,11,10,12,10,13,10,14,10,15,10,16,10,17,10,18,10,19,11,20,11,11 };// cant read last
const uint16_t timerValues[]= {34400,34400,32469,30647,28927,27303,25771,24324,22959,21670,20454,19306,18222,17200,16234,15323,14463,13651,12885,12162,11479,10835,10227,9653,9111,8600,8117,7661
,7231,6825,6442,6081,5739,5417,5113,4826,4555,4300,4058,3830,3615,3412,3221,3040,2869,2708,2556,2413,2277} ;   // timer values C2-C6
const uint16_t sample_Noteadd[52]= { 1920,2034,2155,2283,2419,2562,2715,2876,3047,3228,3420,3624,3839,4068,4309,4566,4837,5125,5430,5752,6095,6457,6841,7248,7679,8135,8619,9131
	,9674,10250,10859,11505,12189,12914,13682,14495,15357,16270,17238,18263,19349,20499,21718,23010,24378,25828,27363,28991,30714,32541,34476,
	36526};
const uint16_t sample_counts[52]={35023,33061,31209,29461,27811,26254,24784,23396,22087,20851,19684,18583,17543,16562,15636,14762,13937,13159,12424,11730,11075,10457,9874,9323,
	8804,8313,7850,7413,7001,6611,6244,5897,5570,5261,4969,4694,4434,4189,3957,3739,3532,3338,3154,2981,2817,2662,2517,2379,2249,2126,2011,1901};


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
uint16_t sample_counts_holder[40];
uint16_t sample_accu_counter[8]={0};   //holds counter value
uint8_t noteTiming;  // set timing shift
uint8_t potValues [EPROM_limit ]={0};   //low res values mostly for display

uint8_t potSource[EPROM_limit ]={0}; // high res version of potValues used when needed 40-0, gonna change to 160 just o break things, need more res for lfo

uint32_t sine_counter[24];  // up counter for sine reading
float sine_counter_float[5];
uint16_t isr_change=0;
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
void patch_target_replace(void);
void display_clear (void);
void patch_target_modify(void);
void gfx_clear(void);
void patch_target_parse(void);
void gfx_send(void);
float filter_pole(float input,float old_ouput,float freq1,float freq2) ;
uint8_t*  menu_vars(char* menu_string,  uint8_t var_index   ); // string +index , returns pointer to struct
void display_process(void);
void displayBuffer2 (void);  // new version
void analogInputloop(void);
void displayLoop(void);
;void timerNote(void);
void timerNote2(void);

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
void note_reset (void);
uint8_t seq_pos; // sequencer position linked to isrCount for now but maybe change
int32_t sine_count(void);
int32_t sine_count2(uint8_t note_selected,  uint32_t* input_array, uint32_t* return_array );
int32_t sampler_oneshot(uint8_t note_selected,  uint32_t* input_array, uint32_t* return_array );
void  mask_calc(uint8_t mask_select,uint8_t mask_speed);
void sampler_save(void);
void sampler_ram_record(void);
void gfx_reverse(uint8_t cursor_pos,uint8_t cursor_partial);
void gfx_line_fill (void);
void menu3_fill(void);
void encoder2(void);
void gfx_send_DMA(void);
void gfx_TX_block(void);
void LFO_square_one_pulse(void);
void LFO_source_synced(void);
void  frq_point(void);
void patch_lists(void);
void  flash_sector_erase( uint32_t address_erase_s );   // needs 24bit address , returns 1 if fail  else 0
void flash_page_write(uint32_t address_write,uint8_t*  block);  //256 bytes
void flash_page_read (uint32_t address_read);  //512bytes  DMA
void  flash_block_erase(uint32_t address_erase_b);  // 32 kbyte
void byte_swap(uint8_t* to_swap, uint32_t  array_size);
uint8_t  sampler_ram_clear_test(uint16_t sample_number);
void sample_save(uint16_t sample_number, uint8_t* sample_data ,uint16_t  sample_size);
void  sampler_1k_load(uint32_t load_address);
void LCD_Info_feedback(void);
void RAM_normalise(void);
void record_output_to_RAM (void);
void LCD_Info_notes(void);
void sine_count_sample(void);
void ADSR_loop(void);

 uint16_t noteBar[257]={0,12,12,12,12,12,12,12,12,12,1,22,1};  //   8 bar data , start , end ,vel,wave * 8  3*wave note length cant be uint32_ter than next start
uint8_t NoteC; // second channel note
 uint8_t spell[322] = { 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78,
		79, 254,192, 65, 66, 67, 68, 65, 70, 71, 72, 70, 74, 75, 76, 77, 78,79,79,254,128,0 }; //16x2 chars
uint8_t spellB[42]; // temp store



uint16_t sine_length=600; //holds sample size

uint16_t	sine_lut[]= { 33,35,37,39,42,44,47,50,53,56,59,63,66,70,75,79,84,89,94,100,106,112,119,126,133,141,150,159,168,178,189,
		200,212,225,238,252,267,283,300,318,337,357,378,401,425,450,477,505};  // (48 steps) sine + step *32   ,current
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

  uint16_t play_sample[2050]={} ;  //sample storage  , big better
volatile uint16_t sample_point; // pointer for sample array
volatile uint16_t sample_point2;
 volatile  uint16_t play_hold;
 volatile  uint16_t play_hold2;
 uint16_t sample_pointD;  // global store buffer start

 unsigned short bank_write=1;

uint16_t note_channel[31]={ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};  // 10 notes , 10 velocity 20->16bit , 10 mask toggle
 unsigned short mask_result;

static uint16_t adc_source[3072] ;  // for soem reason static maybe important , also 16 bit most def
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

static int32_t sample_accus_hold [6]={0,0,0,0,0,0};  // sample array
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
int8_t enc2_add=0;   // simple adder 1 or 0 or -1 ,starts on 2    , int !
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
float filter_res[5];
float  filter_accus[15];  // hold floats for filter
float filter_hold[5];  //holds some feedback related stuff
float freq_point[8]={0,0,0,0,0,0,0,0} ; // multiplier coeff holder temp
float freq_pointer[4] [9];  // multiplier coeff holder
uint8_t i_frac;  // divide i/64
uint8_t seq_store;  // just an seq_pos holder for adsr
uint16_t trigger_counter;
uint32_t  lfo_accu[10]  [10]; //holds last lfo value , simple 0-255 upcount for now,will change; 10x8
uint16_t  lfo_out[10] [10];   //8x10 values for lfo
uint16_t tempo_lut[256]; // tempo look up 40-200bpm
uint16_t lfo_tempo_lut[256];
uint16_t tempo_mod_hold;
////////////////////////////////////////////////////////////////////////////////////////////////////////////GFX
//uint8_t gfx_ram[64][18] ; //holds data for lcd 64*128bit +2 bytes ver/hor command for lcd
uint8_t  gfx_ram[1153]={0}; ;

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
volatile static uint16_t  error_count=0;

struct LFO_settings{      // use first 5*10 , leave the rest  , no bueno  32 each

	uint8_t rate; // (p130)
	uint8_t depth;	//(p140)
	uint8_t gain;
	uint8_t offset;
	uint8_t delay;  // +   ,,essential feature
	uint8_t phase;  // unsure
	//uint8_t target;      // lfo modulation target  , number for now , 0-16(index) + (0-27)<<4, variable ptr
	//uint8_t target_index;
	uint16_t out[10];       // sine, actual output , needs multiple for loop,,calculated
//	uint8_t* out_ptr;   // address from target (mostly 8 bit,mostly )
	uint16_t out_saw[10];   // saw output
	uint16_t out_tri[10];
	uint8_t out_8b;  // 8bit output from sine
};

struct LFO_settings LFO[10]={0};       // create lfo settings for slave1


struct LFO_settings_slave{      // use first 5*10 , leave the rest  , no bueno  32 each

	uint8_t rate; // (p130)
	uint8_t depth;	//(p140)
	uint8_t gain;
	uint8_t offset;
	uint8_t delay;  // + lfo_accu_temp
	uint8_t phase;  // unsure


};

struct LFO_settings_slave LFO_slave1[10]={0};       // create lfo settings for slave1

struct LFO_square_settings{   // pwm square   , may also use it for trigger
	uint8_t rate; // (p130)
	uint8_t depth;	//(p140)
	uint8_t offset;
	uint8_t delay;  // + lfo_accu_temp
	uint16_t out[8]; //tracks position , gets reset

};
struct LFO_square_settings LFO_square[10];






struct ADSR_settings{   // use initial 5*10  , leave rest
	uint8_t attack;   // presets,  0-15  , simply  time multiplied related to next isr
	uint8_t decay;		//	 time
	uint8_t sustain; 		// level  after decay
	uint8_t release;		// time  for sustain+ release  50/50

	uint8_t attack_trigger;    // start adsr

	int16_t attack_data;        // processing data   , counts down to 0 when complete
	int16_t  decay_data;
	int16_t sustain_data;
	int16_t release_data;
 //   float  buffer_temp;  // this is bad here
	int16_t buffer[8];  //this holds the actual envelope  for ADSR for later processing

};
struct ADSR_settings ADSR[5];   // adsr data




struct note_settings{								//default note/osc/patch settings  14*8 bytes (112), use all
	uint8_t osc;      // waveforms or patches setting
	uint8_t osc2;		// second wave for decays or stacking
	uint8_t pitch;
	uint8_t duration;   // note length
	uint8_t position;   //  use it for sample trigger for now
	uint8_t transpose;   // pshift note pitch up or down (p 72,73) 0-31 major 31-64 minor 64-96 chrom ?
	uint8_t timeshift; // shift position left or right in seuqence for 8 note looper   (pvalues 32,33) ,slide
	uint8_t velocity;				// gain level or output mod
	uint8_t detune; 					// finetune maybe for lfos or some default tune
	uint8_t slide_length;
	uint8_t not_used;

	uint16_t osc_add;   // this is the add value hold for sine/wav/saw etc depends on wave form,calculated
	uint16_t tuned;   // final output after note detune and osc_add calculation , drives the oscillators ,calculated


};
struct note_settings note[10]={[0].velocity=255,[1].velocity=255,[2].velocity=255,[3].velocity=255,[4].velocity=255,[5].velocity=255,[6].velocity=255
															,[0].detune=0,[1].detune=0,[2].detune=0,[3].detune=0,[4].detune=0,[5].detune=0,[6].detune=0

};
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
struct filter_settings filter[4]={[0].cutoff_1=0,[1].cutoff_1=0,[2].cutoff_1=0,[3].cutoff_1=0,[0].level=64,[1].level=64,[2].level=64,[3].level=64
	,[0].resonance=0,[1].resonance=0,[2].resonance=0,[3].resonance=0,
};

struct patch_settings{					// use this instead of lfo  or other modulators
	uint8_t input1;   // in1 ,default , modulation source , lfo+type and adsr for now
	uint8_t target;	//target  type
	uint8_t target_index;	//index
	uint8_t input2;  // secondary input, optional
	uint8_t in_mix;  // mix in1 and in2, 0 default
	uint8_t in_offset;  // offset values
	uint16_t output[10];  // actual output
	uint8_t*  out_ptr;  // output target , might change that
	uint16_t*   in1_ptr;     // use ptr for reading
	uint16_t*   in2_ptr;     // use ptr2 for reading
	uint8_t limiter;   // output limiter for target  8 bit
	uint8_t divider;
};
struct patch_settings patch[20];    // patch board

struct sampler_settings{

    uint8_t start_MSB;
    uint8_t start_LSB;
    uint8_t end_MSB;
    uint8_t end_LSB;
    uint8_t trigger_1;   // uses +1 to enable
    uint8_t trigger_2;
    uint8_t trigger_3;
    uint8_t trigger_4;
    uint8_t repeat;
    uint8_t offset;  // offset to seq.pos or to start of sample ?
    uint8_t sample_select;    // sample to play 0-255 for now , maybe starting from 0 mem


    uint8_t sample_save;  // location to save to , 32-64kb

    uint16_t trigger_position;  // enabled for start with byte position for retirgger  , reset counter to sampler.start  . +1 to enable
    uint8_t record_enable;   //record to ram max 1-2 sec for now
uint8_t sample_location;   // save location on flash ,preset for now
uint8_t sample_save_enable ;   // save flag to flash
uint8_t one_shot; // one shot bit flag , 0-7 notes
uint8_t RAM_free ;  // enables flash playback
uint16_t  start;    // for trimming  start ,calucalted
 uint16_t end;	// for trimming end ,calculated
uint16_t length;  // length for looping  ,calucalted
uint16_t ram_pos;  //record pos

uint16_t* start_ptr;
uint16_t ram_seq;   // seq current position

};
static struct sampler_settings sampler={.record_enable=0, .sample_location=0,.sample_save_enable=0,.ram_pos=0
		,.end_MSB=63, .end_LSB=255,.start_MSB=0,.start_LSB=0,.length=1024,.one_shot=255,.RAM_free=0  };                                     // needs to be protected

struct sample_info{   // stored in the last 64kb
    uint8_t sample_record;
    uint8_t sampler_options[10];   //various option array
    uint16_t tempo_map[10]; // trigger points
    char sample_name[16]; //holds sample name

};


struct sample_info sample_info;

uint16_t string_search=0;   // search position on created menu
uint16_t string_value=0;  // holds the variable result from the search result
uint16_t menu_counter=0;  // 127 per page needs plenty
uint8_t space_check=0;   // look for gaps


uint16_t menu_title_count=0;   // holds the counter for menu_title_lut

uint32_t  menu_var_lut [menu_title_counter_limit]={0};   // hold pointers for variables ,  menu_title_count

uint32_t menu_title_lut[menu_title_counter_limit]={0};  // hold pointer for feedback line , points to default_menu first character(1<<8)   as well the current display loc(0)  , skip empty areas for now

char menu_index_list[menu_title_counter_limit*2]={0};   //  use along the menu_var_lut uses double the records !! gets weird when using 256
char* menu_vars_menu=0;    // return pointer to menu_titles final
uint8_t * menu_vars_var=0;			// return memory location to var !
char menu_vars_in[8];  // incoming string ,ok
uint8_t menu_index_in=0; // gets the struct index ie LFO[1].rate
uint16_t default_menu3_size=0;    //  just set size of menu

uint16_t enc2_mem_dir;
int16_t enc_out1=1;    // menu_title_lut   cursor position
uint32_t  enc2_store[5];
uint8_t enc2_store_count=0;
uint8_t  lcd_temp=0;   //temp hold
uint8_t spi2_send_enable=0;
uint8_t target_display=0;   // enabled when cursor is on LFO.target
uint8_t sampling_position=0; // tracks loop position in sampling for LFO mainly 0-7
uint8_t sampling_position_b=0;    //previous step
uint8_t gfx_clear_flag=0;    // important for clearing screen
uint8_t mem_errors=0;   // eeprom error count
uint8_t mem_verify=0;
int32_t play_holder1[512];    // data banks
int32_t play_holder2[512];
int32_t play_holder3[512];    // data banks
int32_t play_holder0[512];
uint8_t clipping=0;
uint8_t notes_joined[33];
uint8_t menu_vars_ref=0;  // menu vars search reference , used for divider
uint8_t div_limit=0;
uint8_t serial_send[8]={254,210,0,0,254,210,0,0};
uint8_t serial_source[256]={0};
uint8_t serial_source_temp[256]={0};
uint8_t serial_up=0;  //counter for serial send
uint8_t serial_tosend=0;
//uint16_t RAM[16384]={0};


uint16_t* sample_ram=NULL;
uint32_t sampler_offset;
uint16_t enc_mem_dir=0;
uint8_t  last_pos_hold; // multiple cursors
uint8_t cursor_partial;
uint16_t disp_up_counter=0;
uint8_t gfx_send_counter4=0;
uint16_t gfx_dma=3;
uint16_t gf_timer=0;
static uint8_t spi_tx_block[4096]={0};  // tx store for dma
static uint16_t block_counter=0;
uint16_t lfo_tempo_synced[256];     // tempo lut table duplicate every 16 steps
uint8_t LFO_sqr_list[10];
int32_t phase_bank0[32];
int32_t phase_bank1[32];
int32_t phase_bank2[32];
int32_t phase_bank3[32];
uint8_t LCD_Info[99]={0};   // lcd_numbers or text  data anywhere on screen
uint8_t sqr_target_list[20];  // keep record of patch target for LCD_info using  menu_titles_final  as  a list ref

uint16_t seqpos_i;// i+1
int32_t  debug_value;
uint32_t tempo_large;
 static uint8_t flash_read_block[530]={0};
 static uint8_t flash_read_block2[1030];
volatile uint8_t flash_flag=4;
uint8_t  flash_bank_read=0;   // switch fifo for playback
uint16_t counter_16=0;
uint8_t current_spi[4];
uint8_t error_data[128]={0};
uint8_t stop_toggle=0;
uint32_t  millis_stored=0;
uint32_t  millis_stored2=0;
uint16_t record_counter=0;
uint16_t RAM_looper=0;
volatile uint8_t page_skip=0;
volatile uint8_t  record_output=0;
uint8_t output_mix[1024]={0};  // records total output
uint16_t record_out_counter=0;
uint8_t LFO_vars_divider[menu_lookup_count]={0};    // multiply/ divider for LFO output only
uint16_t adc_playback_position=0;
float float_table[256]={0};
uint8_t zero_cross[4]={0};



//static uint16_t tuned_list[10];

// pointer to ram
 // ram current position for playback/record  0-16384
//  USE THE BREAK WITH SWITCH STATEMENT MORON!!!


/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
