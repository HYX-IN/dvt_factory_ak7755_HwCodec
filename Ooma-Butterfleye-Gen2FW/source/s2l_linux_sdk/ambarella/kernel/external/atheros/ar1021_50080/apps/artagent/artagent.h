/*
* Copyright (c) 2010-2011 Atheros Communications Inc.
*
* Permission to use, copy, modify, and/or distribute this software for any
* purpose with or without fee is hereby granted, provided that the above
* copyright notice and this permission notice appear in all copies.
*
* THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
* WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
* MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
* ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
* WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
* ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
* OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/

#ifndef _ARTAGENT_H_
#define _ARTAGENT_H_

/* ATHENV */
#define ART_PORT                5454
//#define ART_PORT                33120
/* ATHENV */
#define LINE_ARRAY_SIZE         4096
#define SEND_ENDPOINT	        0
#define HTC_RAW_WRITE_MAX_LEN   (16 * 5) // 96, 112
#define TC_CMDS_SIZE_MAX  255

#ifndef POSTPACK
#ifdef __GNUC__
#define POSTPACK __attribute__ ((packed))
#else
#define POSTPACK
#endif
#endif

#ifndef PREPACK
#ifdef __GNUC__
#define PREPACK
#else
#define PREPACK
#endif
#endif

typedef enum {
    TCMD_CONT_TX_ID,
    TCMD_CONT_RX_ID,
    TCMD_PM_ID,
    TC_CMDS_ID,
    TCMD_SET_REG_ID,

    INVALID_CMD_ID=255,
} TCMD_ID;

typedef enum {
    TCMD_CONT_RX_PROMIS =0,
    TCMD_CONT_RX_FILTER,
    TCMD_CONT_RX_REPORT,
    TCMD_CONT_RX_SETMAC,
    TCMD_CONT_RX_SET_ANT_SWITCH_TABLE,

    TC_CMD_RESP,
} TCMD_CONT_RX_ACT;

typedef struct cmdReply {
        unsigned int replyCmdLen;
        unsigned int replyCmdId; // command ID of command to which this is a reply
        unsigned int status; // status of the command
        unsigned char cmdBytes[4000]; // bytes of the command reply
} CMD_REPLY;

typedef PREPACK struct {
    unsigned int   testCmdId;
    unsigned int   act;
    PREPACK union {
        unsigned int  enANI;    // to be identical to CONT_RX struct
        struct PREPACK {
            unsigned short   length;
            unsigned char    version;
            unsigned char    bufLen;
        } POSTPACK parm;
    } POSTPACK u;
} POSTPACK TC_CMDS_HDR;

typedef PREPACK struct {
    TC_CMDS_HDR  hdr;
    unsigned char      buf[TC_CMDS_SIZE_MAX+1];
} POSTPACK TC_CMDS;


