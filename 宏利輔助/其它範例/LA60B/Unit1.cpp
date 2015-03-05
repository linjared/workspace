//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "Comm"
#pragma resource "*.dfm"
TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormCreate(TObject *Sender)
{
    ;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormDestroy(TObject *Sender)
{
    if( PLC )
    {
        delete PLC;
        PLC = NULL;
    }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::InitPLCCommButtonClick(TObject *Sender)
{
    PLC = new TPLCComm( this );
    PLC->ComPort        = Comm1;
    PLC->OnAfterSend    = PLCTx;            // you should define the PLCTx function
    PLC->OnAfterReceive = PLCRx;            // you should define the PLCRx function
    PLC->OnAfterTimeout = PLCTimeout;       // you should define the PLCTimeout function
}
//---------------------------------------------------------------------------
void __fastcall TForm1::PLCRx(TObject *Sender,unsigned char CPU_ID,RxState State)
{
    // if you connect to many CPUs
    // you can identify with CPU_ID

	switch( State )
	{
        case COMM_RX_OK:
            CPUStateLabel->Caption  = "ON";
            break;

		case COMM_LENGTH_ERR:
            CPUStateLabel->Caption  = "Len Err";
            break;

        case COMM_CHECKSUM_ERR:
            CPUStateLabel->Caption  = "ChkSum Err";
            break;

        case COMM_DATA_ERR:
        default:
            CPUStateLabel->Caption  = "Data Err";
            break;
	}
    RxLabel->Caption    = RxLabel->Caption.ToIntDef(0) + 1;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::PLCTx(TObject *Sender,unsigned char CPU_ID)
{
    // if you connect to many CPUs
    // you can identify with CPU_ID

    TxLabel->Caption    = TxLabel->Caption.ToIntDef(0) + 1;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::PLCTimeout(TObject *Sender,unsigned char CPU_ID)
{
    // if you connect to many CPUs
    // you can identify with CPU_ID

    CPUStateLabel->Caption  = "OFF";
}
//---------------------------------------------------------------------------
void __fastcall TForm1::OpenPLCCommButtonClick(TObject *Sender)
{
    String setting_path;

    setting_path    = "D:\\PLC60B.ini";
	if( !PLC->LoadParameter( setting_path ) )
		return;

	PLC->ComPort->Close();
	PLC->ComPort->DeviceName	= "Com1";
	PLC->BaudRate		        = 57600;
	PLC->DataBits		        = 7;
	PLC->StopBits		        = 1;
	PLC->Parity			        = "EVEN";
	PLC->ComPort->Open();
	PLC->Enabled	= true;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::DINumberButtonClick(TObject *Sender)
{
    int io_number;
    TColor color;


    io_number   = DINumberEdit->Text.ToIntDef(-1);
    if( PLC->GetDI( io_number ) == 0 )
        color   = clWhite;
    else
        color   = clLime;

    DINumberShape->Brush->Color = color;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::DICodeButtonClick(TObject *Sender)
{
    String code;
    int number;
    TColor color;


    // It must assigned the IO code to PLCComm before used
    // It may assign when load IO paramters
    code    = DICodeEdit->Text.Trim();
    number  = 18;
    if( !PLC->DefineIOCode( ioDI, number, code ) )
    {
        ShowMessage( "Can't find the IO" );
    }

    // After assigned the IO code, we can get the value with code
    if( PLC->GetDI( code ) == 0 )
        color   = clWhite;
    else
        color   = clLime;

    DICodeShape->Brush->Color = color;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::DIbyIOButtonClick(TObject *Sender)
{
    String code;
    TLA60BIO *io;
    TColor color;


    io  = new TLA60BIO( ioDI );     // create by ioDI type

    // you can get value by IO object with 3 methods
    // 1. give the IO number
    io->IONumber    = 20;

    // 2. give the IO code
    //    It also must assigned to PLCComm first.
    //io->Code      = "Button1";

    // 3. give the io device value
    /*
    io->CPU_ID      = 1;
    io->RegType     = REG_M;
    io->RegAddr     = 0;
    */

    if( !PLC->LinkIO( io ) )
    {
        ShowMessage( "Can't find the IO" );
    }

    // After assigned the IO code, we can get the value with code
    if( io->Value == 0 )
        color   = clWhite;
    else
        color   = clLime;

    DIbyIOShape->Brush->Color = color;

    delete io;
    io  = NULL;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::DONumberButtonClick(TObject *Sender)
{
    int io_number;
    TColor color;


    io_number   = DINumberEdit->Text.ToIntDef(-1);
    if( PLC->GetDO( io_number ) == 0 )
    {
        if( PLC->OH( io_number ) )
            color   = clRed;
    }
    else
    {
        if( PLC->OL( io_number ) )
            color   = clWhite;
    }

    DONumberShape->Brush->Color = color;

}
//---------------------------------------------------------------------------

void __fastcall TForm1::DOCodeButtonClick(TObject *Sender)
{
    String code;
    int number;
    TColor color;


    // It must assigned the IO code to PLCComm before used
    // It may assign all IO code when load IO paramters
    code    = DOCodeEdit->Text.Trim();
    number  = 18;
    if( !PLC->DefineIOCode( ioDO, number, code ) )
    {
        ShowMessage( "Can't find the IO" );
    }

    // After assigned the IO code, we can Set the value with code
    if( PLC->GetDO( code ) == 0 )
    {
        if( PLC->OH( code ) )
            color   = clRed;
    }
    else
    {
        if( PLC->OL( code ) )
            color   = clWhite;
    }

    DOCodeShape->Brush->Color = color;

}
//---------------------------------------------------------------------------
void __fastcall TForm1::DObyIOButtonClick(TObject *Sender)
{
    String code;
    TLA60BIO *io;
    TColor color;


    io  = new TLA60BIO( ioDO );     // create by ioDO type

    // you can get value by IO object with 3 methods
    // 1. give the IO number
    io->IONumber    = 20;

    // 2. give the IO code
    //    It also must assigned to PLCComm first.
    //io->Code      = "Light1";

    // 3. give the io device value
    /*
    io->CPU_ID      = 1;
    io->RegType     = REG_M;
    io->RegAddr     = 400;
    */

    if( !PLC->LinkIO( io ) )
    {
        ShowMessage( "Can't find the IO" );
    }

    // After assigned the IO code, we can Set the value with code
    if( io->Value == 0 )
    {
        io->Value   = 1;
        color       = clRed;
    }
    else
    {
        io->Value   = 0;
        color       = clWhite;
    }

    DObyIOShape->Brush->Color = color;

    delete io;
    io  = NULL;

}
//---------------------------------------------------------------------------

void __fastcall TForm1::AINumberButtonClick(TObject *Sender)
{
    int io_number;

    io_number   = AINumberEdit->Text.ToIntDef(-1);
    AINumberValueEdit->Text =  PLC->GetAI( io_number );
}
//---------------------------------------------------------------------------

void __fastcall TForm1::AICodeButtonClick(TObject *Sender)
{
    String code;
    int number;

    // It must assigned the IO code to PLCComm before used
    // It may assign when load IO paramters
    code    = AICodeEdit->Text.Trim();
    number  = 0;
    if( !PLC->DefineIOCode( ioAI, number, code ) )
    {
        ShowMessage( "Can't find the IO" );
    }

    // After assigned the IO code, we can get the value with code
    AICodeValueEdit->Text =  PLC->GetAI( code );
}
//---------------------------------------------------------------------------

void __fastcall TForm1::AIbyIOButtonClick(TObject *Sender)
{
    String code;
    TLA60BIO *io;
    TColor color;


    io  = new TLA60BIO( ioAI );     // create by ioAI type

    // you can get value by IO object with 3 methods
    // 1. give the IO number
    io->IONumber    = 1;

    // 2. give the IO code
    //    It also must assigned to PLCComm first.
    //io->Code      = "AI-1";

    // 3. give the io device value
    /*
    io->CPU_ID      = 1;
    io->RegType     = REG_D;
    io->RegAddr     = 0;
    */

    if( !PLC->LinkIO( io ) )
    {
        ShowMessage( "Can't find the IO" );
    }

    AIbyIOValueEdit->Text =  io->Value;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::TempButtonClick(TObject *Sender)
{
    // It shows how to transfer AI value to Temperature with IO number
    // you also can used IO's Code or IO object to transfer it.

    int io_number;

    io_number   = AINumberEdit->Text.ToIntDef(-1);
    TempValueEdit->Text = FormatFloat( "0.0", PLC->GetTemperature( io_number ) );
//    TempValueEdit->Text = FormatFloat( "0.0", PLC->GetTemperature( code ) );          // with code
//    TempValueEdit->Text = FormatFloat( "0.0", PLC->GetTemperature( io ) );            // with TLA60BIO
}
//---------------------------------------------------------------------------

void __fastcall TForm1::AONumberButtonClick(TObject *Sender)
{
    int io_number;
    WORD value;

    io_number   = AONumberEdit->Text.ToIntDef(-1);
    value       = 128;      // set the value you want.

    if( PLC->SetAO( io_number, value ) )
        AONumberValueEdit->Text = PLC->GetAO( io_number );
}
//---------------------------------------------------------------------------

void __fastcall TForm1::AOCodeButtonClick(TObject *Sender)
{
    String code;
    int number;
    WORD value;


    // It must assigned the IO code to PLCComm before used
    // It may assign when load IO paramters
    code    = AOCodeEdit->Text.Trim();
    number  = 0;
    if( !PLC->DefineIOCode( ioAO, number, code ) )
    {
        ShowMessage( "Can't find the IO" );
    }

    // After assigned the IO code, we can Set the value with code
    value       = 127;      // set the value you want.
    if( PLC->SetAO( code, value ) )
        AOCodeValueEdit->Text = PLC->GetAO( code );
}
//---------------------------------------------------------------------------

void __fastcall TForm1::AObyIOButtonClick(TObject *Sender)
{
    String code;
    TLA60BIO *io;
    TColor color;


    io  = new TLA60BIO( ioAO );     // create by ioAI type

    // you can get value by IO object with 3 methods
    // 1. give the IO number
    io->IONumber    = 1;

    // 2. give the IO code
    //    It also must assigned to PLCComm first.
    //io->Code      = "AO-1";

    // 3. give the io device value
    /*
    io->CPU_ID      = 1;
    io->RegType     = REG_D;
    io->RegAddr     = 0;
    */

    if( !PLC->LinkIO( io ) )
    {
        ShowMessage( "Can't find the IO" );
    }

    AObyIOValueEdit->Text =  io->Value;

}
//---------------------------------------------------------------------------

void __fastcall TForm1::CounterNumberButtonClick(TObject *Sender)
{
    int io_number;

    io_number   = CounterNumberEdit->Text.ToIntDef(-1);
    CounterNumberValueEdit->Text =  PLC->GetCounter( io_number );
}
//---------------------------------------------------------------------------

void __fastcall TForm1::CounterCodeButtonClick(TObject *Sender)
{
    String code;
    int number;

    // It must assigned the IO code to PLCComm before used
    // It may assign when load IO paramters
    code    = CounterCodeEdit->Text.Trim();
    number  = 0;
    if( !PLC->DefineIOCode( ioCOUNTER, number, code ) )
    {
        ShowMessage( "Can't find the IO" );
    }

    // After assigned the IO code, we can get the value with code
    CounterCodeValueEdit->Text =  PLC->GetCounter( code );

}
//---------------------------------------------------------------------------

void __fastcall TForm1::CounterbyIOButtonClick(TObject *Sender)
{
    String code;
    TLA60BIO *io;
    TColor color;


    io  = new TLA60BIO( ioCOUNTER );     // create by ioAI type

    // you can get value by IO object with 3 methods
    // 1. give the IO number
    io->IONumber    = 1;

    // 2. give the IO code
    //    It also must assigned to PLCComm first.
    //io->Code      = "Counter-1";

    // 3. give the io device value
    /*
    io->CPU_ID      = 1;
    io->RegType     = REG_D;
    io->RegAddr     = 0;
    */

    if( !PLC->LinkIO( io ) )
    {
        ShowMessage( "Can't find the IO" );
    }

    CounterbyIOValueEdit->Text =  io->Value;

}
//---------------------------------------------------------------------------

