//---------------------------------------------------------------------------

#ifndef TPLCBasicH
#define TPLCBasicH
//---------------------------------------------------------------------------
#include <Classes.hpp>

#include "PLCDefine.h"
//---------------------------------------------------------------------------
class TPLCBasic
{
private:
	TComponent *MyOwner;

	plcTYPE pPLCType;
	unsigned char pCPU_ID;

public:
	__fastcall TPLCBasic(TComponent *Owner,plcTYPE Type,unsigned char cpu_id);
	__fastcall ~TPLCBasic();

	__property plcTYPE PLC			=	{ read=pPLCType	};
	__property unsigned char CPU_ID	=	{ read=pCPU_ID 	};
	TPLCState State;
	int TimeoutCount;
	int ErrorCount;
};
//---------------------------------------------------------------------------
#endif
