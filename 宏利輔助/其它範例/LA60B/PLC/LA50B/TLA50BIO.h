//---------------------------------------------------------------------------

#ifndef TLA50BIOH
#define TLA50BIOH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "TCommonIO.h"
//---------------------------------------------------------------------------

//  Digital IO
#define DIO_MAX_MODULE						4
#define DIO_MODULE_POINTS					16
#define DIO_OUTPUT_POINTS                   16
#define DIO_INPUT_POINTS                    16

// AIO
#define AIO_MAX_MODULE						2
#define AIO_MODULE_REAL_POINTS				8
#define AIO_MODULE_POINTS					8

// Counter
#define COUNTER_MODULE_POINTS				8
#define COUNTER_MODULE_REAL_POINTS          8

//---------------------------------------------------------------------------
class TLA50BIO : public TCommonIO
{
private:

	ioTYPE IOType;
	int pModule;
	int pPoint;
	void __fastcall SetModule(int Value);
	void __fastcall SetPoint(int Value);
	int __fastcall GetCalcPointNo(void);
	void __fastcall SetCalcPointNo(int Value);

public:
	__fastcall TLA50BIO(ioTYPE Type);
	__fastcall ~TLA50BIO();

	__property int Module		= { read = pModule			,write = SetModule	};
	__property int Point		= { read = pPoint			,write = SetPoint	};
	__property ioTYPE Type		= { read = IOType                                   };
    __property int CalcPointNo  = { read = GetCalcPointNo   ,write = SetCalcPointNo };
};
//---------------------------------------------------------------------------
#endif
