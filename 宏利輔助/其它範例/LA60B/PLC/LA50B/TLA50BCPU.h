//---------------------------------------------------------------------------

#ifndef TLA50BCPUH
#define TLA50BCPUH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "TPLCBasic.h"
#include "TLA50BIO.h"
//---------------------------------------------------------------------------

#define INPUT_BYTES                         16
#define OUTPUT_BYTES                        16
#define BALANCE_COMMAND_BYTES               16
#define BALANCE_STR_BYTES                   16
#define ANALOG_INPUT_BYTES                  16
#define ANALOG_OUTPUT_BYTES                 16
#define REGISTER_OUTPUT_BYTES               40
#define REGISTER_INPUT_BYTES                40
#define MACHINE_BYTES                       10
#define HEXCODE_BYTES                       16
#define COUNTER_BYTES                       16

#define LEADING_ID                          '@'
#define LENGTH_ID                           'L'
#define OUTPUT_ID                           'O'
#define INPUT_ID                            'I'
#define ANALOG_ID                           'A'
#define REGISTER_ID                         'R'
#define MACHINE_ID                          'M'
#define HEXCODE_ID                          'H'
#define COUNTER_ID                          'C'
#define BALANCE_COMMAND_ID                  'B'
#define CHECKSUM_ID                         'S'


#define MAX_DIGITAL_POINT                   128

//  Digital IO
#define DIO_MAX_LENGTH						DIO_MAX_MODULE * DIO_MODULE_POINTS

// AIO
#define AIO_MAX_POINTS						16
#define AIO_MAX_LENGTH						AIO_MAX_POINTS / 2		// 1 Analog consists of 2 points

// Counter
#define COUNTER_MAX_POINTS					16
#define COUNTER_MAX_LENGTH					COUNTER_MAX_POINTS / 2		// 1 Counter consits of 2 points



//  Temperature
enum
{
	TEMP_HIGH_BYTE
	,TEMP_LOW_BYTE
	,TEMP_BYTES
};


#pragma option -a1
struct PC_DATA_DETAIL   // 110 BYTES
{
        char leading_id;                                                //1
        unsigned char plc_id;                                           //1
        char length_id;                                                 //1
        unsigned char length;                                           //1
        char output_id;                                                 //1
		unsigned char output[OUTPUT_BYTES];                             //16 bytes
        char analog_id;                                                 //1
        unsigned char analog[ANALOG_OUTPUT_BYTES];                      //16 bytes
        char register_id;                                               //1
        unsigned short register_command[REGISTER_OUTPUT_BYTES/2];       //40 bytes
        char balance_command_id;                                        //1
        unsigned char balance_command[BALANCE_COMMAND_BYTES];           //16 bytes
        char machine_id;                                                //1
        unsigned char machine_command[MACHINE_BYTES];                   //10 bytes
        char checksum_id;                                               //1
        unsigned short checksum;                                        //2
};
struct PLC_DATA_DETAIL   //133
{
        char leading_id;
        unsigned char plc_id;
        char length_id;
        unsigned char length;
        char input_id;
        unsigned char input[INPUT_BYTES];
        char analog_id;
		unsigned char analog[ANALOG_INPUT_BYTES];
		char counter_id;
		unsigned char counter[COUNTER_BYTES];
        char register_id;
        unsigned char register_str[REGISTER_INPUT_BYTES];
        char hexcode_id;
        unsigned char hexcode[HEXCODE_BYTES];
        char balance_str_id;
        unsigned char balance_str[BALANCE_STR_BYTES];
        char checksum_id;
        unsigned short checksum;
};
#pragma option -a

//---------------------------------------------------------------------------
class TLA50BCPU : public TPLCBasic
{
private:
    TComponent *MyOwner;
	int DIOStartNumber;
	int DIOLength;
	int AIOStartNumber;
	int AIOLength;
	int CounterStartNumber;
	int CounterLength;

	struct PC_DATA_DETAIL  pc_2_plc_data_detail;
	struct PLC_DATA_DETAIL plc_2_pc_data_detail;
	AnsiString BalanceWeight;

	TList *DIList;
	TList *DOList;
	TList *AIList;
	TList *AOList;
	TList *CounterList;
	bool __fastcall DeleteIO(TList *IOList,int Index);
	bool __fastcall AddIOs(TList *IOList,ioTYPE Type,int StartNumber,int Count);
	int __fastcall NumberOfIOCode(TList *IOList,String Code);
	int __fastcall IndexOfIONumber(TList *IOList,int IONumber);

