//---------------------------------------------------------------------------

#ifndef PLCDefineH
#define PLCDefineH
//---------------------------------------------------------------------------
#include <ADODB.hpp>
//---------------------------------------------------------------------------

//
//	Common Definition
//
enum plcTYPE
{
	UNKNOW_PLC	= 0
	,LA60B
	,LA50B
	,SCALE
};

#define LA50B_STR					"LA50B"
#define LA60B_STR					"LA60B"
#define SCALE_STR					"SCALE"

plcTYPE StringToPLCType(String PLCStr);
String PLCTypeToString(plcTYPE Type);


enum ioTYPE
{
	ioUNKNOW	= 0
	,ioDI
	,ioDO
	,ioAI
	,ioAO
	,ioCOUNTER
//	,DATA
};

enum TPLCState
{
	PLC_OFF 	= 0
	,PLC_ONLINE
	,PLC_LENGTH_ERR
	,PLC_CHECKSUM_ERR
	,PLC_DATA_ERR
};


//
//	Serial Port Communition Definition
//
enum RxState		// Communication Acknowledge
{
	COMM_WAIT_RESPONSE = 0
	,COMM_LENGTH_ERR
	,COMM_DATA_ERR
	,COMM_CHECKSUM_ERR
	,COMM_RX_OK
};


//
//	LA-60B Definition
//
#define LA60B_MIN_CPU_ID				1
enum LA60BRegType
{
	REG_NO_ASSIGN = 0

	,REG_X
	,REG_Y
	,REG_M
	,REG_R
	,REG_D
};

char LA60BRegToSymbol(LA60BRegType Type);
LA60BRegType SymbolToLA60BReg(char Symbol);
LA60BRegType SymbolToLA60BReg(String SymbolStr);


// Parameter Field
#define SECT_MAIN						"Main"
#define FLD_PLC_TYPE					"PLCType"
#define FLD_CPU_COUNT					"CPUCount"
#define FLD_IS_ONLINE_CHECK				"IsOnlineCheck"
#define FLD_ONLINE_CHECK_REG			"OnLineCheckReg"
#define FLD_ONLINE_CHECK_ADDR           "OnLineCheckAddr"
#define FLD_ONLINE_CHECK_TIME			"OnLineCheckTime"

#define FLD_CPU_ID						"ID"
#define SECT_CPU_LEADER					"CPU"
#define SECT_DI_LEADER                  "DI"
#define SECT_DO_LEADER                  "DO"
#define SECT_AI_LEADER                  "AI"
#define SECT_AO_LEADER                  "AO"
#define SECT_COUNTER_LEADER				"COUNTER"
#define FLD_LA50B_DIO_COUNT				"DIOCount"
#define FLD_LA50B_AIO_COUNT				"AIOCount"

#define FLD_LA60B_DI_COUNT				"DICount"
#define FLD_LA60B_DI_REG				"DIReg"
#define FLD_LA60B_DO_COUNT				"DOCount"
#define FLD_LA60B_DO_REG				"DOReg"
#define FLD_LA60B_AI_COUNT				"AICount"
#define FLD_LA60B_AI_REG                "AIReg"
#define FLD_LA60B_AO_COUNT				"AOCount"
#define FLD_LA60B_AO_REG                "AOReg"
#define FLD_LA60B_DI_START				"DIStart"
#define FLD_LA60B_DO_START				"DOStart"
#define FLD_LA60B_AI_START				"AIStart"
#define FLD_LA60B_AO_START				"AOStart"
#define FLD_LA60B_DI_LENGTH				"DILength"
#define FLD_LA60B_DO_LENGTH				"DOLength"
#define FLD_LA60B_AI_LENGTH				"AILength"
#define FLD_LA60B_AO_LENGTH				"AOLength"

#define SECT_CMD						"Cmd"
#define FLD_COUNT						"Count"
#define FLD_CMD							"Cmd"

//---------------------------------------------------------------------------
#endif
