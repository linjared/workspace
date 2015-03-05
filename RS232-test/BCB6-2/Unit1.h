//---------------------------------------------------------------------------

#ifndef Unit1H
#define Unit1H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include "Comm.h"
void Delay(DWORD DT);
String InCheckSum(String InBuf);
String OutCheckSum(String OutStr);
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
    TLabel *Label1;
    TLabel *Label4;
    TEdit *txtInput;
    TEdit *txtCS;
    TBitBtn *BitBtn1;
    TBitBtn *BitBtn2;
    TComm *Comm1;
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall BitBtn1Click(TObject *Sender);
    void __fastcall BitBtn2Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TForm1(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
