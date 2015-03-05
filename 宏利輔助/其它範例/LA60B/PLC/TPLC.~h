//---------------------------------------------------------------------------

#ifndef TPLCH
#define TPLCH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <ExtCtrls.hpp>

#include "TLA60BConverter.h"
#include "TLA50BConverter.h"
#include <inifiles.hpp>"
#include "TScaleComm.h"
//---------------------------------------------------------------------------
class TPLC
{
private:
	TComponent *MyOwner;

	plcTYPE pType;
	bool __fastcall SetPLCType(plcTYPE PLC_Type,String ParamPath);

	TLA50BConverter *LA50Bs;
	TLA60BConverter *LA60Bs;
	TScaleComm *Scale;

	void __fastcall ClearPLC(void);
	int __fastcall CountAllCPU(void);

	TPLCBasic* __fastcall GetCommCPU(void);

protected:

	int __fastcall NextTxData(char *Data);
	int __fastcall ReGetTxData(char *Data);
	RxState __fastcall CheckRxData(char *Data,int Length);

public:
	__fastcall TPLC(TComponent *Owner);
	__fastcall ~TPLC();

	bool __fastcall LoadParameter(String ParamPath);
	__property plcTYPE Type			= { read = pType		};
	__property TPLCBasic* CommCPU	= { read = GetCommCPU	};

	TPLCState __fastcall GetPLCState(unsigned char CPU_ID);
	bool __fastcall SetPLCState(unsigned char CPU_ID,TPLCState State);

	bool __fastcall GetDI(int Number);
	bool __fastcall GetDI(String Code);
	bool __fastcall GetDO(int Number);
	bool __fastcall GetDO(String Code);

	bool __fastcall OH(int Number);
	bool __fastcall OH(String Code);
	bool __fastcall OL(int Number);
	bool __fastcall OL(String Code);

	WORD __fastcall GetAI(int Number);
	WORD __fastcall GetAI(String Code);
	WORD __fastcall GetAO(int Number);
	WORD __fastcall GetAO(String Code);

	bool __fastcall SetAO(int Number,WORD Value);
	bool __fastcall SetAO(String Code,WORD Value);
	bool __fastcall SetAO(int Number,double Percent);
	bool __fastcall SetAO(String Code,double Percent);

	WORD __fastcall GetCounter(int Number);
	WORD __fastcall GetCounter(String Code);
	double __fastcall GetTemperature(int Number);
	double __fastcall GetTemperature(String Code);
	double __fastcall GetTemperature(TCommonIO *IO);

	// for Diagnostic, normally these not used.
	bool __fastcall SetDI(int Number,bool High);
	bool __fastcall SetDI(String Code,bool High);
	bool __fastcall SetAI(int Number,WORD Value);
	bool __fastcall SetAI(String Code,WORD Value);
	bool __fastcall SetCounter(int Number,WORD Value);
	bool __fastcall SetCounter(String Code,WORD Value);

	bool __fastcall CheckWtAvailable(unsigned char CPU_ID);
	double __fastcall ScaleWeight(unsigned char CPU_ID);
//	String __fastcall ScaleUnit(unsigned char CPU_ID);
	void __fastcall ScaleZero(unsigned char CPU_ID);
	void __fastcall ScaleTare(unsigned char CPU_ID);

	// Communication Methods
	__property int TotalCPUs	= { read=CountAllCPU };

	void __fastcall OLAll(void);
	bool __fastcall DefineIOCode(ioTYPE Type,int Number,String Code);
	bool __fastcall LinkIO(TBasicIO *IO);
};
//---------------------------------------------------------------------------
#endif

