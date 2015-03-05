//---------------------------------------------------------------------------


#pragma hdrstop

#include "TPLC.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)
//---------------------------------------------------------------------------
__fastcall TPLC::TPLC(TComponent *Owner)
{
	MyOwner	= Owner;

	pType	= UNKNOW_PLC;
	LA50Bs	= NULL;
	LA60Bs	= NULL;
	Scale	= NULL;
}
//---------------------------------------------------------------------------
__fastcall TPLC::~TPLC()
{
	ClearPLC();
}
//---------------------------------------------------------------------------
void __fastcall TPLC::ClearPLC(void)
{
	if( LA50Bs )
	{
		delete LA50Bs;
		LA50Bs	= NULL;
	}
	if( LA60Bs )
	{
		delete LA60Bs;
		LA60Bs	= NULL;
	}
	if( Scale )
	{
		delete Scale;
		Scale	= NULL;
    }
}
//---------------------------------------------------------------------------
bool __fastcall TPLC::SetPLCType(plcTYPE PLC_Type,String ParamPath)
{
	bool result;

	ClearPLC();
	result	= false;
	switch( PLC_Type )
	{
		case LA50B:
			LA50Bs		= new TLA50BConverter( MyOwner );
			if( LA50Bs )
				result	= LA50Bs->LoadParameter( ParamPath );
			break;

		case LA60B:
			LA60Bs		= new TLA60BConverter( MyOwner );
			if( LA60Bs )
				result	= LA60Bs->LoadParameter( ParamPath );
			break;

		case SCALE:
			Scale	= new TScaleComm( MyOwner );
			if( Scale )
				result	= Scale->LoadParameter( ParamPath );
			break;

		default:
			PLC_Type	= UNKNOW_PLC;
			break;
	}

	pType	= PLC_Type;
	return result;
}
//---------------------------------------------------------------------------
bool __fastcall TPLC::LoadParameter(String ParamPath)
{
	TIniFile *ini;
	String type;

	if( !FileExists(ParamPath) )
		return false;

	ini		= new TIniFile( ParamPath );
	type	= ini->ReadString(	SECT_MAIN,	FLD_PLC_TYPE,	""	);
	delete ini;
	ini		= NULL;

	return SetPLCType( StringToPLCType(type), ParamPath );
}
//---------------------------------------------------------------------------
TPLCBasic* __fastcall TPLC::GetCommCPU(void)
{
	switch( Type )
	{
		case LA50B:
			return LA50Bs->CommCPU;

		case LA60B:
			return LA60Bs->CommCPU;

		case SCALE:
			return ( (TPLCBasic*)Scale );

		default:
			return NULL;
	}
}
//---------------------------------------------------------------------------
TPLCState __fastcall TPLC::GetPLCState(unsigned char CPU_ID)
{
	TPLCState result;

	switch( Type )
	{
		case LA50B:
			result	= LA50Bs->GetCPUState( CPU_ID );
			break;

		case LA60B:
			result	= LA60Bs->GetCPUState( CPU_ID );
			break;

		case SCALE:
			result	= Scale->State;
			break;

		default:
			result	= PLC_OFF;
			break;
	}
	return result;
}
//---------------------------------------------------------------------------
bool __fastcall TPLC::SetPLCState(unsigned char CPU_ID,TPLCState State)
{
	switch( Type )
	{
		case LA50B:
			return LA50Bs->SetCPUState( CPU_ID, State );

		case LA60B:
			return LA60Bs->SetCPUState( CPU_ID, State );

		default:
			return false;
	}
}
//---------------------------------------------------------------------------
bool __fastcall TPLC::GetDI(int Number)
{
	switch( Type )
	{
		case LA50B:
			return LA50Bs->GetDI( Number );

		case LA60B:
			return LA60Bs->GetDIO( ioDI, Number );

		default:
			return false;
	}
}
//---------------------------------------------------------------------------
bool __fastcall TPLC::GetDI(String Code)
{
	switch( Type )
	{
		case LA50B:
			return LA50Bs->GetDI( Code );

		case LA60B:
			return LA60Bs->GetDIO( ioDI, Code );

		default:
			return false;
	}
}
//---------------------------------------------------------------------------
bool __fastcall TPLC::GetDO(int Number)
{
	switch( Type )
	{
		case LA50B:
			return LA50Bs->GetDO( Number );

		case LA60B:
			return LA60Bs->GetDIO( ioDO, Number );

		default:
			return false;
	}
}
//---------------------------------------------------------------------------
bool __fastcall TPLC::GetDO(String Code)
{
	switch( Type )
	{
		case LA50B:
			return LA50Bs->GetDO( Code );

		case LA60B:
			return LA60Bs->GetDIO( ioDO, Code );

		default:
			return false;
	}
}
//---------------------------------------------------------------------------
bool __fastcall TPLC::OH(int Number)
{
	switch( Type )
	{
		case LA50B:
			return LA50Bs->OH( Number );

		case LA60B:
			return LA60Bs->OH( Number );

		default:
			return false;
	}
}
//---------------------------------------------------------------------------
bool __fastcall TPLC::OH(String Code)
{
	switch( Type )
	{
		case LA50B:
			return LA50Bs->OH( Code );

		case LA60B:
			return LA60Bs->OH( Code );

		default:
			return false;
	}
}
//---------------------------------------------------------------------------
bool __fastcall TPLC::OL(int Number)
{
	switch( Type )
	{
		case LA50B:
			return LA50Bs->OL( Number );

		case LA60B:
			return LA60Bs->OL( Number );

		default:
			return false;
	}
}
//---------------------------------------------------------------------------
bool __fastcall TPLC::OL(String Code)
{
	switch( Type )
	{
		case LA50B:
			return LA50Bs->OL( Code );

		case LA60B:
			return LA60Bs->OL( Code );

		default:
			return false;
	}
}
//---------------------------------------------------------------------------
WORD __fastcall TPLC::GetAI(int Number)
{
	switch( Type )
	{
		case LA50B:
			return LA50Bs->GetAI( Number );

		case LA60B:
			return LA60Bs->GetAI( Number );

		default:
			return 0;
	}
}
//---------------------------------------------------------------------------
WORD __fastcall TPLC::GetAI(String Code)
{
	switch( Type )
	{
		case LA50B:
			return LA50Bs->GetAI( Code );

		case LA60B:
			return LA60Bs->GetAI( Code );

		default:
			return 0;
	}
}
//---------------------------------------------------------------------------
WORD __fastcall TPLC::GetAO(int Number)
{
	switch( Type )
	{
		case LA50B:
			return LA50Bs->GetAO( Number );

		case LA60B:
			return LA60Bs->GetAO( Number );

		default:
			return 0;
	}
}
//---------------------------------------------------------------------------
WORD __fastcall TPLC::GetAO(String Code)
{
	switch( Type )
	{
		case LA50B:
			return LA50Bs->GetAO( Code );

		case LA60B:
			return LA60Bs->GetAO( Code );

		default:
			return 0;
	}
}
//---------------------------------------------------------------------------
bool __fastcall TPLC::SetAO(int Number,WORD Value)
{
	switch( Type )
	{
		case LA50B:
			return LA50Bs->SetAO( Number, Value );

		case LA60B:
			return LA60Bs->SetAO( Number, Value );

		default:
			return false;
	}
}
//---------------------------------------------------------------------------
bool __fastcall TPLC::SetAO(int Number,double Percent)
{
	switch( Type )
	{
		case LA50B:
			return LA50Bs->SetAO( Number, Percent );

		case LA60B:
			return LA60Bs->SetAO( Number, Percent );

		default:
			return false;
	}
}
//---------------------------------------------------------------------------
bool __fastcall TPLC::SetAO(String Code,WORD Value)
{
	switch( Type )
	{
		case LA50B:
			return LA50Bs->SetAO( Code, Value );

		case LA60B:
			return LA60Bs->SetAO( Code, Value );

		default:
			return false;
	}
}
//---------------------------------------------------------------------------
bool __fastcall TPLC::SetAO(String Code,double Percent)
{
	switch( Type )
	{
		case LA50B:
			return LA50Bs->SetAO( Code, Percent );

		case LA60B:
			return LA60Bs->SetAO( Code, Percent );

		default:
			return false;
	}
}
//---------------------------------------------------------------------------
WORD __fastcall TPLC::GetCounter(int Number)
{
	switch( Type )
	{
		case LA50B:
			return LA50Bs->GetCounter( Number );

		case LA60B:
			return LA60Bs->GetAI( Number );

		default:
			return 0;
	}
}
//---------------------------------------------------------------------------
WORD __fastcall TPLC::GetCounter(String Code)
{
	switch( Type )
	{
		case LA50B:
			return LA50Bs->GetCounter( Code );

		case LA60B:
			return LA60Bs->GetAI( Code );

		default:
			return 0;
	}
}
//---------------------------------------------------------------------------
double __fastcall TPLC::GetTemperature(int Number)
{
	switch( Type )
	{
		case LA50B:
			return LA50Bs->GetTemperature( Number );

		case LA60B:
			return LA60Bs->GetTemperature( Number );

		default:
			return 0;
	}
}
//---------------------------------------------------------------------------
double __fastcall TPLC::GetTemperature(String Code)
{
	switch( Type )
	{
		case LA50B:
			return LA50Bs->GetTemperature( Code );

		case LA60B:
			return LA60Bs->GetTemperature( Code );

		default:
			return 0;
	}
}
//---------------------------------------------------------------------------
double __fastcall TPLC::GetTemperature(TCommonIO *IO)
{
	switch( IO->PLCType )
	{
		case LA50B:
			if( LA50Bs )
				return LA50Bs->GetTemperature( ( (TLA50BIO*)IO ) );

		case LA60B:
			if( LA60Bs )
				return LA60Bs->GetTemperature( ( (TLA60BIO*)IO ) );

		default:
			;
	}
	return 0;
}
//---------------------------------------------------------------------------
int __fastcall TPLC::CountAllCPU(void)
{
	switch( Type )
	{
		case LA50B:
			return LA50Bs->CPUCount;

		case LA60B:
			return LA60Bs->CPUCount;

		default:
			return -1;
	}
}
//---------------------------------------------------------------------------
//
//	Method : NextTxData
//
//	Note :
// 			Polling all type CPUs
//          But if last communicated is need check, check it first
//
//---------------------------------------------------------------------------
int __fastcall TPLC::NextTxData(char *Data)
{
	int length = 0;

	switch( Type )
	{
		case LA50B:
			length	= LA50Bs->TxNextData( Data );
			break;

		case LA60B:
			length	= LA60Bs->TxNextCmdData( Data );
			break;

		case SCALE:
			length	= Scale->TxNextCmdData( Data );
			break;

		default:
			break;
	}

	return length;
}
//---------------------------------------------------------------------------
int __fastcall TPLC::ReGetTxData(char *Data)
{
	int length=0;

	switch( Type )
	{
		case LA50B:
			length	= LA50Bs->ReGetTxData( Data );
			break;

		case LA60B:
			length	= LA60Bs->ReGetTxCmdData( Data );
			break;

		case SCALE:
			length	= Scale->TxNextCmdData( Data );
			break;

		default:
			break;
	}
	return length;
}
//---------------------------------------------------------------------------
RxState __fastcall TPLC::CheckRxData(char *Data,int Length)
{
	RxState result = COMM_WAIT_RESPONSE;

	switch( Type )
	{
		case LA50B:
			result	= LA50Bs->CheckRxData( Data, Length );
			break;

		case LA60B:
			result	= LA60Bs->CheckRxData( Data, Length );
			break;

		case SCALE:
        	result	= Scale->CheckRxData( Data, Length );
			break;

		default:
			break;
	}
	return result;
}
//---------------------------------------------------------------------------
bool __fastcall TPLC::SetDI(int Number,bool High)
{
	switch( Type )
	{
		case LA50B:
			return LA50Bs->SetDI( Number, High );

		case LA60B:
			return LA60Bs->SetDI( Number, High );

		default:
			return false;
	}
}
//---------------------------------------------------------------------------
bool __fastcall TPLC::SetDI(String Code,bool High)
{
	switch( Type )
	{
		case LA50B:
			return LA50Bs->SetDI( Code, High );

		case LA60B:
			return LA60Bs->SetDI( Code, High );

		default:
			return false;
	}
}
//---------------------------------------------------------------------------
bool __fastcall TPLC::SetAI(int Number,WORD Value)
{
	switch( Type )
	{
		case LA50B:
			return LA50Bs->SetAI( Number, Value );

		case LA60B:
			return LA60Bs->SetAI( Number, Value );

		default:
			return false;
	}
}
//---------------------------------------------------------------------------
bool __fastcall TPLC::SetAI(String Code,WORD Value)
{
	switch( Type )
	{
		case LA50B:
			return LA50Bs->SetAI( Code, Value );

		case LA60B:
			return LA60Bs->SetAI( Code, Value );

		default:
			return false;
	}
}
//---------------------------------------------------------------------------
bool __fastcall TPLC::SetCounter(int Number,WORD Value)
{
	switch( Type )
	{
		case LA50B:
			return LA50Bs->SetCounter( Number, Value );

		case LA60B:
			return LA60Bs->SetAI( Number, Value );

		default:
			return false;
	}
}
//---------------------------------------------------------------------------
bool __fastcall TPLC::SetCounter(String Code,WORD Value)
{
	switch( Type )
	{
		case LA50B:
			return LA50Bs->SetCounter( Code, Value );

		case LA60B:
			return LA60Bs->SetAI( Code, Value );

		default:
			return false;
	}
}
//---------------------------------------------------------------------------
double __fastcall TPLC::ScaleWeight(unsigned char CPU_ID)
{
	switch( Type )
	{
		case LA50B:
			return LA50Bs->ScaleWeight( CPU_ID );

		case SCALE:
			return Scale->Weight;

		case LA60B:
		default:
			return 0;
	}
}
//---------------------------------------------------------------------------
/*
String __fastcall TPLC::ScaleUnit(unsigned char CPU_ID)
{
	switch( Type )
	{
		case SCALE:
			return Scale->Unit;

		case LA50B:
		case LA60B:
		default:
			return "";
	}
}
*/
//---------------------------------------------------------------------------
void __fastcall TPLC::ScaleZero(unsigned char CPU_ID)
{
	switch( Type )
	{
		case LA50B:
			LA50Bs->ScaleZero( CPU_ID );
			break;

		case SCALE:
			Scale->Zero();
			break;

		case LA60B:
		default:
			break;
	}
}
//---------------------------------------------------------------------------
void __fastcall TPLC::ScaleTare(unsigned char CPU_ID)
{
	switch( Type )
	{
		case LA50B:
			LA50Bs->ScaleTare( CPU_ID );
			break;

		case SCALE:
			Scale->Tare();
			break;

		case LA60B:
		default:
			break;
	}
}
//---------------------------------------------------------------------------
bool __fastcall TPLC::CheckWtAvailable(unsigned char CPU_ID)
{
	switch( Type )
	{
		case LA50B:
			return LA50Bs->CheckWtAvailable( CPU_ID );

		case SCALE:
			return Scale->IsWeightAvailable;

		case LA60B:			// not connect Scale
		default:
			return false;
	}
}
//---------------------------------------------------------------------------
bool __fastcall TPLC::DefineIOCode(ioTYPE Type,int Number,String Code)
{
	switch( this->Type )
	{
		case LA50B:
			return LA50Bs->DefineIOCode( Type, Number, Code );

		case LA60B:
			return LA60Bs->DefineIOCode( Type, Number, Code );

		case SCALE:
		default:
			;
	}
	return false;
}
//---------------------------------------------------------------------------
bool __fastcall TPLC::LinkIO(TBasicIO *IO)
{
	if( this->Type != IO->PLCType )
		return false;

	switch( IO->PLCType )
	{
		case LA50B:
			return LA50Bs->LinkIO( ((TLA50BIO*)IO) );

		case LA60B:
			return LA60Bs->LinkIO( ((TLA60BIO*)IO) );

		case SCALE:
		default:
			;
	}
	return false;
}
//---------------------------------------------------------------------------
void __fastcall TPLC::OLAll(void)
{
	switch( Type )
	{
		case LA50B:
			LA50Bs->OLAll();
			break;

		case LA60B:
			LA60Bs->OLAll();
			break;

		case SCALE:
		default:
			break;
	}
}
//---------------------------------------------------------------------------

