//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit1.h"
#include "inifiles.hpp"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
int i=0;          // i為密碼正確與否變數
int j,k;          // j為名稱LA  k為密碼
TForm1 *Form1;
//int __fastcall MessageBox(const char * Text,const char * Caption,int Flags);
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
    : TForm(Owner)
{

}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button1Click(TObject *Sender)
{
          TIniFile *StartUp = new TIniFile("C:\\users\\user\\Desktop\\Jared.Lin\\work1\\password.ini");                //建立.ini檔路徑
          AnsiString k = StartUp->ReadString("password", "key", 123);                                                  //設定密碼
          AnsiString j="LA";                                                                                           //設定名稱

                if (i==0&&Edit1->Text==j&&Edit2->Text==k)                                                              //確認名稱.密碼
                 {
                 Application->MessageBox("正確","訊息",0);
                 Button2->Enabled=true;
                 Edit2->Text="";
                 }
                 else if(i==1&&Edit2->Text==Edit3->Text )                                                              //判斷正確與否
                      {
                      AnsiString a=Edit2->Text.Trim();
                      Application->MessageBox("正確，已儲存","訊息",0);
                      Button2->Enabled=false;
                      Label3->Enabled=false;
                      Edit3->Enabled=false;
                      Edit1->Enabled=true;
                      Edit2->Text="";
                      Edit3->Text="";
                      Label2->Caption="密碼";
                      TIniFile *StartUp = new TIniFile("C:\\users\\user\\Desktop\\Jared.Lin\\work1\\password.ini");    //儲存.ini檔路徑
                      StartUp->WriteString("password","key",a);                                                        //寫入的名稱
                      StartUp->UpdateFile();
                      i=0;
                      }
                       else if(i==1&&Edit2->Text!=Edit3->Text)                                                         //判斷更改密碼
                            {
                             Application->MessageBox("密碼不一致","警告",0+48);
                             Edit2->Text="";
                             Edit3->Text="";
                             i=1;
                            }
                             else                                                                                      //密碼變認
                               {
                               Application->MessageBox("錯誤，在試一次","警告",1+64);
                               Button2->Enabled=false;
                               }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button2Click(TObject *Sender)                                                                  //修改密碼
{
     Label3->Enabled=true;
     Edit3->Enabled=true;
     Edit1->Enabled=false;
     Label2->Caption="新密碼";
     Edit2->Text="";
     Edit3->Text="";
     i=1;
}
//---------------------------------------------------------------------------

