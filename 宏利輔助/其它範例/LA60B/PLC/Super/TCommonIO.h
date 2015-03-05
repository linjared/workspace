//---------------------------------------------------------------------------

#ifndef TCommonIOH
#define TCommonIOH
//---------------------------------------------------------------------------
#include "TBasicIO.h"
//---------------------------------------------------------------------------

typedef WORD __fastcall PACKAGE (__closure *TGetValue)(TBasicIO *IO);
typedef void __fastcall PACKAGE (__closure *TSetValue)(TBasicIO *IO,WORD Value);

//---------------------------------------------------------------------------
class TCommonIO : public TBasicIO
{
private:

	WORD __fastcall GetValue(void);
	void __fastcall SetValue(WORD NewValue);

public:
	__fastcall TCommonIO(plcTYPE Type);
	virtual __fastcall ~TCommonIO();

	TGetValue OnGetValue;
	TSetValue OnSetValue;
	
	__property WORD Value	= { read = GetValue	, write = SetValue	};
};
//---------------------------------------------------------------------------
#endif
