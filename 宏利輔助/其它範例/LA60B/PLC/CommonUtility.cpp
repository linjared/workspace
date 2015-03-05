//---------------------------------------------------------------------------


#pragma hdrstop

#include "CommonUtility.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
unsigned int TimeGoesBy(unsigned int LastTick)
{
	unsigned int new_tick;

	new_tick	= GetTickCount();
	if( new_tick >= LastTick )
		return ( new_tick - LastTick );
	else
		return ( 0xFFFFFFFF - LastTick + 1 + new_tick );
}
//---------------------------------------------------------------------------
String GetParamValue(String Parameter,String Name,char SepaSymbol)
{
	TStringList *list;
	TSysCharSet sep_char;
	String result;

	if( Parameter.Trim().IsEmpty() || Name.Trim().IsEmpty() )
		return "";
	list	= new TStringList();
	if( !list )
		return "";

	sep_char	= TSysCharSet() << SepaSymbol;
	ExtractStrings( sep_char, sep_char, Parameter.c_str(), list );
	result	= list->Values[ Name ];
	delete list;
	list	= NULL;

	return result;
}
//---------------------------------------------------------------------------

