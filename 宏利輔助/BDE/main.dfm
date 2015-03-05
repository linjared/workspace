object Form1: TForm1
  Left = 228
  Top = 121
  Width = 913
  Height = 580
  Caption = 'DBF Viewer'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poDesktopCenter
  OnResize = FormResize
  PixelsPerInch = 96
  TextHeight = 13
  object DBGrid1: TDBGrid
    Left = 9
    Top = 45
    Width = 887
    Height = 489
    DataSource = DataSource1
    TabOrder = 0
    TitleFont.Charset = DEFAULT_CHARSET
    TitleFont.Color = clWindowText
    TitleFont.Height = -11
    TitleFont.Name = 'MS Sans Serif'
    TitleFont.Style = []
  end
  object OpenButton: TButton
    Left = 10
    Top = 8
    Width = 75
    Height = 25
    Caption = 'Open'
    TabOrder = 1
    OnClick = OpenButtonClick
  end
  object DBNavigator1: TDBNavigator
    Left = 184
    Top = 8
    Width = 240
    Height = 25
    DataSource = DataSource1
    TabOrder = 2
  end
  object DataSource1: TDataSource
    DataSet = Query1
    Left = 232
    Top = 224
  end
  object Table1: TTable
    Left = 264
    Top = 224
  end
  object Query1: TQuery
    Left = 296
    Top = 224
  end
  object OpenDialog1: TOpenDialog
    FileName = 'Z:\Recipe2.DBF'
    Filter = 'DBase File|*.DBF|All Files (*.*)|*.*'
    Left = 328
    Top = 224
  end
end
