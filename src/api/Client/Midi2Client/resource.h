//
//    Copyright (C) Microsoft.  All rights reserved.
//

#define IDS_MIDI2_CLIENT 100


//{{NO_DEPENDENCIES}}
// Microsoft Visual C++ generated include file.
// Used by Windows.Devices.Midi2.rc

// Next default values for new objects
// 
#ifdef APSTUDIO_INVOKED
#ifndef APSTUDIO_READONLY_SYMBOLS
#define _APS_NEXT_RESOURCE_VALUE        101
#define _APS_NEXT_COMMAND_VALUE         40001
#define _APS_NEXT_CONTROL_VALUE         1001
#define _APS_NEXT_SYMED_VALUE           101
#endif
#endif



// Common MIDI Strings

#define IDS_MIDI_COMMON_LABEL_GROUP_FULL							50100
#define IDS_MIDI_COMMON_LABEL_GROUP_SHORT							50101

#define IDS_MIDI_COMMON_LABEL_CHANNEL_FULL							50105
#define IDS_MIDI_COMMON_LABEL_CHANNEL_SHORT							50106

#define IDS_MIDI_COMMON_LABEL_MUID_FULL								50108
#define IDS_MIDI_COMMON_LABEL_MUID_SHORT							50109


// Message type descriptions. These are optionally displayed by apps using the API


// Message Type ?

#define IDS_MESSAGE_DESC_MT_UNKNOWN									50200


// Message Type 0 - Utility

#define IDS_MESSAGE_DESC_MT0_NOOP									50201
#define IDS_MESSAGE_DESC_MT0_JR_CLOCK								50202
#define IDS_MESSAGE_DESC_MT0_JR_TS									50203
#define IDS_MESSAGE_DESC_MT0_TICKS_PQN								50204
#define IDS_MESSAGE_DESC_MT0_DELTA_TICKS							50205
#define IDS_MESSAGE_DESC_MT0_UNKNOWN								50206

// Message Type 1 - System Common

#define IDS_MESSAGE_DESC_MT1_F0_RESERVED							50300
#define IDS_MESSAGE_DESC_MT1_F1_TIME_CODE							50301
#define IDS_MESSAGE_DESC_MT1_F2_SONG_POSITION						50302
#define IDS_MESSAGE_DESC_MT1_F3_SONG_SELECT							50303
#define IDS_MESSAGE_DESC_MT1_F4_RESERVED							50304
#define IDS_MESSAGE_DESC_MT1_F5_RESERVED							50305
#define IDS_MESSAGE_DESC_MT1_F6_TUNE_REQUEST						50306
#define IDS_MESSAGE_DESC_MT1_F7_RESERVED							50307
#define IDS_MESSAGE_DESC_MT1_F8_TIMING_CLOCK						50308
#define IDS_MESSAGE_DESC_MT1_F9_RESERVED							50309
#define IDS_MESSAGE_DESC_MT1_FA_START								50310
#define IDS_MESSAGE_DESC_MT1_FB_CONTINUE							50311
#define IDS_MESSAGE_DESC_MT1_FC_STOP								50312
#define IDS_MESSAGE_DESC_MT1_FD_RESERVED							50313
#define IDS_MESSAGE_DESC_MT1_FE_ACTIVE_SENSE						50314
#define IDS_MESSAGE_DESC_MT1_FF_RESET								50315
#define IDS_MESSAGE_DESC_MT1_UNKNOWN								50316

// Message Type 2 - MIDI 1.0 Channel Voice

#define IDS_MESSAGE_DESC_MT2_8_NOTE_OFF								50400
#define IDS_MESSAGE_DESC_MT2_9_NOTE_ON								50401
#define IDS_MESSAGE_DESC_MT2_A_POLY_PRESSURE						50402
#define IDS_MESSAGE_DESC_MT2_B_CONTROL_CHANGE						50403
#define IDS_MESSAGE_DESC_MT2_C_PROGRAM_CHANGE						50404
#define IDS_MESSAGE_DESC_MT2_D_CHANNEL_PRESSURE						50405
#define IDS_MESSAGE_DESC_MT2_E_PITCH_BEND							50406
#define IDS_MESSAGE_DESC_MT2_UNKNOWN								50407

// Message Type 3 - Data Messages

#define IDS_MESSAGE_DESC_MT3_0_SYSEX7_COMPLETE						50500
#define IDS_MESSAGE_DESC_MT3_1_SYSEX7_START							50501
#define IDS_MESSAGE_DESC_MT3_2_SYSEX7_CONTINUE						50502
#define IDS_MESSAGE_DESC_MT3_3_SYSEX7_END							50503
#define IDS_MESSAGE_DESC_MT3_DATA64_UNKNOWN							50504

// Message Type 4 - MIDI 2.0 Channel Voice

