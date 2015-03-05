//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "main.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
    : TForm(Owner)
{
/*
  this->Constraints->MinHeight=this->Height;
  this->Constraints->MinWidth=this->Width;

  for(int i=0;i<this->ComponentCount;i++)
  {
    TControl *control=dynamic_cast<TControl *>(this->Components[i]);
    if(control!=NULL)
      control->Anchors.Clear();
  }
*/
}
//---------------------------------------------------------------------------

void __fastcall TForm1::OpenButtonClick(TObject *Sender)
{
    AnsiString Selected_File , File_Name , File_Drive , File_Path ;
    OpenDialog1->Title = "Select DBF File";
    if (OpenDialog1->Execute())
    {
        if (FileExists(OpenDialog1->FileName))
        {
            Selected_File = OpenDialog1->FileName ;
            File_Drive = ExtractFileDrive(Selected_File) ;
            File_Path = ExtractFileDir(Selected_File) ;
            File_Name = ExtractFileName(Selected_File) ;
            /*
            Table1->Active = false;
            Table1->DatabaseName = File_Path ;
            Table1->TableName = File_Name ;
            Table1->Active = true ;
            */
            Query1->DatabaseName = File_Path ;
            Query1->Close();
            Query1->SQL->Clear();
            Query1->SQL->Text = "SELECT * FROM " + File_Name ;
            Query1->ExecSQL();
            Query1->Active=true;
            DBGrid1->DataSource = DataSource1 ;
            DataSource1->DataSet = Query1 ;
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormResize(TObject *Sender)
{
    OpenButton->Top = 10 ;
    OpenButton->Left = 10 ;
    DBGrid1->Left = 10 ;
    DBGrid1->Width = Form1->Width - 30 ;
    DBGrid1->Height = Form1->Height - 90 ;
    DBGrid1->Refresh() ;
    Application->ProcessMessages() ;
}
//---------------------------------------------------------------------------

