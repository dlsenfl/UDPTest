object Form2: TForm2
  Left = 0
  Top = 0
  Caption = 'Form2'
  ClientHeight = 482
  ClientWidth = 635
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object receive: TButton
    Left = 384
    Top = 248
    Width = 75
    Height = 25
    Caption = 'receive'
    TabOrder = 0
    OnClick = receiveClick
  end
  object Button2: TButton
    Left = 288
    Top = 248
    Width = 75
    Height = 25
    Caption = 'Button2'
    TabOrder = 1
    OnClick = Button2Click
  end
  object btStart: TButton
    Left = 536
    Top = 40
    Width = 75
    Height = 25
    Caption = 'Start'
    TabOrder = 2
    OnClick = btStartClick
  end
  object btSend: TButton
    Left = 536
    Top = 71
    Width = 75
    Height = 25
    Caption = 'Sned'
    TabOrder = 3
    OnClick = btSendClick
  end
  object IdUDPServer1: TIdUDPServer
    Bindings = <>
    DefaultPort = 50000
    OnUDPRead = IdUDPServer1UDPRead
    Left = 96
    Top = 72
  end
end
