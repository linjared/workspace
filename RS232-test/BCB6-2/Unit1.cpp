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
  //開啟預設的通訊埠(預設是pnCOM1)
  Comm1->PortOpen = true;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::BitBtn1Click(TObject *Sender)
{
  //宣告String變數使用
  String  InputStr,tmpStr,RStr;
   //字串指定
   InputStr=txtInput->Text;
   //先作CheckSum的運算
   tmpStr=OutCheckSum(InputStr);
   //將字串送出
   Comm1->OutputString(tmpStr);
   Delay(100); //延遲100毫秒
   RStr=Comm1->Input; //收取結果
   //將結果顯示出來
   if (RStr.Length()==0)
     txtCS->Text="無資料或錯誤";
   else
     txtCS->Text=InCheckSum(RStr); //傳回檢查的結果

}
//--------------------------------------------------------------------------
//以下執行函式CheckSum的運算
String OutCheckSum(String OutStr)
{
  int BufLen,i;
  String CHKBuf;
  DWORD CheckSum; //CheckSum數值

  //取得傳入字串的長度
  BufLen = OutStr.Length();
  //初始化
  CheckSum = 0;
  for (i = 1 ;i<= BufLen;i++)
    //取出字元,和0xFF作AND運算可以將結果限定在0xFF以內
    CheckSum=(CheckSum+(int)OutStr[i]) & 0xFF ;
  CHKBuf = CHKBuf.IntToHex(CheckSum,2);
  //字串重組，並加上結尾字元Cr
  return(OutStr + CHKBuf + '\x0d');
}
//--------------------------------------------------------------
//以下是收到資料的CheckSum運算
String InCheckSum(String InBuf)
{
  int BufLen,i;
  DWORD CheckSum,OrgCHK;

  //取得傳入字串的長度
  BufLen = InBuf.Length();
  //初始化
  CheckSum = 0;
  for (i = 1;i< BufLen-2;i++)
    //取出字元,和0xFF作AND運算可以將結果限定在0xFF以內
    CheckSum=(CheckSum+(int)InBuf[i]) & 0xFF;
  //取得原始的CheckSum，並將其轉換為數值
  OrgCHK=StrToInt("0x" + InBuf.SubString(BufLen-3+1,2));
  if (OrgCHK==CheckSum)
  //字串重組，傳回不含CheckSum的原字串
    return(InBuf.SubString(1,BufLen-3));
  else
    //或傳回空字串，表示接收失敗
    return("");
}



//---------------------------------------------------------------------------
//時間延遲函式，單位是毫秒
void Delay(DWORD DT)
{
 long tt;
 tt=GetTickCount();
 while (GetTickCount()-tt<DT)
 {
  Application->ProcessMessages();
  if ((GetTickCount()-tt)<=0)
    tt=GetTickCount();
 }
}


void __fastcall TForm1::BitBtn2Click(TObject *Sender)
{
 //結束程式
 exit(0);
}
//---------------------------------------------------------------------------


