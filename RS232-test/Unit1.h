//---------------------------------------------------------------------------

#ifndef Unit1H
#define Unit1H
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Forms.hpp>
#include "Comm.h"
#include <Vcl.Buttons.hpp>
void Delay(DWORD DT);
String InCheckSum(String InBuf);
String OutCheckSum(String OutStr);
//---------------------------------------------------------------------------
class TFrom1 : public TForm
{
__published:	// IDE-managed Components
	TComm *Comm1;
	TEdit *txtInput;
	TEdit *txtCS;
	TLabel *Label1;
	TLabel *Label2;
	TBitBtn *BitBtn1;
	TBitBtn *BitBtn2;
	void __fastcall BitBtn1Click(TObject *Sender);
	void __fastcall BitBtn2Click(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TFrom1(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFrom1 *From1;
//---------------------------------------------------------------------------
#endif
