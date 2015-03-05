//---------------------------------------------------------------------------


#pragma hdrstop

#include "LA60BRegistry.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)
//---------------------------------------------------------------------------
__fastcall TLA60BRegistry::TLA60BRegistry(LA60BRegType Type,int StartNo,int Count)
{
	pRegType	= Type;
	pRegStartNo	= StartNo;
	pRegCount	= Count;
	if( StartNo >= 0 && Count > 0 )
	{
		Data	= new WORD[pRegCount];
		if( Data )
			memset( Data, 0, Count * sizeof(WORD) );
	}
	else
	{
		Data	= NULL;
	}
}
//---------------------------------------------------------------------------
__fastcall TLA60BRegistry::~TLA60BRegistry()
{
	if( Data )
	{
		delete []Data;
		Data	= NULL;
    }
}
//---------------------------------------------------------------------------
int __fastcall TLA60BRegistry::GetDataIndex(int RegNo)
{
	int idx;

	idx	= RegNo - pRegStartNo;
	if( idx < 0 || idx >= pRegCount )		// out of range
		return -1;
	return idx;
}
//---------------------------------------------------------------------------
bool __fastcall TLA60BRegistry::SetRegValue(int RegNo,WORD Value)
{
	int idx;

	idx	= GetDataIndex( RegNo );
	if( idx < 0 )
		return false;
	Data[idx]	= Value;
	return true;
}
//---------------------------------------------------------------------------
WORD __fastcall TLA60BRegistry::GetRegValue(int RegNo)
{
	int idx;

	idx	= GetDataIndex( RegNo );
	if( idx < 0 )
		return 0;
	return Data[idx];
}
//---------------------------------------------------------------------------
bool __fastcall TLA60BRegistry::RegExists(LA60BRegType Type,int RegNo)
{
	if( Type != pRegType )
		return false;

	if( GetDataIndex(RegNo) < 0 )
		return false;
	return true;
}
//---------------------------------------------------------------------------

