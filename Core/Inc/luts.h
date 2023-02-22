

const char default_menu[] ={" 00SeqPos     00Tempo    01Transpos 02Transpos 03Transpos 05Transpos  "  // was getting corrupted then screwed everything
		"00Notes1  01Notes1  02Notes1  03Notes1  04Notes1  05Notes1  06Notes1  07Notes1  08Notes1  09Notes1  10Notes1  11Notes1  12Notes1  13Notes1  "
		"14Notes1  15Notes1  00Notes2  01Notes2  02Notes2  03Notes2  04Notes2  05Notes2  "
		"06Notes2  07Notes2  08Notes2  09Notes2  10Notes2  11Notes2  12Notes2  13Notes2  14Notes2  15Notes1  "
		"00Rate     00Depth    00Offset      01Rate     01Depth    01Offset    "
		"02Rate     02Depth    02Offset      03Rate     03Depth    03Offset    "
		"04Rate     04Depth    04Offset      05Rate     05Depth    05Offset    "
		"06Rate     06Depth    06Offset      07Rate     07Depth    07Offset    "

		"00Rate     00Depth    00Offset   00Target  00Targindx        "
		"01Rate     01Depth    01Offset   01Target  01Targindx        "
		"02Rate     02Depth    02Offset   02Target  02Targindx        "
		"03Rate     02Depth    02Offset   03Target  03Targindx        "
		"04Rate     04Depth    04Offset   04Target  04Targindx        "
		"05Rate     05Depth    05Offset   05Target  05Targindx        "
		"06Rate     06Depth    06Offset   06Target  06Targindx        "
	//	"07Rate     07Depth    07Offset   07Target  07Targindx        "
		"00Attack   00Decay    00Sustain  00Release          "
		"01Attack   01Decay    01Sustain  01Release          "};   // just for testing text memory , will be modifiable ,  lut
//  BEWARE OF TAB , CHECK SPACING !     , should ok once auto generated

char default_menu3[1024]; // hold all string for output  128 per page , needs to resize
const uint8_t  menu_vars_index_limit[36]= {0,9,9,9,9,9,0,5,5,5,5,0,6,6,6,6,6,6,6,6,6,0,0,0,15,15,9,3,3,3,3,3,3,3,3,9} ; // index number limiter ,fixed
const uint8_t  menu_vars_divider[36] = {0,0,0,0,0,0,0,0,0,0,
																				0,0,0,0,5,0,4,5,5,1,
																				0,0,0,4,4,5,5,0,0                                         };   // right shift divider mainly for LFO  , maybe for lcd too


const char* menu_titles_final[]= {"LFO     ", "Rate    ","Depth   " ,"Gain    ", "Offset  ", "Target  ","ADSR    ",
		"Attack  ", "Decay   ","Sustain ","Release ",
		"Note    ","OSC1    ","OSC2    ","Pitch   ","Length  ","Note Pos","Transpos","Slide   ","Velocity","Detune  ",
		"Sequencr", "SeqPos  ","Tempo   " ,"Notes1  ","Notes2  ","Loop    ","Cutoff_1","Cutoff_2","Resnance","Q_level ","Level   ","Feedback","Out_mix ","Poles   ",
		"Targindx"
};   // 36 *8
uint8_t lfo_skip_list[36]={ 0,0,1,0,1,1,0,0,0,0,
		1,1,1,1,0,1,1,1,0,0,
		1,1,1,1,1,1,1,0,1,1,
		1,1,1,1,1,1

};


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
