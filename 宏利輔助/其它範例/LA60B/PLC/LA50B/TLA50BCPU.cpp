//---------------------------------------------------------------------------


#pragma hdrstop

#include "TLA50BCPU.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
__fastcall TLA50BCPU::TLA50BCPU(TComponent* Owner,unsigned char cpu_id)
	: TPLCBasic(Owner,LA50B,cpu_id)
{
    int j;

	MyOwner = Owner;
	State	= PLC_OFF;

	memset( &pc_2_plc_data_detail, 0, sizeof( pc_2_plc_data_detail ) );

	pc_2_plc_data_detail.plc_id             = cpu_id;
    pc_2_plc_data_detail.leading_id         = LEADING_ID;
    pc_2_plc_data_detail.length_id          = LENGTH_ID;
    pc_2_plc_data_detail.output_id          = OUTPUT_ID;
    pc_2_plc_data_detail.analog_id          = ANALOG_ID;
    pc_2_plc_data_detail.register_id        = REGISTER_ID;
    pc_2_plc_data_detail.machine_id         = MACHINE_ID;
    pc_2_plc_data_detail.balance_command_id = BALANCE_COMMAND_ID;
	pc_2_plc_data_detail.checksum_id        = CHECKSUM_ID;
    pc_2_plc_data_detail.length             = sizeof( pc_2_plc_data_detail );

    for( j=0;j<OUTPUT_BYTES;j++)
		pc_2_plc_data_detail.output[j]  = 0;      // clear all output

	// Temperature parametr default value
	TempPLCValue[ TEMP_LOW_BYTE ]	= 144;
	TempPLCValue[ TEMP_HIGH_BYTE ]	= 1483;
	TempRealValue[ TEMP_LOW_BYTE ]	= 12.5;
	TempRealValue[ TEMP_HIGH_BYTE ]	= 138.8;

	DIList	= new TList();
	if( DIList )
		DIList->Clear();
	DOList	= new TList();
	if( DOList )
		DOList->Clear();
	AIList	= new TList();
	if( AIList )
		AIList->Clear();
	AOList	= new TList();
	if( AOList )
		AOList->Clear();
	CounterList	= new TList();
	if( CounterList )
		CounterList->Clear();

	BalanceWeightFlag	= false;
}
//---------------------------------------------------------------------------
__fastcall TLA50BCPU::~TLA50BCPU()
{
	if( DIList )
	{
		while( DIList->Count > 0 )
			DeleteIO( DIList, 0 );
		delete DIList;
		DIList	= NULL;
	}
	if( DOList )
	{
		while( DOList->Count > 0 )
			DeleteIO( DOList, 0 );
		delete DOList;
		DOList	= NULL;
	}
	if( AIList )
	{
		while( AIList->Count > 0 )
			DeleteIO( AIList, 0 );
		delete AIList;
		AIList	= NULL;
	}
	if( AOList )
	{
		while( AOList->Count > 0 )
			DeleteIO( AOList, 0 );
		delete AOList;
		AOList	= NULL;
	}
	if( CounterList )
	{
		while( CounterList->Count > 0 )
			DeleteIO( CounterList, 0 );
		delete CounterList;
		CounterList	= NULL;
	}
}
//---------------------------------------------------------------------------
bool __fastcall TLA50BCPU::DeleteIO(TList *IOList,int Index)
{
	TLA50BIO *io;

	if( !IOList )
		return false;

	if( Index < 0 || Index >= IOList->Count )
		return false;

	io	= (TLA50BIO*)IOList->Items[Index];
	IOList->Delete( Index );
	delete io;
	io	= NULL;
	return true;
}
//---------------------------------------------------------------------------
bool __fastcall TLA50BCPU::AddIOs(TList *IOList,ioTYPE Type,int StartNumber,int Count)
{
	TLA50BIO *io;
	int i;

	if( !IOList )
		return false;

	for( i=0; i<Count; i++ )
	{
		io	= new TLA50BIO( Type );
		io->IONumber	= StartNumber + i;
		IOList->Add( io );
	}
	return true;
}
//---------------------------------------------------------------------------
int __fastcall TLA50BCPU::NumberOfIOCode(TList *IOList,String Code)
{
	int i;

	if( !IOList )
		return -1;
	if( Code.IsEmpty() )
		return -1;

	for( i=0; i<IOList->Count; i++ )
	{
		if( ( (TLA50BIO*)IOList->Items[i] )->Code == Code )
			return ( (TLA50BIO*)IOList->Items[i] )->IONumber;
	}
	return -1;
}
//---------------------------------------------------------------------------
int __fastcall TLA50BCPU::IndexOfIONumber(TList *IOList,int IONumber)
{
	int i;

	if( !IOList )
		return -1;
	if( IONumber < 0 )
		return -1;

	for( i=0; i<IOList->Count; i++ )
	{
		if( ( (TLA50BIO*)IOList->Items[i] )->IONumber == IONumber )
			return i;
	}
	return -1;
}
//---------------------------------------------------------------------------
bool __fastcall TLA50BCPU::DefineDIONumber(int StartNumber,int Length)
{
	bool result;

	if( StartNumber < 0 || Length < 0 || Length > DIO_MAX_LENGTH )
		return false;

	DIOStartNumber	= StartNumber;
	DIOLength		= Length;

	AddIOs( DIList, ioDI, StartNumber, Length );
	AddIOs( DOList, ioDO, StartNumber, Length );

	return true;
}
//---------------------------------------------------------------------------
bool __fastcall TLA50BCPU::DefineAIONumber(int StartNumber,int Length)
{
	if( StartNumber < 0 || Length < 0 || Length > AIO_MAX_LENGTH )
		return false;

	AIOStartNumber	= StartNumber;
	AIOLength		= Length;

	AddIOs( AIList, ioAI, StartNumber, Length );
	AddIOs( AOList, ioAO, StartNumber, Length );

	return true;
}
//---------------------------------------------------------------------------
bool __fastcall TLA50BCPU::DefineCounterNumber(int StartNumber,int Length)
{
	if( StartNumber < 0 || Length < 0 || Length > COUNTER_MAX_LENGTH )
		return false;

	CounterStartNumber	= StartNumber;
	CounterLength		= Length;

	AddIOs( CounterList, ioCOUNTER, StartNumber, Length );

	return true;
}
//---------------------------------------------------------------------------
bool __fastcall TLA50BCPU::DefineDICode(int IONumber,String Code)
{
	int idx;

	if( Code.Trim().IsEmpty() )
		return false;

	idx	= IndexOfIONumber( DIList, IONumber );
	if( idx < 0 )
		return false;

	( (TLA50BIO*)DIList->Items[ idx ] )->Code	= Code.Trim();
	return true;
}
//---------------------------------------------------------------------------
bool __fastcall TLA50BCPU::DefineDOCode(int IONumber,String Code)
{
	int idx;

	if( Code.Trim().IsEmpty() )
		return false;

	idx	= IndexOfIONumber( DOList, IONumber );
	if( idx < 0 )
		return false;

	( (TLA50BIO*)DOList->Items[ idx ] )->Code	= Code.Trim();
	return true;
}
//---------------------------------------------------------------------------
bool __fastcall TLA50BCPU::DefineAICode(int IONumber,String Code)
{
	int idx;

	if( Code.Trim().IsEmpty() )
		return false;

	idx	= IndexOfIONumber( AIList, IONumber );
	if( idx < 0 )
		return false;

	( (TLA50BIO*)AIList->Items[ idx ] )->Code	= Code.Trim();
	return true;
}
//---------------------------------------------------------------------------
bool __fastcall TLA50BCPU::DefineAOCode(int IONumber,String Code)
{
	int idx;

	if( Code.Trim().IsEmpty() )
		return false;

	idx	= IndexOfIONumber( AOList, IONumber );
	if( idx < 0 )
		return false;

	( (TLA50BIO*)AOList->Items[ idx ] )->Code	= Code.Trim();
	return true;
}
//---------------------------------------------------------------------------
bool __fastcall TLA50BCPU::DefineCounterCode(int IONumber,String Code)
{
	int idx;

	if( Code.Trim().IsEmpty() )
		return false;

	idx	= IndexOfIONumber( CounterList, IONumber );
	if( idx < 0 )
		return false;

	( (TLA50BIO*)CounterList->Items[ idx ] )->Code	= Code.Trim();
	return true;
}
//---------------------------------------------------------------------------
bool __fastcall TLA50BCPU::IsDIOExists(int Number)
{
	int idx;

	idx	= Number - DIOStartNumber;
	if( idx >= 0 && idx < DIOLength )
		return true;
	return false;
}
//---------------------------------------------------------------------------
bool __fastcall TLA50BCPU::IsDIOExists(String Code)
{
	if( NumberOfIOCode( DIList, Code ) >= 0 )
		return true;
	if( NumberOfIOCode( DOList, Code ) >= 0 )
		return true;
	return false;
}
//---------------------------------------------------------------------------
bool __fastcall TLA50BCPU::IsAIOExists(int Number)
{
	int idx;

	idx	= Number - AIOStartNumber;
	if( idx >=0 && idx < AIOLength )
		return true;
	return false;
}
//---------------------------------------------------------------------------
bool __fastcall TLA50BCPU::IsAIOExists(String Code)
{
	if( NumberOfIOCode( AIList, Code ) >= 0 )
		return true;
	if( NumberOfIOCode( AOList, Code ) >= 0 )
		return true;
	return false;
}
//---------------------------------------------------------------------------
bool __fastcall TLA50BCPU::IsCounterExists(int Number)
{
	int idx;

	idx	= Number - CounterStartNumber;
	if( idx >= 0 && idx < CounterLength )
		return true;
	return false;
}
//---------------------------------------------------------------------------
bool __fastcall TLA50BCPU::IsCounterExists(String Code)
{
	if( NumberOfIOCode( CounterList, Code ) >= 0 )
		return true;
	return false;
}
//---------------------------------------------------------------------------
bool __fastcall TLA50BCPU::GetDIValue(int Index)
{
	int byte_idx;
	int bit_idx;

	if( Index < 0 || Index >= DIOLength )
		return false;

	byte_idx	= Index / 8;
	bit_idx		= Index % 8;
	return ( ( plc_2_pc_data_detail.input[ byte_idx ] >> bit_idx ) & 0x01 );
}
//---------------------------------------------------------------------------
bool __fastcall TLA50BCPU::GetDOValue(int Index)
{
	int byte_idx;
	int bit_idx;

	if( Index < 0 || Index >= DIOLength )
		return false;

	byte_idx	= Index / 8;
	bit_idx		= Index % 8;
	return ( ( pc_2_plc_data_detail.output[ byte_idx ] >> bit_idx ) & 0x01 );
}
//---------------------------------------------------------------------------
bool __fastcall TLA50BCPU::SetDOValue(int Index,bool IsHigh)
{
	int byte_idx;
	int bit_idx;
	unsigned char mask;

	if( Index < 0 || Index >= DIOLength )
		return false;

	byte_idx	= Index / 8;
	bit_idx		= Index % 8;
	mask		= 0x01 << bit_idx;
	if( IsHigh )
		pc_2_plc_data_detail.output[ byte_idx ]	|= mask;
	else
		pc_2_plc_data_detail.output[ byte_idx ]	&= ~mask;
	return true;
}
//---------------------------------------------------------------------------
bool __fastcall TLA50BCPU::SetDIValue(int Index,bool IsHigh)
{
	int byte_idx;
	int bit_idx;
	unsigned char mask;

	if( Index < 0 || Index >= DIOLength )
		return false;

	byte_idx	= Index / 8;
	bit_idx		= Index % 8;
	mask		= 0x01 << bit_idx;
	if( IsHigh )
		plc_2_pc_data_detail.input[ byte_idx ]	|= mask;
	else
		plc_2_pc_data_detail.input[ byte_idx ]	&= ~mask;
	return true;
}
//---------------------------------------------------------------------------
bool __fastcall TLA50BCPU::GetDI(int Number)
{
	int idx;

	idx	= Number - DIOStartNumber;
	return GetDIValue( idx );
}
//---------------------------------------------------------------------------
bool __fastcall TLA50BCPU::GetDI(String Code)
{
	int num;

	num	= NumberOfIOCode( DIList, Code );
	if( num < 0 )
		return false;
	return GetDI( num );
}
//---------------------------------------------------------------------------
bool __fastcall TLA50BCPU::GetDO(int Number)
{
	int idx;

	idx	= Number - DIOStartNumber;
	return GetDOValue( idx );
}
//---------------------------------------------------------------------------
bool __fastcall TLA50BCPU::GetDO(String Code)
{
	int num;

	num	= NumberOfIOCode( DOList, Code );
	if( num < 0 )
		return false;
	return GetDO( num );
}
//---------------------------------------------------------------------------
bool __fastcall TLA50BCPU::SetDI(int Number,bool High)
{
	int idx;

	idx	= Number - DIOStartNumber;
	if( idx < 0 || idx >= DIOLength )
		return false;

	return SetDIValue( idx, High );
}
//---------------------------------------------------------------------------
bool __fastcall TLA50BCPU::SetDI(String Code,bool High)
{
	int num;

	num	= NumberOfIOCode( DIList, Code );
	if( num < 0 )
		return false;
	return SetDI( num, High );
}
//---------------------------------------------------------------------------
bool __fastcall TLA50BCPU::SetAI(int Number,WORD Value)
{
	int idx;

	idx	= Number - AIOStartNumber;
	if( idx < 0 || idx >= AIOLength )
		return false;

	return SetAIValue( idx, Value );
}
//---------------------------------------------------------------------------
bool __fastcall TLA50BCPU::SetAI(String Code,WORD Value)
{
	int num;

	num	= NumberOfIOCode( AIList, Code );
	if( num < 0 )
		return false;
	return SetAI( num, Value );
}
//---------------------------------------------------------------------------
bool __fastcall TLA50BCPU::OH(int Number)
{
	int idx;

	idx	= Number - DIOStartNumber;
	if( idx < 0 || idx >= DIOLength )
		return false;

	return SetDOValue( idx, true );
}
//---------------------------------------------------------------------------
bool __fastcall TLA50BCPU::OH(String Code)
{
	int num;

	num	= NumberOfIOCode( DOList, Code );
	if( num < 0 )
		return false;
	return OH( num );
}
//---------------------------------------------------------------------------
bool __fastcall TLA50BCPU::OL(int Number)
{
	int idx;

	idx	= Number - DIOStartNumber;
	if( idx < 0 || idx >= DIOLength )
		return false;

	return SetDOValue( idx, false );
}
//---------------------------------------------------------------------------
bool __fastcall TLA50BCPU::OL(String Code)
{
	int num;

	num	= NumberOfIOCode( DOList, Code );
	if( num < 0 )
		return false;
	return OL( num );
}
//---------------------------------------------------------------------------
void __fastcall TLA50BCPU::OLAll(void)
{
	int i;

	for( i=0 ; i<OUTPUT_BYTES ; i++ )
		pc_2_plc_data_detail.output[i]	= 0;

	for( i=0 ; i<ANALOG_OUTPUT_BYTES ; i++ )
		pc_2_plc_data_detail.analog[i]	= 0;
}
//---------------------------------------------------------------------------
bool __fastcall TLA50BCPU::ReadBalanceWeight(AnsiString &weight)
{
    weight = BalanceWeight;
    return BalanceWeightFlag;
}
//---------------------------------------------------------------------------
void __fastcall TLA50BCPU::CalcuBalanceWeight(void)
{
	char TmpBuff[BALANCE_STR_BYTES];
	char WeightStr[80];
	int i;
	int j=0;


	memcpy( TmpBuff, plc_2_pc_data_detail.balance_str, BALANCE_STR_BYTES );
//	strcpy( TmpBuff, plc_2_pc_data_detail.balance_str );
    if( TmpBuff[0] == 'O' )                // balance off-line
    {
        BalanceWeightFlag = false;
        return;
    }
    for( i=0;i<BALANCE_STR_BYTES;i++)
    {
        if ( (TmpBuff[i] >= '0' && TmpBuff[i] <= '9') ||
            TmpBuff[i] == '.' || TmpBuff[i] == '+'  || TmpBuff[i] == '-' )
        {
            WeightStr[j++] = TmpBuff[i];
        }
        else if ( TmpBuff[i] == 13 )
        {
            break;
        }
    }
    WeightStr[j] = 0;
    if (j > 0)
    {
        try
        {
            BalanceWeight       = AnsiString(WeightStr);
			BalanceWeightFlag   = true;
        }
        catch(...)
        {
            BalanceWeightFlag   = false;
        }
    }
    else
    {
        BalanceWeightFlag   = false;
    }
}
//---------------------------------------------------------------------------
void __fastcall TLA50BCPU::TareBalanceT(void)
{
	char TareCommandStr[BALANCE_COMMAND_BYTES];

	memset( TareCommandStr, 0, BALANCE_COMMAND_BYTES );
	TareCommandStr[0] = 'T';
	TareCommandStr[1] = 13;
	TareCommandStr[2] = 10;
	TareCommandStr[3] = 0;
	memcpy( pc_2_plc_data_detail.balance_command, TareCommandStr, BALANCE_COMMAND_BYTES );
//	strcpy( pc_2_plc_data_detail.balance_command, TareCommandStr );
}
//---------------------------------------------------------------------------
void __fastcall TLA50BCPU::TareBalanceZ(void)
{
	char TareCommandStr[BALANCE_COMMAND_BYTES];

	memset( TareCommandStr, 0, BALANCE_COMMAND_BYTES );
	TareCommandStr[0] = 'Z';
	TareCommandStr[1] = 13;
    TareCommandStr[2] = 10;
    TareCommandStr[3] = 0;
	memcpy( pc_2_plc_data_detail.balance_command, TareCommandStr, BALANCE_COMMAND_BYTES );
//	strcpy( pc_2_plc_data_detail.balance_command, TareCommandStr );
}
//---------------------------------------------------------------------------
int __fastcall TLA50BCPU::CalcAnalogIndex(int Module,int Point)
{
	return ( Module * AIO_MODULE_POINTS + Point );
}
//---------------------------------------------------------------------------
int __fastcall TLA50BCPU::CalcCounterIndex(int Module,int Point)
{
	return ( Module * COUNTER_MODULE_POINTS + Point );
}
//---------------------------------------------------------------------------
//
//	Method : GetAIValue( int Index )
//
//	Note : WORD sequence is LowByte + HighByte when Input
//
//---------------------------------------------------------------------------
WORD __fastcall TLA50BCPU::GetAIValue(int Index)
{
	WORD value;
	int low_byte ,high_byte;


	if( Index < 0 || Index > AIOLength )
		return 0;

	low_byte	= Index * 2;
	high_byte	= low_byte + 1;

	value   = ( plc_2_pc_data_detail.analog[ high_byte ] << 8 )
				+ plc_2_pc_data_detail.analog[ low_byte ];

	return value;
}
//---------------------------------------------------------------------------
WORD __fastcall TLA50BCPU::GetAI(int Number)
{
	int idx;

	idx	= Number - AIOStartNumber;
	return GetAIValue( idx );
}
//---------------------------------------------------------------------------
WORD __fastcall TLA50BCPU::GetAI(String Code)
{
	int num;

	num	= NumberOfIOCode( AIList, Code );
	if( num < 0 )
		return false;
	return GetAI( num );
}
//---------------------------------------------------------------------------
//
//	Method : GetAOValue( int Index )
//
//	Note : WORD sequence is HighByte + LowByte when Output
//
//---------------------------------------------------------------------------
WORD __fastcall TLA50BCPU::GetAOValue(int Index)
{
	WORD value;
	int low_byte ,high_byte;


	if( Index < 0 || Index > AIOLength )
		return 0;

	high_byte	= Index * 2;
	low_byte	= high_byte + 1;

	value   = ( pc_2_plc_data_detail.analog[ high_byte ] << 8 )
				+ pc_2_plc_data_detail.analog[ low_byte ];

	return value;
}
//---------------------------------------------------------------------------
WORD __fastcall TLA50BCPU::GetAO(int Number)
{
	int idx;

	idx	= Number - AIOStartNumber;
	return GetAOValue( idx );
}
//---------------------------------------------------------------------------
WORD __fastcall TLA50BCPU::GetAO(String Code)
{
	int num;

	num	= NumberOfIOCode( AOList, Code );
	if( num < 0 )
		return false;
	return GetAO( num );
}
//---------------------------------------------------------------------------
//
//	Method : SetAIValue
//
//	Note : WORD sequence is LowByte + HighByte when Input
//
//---------------------------------------------------------------------------
bool __fastcall TLA50BCPU::SetAIValue(int Index,WORD Value)
{
	int low_byte ,high_byte;

	if( Index < 0 || Index > AIOLength )
		return false;

	low_byte	= Index * 2;
	high_byte	= low_byte + 1;

	pc_2_plc_data_detail.analog[ high_byte ]    = ( Value >> 8 ) & 0x00FF;
	pc_2_plc_data_detail.analog[ low_byte ]		= Value & 0x00FF;
	return true;
}
//---------------------------------------------------------------------------
//
//	Method : SetAOValue
//
//	Note : WORD sequence is HighByte + LowByte when Output
//
//---------------------------------------------------------------------------
bool __fastcall TLA50BCPU::SetAOValue(int Index,WORD Value)
{
	int low_byte ,high_byte;

	if( Index < 0 || Index > AIOLength )
		return false;

	high_byte	= Index * 2;
	low_byte	= high_byte + 1;

	pc_2_plc_data_detail.analog[ high_byte ]    = ( Value >> 8 ) & 0x00FF;
	pc_2_plc_data_detail.analog[ low_byte ]		= Value & 0x00FF;
	return true;
}
//---------------------------------------------------------------------------
bool __fastcall TLA50BCPU::SetAO(int Number,WORD Value)
{
	int idx;

	idx	= Number - AIOStartNumber;
	return SetAOValue( idx, Value );
}
//---------------------------------------------------------------------------
bool __fastcall TLA50BCPU::SetAO(String Code,WORD Value)
{
	int num;

	num	= NumberOfIOCode( AOList, Code );
	if( num < 0 )
		return false;
	return SetAO( num, Value );
}
//---------------------------------------------------------------------------
bool __fastcall TLA50BCPU::SetAO(int Number,double Percent)
{
	WORD value;

	if( Percent < 0 )
		Percent	= 0;
	else if( Percent > 1 )
		Percent	= 1;
	value   = 0xFFFF * Percent;
	return SetAO( Number, value );
}
//---------------------------------------------------------------------------
bool __fastcall TLA50BCPU::SetAO(String Code,double Percent)
{
	int num;

	num	= NumberOfIOCode( AOList, Code );
	if( num < 0 )
		return false;
	return SetAO( num, Percent );
}
//---------------------------------------------------------------------------
//
// Func : SetCounterValue
//
// Note :
//      Serial = 0 : Byte 0 + Byte 1 ( Low Byte + High Byte )
//             = 1 : Byte 2 + Byte 3
//
//      equal ==> Byte[ Serial*2 ] + Byte[ Serial*2 + 1 ]
//
//---------------------------------------------------------------------------
bool __fastcall TLA50BCPU::SetCounterValue(int Index,WORD Value)
{
	int low_byte ,high_byte;

	if( Index < 0 || Index >= CounterLength )
		return false;

	low_byte	= Index * 2;
	high_byte	= low_byte + 1;

	plc_2_pc_data_detail.counter[ high_byte ]	= ( Value >> 8 ) & 0x00FF;;
	plc_2_pc_data_detail.counter[ low_byte ]	= Value & 0x00FF;;
	return true;
}
//---------------------------------------------------------------------------
bool __fastcall TLA50BCPU::SetCounter(int Number,WORD Value)
{
	int idx;

	idx	= Number - CounterStartNumber;
	return SetCounterValue( idx, Value );
}
//---------------------------------------------------------------------------
bool __fastcall TLA50BCPU::SetCounter(String Code,WORD Value)
{
	int num;

	num	= NumberOfIOCode( CounterList, Code );
	if( num < 0 )
		return false;
	return SetCounter( num, Value );
}
//---------------------------------------------------------------------------
//
// Func : GetCounterValue
//
// Note :
//      Serial = 0 : Byte 0 + Byte 1 ( Low Byte + High Byte )
//             = 1 : Byte 2 + Byte 3
//
//      equal ==> Byte[ Serial*2 ] + Byte[ Serial*2 + 1 ]
//
//---------------------------------------------------------------------------
WORD __fastcall TLA50BCPU::GetCounterValue(int Index)
{
	int low_byte ,high_byte;
	WORD value;

	if( Index < 0 || Index >= CounterLength )
		return 0;

	low_byte	= Index * 2;
	high_byte	= low_byte + 1;

	value	= plc_2_pc_data_detail.counter[ high_byte ] << 8
				+ plc_2_pc_data_detail.counter[ low_byte ];
	return value;
}
//---------------------------------------------------------------------------
WORD __fastcall TLA50BCPU::GetCounter(int Number)
{
	int idx;

	idx	= Number - CounterStartNumber;
	return GetCounterValue( idx );
}
//---------------------------------------------------------------------------
WORD __fastcall TLA50BCPU::GetCounter(String Code)
{
	int num;

	num	= NumberOfIOCode( CounterList, Code );
	if( num < 0 )
		return false;
	return GetCounter( num );
}
//---------------------------------------------------------------------------
double __fastcall TLA50BCPU::CalcTemperatureValue(int PLCValue)
{
	double TempValue;
	double Multiple;

	Multiple	= double(( PLCValue - TempPLCValue[TEMP_LOW_BYTE] ))
					/ double(( TempPLCValue[TEMP_HIGH_BYTE] - TempPLCValue[TEMP_LOW_BYTE] ));

	TempValue   = TempRealValue[TEMP_LOW_BYTE] + ( TempRealValue[TEMP_HIGH_BYTE] - TempRealValue[TEMP_LOW_BYTE] ) * Multiple;

	return TempValue;
}
//---------------------------------------------------------------------------
double __fastcall TLA50BCPU::GetTemperature(int Number)
{
	WORD plc_value;

	plc_value	= GetAI( Number );
	return CalcTemperatureValue( plc_value );
}
//---------------------------------------------------------------------------
double __fastcall TLA50BCPU::GetTemperature(String Code)
{
	int num;

	num	= NumberOfIOCode( AIList, Code );
	if( num < 0 )
		return false;
	return GetTemperature( num );
}
//---------------------------------------------------------------------------
int __fastcall TLA50BCPU::TxData(char *Data)
{
    int i;
    unsigned short checksum_value = 0;


    memcpy( Data, &pc_2_plc_data_detail, pc_2_plc_data_detail.length );

    for ( i = 0 ; i<(pc_2_plc_data_detail.length-2) ; i++)                           // calculate checksum
        checksum_value = checksum_value + Data[i];
    pc_2_plc_data_detail.checksum = checksum_value;

    memcpy( Data, &pc_2_plc_data_detail, pc_2_plc_data_detail.length );

    if( pc_2_plc_data_detail.balance_command[0] != 0 )
    {
        for( i=0;i<BALANCE_COMMAND_BYTES;i++ )       // clear balance command after transmit
            pc_2_plc_data_detail.balance_command[i] = 0;
	}
	return pc_2_plc_data_detail.length;
}
//---------------------------------------------------------------------------
int __fastcall TLA50BCPU::GetRxLength(void)
{
	return sizeof( plc_2_pc_data_detail );
}
//---------------------------------------------------------------------------
RxState __fastcall TLA50BCPU::ProcessRxData(unsigned char *RxData,int Length)
{
    int i;
	unsigned short checksum_value;
	PLC_DATA_DETAIL Buffer;

	if( Length == RxDataLength )
	{
		memcpy( &Buffer, RxData, Length );
		checksum_value = 0;

		for( i = 0 ; i < Length - 2; i++ )
			checksum_value = checksum_value + RxData[i];

		if (checksum_value != Buffer.checksum )
		{
			return COMM_CHECKSUM_ERR;    // checksum error
		}
        else
		{                                   // successful
			GetRxData( RxData );
			return COMM_RX_OK;
		}
    }
	else if( Length > RxDataLength )
	{
		return COMM_LENGTH_ERR;     // over length
    }


	return COMM_WAIT_RESPONSE;	// not transfer completed
}
//---------------------------------------------------------------------------
void __fastcall TLA50BCPU::GetRxData(unsigned char *Data)
{
	memcpy( &plc_2_pc_data_detail, Data, RxDataLength );
	CalcuBalanceWeight();
}
//---------------------------------------------------------------------------
bool __fastcall TLA50BCPU::GetDI(int Module,int Point)
{
	int idx;

	idx = Module * DIO_INPUT_POINTS + Point;
	return GetDIValue( idx );
}
//---------------------------------------------------------------------------
bool __fastcall TLA50BCPU::GetDO(int Module,int Point)
{
	int idx;

	idx = Module * DIO_OUTPUT_POINTS + Point;
	return GetDOValue( idx );
}
//---------------------------------------------------------------------------
bool __fastcall TLA50BCPU::OH(int DIO,int Point)
{
	int idx;

	idx = DIO * DIO_OUTPUT_POINTS + Point;
	return SetDOValue( idx, true );
}
//---------------------------------------------------------------------------
bool __fastcall TLA50BCPU::OL(int DIO,int Point)
{
	int idx;

	idx = DIO * DIO_OUTPUT_POINTS + Point;
	return SetDOValue( idx, false );
}
//---------------------------------------------------------------------------
//
//	Method : GetAI( int Moduel,int Point )
//
//	Note : Point variable should be 0, 2, 4, 6
//
//---------------------------------------------------------------------------
WORD __fastcall TLA50BCPU::GetAI(int Module,int Point)
{
	int idx;

	idx    = CalcAnalogIndex( Module, Point );
	return GetAIValue( idx );
}
//---------------------------------------------------------------------------
//
//	Method : GetAO( int Moduel,int Point )
//
//	Note : Point variable should be 0, 2, 4, 6
//
//---------------------------------------------------------------------------
WORD __fastcall TLA50BCPU::GetAO(int Module,int Point)
{
	int idx;

	idx	= CalcAnalogIndex( Module, Point );
	return GetAOValue( idx );
}
//---------------------------------------------------------------------------
bool __fastcall TLA50BCPU::SetAO(int Module,int Point,WORD Value)
{
	int idx;

	idx	= CalcAnalogIndex( Module, Point );
	return SetAOValue( idx, Value );
}
//---------------------------------------------------------------------------
bool __fastcall TLA50BCPU::SetAO(int Module,int Point,double Percent)
{
	int idx;
	WORD value;

	if( Percent < 0 )
		Percent	= 0;
	value   = 0xFFFF * Percent;

	idx	= CalcAnalogIndex( Module, Point );
	return SetAOValue( idx, value );
}
//---------------------------------------------------------------------------
WORD __fastcall TLA50BCPU::GetCounter(int Module,int Point)
{
	int idx;

	idx	= CalcCounterIndex( Module, Point );
	return GetCounterValue( idx );
}
//---------------------------------------------------------------------------
double __fastcall TLA50BCPU::GetTemperature(int Module,int Point)
{
	WORD plc_value;

	plc_value	= GetAI( Module, Point );
	return CalcTemperatureValue( plc_value );
}
//---------------------------------------------------------------------------
double __fastcall TLA50BCPU::GetTemperature(TLA50BIO *IO)
{
	if( !IO )
		return 0;

	return GetTemperature( IO->Module, IO->Point );
}
//---------------------------------------------------------------------------
WORD __fastcall TLA50BCPU::GetIOValue(TBasicIO *IO)
{
	if( !IO )
		return 0;

	TLA50BIO *src;

	/*
	src	= dynamic_cast<TLA50BIO*>(IO);
	if( !src )
		return 0;
	*/
	src	= (TLA50BIO*)IO;
	switch( src->Type )
	{
		case ioDI:
			if( GetDI( src->IONumber ) )
				return 1;
			return 0;

		case ioDO:
			if( GetDO( src->IONumber ) )
				return 1;
			return 0;

		case ioAI:
			return GetAI( src->IONumber );

		case ioAO:
			return GetAO( src->IONumber );

		case ioCOUNTER:
			return GetCounter( src->IONumber );

		default:
			return 0;
	}
}
//---------------------------------------------------------------------------
void __fastcall TLA50BCPU::SetIOValue(TBasicIO *IO,WORD Value)
{
	if( !IO )
		return;

	TLA50BIO *src;

	/*
	src	= dynamic_cast<TLA50BIO*>(IO);
	if( !src )
		return;
	*/
	src	= (TLA50BIO*)IO;
	switch( src->Type )
	{
		case ioDI:
			SetDI( src->IONumber, Value );
			break;

		case ioDO:
			if( Value == 0 )
				OL( src->IONumber );
			else
				OH( src->IONumber );
			break;

		case ioAI:
			SetAI( src->IONumber, Value );
			break;

		case ioAO:
			SetAO( src->IONumber, Value );
			break;

		case ioCOUNTER:
			SetCounter( src->IONumber, Value );
			break;

		default:
			break;
	}
}
//---------------------------------------------------------------------------
String __fastcall TLA50BCPU::GetIOCode(TLA50BIO *IO)
{
	int idx;
	TLA50BIO *target;
	TList *list;

	switch( IO->Type )
	{
		case ioDI:
			list	= DIList;
			break;

		case ioDO:
			list	= DOList;
			break;

		case ioAI:
			list	= AIList;
			break;

		case ioAO:
			list	= AOList;
			break;

		case ioCOUNTER:
			list	= CounterList;
			break;

		default:
			return "";
	}

	idx	= IndexOfIONumber( list, IO->IONumber );
	if( idx < 0 )
		return "";
	target	= (TLA50BIO*)list->Items[idx];
	return target->Code;
}
//---------------------------------------------------------------------------
bool __fastcall TLA50BCPU::SetIOCode(TLA50BIO *IO)
{
	int idx;
	TLA50BIO *target;
	TList *list;

	switch( IO->Type )
	{
		case ioDI:
			list	= DIList;
			break;

		case ioDO:
			list	= DOList;
			break;

		case ioAI:
			list	= AIList;
			break;

		case ioAO:
			list	= AOList;
			break;

		case ioCOUNTER:
			list	= CounterList;
			break;

		default:
			return false;
	}

	idx	= IndexOfIONumber( list, IO->IONumber );
	if( idx < 0 )
		return false;
	target	= (TLA50BIO*)list->Items[idx];
	target->Code	= IO->Code;
	return true;
}
//---------------------------------------------------------------------------
bool __fastcall TLA50BCPU::DefineIOCode(ioTYPE Type,int Number,String Code)
{
	TLA50BIO *io;
	bool result;

	switch( Type )
	{
		case ioDI:
		case ioDO:
		case ioAI:
		case ioAO:
		case ioCOUNTER:
			break;

		default:
			return false;
	}

	io	= new TLA50BIO( Type );
	io->IONumber	= Number;
	io->Code		= Code;

	result	= SetIOCode( io );
	delete io;
	io	= NULL;
	return result;
}
//---------------------------------------------------------------------------
bool __fastcall TLA50BCPU::LinkIO(TLA50BIO *IO)
{
	if( !IO )
		return false;
	if( IO->PLCType != LA50B )
		return false;


	int serial;

	switch( IO->Type )
	{
		case ioDI:
		case ioDO:
			if( IO->Module >=0 && IO->Point >= 0 )
			{
				serial	= IO->Module * DIO_INPUT_POINTS + IO->Point;
				if( serial < 0 || serial >= DIOLength )
					return false;

				IO->IONumber	= DIOStartNumber + serial;
			}
			else if( IO->IONumber >= 0 )
			{
				if( !IsDIOExists( IO->IONumber ) )
					return false;

				serial		= IO->IONumber - DIOStartNumber;
				IO->Module	= serial / DIO_INPUT_POINTS;
				IO->Point	= serial % DIO_INPUT_POINTS;
			}
			else if( !IO->Code.Trim().IsEmpty() )
			{
				if( !IsDIOExists( IO->Code ) )
					return false;

				if( IO->Type == ioDI )
					IO->IONumber	= NumberOfIOCode( DIList, IO->Code );
				else
					IO->IONumber	= NumberOfIOCode( DOList, IO->Code );

				serial		= IO->IONumber - DIOStartNumber;
				IO->Module	= serial / DIO_INPUT_POINTS;
				IO->Point	= serial % DIO_INPUT_POINTS;
			}
			else
			{
				return false;
			}
			break;

		case ioAI:
		case ioAO:
			if( IO->Module >=0 && IO->Point >= 0 )
			{
				serial	= IO->Module * AIO_MODULE_REAL_POINTS + IO->Point;
				if( serial < 0 || serial >= AIOLength )
					return false;

				IO->IONumber	= AIOStartNumber + serial;
			}
			else if( IO->IONumber >= 0 )
			{
				if( !IsAIOExists( IO->IONumber ) )
					return false;

				serial		= IO->IONumber - AIOStartNumber;
				IO->Module	= serial / AIO_MODULE_REAL_POINTS;
				IO->Point	= serial % AIO_MODULE_REAL_POINTS;
			}
			else if( !IO->Code.Trim().IsEmpty() )
			{
				if( !IsAIOExists( IO->Code ) )
					return false;

				if( IO->Type == ioAI )
					IO->IONumber	= NumberOfIOCode( AIList, IO->Code );
				else
					IO->IONumber	= NumberOfIOCode( AOList, IO->Code );

				serial		= IO->IONumber - AIOStartNumber;
				IO->Module	= serial / AIO_MODULE_REAL_POINTS;
				IO->Point	= serial % AIO_MODULE_REAL_POINTS;
			}
			else
			{
				return false;
			}
			break;


		case ioCOUNTER:
			if( IO->Module >=0 && IO->Point >= 0 )
			{
				serial	= IO->Module * COUNTER_MODULE_REAL_POINTS + IO->Point;
				if( serial < 0 || serial >= CounterLength )
					return false;

				IO->IONumber	= CounterStartNumber + serial;
			}
			else if( IO->IONumber >= 0 )
			{
				if( !IsCounterExists( IO->IONumber ) )
					return false;

				serial		= IO->IONumber - CounterStartNumber;
				IO->Module	= serial / COUNTER_MODULE_REAL_POINTS;
				IO->Point	= serial % COUNTER_MODULE_REAL_POINTS;
			}
			else if( !IO->Code.Trim().IsEmpty() )
			{
				if( !IsCounterExists( IO->Code ) )
					return false;

				IO->IONumber	= NumberOfIOCode( CounterList, IO->Code );

				serial		= IO->IONumber - CounterStartNumber;
				IO->Module	= serial / COUNTER_MODULE_REAL_POINTS;
				IO->Point	= serial % COUNTER_MODULE_REAL_POINTS;
			}
			else
			{
				return false;
			}
			break;

		default:
			return false;
	}

	if( IO->Code.IsEmpty() )
	{
		IO->Code	= GetIOCode( IO );
	}
	else
	{
		if( !SetIOCode( IO ) )
			return false;
	}
	IO->CPU_ID		= this->CPU_ID;
	IO->OnGetValue	= GetIOValue;
	IO->OnSetValue	= SetIOValue;
	return true;
}
//---------------------------------------------------------------------------