	bool __fastcall GetDIValue(int Index);
	bool __fastcall GetDOValue(int Index);
	bool __fastcall SetDOValue(int Index,bool IsHigh);
	bool __fastcall SetDIValue(int Index,bool IsHigh);
	WORD __fastcall GetAIValue(int Index);
	WORD __fastcall GetAOValue(int Index);
	bool __fastcall SetAOValue(int Index,WORD Value);
	bool __fastcall SetAIValue(int Index,WORD Value);
	WORD __fastcall GetCounterValue(int Index);
	bool __fastcall SetCounterValue(int Index,WORD Value);

	int __fastcall CalcAnalogIndex(int Module,int Point);
	int __fastcall CalcCounterIndex(int Module,int Point);
	double __fastcall CalcTemperatureValue(int PLCValue);

	int __fastcall GetRxLength(void);
	void __fastcall CalcuBalanceWeight(void);
	void __fastcall GetRxData(unsigned char *Data);
	__property int RxDataLength = { read = GetRxLength };

	WORD __fastcall GetIOValue(TBasicIO *IO);
	void __fastcall SetIOValue(TBasicIO *IO,WORD Value);

	String __fastcall GetIOCode(TLA50BIO *IO);
	bool __fastcall SetIOCode(TLA50BIO *IO);

public:
	__fastcall TLA50BCPU(TComponent* Owner,unsigned char cpu_id);
	__fastcall ~TLA50BCPU();

	bool __fastcall DefineDIONumber(int StartNumber,int Length);
	bool __fastcall DefineAIONumber(int StartNumber,int Length);
	bool __fastcall DefineCounterNumber(int StartNumber,int Length);
	bool __fastcall DefineDICode(int IONumber,String Code);
	bool __fastcall DefineDOCode(int IONumber,String Code);
	bool __fastcall DefineAICode(int IONumber,String Code);
	bool __fastcall DefineAOCode(int IONumber,String Code);
	bool __fastcall DefineCounterCode(int IONumber,String Code);

	bool __fastcall IsDIOExists(int Number);
	bool __fastcall IsDIOExists(String Code);
	bool __fastcall IsAIOExists(int Number);
	bool __fastcall IsAIOExists(String Code);
	bool __fastcall IsCounterExists(int Number);
	bool __fastcall IsCounterExists(String Code);

	bool __fastcall GetDI(int Number);
	bool __fastcall GetDI(String Code);
	bool __fastcall GetDI(int Module,int Point);
	bool __fastcall GetDO(int Number);
	bool __fastcall GetDO(String Code);
	bool __fastcall GetDO(int Module,int Point);

	bool __fastcall OH(int DIO,int Point);
	bool __fastcall OH(int Number);
	bool __fastcall OH(String Code);
	bool __fastcall OL(int DIO,int Point);
	bool __fastcall OL(int Number);
	bool __fastcall OL(String Code);

	// for Diagnostic, normally these not used.
	bool __fastcall SetDI(int Number,bool High);
	bool __fastcall SetDI(String Code,bool High);
	bool __fastcall SetAI(int Number,WORD Value);
	bool __fastcall SetAI(String Code,WORD Value);
	bool __fastcall SetCounter(int Number,WORD Value);
	bool __fastcall SetCounter(String Code,WORD Value);

	bool __fastcall SetAO(int Number,double Percent);
	bool __fastcall SetAO(int Number,WORD Value);
	bool __fastcall SetAO(String Code,double Percent);
	bool __fastcall SetAO(String Code,WORD Value);

	WORD __fastcall GetAI(int Number);
	WORD __fastcall GetAO(int Number);
	WORD __fastcall GetCounter(int Number);
	double __fastcall GetTemperature(int Number);
	WORD __fastcall GetAI(String Code);
	WORD __fastcall GetAO(String Code);
	WORD __fastcall GetCounter(String Code);
	double __fastcall GetTemperature(String Code);
	bool __fastcall SetAO(int Module,int Point,double Percent);
	bool __fastcall SetAO(int Module,int Point,WORD Value);
	WORD __fastcall GetAI(int Module,int Point);
	WORD __fastcall GetAO(int Module,int Point);
	WORD __fastcall GetCounter(int Module,int Point);
	double __fastcall GetTemperature(int Module,int Point);
	double __fastcall GetTemperature(TLA50BIO *IO);

	void __fastcall OLAll(void);

	bool BalanceWeightFlag;
	bool __fastcall ReadBalanceWeight(AnsiString &weight);  //Åª¨ú¤Ñ¥­­È
	void __fastcall TareBalanceT(void);
	void __fastcall TareBalanceZ(void);

	bool __fastcall DefineIOCode(ioTYPE Type,int Number,String Code);
	bool __fastcall LinkIO(TLA50BIO *IO);

	int TempPLCValue[TEMP_BYTES];
	double TempRealValue[TEMP_BYTES];

	int __fastcall TxData(char *Data);
	RxState __fastcall ProcessRxData(unsigned char *RxData,int Length);
};
//---------------------------------------------------------------------------
#endif
