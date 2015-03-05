object Form1: TForm1
  Left = 0
  Top = 0
  Caption = 'Form1'
  ClientHeight = 292
  ClientWidth = 418
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object DBGrid1: TDBGrid
    Left = 0
    Top = 0
    Width = 418
    Height = 201
    Align = alTop
    DataSource = DataSource1
    TabOrder = 0
    TitleFont.Charset = DEFAULT_CHARSET
    TitleFont.Color = clWindowText
    TitleFont.Height = -11
    TitleFont.Name = 'Tahoma'
    TitleFont.Style = []
    Columns = <
      item
        Expanded = False
        FieldName = 'name'
        Width = 50
        Visible = True
      end
      item
        Expanded = False
        FieldName = 'id'
        Width = 50
        Visible = True
      end
      item
        Expanded = False
        FieldName = 'xtype'
        Width = 50
        Visible = True
      end
      item
        Expanded = False
        FieldName = 'uid'
        Width = 50
        Visible = True
      end
      item
        Expanded = False
        FieldName = 'info'
        Width = 50
        Visible = True
      end
      item
        Expanded = False
        FieldName = 'status'
        Width = 50
        Visible = True
      end
      item
        Expanded = False
        FieldName = 'base_schema_ver'
        Width = 50
        Visible = True
      end
      item
        Expanded = False
        FieldName = 'replinfo'
        Width = 50
        Visible = True
      end
      item
        Expanded = False
        FieldName = 'parent_obj'
        Width = 50
        Visible = True
      end
      item
        Expanded = False
        FieldName = 'crdate'
        Width = 50
        Visible = True
      end
      item
        Expanded = False
        FieldName = 'ftcatid'
        Width = 50
        Visible = True
      end
      item
        Expanded = False
        FieldName = 'schema_ver'
        Width = 50
        Visible = True
      end
      item
        Expanded = False
        FieldName = 'stats_schema_ver'
        Width = 50
        Visible = True
      end
      item
        Expanded = False
        FieldName = 'type'
        Width = 50
        Visible = True
      end
      item
        Expanded = False
        FieldName = 'userstat'
        Width = 50
        Visible = True
      end
      item
        Expanded = False
        FieldName = 'sysstat'
        Width = 50
        Visible = True
      end
      item
        Expanded = False
        FieldName = 'indexdel'
        Width = 50
        Visible = True
      end
      item
        Expanded = False
        FieldName = 'refdate'
        Width = 50
        Visible = True
      end
      item
        Expanded = False
        FieldName = 'version'
        Width = 50
        Visible = True
      end
      item
        Expanded = False
        FieldName = 'deltrig'
        Width = 50
        Visible = True
      end
      item
        Expanded = False
        FieldName = 'instrig'
        Width = 50
        Visible = True
      end
      item
        Expanded = False
        FieldName = 'updtrig'
        Width = 50
        Visible = True
      end
      item
        Expanded = False
        FieldName = 'seltrig'
        Width = 50
        Visible = True
      end
      item
        Expanded = False
        FieldName = 'category'
        Width = 50
        Visible = True
      end
      item
        Expanded = False
        FieldName = 'cache'
        Width = 50
        Visible = True
      end>
  end
  object DBNavigator1: TDBNavigator
    Left = 48
    Top = 207
    Width = 240
    Height = 25
    DataSource = DataSource1
    TabOrder = 1
  end
  object ADOConnection1: TADOConnection
    Connected = True
    ConnectionString = 
      'Provider=SQLOLEDB.1;Password=12345;Persist Security Info=True;Us' +
      'er ID=jared;Initial Catalog=test;Data Source=ARVIN-PC\SQLEXPRESS'
    Provider = 'SQLOLEDB.1'
    Left = 64
    Top = 40
  end
  object ADOQuery1: TADOQuery
    Active = True
    Connection = ADOConnection1
    CursorType = ctStatic
    Parameters = <>
    SQL.Strings = (
      'Select * from SysObjects')
    Left = 160
    Top = 40
  end
  object DataSource1: TDataSource
    DataSet = ADOQuery1
    Left = 232
    Top = 40
  end
end
