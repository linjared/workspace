//---------------------------------------------------------------------------


#pragma hdrstop

#include "TLA50BIO.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
__fastcall TLA50BIO::TLA50BIO(ioTYPE Type)
	: TCommonIO( LA50B )
{
	IOType   	= Type;

	pModule  	= -1;
	pPoint   	= -1;
}
//---------------------------------------------------------------------------
__fastcall TLA50BIO::~TLA50BIO()
{
}
//---------------------------------------------------------------------------
int __fastcall TLA50BIO::GetCalcPointNo(void)
{
    if( Module < 0 || Point < 0 )
        return -1;

	switch( IOType )
	{
		case ioDI:
		case ioDO:
			return ( Module * DIO_MODULE_POINTS + Point );

		case ioAI:
		case ioAO:                                                  // Analog data consist of 2 bytes
			return ( Module * AIO_MODULE_POINTS + Point * 2 );

		case ioCOUNTER:                                             // Counter data consist of 2 bytes
			return ( Module * COUNTER_MODULE_POINTS + Point * 2 );

		default:
			break;
	}
	return -1;
}
//---------------------------------------------------------------------------
void __fastcall TLA50BIO::SetCalcPointNo(int Value)
{
	if( Value < 0 )
		return;

	switch( IOType )
	{
		case ioDI:
		case ioDO:
			Module  = Value / DIO_MODULE_POINTS;
			Point   = Value % DIO_MODULE_POINTS;
			break;

		case ioAI:
		case ioAO:										// Analog data consist of 2 bytes
			Module  = Value / AIO_MODULE_POINTS;
			Point   = Value % AIO_MODULE_POINTS / 2;
			break;

		case ioCOUNTER:									// Counter data consist of 2 bytes
			Module  = Value / AIO_MODULE_POINTS;
			Point   = Value % AIO_MODULE_POINTS / 2;
			break;

		default:
			break;
	}
}
//---------------------------------------------------------------------------
void __fastcall TLA50BIO::SetModule(int Value)
{
	switch( IOType )
	{
		case ioDI:
		case ioDO:
			if( Value >= 0 && Value < DIO_MAX_MODULE )
				pModule	= Value;
			break;

		case ioAI:
		case ioAO:
		case ioCOUNTER:
			if( Value >= 0 && Value < AIO_MAX_MODULE )
				pModule	= Value;
			break;

		default:
			break;
	}
}
//---------------------------------------------------------------------------
void __fastcall TLA50BIO::SetPoint(int Value)
{
	switch( IOType )
	{
		case ioDI:
		case ioDO:
			if( Value >= 0 && Value < DIO_MODULE_POINTS )
				pPoint	= Value;
			break;

		case ioAI:
		case ioAO:
			if( Value >= 0 && Value < AIO_MODULE_REAL_POINTS )
				pPoint	= Value;
			break;

		case ioCOUNTER:
			if( Value >= 0 && Value < COUNTER_MODULE_REAL_POINTS )
				pPoint	= Value;
			break;

		default:
			break;
	}
}
//---------------------------------------------------------------------------

