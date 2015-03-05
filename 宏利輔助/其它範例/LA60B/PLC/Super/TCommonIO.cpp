//---------------------------------------------------------------------------


#pragma hdrstop

#include "TCommonIO.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
__fastcall TCommonIO::TCommonIO(plcTYPE Type)
	: TBasicIO( Type )
{
	OnGetValue	= NULL;
	OnSetValue	= NULL;
}
//---------------------------------------------------------------------------
__fastcall TCommonIO::~TCommonIO()
{
}
//---------------------------------------------------------------------------
WORD __fastcall TCommonIO::GetValue(void)
{
	if( !OnGetValue )
		return 0;
	//return OnGetValue( dynamic_cast<TBasicIO*>( this ) );
	return OnGetValue( (TBasicIO*)( this ) );
}
//---------------------------------------------------------------------------
void __fastcall TCommonIO::SetValue(WORD NewValue)
{
	if( !OnSetValue )
		return;
	//OnSetValue( dynamic_cast<TBasicIO*>( this ), NewValue );
	OnSetValue( (TBasicIO*)( this ), NewValue );
}
//---------------------------------------------------------------------------
