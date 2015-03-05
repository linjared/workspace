//---------------------------------------------------------------------------


#pragma hdrstop

#include "TPLCBasic.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
__fastcall TPLCBasic::TPLCBasic(TComponent *Owner,plcTYPE Type,unsigned char cpu_id)
{
	MyOwner			= Owner;
	pPLCType		= Type;
	pCPU_ID			= cpu_id;
	State			= PLC_ONLINE;
	TimeoutCount	= 0;
	ErrorCount		= 0;
}
//---------------------------------------------------------------------------
__fastcall TPLCBasic::~TPLCBasic()
{

}
//---------------------------------------------------------------------------

