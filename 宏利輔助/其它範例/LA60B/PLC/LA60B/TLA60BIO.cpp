//---------------------------------------------------------------------------


#pragma hdrstop

#include "TLA60BIO.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
__fastcall TLA60BIO::TLA60BIO(ioTYPE Type)
	: TCommonIO( LA60B )
{
	IOType		= Type;
	CPU_ID		= LA60B_MIN_CPU_ID;
	RegType		= REG_NO_ASSIGN;
	RegAddr		= -1;
}
//---------------------------------------------------------------------------
__fastcall TLA60BIO::~TLA60BIO()
{
}
//---------------------------------------------------------------------------
bool __fastcall TLA60BIO::GetIOAvailable(void)
{
	if( CPU_ID == 0 || RegType == REG_NO_ASSIGN || RegAddr < 0 )
		return false;
	return true;
}
//---------------------------------------------------------------------------