// command ID's for the commands 
enum COMMAND_IDS {
	INIT_F2_CMD_ID,                         //0
	SELECT_HW_CMD_ID,						//1
	MEM_WRITE_CMD_ID,						//2
	MEM_READ_CMD_ID,						//3
	REG_READ_CMD_ID,						//4
	REG_WRITE_CMD_ID,						//5
	CFG_READ_CMD_ID,						//6
    	CFG_WRITE_CMD_ID,					//7
    	MEM_ALLOC_CMD_ID,					//8
    	MEM_FREE_CMD_ID,					//9
    	MEM_WRITE_BLOCK_CMD_ID,				//10
    	MEM_READ_BLOCK_CMD_ID,				//11
    	REMAP_HW_CMD_ID,					//12
    	CREATE_EVENT_CMD_ID,				//13
    	ANY_EVENTS_CMD_ID,					//14
    	GET_EVENT_CMD_ID,					//15
    	DISCONNECT_PIPE_CMD_ID,				//16
    	CLOSE_PIPE_CMD_ID,					//17
    	ISR_FEATURE_ENABLE_CMD_ID,			//18
    	ISR_GET_RX_STATS_CMD_ID,			//19
    	ISR_GET_TX_STATS_CMD_ID,			//20
    	ISR_SINGLE_RX_STAT_CMD_ID,			//21
    	ISR_SINGLE_TX_STAT_CMD_ID,			//22
    	WAIT_ON_EVENT_CMD_ID,				//23
	WAIT_ON_TIME_CMD_ID,					//24
	ISR_FEATURE_DISABLE_CMD_ID,				//25
	M_EEPROM_READ_CMD_ID,					//26
	M_EEPROM_WRITE_CMD_ID,					//27
	M_EEPROM_READ_BLOCK_CMD_ID,				//28
	M_EEPROM_WRITE_BLOCK_CMD_ID,			//29
	M_RESET_DEVICE_CMD_ID,					//30
	M_GENERIC_CMD_ID,						//31
    	M_CHECK_REGS_CMD_ID,				//31
    	M_CHANGE_CHANNEL_CMD_ID,			//33
    	M_CHECK_PROM_CMD_ID,				//34
    	M_REREAD_PROM_CMD_ID,				//35
    	M_TX_DATA_SETUP_CMD_ID,				//36
    	M_TX_DATA_BEGIN_CMD_ID,				//37
	M_TX_DATA_START_CMD_ID,					//38
   	M_TX_DATA_COMPLETE_CMD_ID,				//39
    	M_RX_DATA_SETUP_CMD_ID,				//40
    	M_RX_DATA_BEGIN_CMD_ID,				//41
	M_RX_DATA_START_CMD_ID,					//42
   	M_RX_DATA_COMPLETE_CMD_ID,				//43
	M_RX_STATS_SNAPSHOT_CMD_ID,				//44
    	M_TXRX_DATA_BEGIN_CMD_ID,			//45
	M_CLEANUP_TXRX_MEMORY_CMD_ID,			//46
    	M_TX_GET_STATS_CMD_ID,				//47
    	M_RX_GET_STATS_CMD_ID,				//48
    	M_RX_GET_DATA_CMD_ID,				//49
    	M_TX_CONT_BEGIN_CMD_ID,				//50
	M_TX_CONT_FRAME_BEGIN_CMD_ID,			//51
    	M_TX_CONT_END_CMD_ID,				//52
    	M_SET_ANTENNA_CMD_ID,				//53
    	M_SET_POWER_SCALE_CMD_ID,			//54
    	M_SET_TRANSMIT_POWER_CMD_ID,		//55
    	M_SET_SINGLE_TRANSMIT_POWER_CMD_ID,	//56
    	M_DEV_SLEEP_CMD_ID,					//57
    	M_CLOSE_DEVICE_CMD_ID,				//58
	M_CHANGE_FIELD_CMD_ID,					//59
	M_ENABLE_WEP_CMD_ID,					//60
	M_ENABLE_PA_PRE_DIST_CMD_ID,			//61
	M_DUMP_REGS_CMD_ID,						//62
	M_DUMP_PCI_WRITES_CMD_ID,				//63
	M_TEST_LIB_CMD_ID,						//64
	M_DISPLAY_FIELD_VALUES_CMD_ID,			//65
	M_GET_FIELD_VALUE_CMD_ID,				//66
	M_READ_FIELD_CMD_ID,					//67
	M_WRITE_FIELD_CMD_ID,					//68
	M_SET_RESET_PARAMS_CMD_ID,				//69
	M_CHANGE_MULTIPLE_FIELDS_ALL_MODES_CMD_ID, //70
	M_CHANGE_MULTIPLE_FIELDS_CMD_ID,		//71
	M_GET_FIELD_FOR_MODE_CMD_ID,			//72
	LOAD_AND_RUN_CODE_CMD_ID,				//73
	M_FORCE_SINGLE_PCDAC_TABLE_CMD_ID,		//74
	M_FORCE_PCDAC_TABLE_CMD_ID,				//75
	M_FORCE_POWER_TX_MAX_CMD_ID,			//76
	M_GET_EEPROM_STRUCT_CMD_ID,				//77
	M_GET_DEVICE_INFO_CMD_ID,				//78
	M_WRITE_PROD_DATA_CMD_ID,				//79
	M_WRITE_NEW_PROD_DATA_CMD_ID,			//80
	M_SPECIFY_SUBSYSTEM_CMD_ID,				//81
	M_FORCE_SINGLE_POWER_TX_MAX_CMD_ID,		//82
	M_SET_QUEUE_CMD_ID,						//83
	M_MAP_QUEUE_CMD_ID,						//84
	M_CLEAR_KEY_CACHE_CMD_ID,				//85
	RUN_SCREENING_TEST_CMD_ID,				//86
	DIAG_CMD,								//87
	ENABLE_HW_CAL_CMD,						//88
   	TRAM_WRITE_BLOCK_CMD_ID,				//89
   	TRAM_READ_BLOCK_CMD_ID,					//90
	M_GET_MAX_POWER_CMD_ID,					//91
	M_GET_PCDAC_FOR_POWER_CMD_ID,			//92
	M_FALSE_DETECT_BACKOFF_VALS_CMD_ID,		//93
	M_GET_MAC_ADDR_CMD_ID,					//94
	M_MASK_TRIGGER_SWEEP_CMD_ID,			//95
	M_MASK_DETECT_SIGNAL_CMD_ID,			//96
	M_MASK_SET_DEV_NUMS_CMD_ID,				//97
	M_MASK_FORCE_MIN_CCAPWR_CMD_ID,			//98
	M_MASK_CONFIG_CAPTURE_CMD_ID,			//99
	AP_REG_READ_CMD_ID,						//100
	AP_REG_WRITE_CMD_ID,					//101
	M_SET_LIB_CONFIG_CMD_ID,				//102
	M_GET_XPDGAIN_CMD_ID,					//103
	M_SELECT_DEV_NUM_CMD_ID,				//104
	M_GET_POWER_INDEX_CMD_ID,				//105
	M_GET_ART_ANI_LEVEL_CMD_ID,				//106
	M_SET_ART_ANI_LEVEL_CMD_ID,				//107
	M_FTP_DOWNLOAD_FILE_CMD_ID,				//108
	M_GET_CTL_INFO_CMD_ID,					//109
	M_GET_MAX_LIN_PWR_CMD_ID,				//110
	M_SET_CHAIN_CMD_ID,                     //111
	M_GET_PHASE_CAL_CMD_ID,                 //112
   	M_TX_DATA_SETUP_NO_END_PACKET_CMD_ID,   //113
	M_HW_RESET_CMD_ID,						//114
	M_PLL_PROGRAM_CMD_ID,					//115
	M_PCI_WRITE_CMD_ID,						//116
	M_CAL_CHECK_CMD_ID,						//117
	M_FILL_TX_STATS_CMD_ID,					//118
	M_CREATE_DESC_CMD_ID,					//119
	LOAD_AND_PROGRAM_CODE_CMD_ID,			//120
	M_FORCE_SINGLE_PCDAC_TABLE_GRIFFIN_CMD_ID, 	//121
	PHS_CHANNEL_TOGGLE_CMD_ID, 				//122
	STOP_PHS_CHANNEL_TOGGLE_CMD_ID, 		//123
	GENERIC_FN_CALL_CMD_ID, 				//124
	SEND_FRAME_CMD_ID,					    //125
	RECV_FRAME_CMD_ID,					    //126
	M_EEPROM_WRITE_BYTEBASED_BLOCK_CMD_ID,	//127
    GET_ENDIAN_MODE_CMD_ID,                 //128
	M_TX_DATA_AGG_SETUP_CMD_ID,             //129
	M_RX_DATA_AGG_SETUP_CMD_ID,             //130
	IQ_CAL_CMD_ID,						    //131
    RTC_REG_WRITE_CMD_ID,                   //132
    RTC_REG_READ_CMD_ID,                    //133
    M_CHANGE_ADDAC_FIELD_CMD_ID,            //134
    M_SAVE_XPA_BIAS_FREQ_CMD_ID,            //135
    M_PA_OFFSET_CAL_CMD_ID,                 //136

