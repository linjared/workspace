//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit1.h"
#include "inifiles.hpp"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
int i=0;          // i���K�X���T�P�_�ܼ�
int j,k;          // j���W��LA  k���K�X
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
          TIniFile *StartUp = new TIniFile("C:\\users\\user\\Desktop\\Jared.Lin\\work1\\password.ini");                //�إ�.ini�ɸ��|
          AnsiString k = StartUp->ReadString("password", "key", 123);                                                  //�]�w�K�X
          AnsiString j="LA";                                                                                           //�]�w�W��

                if (i==0&&Edit1->Text==j&&Edit2->Text==k)                                                              //�T�{�W��.�K�X
                 {
                 Application->MessageBox("���T","�T��",0);
                 Button2->Enabled=true;
                 Edit2->Text="";
                 }
                 else if(i==1&&Edit2->Text==Edit3->Text )                                                              //�P�_���T�P�_
                      {
                      AnsiString a=Edit2->Text.Trim();
                      Application->MessageBox("���T�A�w�x�s","�T��",0);
                      Button2->Enabled=false;
                      Label3->Enabled=false;
                      Edit3->Enabled=false;
                      Edit1->Enabled=true;
                      Edit2->Text="";
                      Edit3->Text="";
                      Label2->Caption="�K�X";
                      TIniFile *StartUp = new TIniFile("C:\\users\\user\\Desktop\\Jared.Lin\\work1\\password.ini");    //�x�s.ini�ɸ��|
                      StartUp->WriteString("password","key",a);                                                        //�g�J���W��
                      StartUp->UpdateFile();
                      i=0;
                      }
                       else if(i==1&&Edit2->Text!=Edit3->Text)                                                         //�P�_���K�X
                            {
                             Application->MessageBox("�K�X���@�P","ĵ�i",0+48);
                             Edit2->Text="";
                             Edit3->Text="";
                             i=1;
                            }
                             else                                                                                      //�K�X�ܻ{
                               {
                               Application->MessageBox("���~�A�b�դ@��","ĵ�i",1+64);
                               Button2->Enabled=false;
                               }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button2Click(TObject *Sender)                                                                  //�ק�K�X
{
     Label3->Enabled=true;
     Edit3->Enabled=true;
     Edit1->Enabled=false;
     Label2->Caption="�s�K�X";
     Edit2->Text="";
     Edit3->Text="";
     i=1;
}
//---------------------------------------------------------------------------

