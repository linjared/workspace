//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;              
HANDLE hComm;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------


void __fastcall TForm1::Button1Click(TObject *Sender)
{
char *ComNo;
DCB dcb;
String Temp;
//取得預開啟的通訊埠
Temp="COM"+IntToStr(rdCom->ItemIndex+1);
//轉換至指標型態Char
ComNo=Temp.c_str();
hComm=CreateFile(ComNo,GENERIC_READ | GENERIC_WRITE,0,NULL,OPEN_EXISTING,0,0);
    if(hComm == INVALID_HANDLE_VALUE)    //如果COM未開啟
    {
    MessageBox(0,"開啟通訊埠錯誤!!","Comm Error",MB_OK);
    return;
    }
//將dcb位址傳入，以取得通訊參數
GetCommState(hComm,&dcb);  //得知目前COM的狀態
dcb.BaudRate=CBR_9600;     //設定鮑率為9600
dcb.ByteSize=8;            //位元組為8bit
dcb.Parity=NOPARITY;       //Parity為None
dcb.StopBits=ONESTOPBIT;   //1個Stop bit
//通訊埠設定
    if(!SetCommState(hComm,&dcb))
    {                 //設定COM的狀態
    MessageBox(0,"通訊埠設定錯誤!!!","Set Error",MB_OK);
    CloseHandle(hComm);
    return;
    }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button3Click(TObject *Sender)
{
String Temp;
char *SendData;
int ln;
unsigned long lrc,BS;
if(hComm==0)return;       //檢查Handle值
Temp = mSend->Text;       //取得傳送的字串
SendData = Temp.c_str();  //字串轉換
//取得傳送的字串數
BS = Temp.Length();
//BS=StrLen(SendData); //也可以使用此種方式取得字串長度
//實際的傳送動作
WriteFile(hComm,SendData,BS,&lrc,NULL);  //送出資料
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button4Click(TObject *Sender)
{
String Temp;
char inbuff[1024];
DWORD nBytesRead, dwEvent,dwError;
COMSTAT cs;
//取得狀態
ClearCommError(hComm,&dwError,&cs);
//資料是否大於我們所準備的Buffer
    if(cs.cbInQue > sizeof(inbuff))
    {
    PurgeComm(hComm,PURGE_RXCLEAR);  //清除COM資料
    return;
    }
ReadFile(hComm, inbuff,cs.cbInQue,&nBytesRead,NULL);  //接收COM的資料
//轉移資料到變數中
inbuff[cs.cbInQue] = '\0';
//將資料顯示於Memo1上
mReceive->Text = inbuff;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button2Click(TObject *Sender)
{
if(hComm!=INVALID_HANDLE_VALUE) CloseHandle(hComm);
exit(EXIT_SUCCESS);
}
//---------------------------------------------------------------------------

