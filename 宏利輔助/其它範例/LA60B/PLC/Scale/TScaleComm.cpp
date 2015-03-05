//---------------------------------------------------------------------------


#pragma hdrstop

#include "TScaleComm.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
__fastcall TScaleComm::TScaleComm(TComponent *Owner)
	: TPLCBasic(Owner,SCALE,0)
{
	MyOwner	= Owner;

	pLastWeight	= 0;
	pLastUnit	= "";

	memset( TareCommand, 0, SCALE_COMMAND_BYTES );
	memset( ZeroCommand, 0, SCALE_COMMAND_BYTES );
	IsTare		= false;
	IsZero		= false;
	CommTick	= 0;
	TimeoutMS	= 500;		// default value
	pScaleType	= mdlMETTLER;
}
//---------------------------------------------------------------------------
__fastcall TScaleComm::~TScaleComm()
{
}
//---------------------------------------------------------------------------
RxState __fastcall TScaleComm::CheckRxData(char *Data,int Length)
{
	CommTick	= GetTickCount();

	if( !IsRxFinish( Data, Length ) )
		return COMM_WAIT_RESPONSE;
	if( !CheckData( Data, Length ) )
		return COMM_DATA_ERR;

	PickValue( Data, Length );
	return COMM_RX_OK;
}
//---------------------------------------------------------------------------
bool __fastcall TScaleComm::IsRxFinish(char *Data,int Length)
{
	if( Length <= 2 )
		return false;


	for( int i=0; i<Length-1; i++ )
	{
		if( Data[ i ] == SCALE_DATA_CR && Data[ i + 1 ] == SCALE_DATA_LF )
			return true;
	}
	return false;
}
//---------------------------------------------------------------------------
bool __fastcall TScaleComm::CheckData(char *Data,int Length)
{
	return true;
}
//---------------------------------------------------------------------------
void __fastcall TScaleComm::PickValue(char *Data,int Length)
{
	switch( pScaleType )
	{
		case mdlLOADCELL:
			LoadcellPickValue( Data, Length );
			break;

		case mdlMETTLER:
		default:
			MettlerPickValue( Data, Length );
			break;
    }
}
//---------------------------------------------------------------------------
void __fastcall TScaleComm::MettlerPickValue(char *Data,int Length)
{
	int index;
	int len ,start;
	char buf[SCALE_BUF_LENGTH];

	start	= -1;
	for( int i=0; i<Length; i++ )
	{
		if( start < 0 )
		{
			if( ( Data[i] >= '0' && Data[i] <= '9' )
				|| Data[i] == '-' || Data[i] == '+' || Data[i] == '.'
				)
			{
				start	= i;
			}
		}
		else //if( start >= 0 ) , start collect numbers
		{
			if( ( Data[i] < '0' || Data[i] > '9' )
				&& Data[i] != '.'
				)
			{
				index	= i;
				len		= i - start;
				break;
			}
		}
	}
	if( start < 0 )
		return;

	memset( buf, 0, SCALE_BUF_LENGTH );
	memcpy( buf, Data + start, len );
	pLastWeight	= StrToFloatDef( buf, 0 );

	for( int i=index; i<Length; i++ )
	{
		if( Data[i] == SCALE_DATA_CR || Data[i] == SCALE_DATA_LF )
		{
			len	= i - index;
			memset( buf, 0, SCALE_BUF_LENGTH );
			memcpy( buf, Data + index, len );
			pLastUnit	= String( buf ).Trim();
			break;
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TScaleComm::LoadcellPickValue(char *Data,int Length)
{
	int index;
	int len ,start;
	char buf[SCALE_BUF_LENGTH];
	bool minus;

	minus	= false;
	start	= -1;
	for( int i=0; i<Length; i++ )
	{
		if( start < 0 )
		{
			if( ( Data[i] >= '0' && Data[i] <= '9' )
				|| Data[i] == '-' || Data[i] == '+' || Data[i] == '.'
				)
			{
				start	= i;
			}
		}
		else if( Data[i] == ' ' )
		{
			if( start >= 0 )
			{
				if( Data[ start ] == '-' )
					minus	= true;
			}
			start	= -1;
		}
		else //if( start >= 0 ) , start collect numbers
		{
			if( ( Data[i] < '0' || Data[i] > '9' )
				&& Data[i] != '.'
				)
			{
				index	= i;
				len		= i - start;
				break;
			}
		}
	}
	if( start < 0 )
		return;

	memset( buf, 0, SCALE_BUF_LENGTH );
	memcpy( buf, Data + start, len );
	pLastWeight	= StrToFloatDef( buf, 0 );
	if( minus )
		pLastWeight	= -pLastWeight;

	switch( Data[ index ] )			// next charactors is unit symbol
	{
		case 'L':
			pLastUnit	= "Pound";
			index++;
			break;

		case 'K':
			pLastUnit	= "KG";
			index++;
			break;

		case 'P':
			pLastUnit	= "PCS";
			index	+= 3;			// the original charactors is "PCS"
			break;

		default:
			pLastUnit	= "";
			index++;
			break;
	}

	/*
	if( Data[index] == 'G' )
		is_gross;
	else if( Data[index] == 'G' )
		is_net;

	index++;
	switch( Data[index] )
	{
		case ' ':
			valid;
			break;

		case 'M':
			Motion;
			break;

		case 'O':
			Over_range;	// or under range
			break;
	}
	*/
}
//---------------------------------------------------------------------------
int __fastcall TScaleComm::TxNextCmdData(char *Data)
{
	int length;

	if( IsTare )
	{
		length	= String( TareCommand ).Length();
		memcpy( Data, TareCommand, length );
		IsTare	= false;
		return length;
	}
	else if( IsZero )
	{
		length	= String( ZeroCommand ).Length();
		memcpy( Data, ZeroCommand, length );
		IsZero	= false;
		return length;
	}

	return 0;
}
//---------------------------------------------------------------------------
bool __fastcall TScaleComm::LoadParameter(String FilePath)
{
	TIniFile *ini;
	int type;

	memset( TareCommand, 0, SCALE_COMMAND_BYTES );
	memset( ZeroCommand, 0, SCALE_COMMAND_BYTES );

	if( !FileExists(FilePath) )
		return false;

	String buf;

	ini	= new TIniFile( FilePath );

	type	= ini->ReadInteger(	"Main",	"ScaleType",	mdlMETTLER	);
	switch( type )
	{
		case mdlLOADCELL:
			pScaleType	= mdlLOADCELL;
			break;

		case mdlMETTLER:
		default:
			pScaleType	= mdlMETTLER;
			break;
    }

	buf	= ini->ReadString(	"CMD",	"Tare",	""	);
	if( !buf.IsEmpty() )
	{
		memcpy( TareCommand, buf.c_str(), buf.Length() );
		//strcpy( TareCommand, buf.c_str() );
		TareCommand[ buf.Length() ]		= SCALE_DATA_CR;
		TareCommand[ buf.Length() + 1 ]	= SCALE_DATA_LF;
	}

	buf	= ini->ReadString(	"CMD",	"Zero",	""	);
	if( !buf.IsEmpty() )
	{
		memcpy( ZeroCommand, buf.c_str(), buf.Length() );
		//strcpy( ZeroCommand, buf.c_str() );
		ZeroCommand[ buf.Length() ]		= SCALE_DATA_CR;
		ZeroCommand[ buf.Length() + 1 ]	= SCALE_DATA_LF;
	}

	delete ini;
	ini	= NULL;
	return true;
}
//---------------------------------------------------------------------------
bool __fastcall TScaleComm::CheckWtAvailable(void)
{
	if( CommTick == 0
		|| TimeGoesBy( CommTick ) >= TimeoutMS )
	{
		return false;
	}
	return true;
}
//---------------------------------------------------------------------------
void __fastcall TScaleComm::Zero(void)
{
	IsZero	= true;
}
//---------------------------------------------------------------------------
void __fastcall TScaleComm::Tare(void)
{
	IsTare	= true;
}
//---------------------------------------------------------------------------

