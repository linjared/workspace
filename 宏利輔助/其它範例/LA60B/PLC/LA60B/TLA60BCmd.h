//---------------------------------------------------------------------------

#ifndef TLA60BCmdH
#define TLA60BCmdH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "PLCDefine.h"
//---------------------------------------------------------------------------

#define MULTI_POINT_R				0x44
#define MULTI_POINT_W				0x45
#define MULTI_REG_R					0x46
#define MULTI_REG_W					0x47

//---------------------------------------------------------------------------
class TLA60BCmd
{
private:
	TComponent *MyOwner;
public:
	__fastcall TLA60BCmd(TComponent *Owner);
	__fastcall ~TLA60BCmd();

	unsigned char CPU;
	unsigned char Cmd;
	LA60BRegType Reg;
	int Addr;
	int Length;
	bool IsRoutine;
};
//---------------------------------------------------------------------------
#endif

