//---------------------------------------------------------------------------

#ifndef Unit1H
#define Unit1H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "Comm.h"

#include "TPLCComm.h"
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
    TComm *Comm1;
    TPanel *Panel1;
    TButton *InitPLCCommButton;
    TButton *OpenPLCCommButton;
    TLabel *Label2;
    TLabel *Label3;
    TLabel *Label1;
    TLabel *Label4;
    TLabel *Label5;
    TPageControl *PageControl1;
    TTabSheet *TabSheet1;
    TTabSheet *TabSheet2;
    TTabSheet *TabSheet3;
    TTabSheet *TabSheet4;
    TTabSheet *TabSheet5;
    TEdit *DINumberEdit;
    TLabel *Label6;
    TLabel *Label7;
    TEdit *DICodeEdit;
    TLabel *Label8;
    TLabel *Label9;
    TLabel *Label10;
    TButton *DINumberButton;
    TButton *DICodeButton;
    TButton *DIbyIOButton;
    TLabel *Label11;
    TShape *DINumberShape;
    TShape *DICodeShape;
    TShape *DIbyIOShape;
    TPanel *Panel2;
    TLabel *Label12;
    TLabel *CPUStateLabel;
    TLabel *RxLabel;
    TLabel *Label15;
    TLabel *TxLabel;
    TLabel *Label17;
    TLabel *Label13;
    TLabel *Label14;
    TEdit *DONumberEdit;
    TButton *DONumberButton;
    TShape *DONumberShape;
    TShape *DOCodeShape;
    TButton *DOCodeButton;
    TEdit *DOCodeEdit;
    TLabel *Label16;
    TLabel *Label18;
    TLabel *Label19;
    TLabel *Label20;
    TButton *DObyIOButton;
    TShape *DObyIOShape;
    TLabel *Label21;
    TLabel *Label22;
    TEdit *AINumberEdit;
    TButton *AINumberButton;
    TButton *AICodeButton;
    TEdit *AICodeEdit;
    TLabel *Label23;
    TLabel *Label24;
    TLabel *Label25;
    TLabel *Label26;
    TButton *AIbyIOButton;
    TLabel *Label27;
    TLabel *Label28;
    TEdit *AONumberEdit;
    TButton *AONumberButton;
    TButton *AOCodeButton;
    TEdit *AOCodeEdit;
    TLabel *Label29;
    TLabel *Label30;
    TLabel *Label31;
    TLabel *Label32;
    TButton *AObyIOButton;
    TLabel *Label33;
    TLabel *Label34;
    TEdit *CounterNumberEdit;
    TButton *CounterNumberButton;
    TButton *CounterCodeButton;
    TEdit *CounterCodeEdit;
    TLabel *Label35;
    TLabel *Label36;
    TLabel *Label37;
    TLabel *Label38;
    TButton *CounterbyIOButton;
    TLabel *Label39;
    TEdit *AINumberValueEdit;
    TEdit *AICodeValueEdit;
    TEdit *AIbyIOValueEdit;
    TLabel *Label40;
    TEdit *AONumberValueEdit;
    TEdit *AOCodeValueEdit;
    TEdit *AObyIOValueEdit;
    TLabel *Label41;
    TLabel *Label42;
    TEdit *TempNumberEdit;
    TButton *TempButton;
    TEdit *TempValueEdit;
    TLabel *Label43;
    TEdit *CounterNumberValueEdit;
    TEdit *CounterCodeValueEdit;
    TEdit *CounterbyIOValueEdit;
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall FormDestroy(TObject *Sender);
    void __fastcall OpenPLCCommButtonClick(TObject *Sender);
    void __fastcall InitPLCCommButtonClick(TObject *Sender);
    void __fastcall DINumberButtonClick(TObject *Sender);
    void __fastcall DICodeButtonClick(TObject *Sender);
    void __fastcall DIbyIOButtonClick(TObject *Sender);
    void __fastcall DONumberButtonClick(TObject *Sender);
    void __fastcall DOCodeButtonClick(TObject *Sender);
    void __fastcall DObyIOButtonClick(TObject *Sender);
    void __fastcall AINumberButtonClick(TObject *Sender);
    void __fastcall AICodeButtonClick(TObject *Sender);
    void __fastcall AIbyIOButtonClick(TObject *Sender);
    void __fastcall TempButtonClick(TObject *Sender);
    void __fastcall AONumberButtonClick(TObject *Sender);
    void __fastcall AOCodeButtonClick(TObject *Sender);
    void __fastcall AObyIOButtonClick(TObject *Sender);
    void __fastcall CounterNumberButtonClick(TObject *Sender);
    void __fastcall CounterCodeButtonClick(TObject *Sender);
    void __fastcall CounterbyIOButtonClick(TObject *Sender);
private:	// User declarations

	void __fastcall PLCRx(TObject *Sender,unsigned char CPU_ID,RxState State);
	void __fastcall PLCTx(TObject *Sender,unsigned char CPU_ID);
	void __fastcall PLCTimeout(TObject *Sender,unsigned char CPU_ID);



public:		// User declarations
    __fastcall TForm1(TComponent* Owner);

    TPLCComm *PLC;
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
