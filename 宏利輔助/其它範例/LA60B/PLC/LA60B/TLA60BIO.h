//---------------------------------------------------------------------------

#ifndef TLA60BIOH
#define TLA60BIOH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "TCommonIO.h"
//---------------------------------------------------------------------------
class TLA60BIO : public TCommonIO
{
private:

	ioTYPE IOType;
	bool __fastcall GetIOAvailable(void);

public:
	__fastcall TLA60BIO(ioTYPE Type);
	__fastcall ~TLA60BIO();

	LA60BRegType RegType;
	int RegAddr;

	__property ioTYPE Type		= { read = IOType         };
	__property bool IsAvailable	= { read = GetIOAvailable };

};
//---------------------------------------------------------------------------
#endif
