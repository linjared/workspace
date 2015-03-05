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
  //�}�ҹw�]���q�T��(�w�]�OpnCOM1)
  Comm1->PortOpen = true;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::BitBtn1Click(TObject *Sender)
{
  //�ŧiString�ܼƨϥ�
  String  InputStr,tmpStr,RStr;
   //�r����w
   InputStr=txtInput->Text;
   //���@CheckSum���B��
   tmpStr=OutCheckSum(InputStr);
   //�N�r��e�X
   Comm1->OutputString(tmpStr);
   Delay(100); //����100�@��
   RStr=Comm1->Input; //�������G
   //�N���G��ܥX��
   if (RStr.Length()==0)
     txtCS->Text="�L��Ʃο��~";
   else
     txtCS->Text=InCheckSum(RStr); //�Ǧ^�ˬd�����G

}
//--------------------------------------------------------------------------
//�H�U����禡CheckSum���B��
String OutCheckSum(String OutStr)
{
  int BufLen,i;
  String CHKBuf;
  DWORD CheckSum; //CheckSum�ƭ�

  //���o�ǤJ�r�ꪺ����
  BufLen = OutStr.Length();
  //��l��
  CheckSum = 0;
  for (i = 1 ;i<= BufLen;i++)
    //���X�r��,�M0xFF�@AND�B��i�H�N���G���w�b0xFF�H��
    CheckSum=(CheckSum+(int)OutStr[i]) & 0xFF ;
  CHKBuf = CHKBuf.IntToHex(CheckSum,2);
  //�r�꭫�աA�å[�W�����r��Cr
  return(OutStr + CHKBuf + '\x0d');
}
//--------------------------------------------------------------
//�H�U�O�����ƪ�CheckSum�B��
String InCheckSum(String InBuf)
{
  int BufLen,i;
  DWORD CheckSum,OrgCHK;

  //���o�ǤJ�r�ꪺ����
  BufLen = InBuf.Length();
  //��l��
  CheckSum = 0;
  for (i = 1;i< BufLen-2;i++)
    //���X�r��,�M0xFF�@AND�B��i�H�N���G���w�b0xFF�H��
    CheckSum=(CheckSum+(int)InBuf[i]) & 0xFF;
  //���o��l��CheckSum�A�ñN���ഫ���ƭ�
  OrgCHK=StrToInt("0x" + InBuf.SubString(BufLen-3+1,2));
  if (OrgCHK==CheckSum)
  //�r�꭫�աA�Ǧ^���tCheckSum����r��
    return(InBuf.SubString(1,BufLen-3));
  else
    //�ζǦ^�Ŧr��A��ܱ�������
    return("");
}



//---------------------------------------------------------------------------
//�ɶ�����禡�A���O�@��
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
 //�����{��
 exit(0);
}
//---------------------------------------------------------------------------


