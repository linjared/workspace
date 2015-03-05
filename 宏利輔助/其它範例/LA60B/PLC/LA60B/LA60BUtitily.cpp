//---------------------------------------------------------------------------
#include "LA60BUtitily.h"

//---------------------------------------------------------------------------
char IntToBCD(int integer)
{
	switch( integer )
	{
		case 0:
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
		case 8:
		case 9:
			return integer + 48;

		case 10:
		case 11:
		case 12:
		case 13:
		case 14:
		case 15:
			return integer + 55;

		default:
			return 0;
	}
}
//---------------------------------------------------------------------------
int BCDToInt(char value)
{
	// Change to Upper Case
	if( value >= 'a' && value <= 'f' )
		value -= 32;

	switch( value )
	{
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			return (value - 48);

		case 'A':
		case 'B':
		case 'C':
		case 'D':
		case 'E':
		case 'F':
			return (value - 55);

		default:
			return 0;
	}
}
//---------------------------------------------------------------------------
unsigned short TransBufferBCDData(char *Data,int GetPos,int BCDs)
{
	unsigned short result;
	int i;

	result	= 0;
	for( i=0; i<BCDs; i++ )
	{
		//if( i > 0 )
			result	= result 	<< 4;
		result	+= BCDToInt( Data[GetPos+i] );
	}
	return result;
}
//---------------------------------------------------------------------------
unsigned char LRCGeneration(unsigned char *Data,int DataLen)
{
	int i;
	unsigned char sum = 0;

	for( i=0; i<DataLen; i++ )
		sum	+= Data[i];
	return sum;
}
//---------------------------------------------------------------------------
int InsData(unsigned char *Data,int InsPos,unsigned short Value,int Length)
{
	if( Length == 1 )	// Use 2 Character to Present 1 Byte
	{
		Data[InsPos++]	= IntToBCD( (Value&0xf0)>>4 );
		Data[InsPos++]	= IntToBCD( (Value&0x0f) );
	}
	else //if( Length == 2 ) // Use 4 Character to Present 2 Byte ( 1 Word )
	{
		Data[InsPos++]	= IntToBCD( (Value&0xf000)>>12 );
		Data[InsPos++]	= IntToBCD( (Value&0x0f00)>>8 );
		Data[InsPos++]	= IntToBCD( (Value&0x00f0)>>4 );
		Data[InsPos++]	= IntToBCD( (Value&0x000f) );
	}

	return InsPos;
}
//---------------------------------------------------------------------------
//
//	Function : InsLA60BAddrData
//	Note :
//       	LA60B Address Data is decimal not Hex
//
//---------------------------------------------------------------------------
int InsLA60BAddrData(unsigned char *Data,int InsPos,unsigned short Value,int BCDs)
{
	if( BCDs == 5 )
		Data[InsPos++]	= '0';
	Data[InsPos++]	= IntToBCD( Value/1000 );			// get thousand value
	Data[InsPos++]	= IntToBCD( (Value%1000)/100 );		// get hundred value
	Data[InsPos++]	= IntToBCD( (Value%100)/10 );		// get tens value
	Data[InsPos++]	= IntToBCD( Value%10 );				// get first digit

	return InsPos;
}
//---------------------------------------------------------------------------

