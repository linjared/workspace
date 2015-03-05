//---------------------------------------------------------------------------

#ifndef TBasicIOH
#define TBasicIOH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "PLCDefine.h"
//---------------------------------------------------------------------------
class TBasicIO
{
private:

	plcTYPE pPLCType;

public:
	__fastcall TBasicIO(plcTYPE Type);
	virtual __fastcall ~TBasicIO();

	__property plcTYPE PLCType	= { read = pPLCType };
	unsigned char CPU_ID;
	int IONumber;
	String Code;

	virtual void f(){};

};
//---------------------------------------------------------------------------
#endif
