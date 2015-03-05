//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
    : TForm(Owner)
{               
Edit4->Enabled=false;        //將Edit4禁止輸入
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button1Click(TObject *Sender)
{

int i = Edit1->Text.ToInt();
int j = Edit2->Text.ToInt();       //將輸入值取出計算
int k = Edit3->Text.ToInt();

int a=1,b=1;
for(int y=1;y<=j;y++)
a*=y;                         //計算N!
for(int z=1;z<=k;z++)
b*=z;

/*
TLabel *count = new TLabel(this);
count->Caption=i*(b/a);     顯示在面板上
count->Parent=Form1;
*/
Edit4->Text=i*(b/a);
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------


//---------------------------------------------------------------------------


