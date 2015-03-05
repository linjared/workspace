object Form1: TForm1
  Left = 0
  Top = 0
  Caption = 'Form1'
  ClientHeight = 466
  ClientWidth = 692
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object Label4: TLabel
    Left = 342
    Top = 298
    Width = 3
    Height = 13
  end
  object DBGrid1: TDBGrid
    Left = 8
    Top = 8
    Width = 369
    Height = 201
    DataSource = DataSource1
    TabOrder = 0
    TitleFont.Charset = DEFAULT_CHARSET
    TitleFont.Color = clWindowText
    TitleFont.Height = -11
    TitleFont.Name = 'Tahoma'
    TitleFont.Style = []
  end
  object Button1: TButton
    Left = 32
    Top = 249
    Width = 75
    Height = 25
    Caption = #38283#21855#36039#26009#24235
    TabOrder = 1
    OnClick = Button1Click
  end
  object Button2: TButton
    Left = 128
    Top = 249
    Width = 75
    Height = 25
    Caption = #36899#32218
    TabOrder = 2
    OnClick = Button2Click
  end
  object DBNavigator1: TDBNavigator
    Left = 104
    Top = 218
    Width = 240
    Height = 25
    DataSource = DataSource1
    TabOrder = 3
  end
  object ADOConnection1: TADOConnection
    Connected = True
    ConnectionString = 
      'Provider=SQLOLEDB.1;Password=12345;Persist Security Info=True;Us' +
      'er ID=jared;Initial Catalog=test;Data Source=ARVIN-PC\SQLEXPRESS'
    LoginPrompt = False
    Provider = 'SQLOLEDB.1'
    Left = 104
    Top = 72
  end
  object ADOTable1: TADOTable
    Connection = ADOConnection1
    Left = 200
    Top = 144
  end
  object DataSource1: TDataSource
    DataSet = ADOTable1
    Left = 200
    Top = 72
  end
  object ADOStoredProc1: TADOStoredProc
    Connection = ADOConnection1
    ProcedureName = 'sp_setapprole;1'
    Parameters = <
      item
        Name = '@RETURN_VALUE'
        DataType = ftInteger
        Direction = pdReturnValue
        Precision = 10
        Value = Null
      end
      item
        Name = '@rolename'
        Attributes = [paNullable]
        DataType = ftWideString
        Size = 128
        Value = 'mytest'
      end
      item
        Name = '@password'
        Attributes = [paNullable]
        DataType = ftWideString
        Size = 128
        Value = '12345'
      end
      item
        Name = '@encrypt'
        Attributes = [paNullable]
        DataType = ftString
        Size = 10
        Value = 'none'
      end
      item
        Name = '@fCreateCookie'
        Attributes = [paNullable]
        DataType = ftBoolean
        Value = Null
      end
      item
        Name = '@cookie'
        Attributes = [paNullable]
        DataType = ftVarBytes
        Direction = pdInputOutput
        Size = 50
        Value = Null
      end>
    Left = 288
    Top = 72
  end
end
