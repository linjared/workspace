//---------------------------------------------------------------------------

#ifndef LA60BRegistryH
#define LA60BRegistryH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "PLCDefine.h"
//---------------------------------------------------------------------------
class TLA60BRegistry
{
private:
	LA60BRegType pRegType;
	WORD *Data;
	int pRegStartNo;
	int pRegCount;

	int __fastcall GetDataIndex(int RegNo);

public:
	__fastcall TLA60BRegistry(LA60BRegType Type,int StartNo,int Count);
	__fastcall ~TLA60BRegistry();

	__property LA60BRegType RegType	= { read=pRegType 		};
	__property int RegStartNo		= { read=pRegStartNo 	};
	__property int RegCount			= { read=pRegCount 		};

	bool __fastcall SetRegValue(int RegNo,WORD Value);
	WORD __fastcall GetRegValue(int RegNo);
	bool __fastcall RegExists(LA60BRegType Type,int RegNo);

};
//---------------------------------------------------------------------------
#endif
