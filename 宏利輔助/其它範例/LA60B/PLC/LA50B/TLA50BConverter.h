//---------------------------------------------------------------------------

#ifndef TLA50BConverterH
#define TLA50BConverterH
//---------------------------------------------------------------------------
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <ExtCtrls.hpp>
#include "TLA50BCPU.h"
#include "PLCParamDef.h"
#include <inifiles.hpp>"
//---------------------------------------------------------------------------

#define MAX_CPU_COUNT                   8

//---------------------------------------------------------------------------
class TLA50BConverter
{
private:
	TComponent *MyOwner;

    TList *CPUs;
    bool __fastcall DeleteCPU(int Index);
    TLA50BCPU* __fastcall GetItem(int Index);
    int __fastcall GetCPUCount(void);

	int LastCommCPUIdx;

	TPLCBasic* __fastcall GetCommCPU(void);

public:
    __fastcall TLA50BConverter(TComponent *Owner);
    __fastcall ~TLA50BConverter();

	bool __fastcall LoadParameter(String ParamPath);
	bool __fastcall AddCPU(unsigned char ID);
	bool __fastcall DeleteCPU(unsigned char ID);
	bool __fastcall DefineDIO(unsigned char ID,int Start,int Length);
	bool __fastcall DefineAIO(unsigned char ID,int Start,int Length);
	bool __fastcall DefineCounter(unsigned char ID,int Start,int Length);

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

	__property TLA50BCPU* Items[int Index]	= { read = GetItem  	};
	__property TPLCBasic* CommCPU			= { read = GetCommCPU	};

	int __fastcall IndexOfCPU(unsigned char ID);
	int __fastcall CPUIdxOfDIO(int IONumber);
	int __fastcall CPUIdxOfDIO(String Code);
	int __fastcall CPUIdxOfAIO(int IONumber);
	int __fastcall CPUIdxOfAIO(String Code);
	int __fastcall CPUIdxOfCounter(int IONumber);
	int __fastcall CPUIdxOfCounter(String Code);

	__property int CPUCount = { read = GetCPUCount };

	TPLCState __fastcall GetCPUState(unsigned char CPU_ID);
	bool __fastcall SetCPUState(unsigned char CPU_ID,TPLCState State);

	bool __fastcall GetDI(int Number);
	bool __fastcall GetDI(String Code);
	bool __fastcall GetDO(int Number);
	bool __fastcall GetDO(String Code);

	bool __fastcall OH(int Number);
	bool __fastcall OH(String Code);
	bool __fastcall OL(int Number);
	bool __fastcall OL(String Code);

	// for Diagnostic, normally these not used.
	bool __fastcall SetDI(int Number,bool High);
	bool __fastcall SetDI(String Code,bool High);
	bool __fastcall GetDI(unsigned char CPU_ID,int Module,int Point);
	bool __fastcall GetDO(unsigned char CPU_ID,int Module,int Point);
	bool __fastcall OH(unsigned char CPU_ID,int Module,int Point);
	bool __fastcall OL(unsigned char CPU_ID,int Module,int Point);
	WORD __fastcall GetAI(int Number);
	WORD __fastcall GetAI(String Code);
	WORD __fastcall GetAO(int Number);
	WORD __fastcall GetAO(String Code);
	bool __fastcall SetAO(int Number,WORD Value);
	bool __fastcall SetAO(String Code,WORD Value);
	bool __fastcall SetAI(int Number,WORD Value);
	bool __fastcall SetAI(String Code,WORD Value);
	WORD __fastcall GetAI(unsigned char CPU_ID,int Module,int Point);
	WORD __fastcall GetAO(unsigned char CPU_ID,int Module,int Point);
	bool __fastcall SetAO(unsigned char CPU_ID,int Module,int Point,WORD Value);
	bool __fastcall SetCounter(int Number,WORD Value);
	bool __fastcall SetCounter(String Code,WORD Value);
	WORD __fastcall GetCounter(unsigned char CPU_ID,int Module,int Point);
	WORD __fastcall GetCounter(int Number);
	WORD __fastcall GetCounter(String Code);
	double __fastcall GetTemperature(int Number);
	double __fastcall GetTemperature(String Code);
	double __fastcall GetTemperature(unsigned char CPU_ID,int Module,int Point);
	double __fastcall GetTemperature(TLA50BIO *IO);

	bool __fastcall CheckWtAvailable(unsigned char CPU_ID);
	double __fastcall ScaleWeight(unsigned char CPU_ID);
	void __fastcall ScaleZero(unsigned char CPU_ID);
	void __fastcall ScaleTare(unsigned char CPU_ID);

	void __fastcall OLAll(void);

	int __fastcall TxNextData(char *Data);
	int __fastcall ReGetTxData(char *Data);
	RxState __fastcall CheckRxData(unsigned char *Data,int Length);

	bool __fastcall DefineIOCode(ioTYPE Type,int Number,String Code);
	bool __fastcall LinkIO(TLA50BIO *IO);
};
//---------------------------------------------------------------------------
#endif
