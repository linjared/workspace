//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
Teee *eee;
//---------------------------------------------------------------------------
__fastcall Teee::Teee(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------




void __fastcall Teee::FileListBox1Change(TObject *Sender)
{
if (!FileListBox1->SelCount) return;
   ComboBox1->Text = FileListBox1->FileName;

}
//---------------------------------------------------------------------------

void __fastcall Teee::DirectoryListBox1Change(TObject *Sender)
{
bool bNotAtList=true;

for(int i=0; i<ComboBox1->Items->Count;i++)
   {
   if(ComboBox1->Items->Strings[1] == DirectoryListBox1->Directory)
	  {
	   bNotAtList = false;
	   break;
	  }
   }

if (bNotAtList)
	ComboBox1->Items->Add(DirectoryListBox1->Directory);
	ComboBox1->Text = DirectoryListBox1->Directory;
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------


void __fastcall Teee::ComboBox1Change(TObject *Sender)
{
DirectoryListBox1->Directory = ComboBox1->Text;
}
//---------------------------------------------------------------------------