#define IDS_MESSAGE_DESC_MT4_0_RPNC									50600
#define IDS_MESSAGE_DESC_MT4_1_APNC									50601
#define IDS_MESSAGE_DESC_MT4_2_RC									50602
#define IDS_MESSAGE_DESC_MT4_3_AC									50603
#define IDS_MESSAGE_DESC_MT4_4_REL_RC								50604
#define IDS_MESSAGE_DESC_MT4_5_REL_AC								50605
#define IDS_MESSAGE_DESC_MT4_6_PER_NOTE_BEND						50606
#define IDS_MESSAGE_DESC_MT4_8_NOTE_OFF								50607
#define IDS_MESSAGE_DESC_MT4_9_NOTE_ON								50608
#define IDS_MESSAGE_DESC_MT4_A_POLY_PRESSURE						50609
#define IDS_MESSAGE_DESC_MT4_B_CONTROL_CHANGE						50610
#define IDS_MESSAGE_DESC_MT4_C_PROGRAM_CHANGE						50611
#define IDS_MESSAGE_DESC_MT4_D_CHANNEL_PRESSURE						50612
#define IDS_MESSAGE_DESC_MT4_E_PITCH_BEND							50613
#define IDS_MESSAGE_DESC_MT4_MIDI2_CV_UNKNOWN						50614

// Message Type 5 - Data Messages

#define IDS_MESSAGE_DESC_MT5_0_SYSEX8_COMPLETE						50700
#define IDS_MESSAGE_DESC_MT5_1_SYSEX8_START							50701
#define IDS_MESSAGE_DESC_MT5_2_SYSEX8_CONTINUE						50702
#define IDS_MESSAGE_DESC_MT5_3_SYSEX8_END							50703
#define IDS_MESSAGE_DESC_MT5_8_MIXED_DATA_HEADER					50704
#define IDS_MESSAGE_DESC_MT5_9_MIXED_DATA_PAYLOAD					50705
#define IDS_MESSAGE_DESC_MT5_UNKNOWN								50706

// Message Type 6 - Unallocated

#define IDS_MESSAGE_DESC_MT6_RESERVED								50800

// Message Type 7 - Unallocated

#define IDS_MESSAGE_DESC_MT7_RESERVED								50900

// Message Type 8 - Unallocated

#define IDS_MESSAGE_DESC_MT8_RESERVED								51000

// Message Type 9 - Unallocated

#define IDS_MESSAGE_DESC_MT9_RESERVED								51100

// Message Type A - Unallocated

#define IDS_MESSAGE_DESC_MTA_RESERVED								51200

// Message Type B - Unallocated

#define IDS_MESSAGE_DESC_MTB_RESERVED								51300

// Message Type C - Unallocated

#define IDS_MESSAGE_DESC_MTC_RESERVED								51400

// Message Type D - Flex Data

#define IDS_MESSAGE_DESC_MTD_01_00_METADATA_TEXT					51500
#define IDS_MESSAGE_DESC_MTD_01_01_PROJECT_NAME						51501
#define IDS_MESSAGE_DESC_MTD_01_02_SONG_NAME						51502
#define IDS_MESSAGE_DESC_MTD_01_03_CLIP_NAME						51503
#define IDS_MESSAGE_DESC_MTD_01_04_COPYRIGHT						51504
#define IDS_MESSAGE_DESC_MTD_01_05_COMPOSER_NAME					51505
#define IDS_MESSAGE_DESC_MTD_01_06_LYRICIST_NAME					51506
#define IDS_MESSAGE_DESC_MTD_01_07_ARRANGER_NAME					51507
#define IDS_MESSAGE_DESC_MTD_01_08_PUBLISHER_NAME					51508
#define IDS_MESSAGE_DESC_MTD_01_09_PRIMARY_PERFORMER_NAME			51509
#define IDS_MESSAGE_DESC_MTD_01_0A_ACCOMPANY_PERFORMER_NAME			51510
#define IDS_MESSAGE_DESC_MTD_01_0B_RECORDING_DATE					51511
#define IDS_MESSAGE_DESC_MTD_01_0C_RECORDING_LOCATION				51512
#define IDS_MESSAGE_DESC_MTD_01_UNKNOWN								51513

#define IDS_MESSAGE_DESC_MTD_02_00_PERF_TEXT_EVENT					51600
#define IDS_MESSAGE_DESC_MTD_02_01_LYRICS							51601
#define IDS_MESSAGE_DESC_MTD_02_02_LYRICS_LANGUAGE					51602
#define IDS_MESSAGE_DESC_MTD_02_03_RUBY								51603
#define IDS_MESSAGE_DESC_MTD_02_04_RUBY_LANGUAGE					51604
#define IDS_MESSAGE_DESC_MTD_02_UNKNOWN								51605

#define IDS_MESSAGE_DESC_MTD_UNKNOWN								51700

// Message Type E - Unallocated

#define IDS_MESSAGE_DESC_MTE_RESERVED								51800

// Message Type F - Stream Messages

#define IDS_MESSAGE_DESC_MTF_00_DISCOVERY							51900
#define IDS_MESSAGE_DESC_MTF_01_ENDPOINT_INFO						51901
#define IDS_MESSAGE_DESC_MTF_02_DEVICE_IDENTITY						51902
#define IDS_MESSAGE_DESC_MTF_03_ENDPOINT_NAME						51903
#define IDS_MESSAGE_DESC_MTF_04_PRODUCT_INSTANCE_ID					51904
#define IDS_MESSAGE_DESC_MTF_05_CONFIG_REQUEST						51905
#define IDS_MESSAGE_DESC_MTF_06_CONFIG_NOTIFICATION					51906
#define IDS_MESSAGE_DESC_MTF_11_FUNCTION_BLOCK_INFO					51907
#define IDS_MESSAGE_DESC_MTF_12_FUNCTION_BLOCK_NAME					51908
#define IDS_MESSAGE_DESC_MTF_UNKNOWN								51909



// Errors
