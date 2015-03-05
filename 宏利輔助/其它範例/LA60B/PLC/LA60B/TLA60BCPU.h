//---------------------------------------------------------------------------

#ifndef TLA60BCPUH
#define TLA60BCPUH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <ExtCtrls.hpp>

#include "TPLCBasic.h"
#include "TLA60BIO.h"
#include "LA60BUtitily.h"
#include "TLA60BCmd.h"
#include "LA60BRegistry.h"
#include "CommonUtility.h"
//---------------------------------------------------------------------------

#define CPU_ID_START_NUMBER			1


#define MAX_RW_BYTES				32

#define LA60B_LEADER_SYMBOL			0x02
#define LA60B_END_SYMBOL			0x03

#define PLC_ONLINE_CONFIRM_TIME		1000	// ms

//---------------------------------------------------------------------------
class TLA60BCPU : public TPLCBasic
{
private:
	TComponent *MyOwner;

	TList *Regs;
	bool __fastcall DelReg(int idx);
	TLA60BRegistry* __fastcall GetRegistry(LA60BRegType Type,int RegNo);
	bool __fastcall AddRegistry(LA60BRegType Type,int StartNumber,int Count);
	WORD __fastcall GetRegistryValue(LA60BRegType Type,int RegNumber);
	bool __fastcall SetRegistryValue(LA60BRegType Type,int RegNumber,WORD Value);

	TList *DIList;
	TList *DOList;
	TList *AIList;
	TList *AOList;
	TList* __fastcall GetIOList(ioTYPE Type);
	bool __fastcall AddIOs(ioTYPE IOType,int StartNumber,LA60BRegType RegType,int Addr,int Count);
	int __fastcall NumberOfIO(ioTYPE IOType,LA60BRegType RegType,int Addr);
	TLA60BIO* __fastcall IOOfNumber(ioTYPE Type,int IONumber);
	TLA60BIO* __fastcall IOOfNumber(TList *IOs,int IONumber);
	TLA60BIO* __fastcall IOOfCode(ioTYPE Type,String Code);
	TLA60BIO* __fastcall IOOfCode(TList *IOs,String Code);
	bool __fastcall DeleteIO(TList *IOs,int Index);

	int __fastcall InitTxBuf(char *Data);
	int __fastcall InsCmdData(char *Data,int InsPos,unsigned char Cmd,LA60BRegType RegType,int Addr,unsigned char Length);
	int __fastcall InsRegsData(char *Data,int InsPos,unsigned char Cmd,LA60BRegType RegType,int Addr,unsigned char Length);
	int __fastcall FinishTxBuf(char *Data,int InsPos);
	bool __fastcall IsRxFinish(char *Data,int Length);
	RxState __fastcall SaveRxData(char *RxData,int DataPos);

	TList *CommCmds;
	int CmdIndex;
	TLA60BCmd* __fastcall GetCmd(int Index);
	__property TLA60BCmd* CmdItems[int Index]	= { read = GetCmd };
	bool __fastcall DelCmd(int idx);

	TLA60BCmd *TxCmd;
	TLA60BCmd *OnLineKeepCmd;
	unsigned int OnLineCheckTick;
	unsigned int OnLineCheckInterval;

	bool __fastcall CheckNeedKeepOnLine(void);
	bool __fastcall SetOnLineValue(void);
	bool __fastcall PopCommCommand(void);

	WORD __fastcall GetIOValue(TBasicIO *IO);
	void __fastcall SetIOValue(TBasicIO *IO,WORD Value);

public:
	__fastcall TLA60BCPU(TComponent* Owner,unsigned char cpu_id);
	__fastcall ~TLA60BCPU();

	bool __fastcall DefineIONumber(ioTYPE IOType,int StartNumber,LA60BRegType RegType,int Addr,int Count);
	bool __fastcall DefineIOCode(ioTYPE Type,int IONumber,String Code);

	bool __fastcall IsIOExists(ioTYPE Type,int Number);
	bool __fastcall IsIOExists(ioTYPE Type,String Code);

	bool __fastcall GetDIO(ioTYPE Type,int Number);
	bool __fastcall GetDIO(ioTYPE Type,String Code);
	bool __fastcall SetDIO(ioTYPE Type,int Number,bool High);
	bool __fastcall SetDIO(ioTYPE Type,String Code,bool High);

	bool __fastcall SetAIO(ioTYPE Type,int Number,double Percent);
	bool __fastcall SetAIO(ioTYPE Type,int Number,WORD Value);
	bool __fastcall SetAIO(ioTYPE Type,String Code,double Percent);
	bool __fastcall SetAIO(ioTYPE Type,String Code,WORD Value);

	WORD __fastcall GetAIO(ioTYPE Type,int Number);
	WORD __fastcall GetAIO(ioTYPE Type,String Code);

	void __fastcall OLAll(void);

	bool __fastcall AddCommCmd(String Param);
	bool __fastcall AddCommCmd(TLA60BCmd *Command);
	int __fastcall NextCmdData(char *Data);
	int __fastcall GetTxData(char *Data,TLA60BCmd *Cmd);

	RxState __fastcall ProcessRxData(unsigned char *RxData,int Length);
	int __fastcall GetCommandCount(void);
	TLA60BCmd* __fastcall GetCommand(int Index);

	void __fastcall SetKeepOnLineReg(LA60BRegType Type,int RegNumber,unsigned int CheckInterval);

	__property bool IsNeedKeepOnLine	= { read = CheckNeedKeepOnLine };

	bool __fastcall LinkIO(TLA60BIO *IO);
};
//---------------------------------------------------------------------------
#endif
