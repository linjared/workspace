//---------------------------------------------------------------------------


#pragma hdrstop

#include "TBasicIO.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)
//---------------------------------------------------------------------------
__fastcall TBasicIO::TBasicIO(plcTYPE Type)
{
	pPLCType	= Type;
	CPU_ID		= 0;
	IONumber	= -1;
	Code		= "";
}
//---------------------------------------------------------------------------
__fastcall TBasicIO::~TBasicIO()
{
}
//---------------------------------------------------------------------------
