object From1: TFrom1
  Left = 0
  Top = 0
  Caption = 'Form1'
  ClientHeight = 619
  ClientWidth = 674
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  OnCreate = FormCreate
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 142
    Top = 207
    Width = 48
    Height = 13
    Caption = #36664#20837#23383#20018
  end
  object Label2: TLabel
    Left = 374
    Top = 207
    Width = 48
    Height = 13
    Caption = #25910#21462#36039#26009
  end
  object txtInput: TEdit
    Left = 142
    Top = 239
    Width = 121
    Height = 21
    TabOrder = 0
  end
  object txtCS: TEdit
    Left = 374
    Top = 239
    Width = 121
    Height = 21
    TabOrder = 1
  end
  object BitBtn1: TBitBtn
    Left = 188
    Top = 316
    Width = 75
    Height = 25
    Caption = #23383#20018#36865#20986
    ModalResult = 1
    TabOrder = 2
    OnClick = BitBtn1Click
  end
  object BitBtn2: TBitBtn
    Left = 420
    Top = 316
    Width = 75
    Height = 25
    Caption = #32080#26463
    ModalResult = 8
    TabOrder = 3
    OnClick = BitBtn2Click
  end
  object Comm1: TComm
    DeviceName = 'Com2'
    MonitorEvents = [evBreak, evCts, evDsr, evError, evRing, evRlsd, evRxChar, evRxFlag, evTxEmpty]
    Left = 300
    Top = 261
  end
end
