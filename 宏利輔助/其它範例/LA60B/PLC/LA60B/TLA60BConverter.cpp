//---------------------------------------------------------------------------


#pragma hdrstop

#include "TLA60BConverter.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)
//---------------------------------------------------------------------------
__fastcall TLA60BConverter::TLA60BConverter(TComponent *Owner)
{
	MyOwner		= Owner;
	pPLCType	= LA60B;
	CreateComponent();
}
//---------------------------------------------------------------------------
__fastcall TLA60BConverter::~TLA60BConverter()
{
	DeleteComponent();
}
//---------------------------------------------------------------------------
bool __fastcall TLA60BConverter::CreateComponent(void)
{
	CPUs	= new TList();
	if( !CPUs )
		return false;
	CPUs->Clear();

	LastCommCPUIdx	= -1;
	return true;
}
//---------------------------------------------------------------------------
void __fastcall TLA60BConverter::DeleteComponent(void)
{
	if( CPUs )
	{
		while( CPUs->Count > 0 )
			DeleteByIndex( 0 );
		delete CPUs;
		CPUs	= NULL;
	}
}
//---------------------------------------------------------------------------
int __fastcall TLA60BConverter::GetCPUCount(void)
{
	return CPUs->Count;
}
//---------------------------------------------------------------------------
bool __fastcall TLA60BConverter::AddCPU(unsigned char New_ID)
{
	TLA60BCPU *cpu;

	if( !CPUs )                   	// CPU List not exists
		return false;

	if( FindCPU( New_ID ) )			// CPU_ID already exists
		return false;

	cpu	= new TLA60BCPU( MyOwner, New_ID );
	if( !cpu )						// Can't create new CPU
		return false;
	CPUs->Add(cpu);
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
bool __fastcall TLA60BConverter::DeleteByIndex(int Index)
{
	TLA60BCPU *cpu;

	if( Index >= CPUs->Count )
		return false;

	cpu	= (TLA60BCPU*)CPUs->Items[Index];
	delete cpu;
	cpu	= NULL;
	CPUs->Delete(Index);
	return true;
}
//---------------------------------------------------------------------------
bool __fastcall TLA60BConverter::DeleteCPU(unsigned char Del_ID)
{
	TLA60BCPU *cpu;

	if( !CPUs )                   	// CPU List not exists
		return false;

	cpu	= FindCPU( Del_ID );
	if( !cpu )			// CPU_ID already exists
		return false;

	CPUs->Delete( CPUs->IndexOf(cpu) );
	delete cpu;
	cpu	= NULL;

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
bool __fastcall TLA60BConverter::LoadParameter(String ParamPath)
{
	TIniFile *ini;
	int cpu_count ,i ,j ,addr ,length ,count;
	int di_start ,do_start ,ai_start ,ao_start;
	String sect ,buf ,fld;
	unsigned char id;
	bool result;
	int cmd_count;
	String cmd_param;
	bool is_check;
	int idx;
	unsigned int time;

	if( !FileExists( ParamPath ) )
		return false;

	result		= true;
	di_start	= 0;
	do_start	= 0;
	ai_start	= 0;
	ao_start	= 0;
	ini	= new TIniFile( ParamPath );

	cpu_count	= ini->ReadInteger(	SECT_MAIN,	FLD_CPU_COUNT,	0	);
	for( i=0; i<cpu_count; i++ )
	{
		sect	= SECT_CPU_LEADER + IntToStr( i + 1 );
		buf	= ini->ReadString(	sect,	FLD_CPU_ID,		""	).Trim();
		if( buf.IsEmpty() )
		{
			result	= false;
			break;
		}

		// add CPU
		id	= buf.ToInt();
		if( !AddCPU( id ) )
		{
			result	= false;
			break;
		}

		// OnLineCheck
		is_check	= ini->ReadBool(	sect,	FLD_IS_ONLINE_CHECK,	false	);
		if( is_check )
		{
			idx	= IndexOfCPU( id );

			buf		= ini->ReadString(	sect,	FLD_ONLINE_CHECK_REG,	""	);
			addr	= ini->ReadInteger(	sect,	FLD_ONLINE_CHECK_ADDR,	0	);
			time	= ini->ReadInteger(	sect,	FLD_ONLINE_CHECK_TIME,	0	);
			Items[ idx ]->SetKeepOnLineReg(	SymbolToLA60BReg( buf )
											,addr
											,time
											);
		}

		// add DI
		count	= ini->ReadInteger(	sect,	FLD_LA60B_DI_COUNT,		0	);
		for( j=0; j<count; j++ )
		{
			fld		= String(FLD_LA60B_DI_REG) + IntToStr( j + 1 );
			buf		= ini->ReadString(	sect,	fld,		""	);
			fld		= FLD_LA60B_DI_START + IntToStr( j + 1 );
			addr	= ini->ReadInteger(	sect,	fld,		0	);
			fld		= FLD_LA60B_DI_LENGTH + IntToStr( j + 1 );
			length	= ini->ReadInteger(	sect,	fld,	0	);
			if( !buf.IsEmpty() )
			{
				if( !DefineIONumber( id, ioDI, di_start, SymbolToLA60BReg( buf ), addr, length ) )
				{
					result	= false;
					break;
				}
				di_start	+= length;
			}
		}

		// add DO
		count	= ini->ReadInteger(	sect,	FLD_LA60B_DO_COUNT,		0	);
		for( int j=0; j<count; j++ )
		{
			fld		= FLD_LA60B_DO_REG + IntToStr( j + 1 );
			buf		= ini->ReadString(	sect,	fld,		""	);
			fld		= FLD_LA60B_DO_START + IntToStr( j + 1 );
			addr	= ini->ReadInteger(	sect,	fld,		0	);
			fld		= FLD_LA60B_DO_LENGTH + IntToStr( j + 1 );
			length	= ini->ReadInteger(	sect,	fld,	0	);
			if( !buf.IsEmpty() )
			{
				if( !DefineIONumber( id, ioDO, do_start, SymbolToLA60BReg( buf ), addr, length ) )
				{
					result	= false;
					break;
				}
				do_start	+= length;
			}
		}

		// add AI
		count	= ini->ReadInteger(	sect,	FLD_LA60B_AI_COUNT,		0	);
		for( int j=0; j<count; j++ )
		{
			fld		= FLD_LA60B_AI_REG + IntToStr( j + 1 );
			buf		= ini->ReadString(	sect,	fld,		""	);
			fld		= FLD_LA60B_AI_START + IntToStr( j + 1 );
			addr	= ini->ReadInteger(	sect,	fld,		0	);
			fld		= FLD_LA60B_AI_LENGTH + IntToStr( j + 1 );
			length	= ini->ReadInteger(	sect,	fld,	0	);
			if( !buf.IsEmpty() )
			{
				if( !DefineIONumber( id, ioAI, ai_start, SymbolToLA60BReg( buf ), addr, length ) )
				{
					result	= false;
					break;
				}
				ai_start	+= length;
			}
		}

		// add AO
		count	= ini->ReadInteger(	sect,	FLD_LA60B_AO_COUNT,		0	);
		for( int j=0; j<count; j++ )
		{
			fld		= FLD_LA60B_AO_REG + IntToStr( j + 1 );
			buf		= ini->ReadString(	sect,	fld,		""	);
			fld		= FLD_LA60B_AO_START + IntToStr( j + 1 );
			addr	= ini->ReadInteger(	sect,	fld,		0	);
			fld		= FLD_LA60B_AO_LENGTH + IntToStr( j + 1 );
			length	= ini->ReadInteger(	sect,	fld,	0	);
			if( !buf.IsEmpty() )
			{
				if( !DefineIONumber( id, ioAO, ao_start, SymbolToLA60BReg( buf ), addr, length ) )
				{
					result	= false;
					break;
				}
				ao_start	+= length;
			}
		}


		sect	+=	SECT_CMD;
		cmd_count	= ini->ReadInteger(	sect,	FLD_COUNT,	0	);
		for( int c_idx=0; c_idx<cmd_count; c_idx++ )
		{
			fld	= String( FLD_CMD ) + IntToStr( c_idx + 1 );
			cmd_param	= ini->ReadString(	sect,	fld,	""	);
			if( !cmd_param.Trim().IsEmpty() )
				AddCommCommand( id, cmd_param );
        }
	}

	delete ini;
	ini	= NULL;

	return result;
}
//---------------------------------------------------------------------------
bool __fastcall TLA60BConverter::DefineIONumber(unsigned char CPU_ID,ioTYPE IOType
		,int StartNumber,LA60BRegType RegType,int Addr,int Count)
{
	TLA60BCPU *cpu;

	cpu	= FindCPU( CPU_ID );
	if( !cpu )
		return false;

	if( StartNumber < 0 || Addr < 0 || Count < 0 || RegType == REG_NO_ASSIGN )
		return false;

	return cpu->DefineIONumber( IOType, StartNumber, RegType, Addr, Count );
}
//---------------------------------------------------------------------------
bool __fastcall TLA60BConverter::AddCommCommand(unsigned char CPU_ID,String CmdParam)
{
	TLA60BCPU *cpu;

	cpu	= FindCPU( CPU_ID );
	if( !cpu )
		return false;

	return cpu->AddCommCmd( CmdParam );
}
//---------------------------------------------------------------------------
TPLCBasic* __fastcall TLA60BConverter::GetCommCPU(void)
{
	if( LastCommCPUIdx > CPUs->Count || LastCommCPUIdx < 0 )
		return NULL;

	return ( (TPLCBasic*)CPUs->Items[LastCommCPUIdx] );
}
//---------------------------------------------------------------------------
TLA60BCPU* __fastcall TLA60BConverter::GetItem(int Index)
{
	if( Index >= CPUs->Count )
		return NULL;

	return (TLA60BCPU*)(CPUs->Items[Index]);
}
//---------------------------------------------------------------------------
int __fastcall TLA60BConverter::IndexOfCPU(unsigned char CPU_ID)
{
	int i;

	for( i=0; i<CPUCount; i++ )
	{
		if( Items[i]->CPU_ID == CPU_ID )
			return i;
	}
	return -1;
}
//---------------------------------------------------------------------------
TPLCState __fastcall TLA60BConverter::GetCPUState(unsigned char CPU_ID)
{
	TLA60BCPU *cpu;

	cpu	= FindCPU( CPU_ID );
	if( cpu )
		return cpu->State;
	return PLC_OFF;
}
//---------------------------------------------------------------------------
bool __fastcall TLA60BConverter::SetCPUState(unsigned char CPU_ID,TPLCState State)
{
	TLA60BCPU *cpu;

	cpu	= FindCPU( CPU_ID );
	if( !cpu )
		return false;
	cpu->State	= State;
	return true;
}
//---------------------------------------------------------------------------
TLA60BCPU* __fastcall TLA60BConverter::FindCPU(unsigned char CPU_ID)
{
	int i;

	for( i=0; i<CPUs->Count; i++ )
	{
		if( Items[i]->CPU_ID == CPU_ID )
		{
			return Items[i];
		}
	}
	return NULL;
}
//---------------------------------------------------------------------------
/*
bool __fastcall TLA60BConverter::AddCPUCommCmd(TLA60BCmd *Cmd)
{
	TLA60BCPU *cpu;

	if( !Cmd )
		return false;

	cpu	= FindCPU( Cmd->CPU );
	if( cpu )
		return cpu->AddCommCmd( Cmd );
	return false;
}
//---------------------------------------------------------------------------
int __fastcall TLA60BConverter::GetPLCCommandCount(void)
{
	int result;
	int i;

	result	= 0;
	for( i=0; i<CPUCount; i++ )
	{
		result	+= Items[i]->GetCommandCount();
	}
	return result;
}
//---------------------------------------------------------------------------
TLA60BCmd* __fastcall TLA60BConverter::GetCommand(int Index)
{
	int i ,remain_idx;
	TLA60BCmd *cmd;

	cmd			= NULL;
	remain_idx	= Index;
	for( i=0; i<CPUCount; i++ )
	{
		if( Items[i]->GetCommandCount() > remain_idx )
		{
			cmd	= Items[i]->GetCommand( remain_idx );
			break;
		}
		else
		{
			remain_idx	-= Items[i]->GetCommandCount();
		}
	}

	return cmd;
}
*/
//---------------------------------------------------------------------------
bool __fastcall TLA60BConverter::DefineIOCode(ioTYPE Type,int Number,String Code)
{
	TLA60BCPU *cpu;

	cpu	= FindCPU( Type, Number );
	if( !cpu )
		return false;
	return cpu->DefineIOCode( Type, Number, Code );
}
//---------------------------------------------------------------------------
bool __fastcall TLA60BConverter::LinkIO(TLA60BIO *IO)
{
	TLA60BCPU *cpu;

	if( !IO )
		return false;

	if( IO->IONumber >= 0 )
	{
		cpu	= FindCPU( IO->Type, IO->IONumber );
	}
	else if( !IO->Code.Trim().IsEmpty() )
	{
		cpu	= FindCPU( IO->Type, IO->Code.Trim() );
	}
	else
	{
		cpu	= FindCPU( IO->CPU_ID );
	}
	if( cpu )
		return cpu->LinkIO( IO );
	return false;
}
//---------------------------------------------------------------------------
int __fastcall TLA60BConverter::TxNextCmdData(char *Data)
{
	TLA60BCPU *cpu;
	int len;

	if( CPUs->Count == 0 )
		return -1;
	if( ++LastCommCPUIdx >= CPUs->Count )
		LastCommCPUIdx	= 0;

	cpu	= (TLA60BCPU*)CPUs->Items[ LastCommCPUIdx ];
	len	= cpu->NextCmdData( Data );
	return len;
}
//---------------------------------------------------------------------------
RxState __fastcall TLA60BConverter::CheckRxData(char *Data,int Length)
{
	TLA60BCPU *cpu;

	cpu	= (TLA60BCPU*)CPUs->Items[ LastCommCPUIdx ];
	return cpu->ProcessRxData( Data, Length );
}
//---------------------------------------------------------------------------
void __fastcall TLA60BConverter::SetKeepOnLineReg(int CPU_ID,LA60BRegType RegType
	,int RegNumber,unsigned int CheckInterval)
{
	TLA60BCPU *cpu;

	cpu	= FindCPU( CPU_ID );
	if( !cpu )
		return;
	cpu->SetKeepOnLineReg( RegType, RegNumber, CheckInterval );
}
//---------------------------------------------------------------------------
TLA60BCPU* __fastcall TLA60BConverter::FindCPU(ioTYPE Type,int IONumber)
{
	int i;

	for( i=0; i<CPUCount; i++ )
	{
		if( Items[i]->IsIOExists( Type, IONumber ) )
			return Items[i];
	}
	return NULL;
}
//---------------------------------------------------------------------------
TLA60BCPU* __fastcall TLA60BConverter::FindCPU(ioTYPE Type,String Code)
{
	int i;

	for( i=0; i<CPUCount; i++ )
	{
		if( Items[i]->IsIOExists( Type, Code ) )
			return Items[i];
	}
	return NULL;
}
//---------------------------------------------------------------------------
bool __fastcall TLA60BConverter::GetDIO(ioTYPE Type,int Number)
{
	TLA60BCPU *cpu;

	cpu	= FindCPU( Type, Number );
	if( !cpu )
		return false;

	return cpu->GetDIO(  Type, Number );
}
//---------------------------------------------------------------------------
bool __fastcall TLA60BConverter::GetDIO(ioTYPE Type,String Code)
{
	TLA60BCPU *cpu;

	cpu	= FindCPU( Type, Code );
	if( !cpu )
		return false;

	return cpu->GetDIO(  Type, Code );
}
//---------------------------------------------------------------------------
bool __fastcall TLA60BConverter::OL(int Number)
{
	TLA60BCPU *cpu;

	cpu	= FindCPU( ioDO, Number );
	if( !cpu )
		return false;

	return cpu->SetDIO( ioDO, Number, false );
}
//---------------------------------------------------------------------------
bool __fastcall TLA60BConverter::OL(String Code)
{
	TLA60BCPU *cpu;

	cpu	= FindCPU( ioDO, Code );
	if( !cpu )
		return false;

	return cpu->SetDIO( ioDO, Code, false );
}
//---------------------------------------------------------------------------
bool __fastcall TLA60BConverter::OH(int Number)
{
	TLA60BCPU *cpu;

	cpu	= FindCPU( ioDO, Number );
	if( !cpu )
		return false;

	return cpu->SetDIO( ioDO, Number, true );
}
//---------------------------------------------------------------------------
bool __fastcall TLA60BConverter::OH(String Code)
{
	TLA60BCPU *cpu;

	cpu	= FindCPU( ioDO, Code );
	if( !cpu )
		return false;

	return cpu->SetDIO( ioDO, Code, true );
}
//---------------------------------------------------------------------------
WORD __fastcall TLA60BConverter::GetAI(int Number)
{
	TLA60BCPU *cpu;

	cpu	= FindCPU( ioAI, Number );
	if( !cpu )
		return 0;

	return cpu->GetAIO( ioAI, Number );
}
//---------------------------------------------------------------------------
WORD __fastcall TLA60BConverter::GetAI(String Code)
{
	TLA60BCPU *cpu;

	cpu	= FindCPU( ioAI, Code );
	if( !cpu )
		return 0;

	return cpu->GetAIO( ioAI, Code );
}
//---------------------------------------------------------------------------
double __fastcall TLA60BConverter::GetTemperature(int AINumber)
{
	TLA60BCPU *cpu;
	double result;

	cpu	= FindCPU( ioAO, AINumber );
	if( !cpu )
		return 0;

	result	= cpu->GetAIO( ioAI, AINumber );
	return ( result / 10 );
}
//---------------------------------------------------------------------------
double __fastcall TLA60BConverter::GetTemperature(String Code)
{
	TLA60BCPU *cpu;
	double result;

	cpu	= FindCPU( ioAO, Code );
	if( !cpu )
		return 0;

	result	= cpu->GetAIO( ioAI, Code );
	return ( result / 10 );
}
//---------------------------------------------------------------------------
double __fastcall TLA60BConverter::GetTemperature(TLA60BIO *IO)
{
	if( !IO )
		return 0;
	return GetTemperature( IO->IONumber );
}
//---------------------------------------------------------------------------
WORD __fastcall TLA60BConverter::GetAO(int Number)
{
	TLA60BCPU *cpu;

	cpu	= FindCPU( ioAO, Number );
	if( !cpu )
		return 0;

	return cpu->GetAIO( ioAO, Number );
}
//---------------------------------------------------------------------------
WORD __fastcall TLA60BConverter::GetAO(String Code)
{
	TLA60BCPU *cpu;

	cpu	= FindCPU( ioAO, Code );
	if( !cpu )
		return 0;

	return cpu->GetAIO( ioAO, Code );
}
//---------------------------------------------------------------------------
bool __fastcall TLA60BConverter::SetAO(int Number,WORD Value)
{
	TLA60BCPU *cpu;

	cpu	= FindCPU( ioAO, Number );
	if( !cpu )
		return false;

	return cpu->SetAIO( ioAO, Number, Value );
}
//---------------------------------------------------------------------------
bool __fastcall TLA60BConverter::SetAO(int Number,double Percent)
{
	TLA60BCPU *cpu;

	cpu	= FindCPU( ioAO, Number );
	if( !cpu )
		return false;

	return cpu->SetAIO( ioAO, Number, Percent );
}
//---------------------------------------------------------------------------
bool __fastcall TLA60BConverter::SetAO(String Code,WORD Value)
{
	TLA60BCPU *cpu;

	cpu	= FindCPU( ioAO, Code );
	if( !cpu )
		return false;

	return cpu->SetAIO( ioAO, Code, Value );
}
//---------------------------------------------------------------------------
bool __fastcall TLA60BConverter::SetAO(String Code,double Percent)
{
	TLA60BCPU *cpu;

	cpu	= FindCPU( ioAO, Code );
	if( !cpu )
		return false;

	return cpu->SetAIO( ioAO, Code, Percent );
}
//---------------------------------------------------------------------------
bool __fastcall TLA60BConverter::SetDI(int Number,bool IsHigh)
{
	TLA60BCPU *cpu;

	cpu	= FindCPU( ioDI, Number );
	if( !cpu )
		return false;

	return cpu->SetDIO( ioDI, Number, IsHigh );
}
//---------------------------------------------------------------------------
bool __fastcall TLA60BConverter::SetDI(String Code,bool IsHigh)
{
	TLA60BCPU *cpu;

	cpu	= FindCPU( ioDI, Code );
	if( !cpu )
		return false;

	return cpu->SetDIO( ioDI, Code, IsHigh );
}
//---------------------------------------------------------------------------
bool __fastcall TLA60BConverter::SetAI(int Number,WORD Value)
{
	TLA60BCPU *cpu;

	cpu	= FindCPU( ioAI, Number );
	if( !cpu )
		return false;

	return cpu->SetAIO( ioAI, Number, Value );
}
//---------------------------------------------------------------------------
bool __fastcall TLA60BConverter::SetAI(int Number,double Percent)
{
	TLA60BCPU *cpu;

	cpu	= FindCPU( ioAI, Number );
	if( !cpu )
		return false;

	return cpu->SetAIO( ioAI, Number, Percent );
}
//---------------------------------------------------------------------------
bool __fastcall TLA60BConverter::SetAI(String Code,WORD Value)
{
	TLA60BCPU *cpu;

	cpu	= FindCPU( ioAI, Code );
	if( !cpu )
		return false;

	return cpu->SetAIO( ioAI, Code, Value );
}
//---------------------------------------------------------------------------
bool __fastcall TLA60BConverter::SetAI(String Code,double Percent)
{
	TLA60BCPU *cpu;

	cpu	= FindCPU( ioAI, Code );
	if( !cpu )
		return false;

	return cpu->SetAIO( ioAI, Code, Percent );
}
//---------------------------------------------------------------------------
void __fastcall TLA60BConverter::OLAll(void)
{
	for( int i=0; i<CPUs->Count; i++ )
    	Items[i]->OLAll();
}
//---------------------------------------------------------------------------