    // tbd: in case more cmds added at main branch
    // migrated from AR6002
    M_EEPROM_READ_LOCS_CMD_ID = 160,        //160
    GET_REF_CLK_SPEED_CMD_ID,               //161
    SLEEP_CMD_ID,                           //162

    ART_IMAGE_DOWNLOAD_ID,                  //163
    ART_TEST_UITRON,                        //164
    ART_TEST_CLIENT,                        //165

    ART_CLIENT_REWIND_ID,                   //166
    ART_THROUGHPUT_TEST_CMD_ID,             //167
    ART_THROUGHPUT_TEST_CMD_ID_1,           //168

    ART_BMI_WRITE_SOC_REGISTER_ID,          //169
    ART_BMI_WRITE_SOC_MEMORY_ID,            //170
    ART_BMI_OP_DONE_ID,                     //171
    ART_BMI_READ_SOC_REGISTER_ID,           //172
    ART_BMI_READ_SOC_TARGET_ID,             //173

    // added for Venus
    OTP_WRITE_CMD_ID,                       //174
    OTP_READ_CMD_ID,                        //175
    OTP_RESET_CMD_ID,                       //176
    EFUSE_READ_CMD_ID,                      //177
    M_WRITE_CARD_LABEL_TO_FILE,             //178

    // Bluetooth commands
    BTART_SEND_CMD_ID,                      //179
    BTART_RECEIVE_CMD_ID,                   //180

    BT_WRITE_CMD_ID,                        //181
    
    // added for AR6004 NART
    M_RX_ENABLE_CMD_ID,                     //182
    M_RX_DATA_STOP_CMD_ID,                  //183
    M_RX_DATA_STATUS_CMD_ID,                //184  

    M_TX_ENABLE_CMD_ID,                     //185
    M_TX_DATA_STOP_CMD_ID,                  //186
    M_TX_DATA_STATUS_CMD_ID,                //187

    EFUSE_WRITE_CMD_ID,                     //188    
    
    M_EEPROM_WRITE_ITEMS_CMD_ID,            //189

    M_STICKY_WRITE_CMD_ID,                  //190
    M_STICKY_CLEAR_CMD_ID,                  //191

    OTP_LOAD_CMD_ID,                        //192
    OTP_ERASE_CMD_ID,                       //193
    OTP_REMAINBYTES_CMD_ID,                 //194
    M_PSAT_CALIBRATION_GET_RESULT,          //195
};


#endif /* _ARTAGENT_H_ */
