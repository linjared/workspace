//---------------------------------------------------------------------------

#ifndef TScaleCommH
#define TScaleCommH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "TPLCBasic.h"
#include "CommonUtility.h"
#include <inifiles.hpp>
//---------------------------------------------------------------------------

enum sclType
{
	mdlMETTLER
	,mdlLOADCELL
};


//---------------------------------------------------------------------------
class TScaleComm : public TPLCBasic
{
//#define SCALE_DATA_STR				0x02
#define SCALE_DATA_CR				0x0d
#define SCALE_DATA_LF				0x0a
#define SCALE_BUF_LENGTH			32
#define SCALE_COMMAND_BYTES			16

private:
	TComponent *MyOwner;

	double pLastWeight;
	String pLastUnit;
	char TareCommand[SCALE_COMMAND_BYTES];
	bool IsTare;
	char ZeroCommand[SCALE_COMMAND_BYTES];
	bool IsZero;
	bool __fastcall CheckWtAvailable(void);
	unsigned int CommTick;

	bool __fastcall IsRxFinish(char *Data,int Length);
	bool __fastcall CheckData(char *Data,int Length);
	void __fastcall PickValue(char *Data,int Length);
	void __fastcall MettlerPickValue(char *Data,int Length);
	void __fastcall LoadcellPickValue(char *Data,int Length);

	sclType pScaleType;

public:
	__fastcall TScaleComm(TComponent *Owner);
	__fastcall ~TScaleComm();

	__property sclType ScaleType	= { read = pScaleType };
	bool __fastcall LoadParameter(String FilePath);
	RxState __fastcall CheckRxData(char *Data,int Length);
	int __fastcall TxNextCmdData(char *Data);

	__property double Weight			= { read = pLastWeight		};
	__property String Unit				= { read = pLastUnit		};
	__property bool IsWeightAvailable	= { read = CheckWtAvailable	};
	unsigned int TimeoutMS;

	void __fastcall Zero(void);
	void __fastcall Tare(void);

	
};
//---------------------------------------------------------------------------
#endif
