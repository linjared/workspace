//---------------------------------------------------------------------------

#ifndef TLA60BConverterH
#define TLA60BConverterH
//---------------------------------------------------------------------------
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <ExtCtrls.hpp>
#include "TLA60BCPU.h"
#include "TLA60BCmd.h"
#include <inifiles.hpp>
//---------------------------------------------------------------------------
class TLA60BConverter
{
private:
	TComponent *MyOwner;
	bool __fastcall CreateComponent(void);
	void __fastcall DeleteComponent(void);

	plcTYPE pPLCType;
	TList *CPUs;
	int __fastcall GetCPUCount(void);
	bool __fastcall DeleteByIndex(int Index);

	TLA60BCPU* __fastcall GetItem(int Index);
	TLA60BCPU* __fastcall FindCPU(unsigned char CPU_ID);
	TLA60BCPU* __fastcall FindCPU(ioTYPE Type,int IONumber);
	TLA60BCPU* __fastcall FindCPU(ioTYPE Type,String Code);
	TPLCBasic* __fastcall GetCommCPU(void);

	int LastCommCPUIdx;

	bool __fastcall DefineIONumber(unsigned char CPU_ID,ioTYPE IOType,int StartNumber
			,LA60BRegType RegType,int Addr,int Count);
	bool __fastcall AddCommCommand(unsigned char CPU_ID,String CmdParam);

public:
	__fastcall TLA60BConverter(TComponent *Owner);
	__fastcall ~TLA60BConverter();

	bool __fastcall LoadParameter(String ParamPath);

	__property plcTYPE PLC	= { read=pPLCType };

	__property TPLCBasic* CommCPU			= { read = GetCommCPU	};
	__property TLA60BCPU* Items[int Index]	= { read = GetItem		};
	__property int CPUCount	= { read=GetCPUCount };
	int __fastcall IndexOfCPU(unsigned char CPU_ID);
	bool __fastcall AddCPU(unsigned char New_ID);
	bool __fastcall DeleteCPU(unsigned char Del_ID);

	TPLCState __fastcall GetCPUState(unsigned char CPU_ID);
	bool __fastcall SetCPUState(unsigned char CPU_ID,TPLCState State);


	/*
	bool __fastcall AddCPUCommCmd(TLA60BCmd *Cmd);
	int __fastcall GetPLCCommandCount(void);
	TLA60BCmd* __fastcall GetCommand(int Index);
	*/

	int __fastcall TxNextCmdData(char *Data);
	int __fastcall ReGetTxCmdData(char *Data);
	RxState __fastcall CheckRxData(char *Data,int Length);
	void __fastcall SetKeepOnLineReg(int CPU_ID,LA60BRegType RegType,int RegNumber,unsigned int CheckInterval);


	bool __fastcall GetDIO(ioTYPE Type,int Number);
	bool __fastcall GetDIO(ioTYPE Type,String Code);
	bool __fastcall OL(int Number);
	bool __fastcall OL(String Code);
	bool __fastcall OH(int Number);
	bool __fastcall OH(String Code);
	WORD __fastcall GetAI(int Number);
	WORD __fastcall GetAI(String Code);
	WORD __fastcall GetAO(int Number);
	WORD __fastcall GetAO(String Code);
	bool __fastcall SetAO(int Number,WORD Value);
	bool __fastcall SetAO(int Number,double Percent);
	bool __fastcall SetAO(String Code,WORD Value);
	bool __fastcall SetAO(String Code,double Percent);
	double __fastcall GetTemperature(int AINumber);
	double __fastcall GetTemperature(String Code);
	double __fastcall GetTemperature(TLA60BIO *IO);

	// for disgnostic
	bool __fastcall SetDI(int Number,bool IsHigh);
	bool __fastcall SetDI(String Code,bool IsHigh);
	bool __fastcall SetAI(int Number,WORD Value);
	bool __fastcall SetAI(int Number,double Percent);
	bool __fastcall SetAI(String Code,WORD Value);
	bool __fastcall SetAI(String Code,double Percent);

	void __fastcall OLAll(void);
	bool __fastcall DefineIOCode(ioTYPE Type,int Number,String Code);
	bool __fastcall LinkIO(TLA60BIO *IO);
};
//---------------------------------------------------------------------------
#endif
