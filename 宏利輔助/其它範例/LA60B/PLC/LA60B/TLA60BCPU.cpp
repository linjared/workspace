//---------------------------------------------------------------------------


#pragma hdrstop

#include "TLA60BCPU.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
__fastcall TLA60BCPU::TLA60BCPU(TComponent* Owner,unsigned char cpu_id)
	: TPLCBasic(Owner,LA60B,cpu_id)
{
	MyOwner	= Owner;

	Regs		= new TList();
	CommCmds	= new TList();

	CmdIndex	= 0;
	TxCmd		= NULL;

	OnLineKeepCmd	= new TLA60BCmd( MyOwner );
	if( OnLineKeepCmd )
	{
		OnLineKeepCmd->CPU		= cpu_id;
		OnLineKeepCmd->Cmd		= MULTI_POINT_W;
		OnLineKeepCmd->Length	= 1;
	}
	OnLineCheckTick			= 0;
	OnLineCheckInterval		= 0;

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
}
//---------------------------------------------------------------------------
__fastcall TLA60BCPU::~TLA60BCPU()
{
	if( CommCmds )
	{
		while( CommCmds->Count > 0 )
			DelCmd( 0 );
		delete CommCmds;
		CommCmds	= NULL;
	}
	if( Regs )
	{
		while( Regs->Count > 0 )
			DelReg( 0 );
		delete Regs;
		Regs	= NULL;
	}
	if( OnLineKeepCmd )
	{
		delete OnLineKeepCmd;
		OnLineKeepCmd	= NULL;
	}

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
}
//---------------------------------------------------------------------------
bool __fastcall TLA60BCPU::DeleteIO(TList *IOs,int Index)
{
	TLA60BIO *io;

	if( !IOs )
		return false;
	if( Index < 0 || Index >= IOs->Count )
		return false;

	io	= (TLA60BIO*)IOs->Items[ Index ];
	IOs->Delete( Index );
	delete io;
	io	= NULL;
	return true;
}
//---------------------------------------------------------------------------
bool __fastcall TLA60BCPU::DelCmd(int idx)
{
	TLA60BCmd *cmd;

	if( idx < 0 || idx >= CommCmds->Count )
		return false;
	cmd	= (TLA60BCmd*)CommCmds->Items[idx];
	delete cmd;
	cmd	= NULL;
	CommCmds->Delete(idx);
	return true;
}
//---------------------------------------------------------------------------
bool __fastcall TLA60BCPU::DelReg(int idx)
{
	TLA60BRegistry *reg;

	if( idx < 0 || idx >= Regs->Count )
		return false;
	reg	= (TLA60BRegistry*)Regs->Items[idx];
	delete reg;
	reg	= NULL;
	Regs->Delete(idx);
	return true;
}
//---------------------------------------------------------------------------
TLA60BRegistry* __fastcall TLA60BCPU::GetRegistry(LA60BRegType Type,int RegNo)
{
	int i;
	TLA60BRegistry *reg;

	for( i=0; i<Regs->Count; i++ )
	{
		reg	= (TLA60BRegistry*)Regs->Items[i];
		if( reg->RegExists(Type,RegNo) )
			return reg;
	}
	return NULL;
}
//---------------------------------------------------------------------------
WORD __fastcall TLA60BCPU::GetRegistryValue(LA60BRegType Type,int RegNumber)
{
	TLA60BRegistry *reg;

	reg	= GetRegistry( Type, RegNumber );
	if( !reg )
		return 0;

	return reg->GetRegValue( RegNumber );
}
//---------------------------------------------------------------------------
bool __fastcall TLA60BCPU::SetRegistryValue(LA60BRegType Type,int RegNumber,WORD Value)
{
	TLA60BRegistry *reg;

	reg	= GetRegistry( Type, RegNumber );
	if( !reg )
		return false;
	return reg->SetRegValue( RegNumber, Value );
}
//---------------------------------------------------------------------------
void __fastcall TLA60BCPU::SetIOValue(TBasicIO *IO,WORD Value)
{
	if( !IO )
		return;


	bool result;
	TLA60BIO *src;

	src		= (TLA60BIO*)IO;
	SetRegistryValue( src->RegType, src->RegAddr, Value );
}
//---------------------------------------------------------------------------
WORD __fastcall TLA60BCPU::GetIOValue(TBasicIO *IO)
{
	if( !IO )
		return 0;

	TLA60BIO *src;

	src		= (TLA60BIO*)IO;
	return GetRegistryValue( src->RegType, src->RegAddr );
}
//---------------------------------------------------------------------------
int __fastcall TLA60BCPU::InitTxBuf(char *Data)
{
	int pos = 0;

	Data[pos++]	= LA60B_LEADER_SYMBOL;						// add Lead Symbol
	return pos;
}
//---------------------------------------------------------------------------
int __fastcall TLA60BCPU::InsRegsData(char *Data,int InsPos,unsigned char Cmd
	,LA60BRegType RegType,int Addr,unsigned char Length)
{
	TLA60BRegistry *reg;
	WORD value;
	int i ,regidx;

	reg	= GetRegistry( RegType, Addr );
	for( i=0; i<Length; i++ )
	{
		if( Addr + i >= reg->RegStartNo + reg->RegCount )	// out of area, find next registry area
		{
			reg	= GetRegistry( RegType, Addr + i );
		}

		if( !reg )
			break;
		value	= reg->GetRegValue( Addr + i );
		if( Cmd == MULTI_POINT_W )
		{
			if( value == 0 )
				Data[InsPos++]	= IntToBCD(0);
			else
				Data[InsPos++]	= IntToBCD(1);
		}
		else if( Cmd == MULTI_REG_W )
		{
			InsPos	= InsData( Data, InsPos, value, sizeof(value) );
		}
	}
	return InsPos;
}
//---------------------------------------------------------------------------
int __fastcall TLA60BCPU::InsCmdData(char *Data,int InsPos,unsigned char Cmd,LA60BRegType RegType,int Addr,unsigned char Length)
{
	char bcd;

	// add CPU ID
	InsPos	= InsData( Data, InsPos, CPU_ID,	sizeof(Cmd) );

	// add Command
	InsPos	= InsData( Data, InsPos, Cmd, 		sizeof(Cmd) );

	// add Data Length
	InsPos	= InsData( Data, InsPos, Length,	sizeof(Length) );

	// add Registry Symbol
	Data[InsPos++]	= LA60BRegToSymbol( RegType );

	// add Registry start address
	if( Cmd == MULTI_POINT_R || Cmd == MULTI_POINT_W )			// Use 4 BCDs to present addr
		InsPos	= InsLA60BAddrData( Data, InsPos, Addr,	4 );
	else if( Cmd == MULTI_REG_R || Cmd == MULTI_REG_W )			// Use 5 BCDs to present addr
		InsPos	= InsLA60BAddrData( Data, InsPos, Addr,	5 );

	// add write values
	if( Cmd == MULTI_POINT_W || Cmd == MULTI_REG_W )
		InsPos	= InsRegsData( Data, InsPos, Cmd, RegType, Addr, Length );

	return InsPos;
}
//---------------------------------------------------------------------------
int __fastcall TLA60BCPU::FinishTxBuf(char *Data,int InsPos)
{
	unsigned char checksum;

	checksum	= LRCGeneration( Data, InsPos );
	InsPos		= InsData( Data, InsPos, checksum, sizeof(checksum) );
	Data[InsPos++]	= LA60B_END_SYMBOL;					// add End Symbol
	return InsPos;
}
//---------------------------------------------------------------------------
int __fastcall TLA60BCPU::GetTxData(char *Data,TLA60BCmd *Cmd)
{
	int len = 0;

	len	= InitTxBuf( Data );
	len	= InsCmdData( Data, len, Cmd->Cmd, Cmd->Reg, Cmd->Addr, Cmd->Length );
	len	= FinishTxBuf( Data, len );

	return len;
}
//---------------------------------------------------------------------------
bool __fastcall TLA60BCPU::IsRxFinish(char *Data,int Length)
{
	if( Length < 2 )
		return false;

	if( Data[Length-1] == LA60B_END_SYMBOL )
		return true;
	return false;
}
//---------------------------------------------------------------------------
TLA60BCmd* __fastcall TLA60BCPU::GetCmd(int Index)
{
	if( Index < 0 || Index >= CommCmds->Count )
		return NULL;

	return (TLA60BCmd*)CommCmds->Items[ Index ];
}
//---------------------------------------------------------------------------
RxState __fastcall TLA60BCPU::ProcessRxData(unsigned char *RxData,int Length)
{
	unsigned char cpu_id ,cmd ,checksum ,rx_checksum ,err_code;
	int addr ,idx;

	if( !IsRxFinish(RxData,Length) )
		return COMM_WAIT_RESPONSE;
	if( Length < 8 )				// Length too short to determine
		return COMM_DATA_ERR;

	// CheckSum has 2 BCDs and End_Symbol has 1 BCD = 3 BCDs ( Bytes )
	rx_checksum	= TransBufferBCDData( RxData, Length-3, sizeof(checksum)*2 );
	checksum	= LRCGeneration( RxData, Length-3 );
	if( rx_checksum != checksum )
		return COMM_CHECKSUM_ERR;


	idx	= 1;					// Leader symbol has 1 Byte
	cpu_id	= TransBufferBCDData( RxData, idx, sizeof(cpu_id)*2 );	// 1 Byte need 2 BCD to present
	idx	+= sizeof(cpu_id) * 2;
	if( cpu_id != TxCmd->CPU )
		return COMM_DATA_ERR;

	cmd	= TransBufferBCDData( RxData, idx, sizeof(cmd)*2 );	// 1 Byte need 2 BCD to present
	idx	+= sizeof(cmd) * 2;
	if( cmd != TxCmd->Cmd )
		return COMM_DATA_ERR;

	err_code	= TransBufferBCDData( RxData, idx, 1 );		// Error Code only 1 BCD
	idx	+= 1;
	if( err_code !=0 )
		return COMM_DATA_ERR;


	if( TxCmd->Cmd == MULTI_POINT_W || TxCmd->Cmd == MULTI_REG_W )
		return COMM_RX_OK;

	return SaveRxData( RxData, idx );
}
//---------------------------------------------------------------------------
RxState __fastcall TLA60BCPU::SaveRxData(char *Data,int DataPos)
{
	int i ,reg_num;
	WORD value;
	int value_len;


	value_len	= -1;
	switch( TxCmd->Cmd )
	{
		case MULTI_POINT_R:			// point used 1 BCD to present
			value_len	= 1;
			break;

		case MULTI_REG_R:           // Registry ( WORD ) used 4 BCD to present
			value_len	= 4;
			break;

		default:
			;
	}

	if( value_len <= 0 )
		return COMM_DATA_ERR;

	for( i=0; i<TxCmd->Length; i++ )
	{
		reg_num	= TxCmd->Addr + i;
		value  	= TransBufferBCDData( Data, DataPos, value_len );
		SetRegistryValue( TxCmd->Reg, reg_num, value );
		DataPos	+= value_len;
	}
	return COMM_RX_OK;
}
//---------------------------------------------------------------------------
int __fastcall TLA60BCPU::NextCmdData(char *Data)
{
	int length = 0;

	if( PopCommCommand() )
		length	= GetTxData( Data, TxCmd );
	return length;
}
//---------------------------------------------------------------------------
bool __fastcall TLA60BCPU::SetOnLineValue(void)
{
	if( OnLineKeepCmd->Reg == REG_NO_ASSIGN || OnLineKeepCmd->Addr < 0 )
		return false;

	if( GetRegistryValue( OnLineKeepCmd->Reg, OnLineKeepCmd->Addr ) == 0 )
		SetRegistryValue( OnLineKeepCmd->Reg, OnLineKeepCmd->Addr, 1 );
	else
		SetRegistryValue( OnLineKeepCmd->Reg, OnLineKeepCmd->Addr, 0 );
	return true;
}
//---------------------------------------------------------------------------
bool __fastcall TLA60BCPU::CheckNeedKeepOnLine(void)
{
	if( OnLineKeepCmd->Reg != REG_NO_ASSIGN &&
		( OnLineCheckTick == 0 || TimeGoesBy( OnLineCheckTick ) >= OnLineCheckInterval )
		)
	{
		return true;
	}
	return false;
}
//---------------------------------------------------------------------------
bool __fastcall TLA60BCPU::PopCommCommand(void)
{
	if( !CommCmds )
		return false;

	if( IsNeedKeepOnLine )
	{
		SetOnLineValue();
		TxCmd	= OnLineKeepCmd;

		OnLineCheckTick	= GetTickCount();
	}
	else
	{
		if( CommCmds->Count == 0 )
			return false;

		if( ( (TLA60BCmd*)CommCmds->Items[ CmdIndex ] )->IsRoutine )
		{
			CmdIndex++;
		}
		else
		{
			TLA60BCmd *cmd;

			cmd	= (TLA60BCmd*)CommCmds->Items[ CmdIndex ];
			CommCmds->Delete( CmdIndex );
			delete cmd;
			cmd	= NULL;

			if( CommCmds->Count == 0 )
				return false;
		}

		if( CmdIndex >= CommCmds->Count )
			CmdIndex	= 0;

		TxCmd	= (TLA60BCmd*)CommCmds->Items[ CmdIndex ];
	}

	return true;
}
//---------------------------------------------------------------------------
bool __fastcall TLA60BCPU::AddCommCmd(String Param)
{
	TLA60BCmd *cmd;

	cmd	= new TLA60BCmd( MyOwner );
	cmd->CPU	= CPU_ID;
	cmd->Cmd	= GetParamValue( Param,	"Cmd",	';'	).ToInt();
	cmd->Reg	= SymbolToLA60BReg( GetParamValue( Param,	"Reg",	';'	) );
	cmd->Addr	= GetParamValue( Param,	"Addr",	';'	).ToInt();
	cmd->Length	= GetParamValue( Param,	"Length",	';'	).ToInt();
	return AddCommCmd( cmd );
}
//---------------------------------------------------------------------------
bool __fastcall TLA60BCPU::AddCommCmd(TLA60BCmd *Command)
{
	if( !Command )
		return false;
	CommCmds->Add( Command );
	return true;
}
//---------------------------------------------------------------------------
bool __fastcall TLA60BCPU::AddRegistry(LA60BRegType Type,int StartNumber,int Count)
{
	TLA60BRegistry *reg;

	if( Type == REG_NO_ASSIGN )
		return false;
	if( StartNumber < 0 || Count <= 0 )
		return false;

	reg	= new TLA60BRegistry( Type, StartNumber, Count );
	if( reg )
	{
		Regs->Add(reg);
		return true;
	}
	return false;
}
//---------------------------------------------------------------------------
int __fastcall TLA60BCPU::GetCommandCount(void)
{
	return CommCmds->Count;
}
//---------------------------------------------------------------------------
TLA60BCmd* __fastcall TLA60BCPU::GetCommand(int Index)
{
	TLA60BCmd *cmd;

	if( Index < 0 || Index >= CommCmds->Count )
		return NULL;

	cmd	= (TLA60BCmd*)CommCmds->Items[Index];
	return cmd;
}
//---------------------------------------------------------------------------
void __fastcall TLA60BCPU::SetKeepOnLineReg(LA60BRegType Type,int RegNumber,unsigned int CheckInterval)
{
	OnLineKeepCmd->Reg	= Type;
	OnLineKeepCmd->Addr	= RegNumber;

	OnLineCheckInterval	= CheckInterval;
}
//---------------------------------------------------------------------------
TList* __fastcall TLA60BCPU::GetIOList(ioTYPE Type)
{
	switch( Type )
	{
		case ioDI:
			return DIList;

		case ioDO:
			return DOList;

		case ioAI:
		case ioCOUNTER:
			return AIList;

		case ioAO:
			return AOList;

		default:
			return NULL;
	}
}
//---------------------------------------------------------------------------
bool __fastcall TLA60BCPU::AddIOs(ioTYPE IOType,int StartNumber
		,LA60BRegType RegType,int Addr,int Count)
{
	TLA60BIO *io;
	TList *list;
	int i;

	list	= GetIOList( IOType );
	if( !list )
		return false;
	for( i=0; i<Count; i++ )
	{
		io	= new TLA60BIO( IOType );
		io->CPU_ID		= CPU_ID;
		io->RegType		= RegType;
		io->RegAddr		= Addr + i;
		io->IONumber	= StartNumber + i;

		io->OnGetValue	= GetIOValue;
		io->OnSetValue	= SetIOValue;
		list->Add( io );
	}
	return true;
}
//---------------------------------------------------------------------------
bool __fastcall TLA60BCPU::DefineIONumber(ioTYPE IOType,int StartNumber
		,LA60BRegType RegType,int Addr,int Count)
{
	if( !AddRegistry( RegType, Addr, Count ) )
		return false;

	return AddIOs( IOType, StartNumber, RegType, Addr, Count );
}
//---------------------------------------------------------------------------
TLA60BIO* __fastcall TLA60BCPU::IOOfNumber(ioTYPE Type,int IONumber)
{
	TLA60BIO *io;

	switch( Type )
	{
		case ioDI:
			io	= IOOfNumber( DIList, IONumber );
			break;

		case ioDO:
			io	= IOOfNumber( DOList, IONumber );
			break;

		case ioAI:
		case ioCOUNTER:
			io	= IOOfNumber( AIList, IONumber );
			break;

		case ioAO:
			io	= IOOfNumber( AOList, IONumber );
			break;

		default:
			return NULL;
	}
	return io;
}
//---------------------------------------------------------------------------
TLA60BIO* __fastcall TLA60BCPU::IOOfNumber(TList *IOs,int IONumber)
{
	int i;

	if( !IOs )
		return NULL;

	for( i=0; i<IOs->Count; i++ )
	{
		if( ((TLA60BIO*)IOs->Items[i])->IONumber == IONumber )
			return ( (TLA60BIO*)IOs->Items[i] );
	}
	return NULL;
}
//---------------------------------------------------------------------------
int __fastcall TLA60BCPU::NumberOfIO(ioTYPE IOType,LA60BRegType RegType,int Addr)
{
	TList *list;

	list	= GetIOList( IOType );
	if( !list )
		return -1;

	for( int i=0; i<list->Count; i++ )
	{
		if( ((TLA60BIO*)list->Items[i])->RegType == RegType
			&& ((TLA60BIO*)list->Items[i])->RegAddr == Addr
			 )
		{
			return ( i + 1 );
		}
	}
	return -1;
}
//---------------------------------------------------------------------------
TLA60BIO* __fastcall TLA60BCPU::IOOfCode(ioTYPE Type,String Code)
{
	TLA60BIO *io;

	switch( Type )
	{
		case ioDI:
			io	= IOOfCode( DIList, Code );
			break;

		case ioDO:
			io	= IOOfCode( DOList, Code );
			break;

		case ioAI:
		case ioCOUNTER:
			io	= IOOfCode( AIList, Code );
			break;

		case ioAO:
			io	= IOOfCode( AOList, Code );
			break;

		default:
			return NULL;
	}
	return io;
}
//---------------------------------------------------------------------------
TLA60BIO* __fastcall TLA60BCPU::IOOfCode(TList *IOs,String Code)
{
	int i;

	if( !IOs )
		return NULL;

	for( i=0; i<IOs->Count; i++ )
	{
		if( ((TLA60BIO*)IOs->Items[i])->Code == Code )
			return ( (TLA60BIO*)IOs->Items[i] );
	}
	return NULL;
}
//---------------------------------------------------------------------------
bool __fastcall TLA60BCPU::DefineIOCode(ioTYPE Type,int IONumber,String Code)
{
	TLA60BIO *io;

	io	= IOOfNumber( Type, IONumber );
	if( !io )
		return false;
	io->Code	= Code;
	return true;
}
//---------------------------------------------------------------------------
bool __fastcall TLA60BCPU::IsIOExists(ioTYPE Type,int Number)
{
	if( IOOfNumber( Type, Number ) )
		return true;
	return false;
}
//---------------------------------------------------------------------------
bool __fastcall TLA60BCPU::IsIOExists(ioTYPE Type,String Code)
{
	if( IOOfCode( Type, Code ) )
		return true;
	return false;
}
//---------------------------------------------------------------------------
bool __fastcall TLA60BCPU::GetDIO(ioTYPE Type,int Number)
{
	TLA60BIO *io;

	io	= IOOfNumber( Type, Number );
	if( !io )
		return false;
	return GetRegistryValue( io->RegType, io->RegAddr );
}
//---------------------------------------------------------------------------
bool __fastcall TLA60BCPU::GetDIO(ioTYPE Type,String Code)
{
	TLA60BIO *io;

	io	= IOOfCode( Type, Code );
	if( !io )
		return false;
	return GetRegistryValue( io->RegType, io->RegAddr );
}
//---------------------------------------------------------------------------
bool __fastcall TLA60BCPU::SetDIO(ioTYPE Type,int Number,bool High)
{
	TLA60BIO *io;

	io	= IOOfNumber( Type, Number );
	if( !io )
		return false;
	if( High )
		return SetRegistryValue( io->RegType, io->RegAddr, 1 );
	else
		return SetRegistryValue( io->RegType, io->RegAddr, 0 );
}
//---------------------------------------------------------------------------
bool __fastcall TLA60BCPU::SetDIO(ioTYPE Type,String Code,bool High)
{
	TLA60BIO *io;

	io	= IOOfCode( Type, Code );
	if( !io )
		return false;
	if( High )
		return SetRegistryValue( io->RegType, io->RegAddr, 1 );
	else
		return SetRegistryValue( io->RegType, io->RegAddr, 0 );
}
//---------------------------------------------------------------------------
bool __fastcall TLA60BCPU::SetAIO(ioTYPE Type,int Number,double Percent)
{
	WORD value;

	if( Percent < 0 )
		Percent	= 0;
	else if( Percent > 1 )
		Percent	= 1;

	value	= 0xFFFF * Percent;
	return SetAIO( Type, Number, value );
}
//---------------------------------------------------------------------------
bool __fastcall TLA60BCPU::SetAIO(ioTYPE Type,int Number,WORD Value)
{
	TLA60BIO *io;

	io	= IOOfNumber( Type, Number );
	if( !io )
		return false;
	return SetRegistryValue( io->RegType, io->RegAddr, Value );
}
//---------------------------------------------------------------------------
bool __fastcall TLA60BCPU::SetAIO(ioTYPE Type,String Code,double Percent)
{
	WORD value;

	if( Percent < 0 )
		Percent	= 0;
	else if( Percent > 1 )
		Percent	= 1;

	value	= 0xFFFF * Percent;
	return SetAIO( Type, Code, value );
}
//---------------------------------------------------------------------------
bool __fastcall TLA60BCPU::SetAIO(ioTYPE Type,String Code,WORD Value)
{
	TLA60BIO *io;

	io	= IOOfCode( Type, Code );
	if( !io )
		return false;
	return SetRegistryValue( io->RegType, io->RegAddr, Value );
}
//---------------------------------------------------------------------------
WORD __fastcall TLA60BCPU::GetAIO(ioTYPE Type,int Number)
{
	TLA60BIO *io;

	io	= IOOfNumber( Type, Number );
	if( !io )
		return false;
	return GetRegistryValue( io->RegType, io->RegAddr );
}
//---------------------------------------------------------------------------
WORD __fastcall TLA60BCPU::GetAIO(ioTYPE Type,String Code)
{
	TLA60BIO *io;

	io	= IOOfCode( Type, Code );
	if( !io )
		return false;
	return GetRegistryValue( io->RegType, io->RegAddr );
}
//---------------------------------------------------------------------------
void __fastcall TLA60BCPU::OLAll(void)
{
	int i;

	for( i=0; i<DOList->Count; i++ )
	{
		( (TLA60BIO*)DOList->Items[i] )->Value	= 0;
//		SetDIO( ioDO, ( (TLA60BIO*)DOList->Items[i] )->IONumber, false );
	}
	for( i=0; i<AOList->Count; i++ )
	{
		( (TLA60BIO*)AOList->Items[i] )->Value	= 0;
//		SetAIO( ioAO, ( (TLA60BIO*)AOList->Items[i] )->IONumber, 0 );
	}
}
//---------------------------------------------------------------------------
bool __fastcall TLA60BCPU::LinkIO(TLA60BIO *IO)
{
	if( !IO )
		return false;

	TLA60BIO *inside_io;

	if( IO->IONumber >= 0 )
	{
		inside_io	= IOOfNumber( IO->Type, IO->IONumber );
		if( !inside_io )
			return false;
		IO->RegType	= inside_io->RegType;
		IO->RegAddr	= inside_io->RegAddr;
	}
	else if( !IO->Code.Trim().IsEmpty() )
	{
		inside_io	= IOOfCode( IO->Type, IO->Code.Trim() );
		if( !inside_io )
			return false;
		IO->IONumber	= inside_io->IONumber;
		IO->RegType		= inside_io->RegType;
		IO->RegAddr		= inside_io->RegAddr;
	}
	else if( GetRegistry( IO->RegType, IO->RegAddr ) )
	{
		IO->IONumber	= NumberOfIO( IO->Type, IO->RegType, IO->RegAddr );
		inside_io		= IOOfNumber( IO->Type, IO->IONumber );
		if( !inside_io )
			return false;
	}
	else
	{
		return false;
	}

	if( !IO->Code.Trim().IsEmpty() )
	{
		inside_io->Code	= IO->Code.Trim();
	}
	else if( !inside_io->Code.Trim().IsEmpty() )
	{
    	IO->Code	= inside_io->Code.Trim();
	}
	IO->CPU_ID		= this->CPU_ID;
	IO->OnGetValue	= GetIOValue;
	IO->OnSetValue	= SetIOValue;
	return true;
}
//---------------------------------------------------------------------------

