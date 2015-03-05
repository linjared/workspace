//---------------------------------------------------------------------------


#pragma hdrstop

#include "PLCDefine.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
plcTYPE StringToPLCType(String PLCStr)
{
	if( PLCStr.Trim().UpperCase() == LA50B_STR )
		return LA50B;

	if( PLCStr.Trim().UpperCase() == LA60B_STR )
		return LA60B;

	if( PLCStr.Trim().UpperCase() == SCALE_STR )
		return SCALE;

	return UNKNOW_PLC;
}
//---------------------------------------------------------------------------
String PLCTypeToString(plcTYPE Type)
{
	switch( Type )
	{
		case LA50B:
			return LA50B_STR;

		case LA60B:
			return LA60B_STR;

		case SCALE:
			return SCALE_STR;

		default:
			return "";
	}
}
//---------------------------------------------------------------------------
char LA60BRegToSymbol(LA60BRegType Type)
{
	switch( Type )
	{
		case REG_X:
			return 'X';

		case REG_Y:
			return 'Y';

		case REG_D:
			return 'D';

		case REG_M:
			return 'M';

		case REG_R:
			return 'R';

		default:
			return 0;
	}
}
//---------------------------------------------------------------------------
LA60BRegType SymbolToLA60BReg(char Symbol)
{
	switch( Symbol )
	{
		case 'X':
		case 'x':
			return REG_X;

		case 'Y':
		case 'y':
			return REG_Y;

		case 'D':
		case 'd':
			return REG_D;

		case 'M':
		case 'm':
			return REG_M;

		case 'R':
		case 'r':
			return REG_R;

		default:
			return REG_NO_ASSIGN;
	}
}
//---------------------------------------------------------------------------
LA60BRegType SymbolToLA60BReg(String SymbolStr)
{
	if( SymbolStr.IsEmpty() )
		return REG_NO_ASSIGN;
	return SymbolToLA60BReg( SymbolStr.c_str()[0] );
}
//---------------------------------------------------------------------------

