//---------------------------------------------------------------------------


#pragma hdrstop

#include "TLA50BConverter.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
__fastcall TLA50BConverter::TLA50BConverter(TComponent *Owner)
{
    MyOwner = Owner;

    LastCommCPUIdx  = -1;

    CPUs    = new TList();
    if( CPUs )
        CPUs->Clear();
}
//---------------------------------------------------------------------------
__fastcall TLA50BConverter::~TLA50BConverter()
{
    if( CPUs )
    {
        while( CPUs->Count > 0 )
            DeleteCPU( 0 );

        delete CPUs;
        CPUs    = NULL;
    }
}
//---------------------------------------------------------------------------
bool __fastcall TLA50BConverter::LoadParameter(String ParamPath)
{
	TIniFile *ini;
	int cpu_count ,dio_count ,aio_count;
	int dio_start_num ,aio_start_num ,counter_start_num ,length;
	int i;
	unsigned char id;
	String buf ,sect;
	bool result;


	if( !FileExists(ParamPath) )
		return false;

	result				= true;
	dio_start_num		= 0;
	aio_start_num		= 0;
	counter_start_num	= 0;
	length				= 0;

	ini		= new TIniFile( ParamPath );

	cpu_count	= ini->ReadInteger(	SECT_MAIN,	FLD_CPU_COUNT,	0	);
	for( i=0; i<cpu_count; i++ )
	{
		sect	= SECT_CPU_LEADER + IntToStr(i+1);

		buf	= ini->ReadString(	sect,	FLD_CPU_ID,		""	).Trim();
		if( buf.IsEmpty() )
		{
			result	= false;
			break;
		}

		// add CPU
		if( buf.ToIntDef(-1) < 0 )
		{
			result	= false;
			break;
		}
		id	= buf.ToIntDef(-1);
		if( !AddCPU( id ) )
		{
			result	= false;
			break;
		}

		dio_count	= ini->ReadInteger(	sect,	FLD_LA50B_DIO_COUNT,	-1	);
		aio_count	= ini->ReadInteger(	sect,	FLD_LA50B_AIO_COUNT,	-1	);
		if( dio_count < 0 || dio_count > DIO_MAX_MODULE
			|| aio_count < 0 || aio_count > AIO_MAX_MODULE
			)
		{
			result	= false;
			break;
        }

		// assign DIO Moudule ( Points )
		length		= dio_count * DIO_MODULE_POINTS;
		DefineDIO( id, dio_start_num, length	);
		dio_start_num	+= length;

		// assign AIO Moudule ( Points )
		// Analog IOs and Counters is in the same PLC Module.
		length		= aio_count * AIO_MODULE_REAL_POINTS;
		DefineAIO( id, aio_start_num, length	);
		aio_start_num	+= length;

		length		= aio_count * COUNTER_MODULE_REAL_POINTS;
		DefineCounter( id, counter_start_num, length	);
		counter_start_num	+= length;
	}

	delete ini;
	ini		= NULL;
	return result;
}
//---------------------------------------------------------------------------
bool __fastcall TLA50BConverter::DefineDIO(unsigned char ID,int Start,int Length)
{
	int idx;

	idx	= IndexOfCPU( ID );
	if( idx < 0 )
		return false;
	return Items[ idx ]->DefineDIONumber( Start, Length );
}
//---------------------------------------------------------------------------
bool __fastcall TLA50BConverter::DefineAIO(unsigned char ID,int Start,int Length)
{
	int idx;

	idx	= IndexOfCPU( ID );
	if( idx < 0 )
		return false;
	return Items[ idx ]->DefineAIONumber( Start, Length );
}
//---------------------------------------------------------------------------
bool __fastcall TLA50BConverter::DefineCounter(unsigned char ID,int Start,int Length)
{
	int idx;

	idx	= IndexOfCPU( ID );
	if( idx < 0 )
		return false;
	return Items[ idx ]->DefineCounterNumber( Start, Length );
}
//---------------------------------------------------------------------------
int __fastcall TLA50BConverter::CPUIdxOfDIO(int IONumber)
{
	int i;

	if( IONumber < 0 )
		return -1;

	for( i=0; i<CPUCount; i++ )
	{
		if( Items[i]->IsDIOExists( IONumber ) )
			return i;
	}
	return -1;
}
//---------------------------------------------------------------------------
int __fastcall TLA50BConverter::CPUIdxOfDIO(String Code)
{
	int i;

	if( Code.Trim().IsEmpty() )
		return -1;

	for( i=0; i<CPUCount; i++ )
	{
		if( Items[i]->IsDIOExists( Code ) )
			return i;
	}
	return -1;
}
//---------------------------------------------------------------------------
int __fastcall TLA50BConverter::CPUIdxOfAIO(int IONumber)
{
	int i;

	if( IONumber < 0 )
		return -1;

	for( i=0; i<CPUCount; i++ )
	{
		if( Items[i]->IsAIOExists( IONumber ) )
			return i;
	}
	return -1;
}
//---------------------------------------------------------------------------
int __fastcall TLA50BConverter::CPUIdxOfAIO(String Code)
{
	int i;

	if( Code.Trim().IsEmpty() )
		return -1;

	for( i=0; i<CPUCount; i++ )
	{
		if( Items[i]->IsAIOExists( Code ) )
			return i;
	}
	return -1;
}
//---------------------------------------------------------------------------
int __fastcall TLA50BConverter::CPUIdxOfCounter(int IONumber)
{
	int i;

	if( IONumber < 0 )
		return -1;

	for( i=0; i<CPUCount; i++ )
	{
		if( Items[i]->IsCounterExists( IONumber ) )
			return i;
	}
	return -1;
}
//---------------------------------------------------------------------------
int __fastcall TLA50BConverter::CPUIdxOfCounter(String Code)
{
	int i;

	if( Code.Trim().IsEmpty() )
		return -1;

	for( i=0; i<CPUCount; i++ )
	{
		if( Items[i]->IsCounterExists( Code ) )
			return i;
	}
	return -1;
}
//---------------------------------------------------------------------------
bool __fastcall TLA50BConverter::DefineDICode(int IONumber,String Code)
{
	int idx;

	idx	= CPUIdxOfDIO( IONumber );
	if( idx < 0 )
		return false;

	return Items[ idx ]->DefineDICode( IONumber, Code );
}
//---------------------------------------------------------------------------
bool __fastcall TLA50BConverter::DefineDOCode(int IONumber,String Code)
{
	int idx;

	idx	= CPUIdxOfDIO( IONumber );
	if( idx < 0 )
		return false;

	return Items[ idx ]->DefineDOCode( IONumber, Code );
}
//---------------------------------------------------------------------------
bool __fastcall TLA50BConverter::DefineAICode(int IONumber,String Code)
{
	int idx;

	idx	= CPUIdxOfAIO( IONumber );
	if( idx < 0 )
		return false;

	return Items[ idx ]->DefineAICode( IONumber, Code );
}
//---------------------------------------------------------------------------
bool __fastcall TLA50BConverter::DefineAOCode(int IONumber,String Code)
{
	int idx;

	idx	= CPUIdxOfAIO( IONumber );
	if( idx < 0 )
		return false;

	return Items[ idx ]->DefineAOCode( IONumber, Code );
}
//---------------------------------------------------------------------------
bool __fastcall TLA50BConverter::DefineCounterCode(int IONumber,String Code)
{
	int idx;

	idx	= CPUIdxOfCounter( IONumber );
	if( idx < 0 )
		return false;

	return Items[ idx ]->DefineCounterCode( IONumber, Code );
}
//---------------------------------------------------------------------------
bool __fastcall TLA50BConverter::IsDIOExists(int Number)
{
	if( CPUIdxOfDIO( Number ) < 0 )
		return false;
	return true;
}
//---------------------------------------------------------------------------
bool __fastcall TLA50BConverter::IsDIOExists(String Code)
{
	if( CPUIdxOfDIO( Code ) < 0 )
		return false;
	return true;
}
//---------------------------------------------------------------------------
bool __fastcall TLA50BConverter::IsAIOExists(int Number)
{
	if( CPUIdxOfAIO( Number ) < 0 )
		return false;
	return true;
}
//---------------------------------------------------------------------------
bool __fastcall TLA50BConverter::IsAIOExists(String Code)
{
	if( CPUIdxOfAIO( Code ) < 0 )
		return false;
	return true;
}
//---------------------------------------------------------------------------
bool __fastcall TLA50BConverter::IsCounterExists(int Number)
{
	if( CPUIdxOfCounter( Number ) < 0 )
		return false;
	return true;
}
//---------------------------------------------------------------------------
bool __fastcall TLA50BConverter::IsCounterExists(String Code)
{
	if( CPUIdxOfCounter( Code ) < 0 )
		return false;
	return true;
}
//---------------------------------------------------------------------------
TPLCBasic* __fastcall TLA50BConverter::GetCommCPU(void)
{
	if( LastCommCPUIdx < 0 || LastCommCPUIdx >= CPUCount )
		return NULL;

	return ( (TPLCBasic*)Items[ LastCommCPUIdx ] );
}
//---------------------------------------------------------------------------
TLA50BCPU* __fastcall TLA50BConverter::GetItem(int Index)
{
    if( !CPUs )
        return NULL;
    if( Index < 0 || Index >= CPUs->Count )
        return NULL;

    return (TLA50BCPU*)CPUs->Items[Index];
}
//---------------------------------------------------------------------------
int __fastcall TLA50BConverter::IndexOfCPU(unsigned char ID)
{
    int i;

    for( i=0; i<CPUs->Count; i++ )
    {
        if( Items[i]->CPU_ID == ID )
            return i;
    }

    return -1;
}
//---------------------------------------------------------------------------
bool __fastcall TLA50BConverter::AddCPU(unsigned char ID)
{
    TLA50BCPU *cpu;

    if( CPUCount >= MAX_CPU_COUNT )
        return false;
    if( IndexOfCPU( ID ) >= 0 )     // already exists
        return false;

    cpu = new TLA50BCPU( MyOwner, ID );
	CPUs->Add( cpu );
	if( CPUs->Count > 0 )
	{
		if( LastCommCPUIdx < 0 )
			LastCommCPUIdx	= 0;
	}
	else
	{
		LastCommCPUIdx	= -1;
	}
	return true;
}
//---------------------------------------------------------------------------
bool __fastcall TLA50BConverter::DeleteCPU(unsigned char ID)
{
    int idx;

    idx = IndexOfCPU( ID );
    if( idx < 0 )
        return false;
	return ( DeleteCPU( idx ) );
}
//---------------------------------------------------------------------------
bool __fastcall TLA50BConverter::DeleteCPU(int Index)
{
	TLA50BCPU *cpu;

	if( Index < 0 || Index >= CPUs->Count )
		return false;

	cpu = Items[ Index ];
	CPUs->Delete( Index );
	delete cpu;
	cpu = NULL;

	if( CPUs->Count > 0 )
	{
		if( LastCommCPUIdx < 0 )
			LastCommCPUIdx	= 0;
	}
	else
	{
		LastCommCPUIdx	= -1;
	}
	return true;
}
//---------------------------------------------------------------------------
int __fastcall TLA50BConverter::GetCPUCount(void)
{
    if( !CPUs )
		return -1;
    return CPUs->Count;
}
//---------------------------------------------------------------------------
TPLCState __fastcall TLA50BConverter::GetCPUState(unsigned char CPU_ID)
{
    int idx;

    idx = IndexOfCPU( CPU_ID );
    if( idx < 0 )
        return PLC_OFF;
    return Items[ idx ]->State;
}
//---------------------------------------------------------------------------
bool __fastcall TLA50BConverter::SetCPUState(unsigned char CPU_ID,TPLCState State)
{
	int idx;

	idx = IndexOfCPU( CPU_ID );
	if( idx < 0 )
		return false;

	Items[ idx ]->State = State;
	return true;
}
//---------------------------------------------------------------------------
bool __fastcall TLA50BConverter::GetDI(int Number)
{
	int idx;

	idx	= CPUIdxOfDIO( Number );
	if( idx < 0 )
		return false;
	return Items[ idx ]->GetDI( Number );
}
//---------------------------------------------------------------------------
bool __fastcall TLA50BConverter::GetDI(String Code)
{
	int idx;

	idx	= CPUIdxOfDIO( Code );
	if( idx < 0 )
		return false;
	return Items[ idx ]->GetDI( Code );
}
//---------------------------------------------------------------------------
bool __fastcall TLA50BConverter::GetDO(int Number)
{
	int idx;

	idx	= CPUIdxOfDIO( Number );
	if( idx < 0 )
		return false;
	return Items[ idx ]->GetDO( Number );
}
//---------------------------------------------------------------------------
bool __fastcall TLA50BConverter::GetDO(String Code)
{
	int idx;

	idx	= CPUIdxOfDIO( Code );
	if( idx < 0 )
		return false;
	return Items[ idx ]->GetDO( Code );
}
//---------------------------------------------------------------------------
bool __fastcall TLA50BConverter::OH(int Number)
{
	int idx;

	idx	= CPUIdxOfDIO( Number );
	if( idx < 0 )
		return false;
	return Items[ idx ]->OH( Number );
}
//---------------------------------------------------------------------------
bool __fastcall TLA50BConverter::OH(String Code)
{
	int idx;

	idx	= CPUIdxOfDIO( Code );
	if( idx < 0 )
		return false;
	return Items[ idx ]->OH( Code );
}
//---------------------------------------------------------------------------
bool __fastcall TLA50BConverter::OL(int Number)
{
	int idx;

	idx	= CPUIdxOfDIO( Number );
	if( idx < 0 )
		return false;
	return Items[ idx ]->OL( Number );
}
//---------------------------------------------------------------------------
bool __fastcall TLA50BConverter::OL(String Code)
{
	int idx;

	idx	= CPUIdxOfDIO( Code );
	if( idx < 0 )
		return false;
	return Items[ idx ]->OL( Code );
}
//---------------------------------------------------------------------------
bool __fastcall TLA50BConverter::SetDI(int Number,bool High)
{
	int idx;

	idx	= CPUIdxOfDIO( Number );
	if( idx < 0 )
		return false;
	return Items[ idx ]->SetDI( Number, High );
}
//---------------------------------------------------------------------------
bool __fastcall TLA50BConverter::SetDI(String Code,bool High)
{
	int idx;

	idx	= CPUIdxOfDIO( Code );
	if( idx < 0 )
		return false;
	return Items[ idx ]->SetDI( Code, High );
}
//---------------------------------------------------------------------------
bool __fastcall TLA50BConverter::SetAI(int Number,WORD Value)
{
	int idx;

	idx	= CPUIdxOfAIO( Number );
	if( idx < 0 )
		return false;
	return Items[ idx ]->SetAI( Number, Value );
}
//---------------------------------------------------------------------------
bool __fastcall TLA50BConverter::SetAI(String Code,WORD Value)
{
	int idx;

	idx	= CPUIdxOfAIO( Code );
	if( idx < 0 )
		return false;
	return Items[ idx ]->SetAI( Code, Value );
}
//---------------------------------------------------------------------------
bool __fastcall TLA50BConverter::SetCounter(int Number,WORD Value)
{
	int idx;

	idx	= CPUIdxOfCounter( Number );
	if( idx < 0 )
		return false;
	return Items[ idx ]->SetCounter( Number, Value );
}
//---------------------------------------------------------------------------
bool __fastcall TLA50BConverter::SetCounter(String Code,WORD Value)
{
	int idx;

	idx	= CPUIdxOfCounter( Code );
	if( idx < 0 )
		return false;
	return Items[ idx ]->SetCounter( Code, Value );
}
//---------------------------------------------------------------------------
WORD __fastcall TLA50BConverter::GetAI(int Number)
{
	int idx;

	idx	= CPUIdxOfAIO( Number );
	if( idx < 0 )
		return 0;
	return Items[ idx ]->GetAI( Number );
}
//---------------------------------------------------------------------------
WORD __fastcall TLA50BConverter::GetAI(String Code)
{
	int idx;

	idx	= CPUIdxOfAIO( Code );
	if( idx < 0 )
		return 0;
	return Items[ idx ]->GetAI( Code );
}
//---------------------------------------------------------------------------
WORD __fastcall TLA50BConverter::GetAO(int Number)
{
	int idx;

	idx	= CPUIdxOfAIO( Number );
	if( idx < 0 )
		return 0;
	return Items[ idx ]->GetAO( Number );
}
//---------------------------------------------------------------------------
WORD __fastcall TLA50BConverter::GetAO(String Code)
{
	int idx;

	idx	= CPUIdxOfAIO( Code );
	if( idx < 0 )
		return 0;
	return Items[ idx ]->GetAO( Code );
}
//---------------------------------------------------------------------------
bool __fastcall TLA50BConverter::SetAO(int Number,WORD Value)
{
	int idx;

	idx	= CPUIdxOfAIO( Number );
	if( idx < 0 )
		return false;
	return Items[ idx ]->SetAO( Number, Value );
}
//---------------------------------------------------------------------------
bool __fastcall TLA50BConverter::SetAO(String Code,WORD Value)
{
	int idx;

	idx	= CPUIdxOfAIO( Code );
	if( idx < 0 )
		return false;
	return Items[ idx ]->SetAO( Code, Value );
}
//---------------------------------------------------------------------------
WORD __fastcall TLA50BConverter::GetCounter(int Number)
{
	int idx;

	idx	= CPUIdxOfCounter( Number );
	if( idx < 0 )
		return 0;
	return Items[ idx ]->GetCounter( Number );
}
//---------------------------------------------------------------------------
WORD __fastcall TLA50BConverter::GetCounter(String Code)
{
	int idx;

	idx	= CPUIdxOfCounter( Code );
	if( idx < 0 )
		return 0;
	return Items[ idx ]->GetCounter( Code );
}
//---------------------------------------------------------------------------
double __fastcall TLA50BConverter::GetTemperature(int Number)
{
	int idx;

	idx	= CPUIdxOfAIO( Number );
	if( idx < 0 )
		return 0;
	return Items[ idx ]->GetTemperature( Number );
}
//---------------------------------------------------------------------------
double __fastcall TLA50BConverter::GetTemperature(String Code)
{
	int idx;

	idx	= CPUIdxOfAIO( Code );
	if( idx < 0 )
		return 0;
	return Items[ idx ]->GetTemperature( Code );
}
//---------------------------------------------------------------------------
int __fastcall TLA50BConverter::TxNextData(char *Data)
{
	LastCommCPUIdx++;
	if( LastCommCPUIdx >= CPUCount )
		LastCommCPUIdx  = 0;

	return Items[ LastCommCPUIdx ]->TxData( Data );
}
//---------------------------------------------------------------------------
int __fastcall TLA50BConverter::ReGetTxData(char *Data)
{
    // Resend don't change Polling CPU Index

    return Items[ LastCommCPUIdx ]->TxData( Data );
}
//---------------------------------------------------------------------------
RxState __fastcall TLA50BConverter::CheckRxData(unsigned char *Data,int Length)
{
	return Items[ LastCommCPUIdx ]->ProcessRxData( Data, Length );
}
//---------------------------------------------------------------------------
void __fastcall TLA50BConverter::OLAll(void)
{
    int i;

    for( i=0; i<CPUCount; i++ )
        Items[i]->OLAll();
}
//---------------------------------------------------------------------------
bool __fastcall TLA50BConverter::CheckWtAvailable(unsigned char CPU_ID)
{
	int idx;

	idx	= IndexOfCPU( CPU_ID );
	if( idx < 0 )
		return false;
	return Items[idx]->BalanceWeightFlag;
}
//---------------------------------------------------------------------------
double __fastcall TLA50BConverter::ScaleWeight(unsigned char CPU_ID)
{
	int idx;

	idx	= IndexOfCPU( CPU_ID );
	if( idx < 0 )
		return 0;

	String wt;
	if( Items[idx]->ReadBalanceWeight( wt ) )
		return wt.ToDouble();
	return 0;
}
//---------------------------------------------------------------------------
void __fastcall TLA50BConverter::ScaleZero(unsigned char CPU_ID)
{
	int idx;

	idx	= IndexOfCPU( CPU_ID );
	if( idx < 0 )
		return;
	Items[idx]->TareBalanceZ();
}
//---------------------------------------------------------------------------
void __fastcall TLA50BConverter::ScaleTare(unsigned char CPU_ID)
{
	int idx;

	idx	= IndexOfCPU( CPU_ID );
	if( idx < 0 )
		return;
	Items[idx]->TareBalanceT();
}
//---------------------------------------------------------------------------
bool __fastcall TLA50BConverter::GetDI(unsigned char CPU_ID,int Module,int Point)
{
	int idx;

	idx	= IndexOfCPU( CPU_ID );
	if( idx < 0 )
		return false;
	return Items[idx]->GetDI( Module, Point );
}
//---------------------------------------------------------------------------
bool __fastcall TLA50BConverter::GetDO(unsigned char CPU_ID,int Module,int Point)
{
	int idx;

	idx	= IndexOfCPU( CPU_ID );
	if( idx < 0 )
		return false;
	return Items[idx]->GetDO( Module, Point );
}
//---------------------------------------------------------------------------
bool __fastcall TLA50BConverter::OH(unsigned char CPU_ID,int Module,int Point)
{
	int idx;

	idx = IndexOfCPU( CPU_ID );
	if( idx < 0 )
		return false;

	return Items[ idx ]->OH( Module, Point );
}
//---------------------------------------------------------------------------
bool __fastcall TLA50BConverter::OL(unsigned char CPU_ID,int Module,int Point)
{
	int idx;

	idx = IndexOfCPU( CPU_ID );
	if( idx < 0 )
		return false;

	return Items[ idx ]->OL( Module, Point );
}
//---------------------------------------------------------------------------
WORD __fastcall TLA50BConverter::GetAI(unsigned char CPU_ID,int Module,int Point)
{
	int idx;

	idx = IndexOfCPU( CPU_ID );
	if( idx < 0 )
		return 0;

	return Items[ idx ]->GetAI( Module, Point );
}
//---------------------------------------------------------------------------
WORD __fastcall TLA50BConverter::GetAO(unsigned char CPU_ID,int Module,int Point)
{
	int idx;

	idx = IndexOfCPU( CPU_ID );
	if( idx < 0 )
		return 0;

	return Items[ idx ]->GetAO( Module, Point );
}
//---------------------------------------------------------------------------
bool __fastcall TLA50BConverter::SetAO(unsigned char CPU_ID,int Module,int Point,WORD Value)
{
	int idx;

	idx = IndexOfCPU( CPU_ID );
	if( idx < 0 )
		return false;

	return Items[ idx ]->SetAO( Module, Point, Value );
}
//---------------------------------------------------------------------------
WORD __fastcall TLA50BConverter::GetCounter(unsigned char CPU_ID
	,int Module,int Point)
{
	int idx;

	idx	= IndexOfCPU( CPU_ID );
	if( idx < 0 )
		return 0;
	return Items[idx]->GetCounter( Module, Point );
}
//---------------------------------------------------------------------------
double __fastcall TLA50BConverter::GetTemperature(unsigned char CPU_ID
	,int Module,int Point)
{
	int idx;

	idx	= IndexOfCPU( CPU_ID );
	if( idx < 0 )
		return 0;
	return Items[idx]->GetTemperature( Module, Point );
}
//---------------------------------------------------------------------------
double __fastcall TLA50BConverter::GetTemperature(TLA50BIO *IO)
{
	if( !IO )
		return 0;
	return GetTemperature( IO->CPU_ID, IO->Module, IO->Point );
}
//---------------------------------------------------------------------------
bool __fastcall TLA50BConverter::DefineIOCode(ioTYPE Type,int Number,String Code)
{
	int idx;
	
	switch( Type )
	{
		case ioDI:
		case ioDO:
			idx	= CPUIdxOfDIO( Number );
			break;

		case ioAI:
		case ioAO:
			idx	= CPUIdxOfAIO( Number );
			break;

		case ioCOUNTER:
			idx	= CPUIdxOfCounter( Number );
			break;

		default:
			return false;
	}
	if( idx < 0 )
		return false;
	return Items[ idx ]->DefineIOCode( Type, Number, Code );
}
//---------------------------------------------------------------------------
bool __fastcall TLA50BConverter::LinkIO(TLA50BIO *IO)
{
	int idx;

	if( !IO )
		return false;

	if( IO->IONumber >= 0 )
	{
		switch( IO->Type )
		{
			case ioDI:
			case ioDO:
				idx	= CPUIdxOfDIO( IO->IONumber );
				break;

			case ioAI:
			case ioAO:
				idx	= CPUIdxOfAIO( IO->IONumber );
				break;

			case ioCOUNTER:
				idx	= CPUIdxOfCounter( IO->IONumber );
				break;

			default:
				idx	= -1;
		}
	}
	else if( !IO->Code.Trim().IsEmpty() )
	{
		switch( IO->Type )
		{
			case ioDI:
			case ioDO:
				idx	= CPUIdxOfDIO( IO->Code.Trim() );
				break;

			case ioAI:
			case ioAO:
				idx	= CPUIdxOfAIO( IO->Code.Trim() );
				break;

			case ioCOUNTER:
				idx	= CPUIdxOfCounter( IO->Code.Trim() );
				break;

			default:
				idx	= -1;
		}
	}
	else
	{
		idx	= IndexOfCPU( IO->CPU_ID );
	}
	if( idx < 0 )
		return false;
	return Items[ idx ]->LinkIO( IO );
}
//---------------------------------------------------------------------------

