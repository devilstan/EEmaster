VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
END
Attribute VB_Name = "sheet1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = True
Private Sub sendhexfile_Click()
    '###################################################################
    '
    '###################################################################
    Dim intPortID As Integer ' Ex. 1, 2, 3, 4 for COM1 - COM4
    Dim lngStatus As Long
    Dim strData   As String
    Dim myFile As String, text As String, textline As String, posLat As Integer, posLong As Integer
    
    '###################################################################
    ' set path of hex file and open file as #1
    '###################################################################
    myFile = "C:\Users\devilstan\eeprom.hex"
    Close #1
    Open myFile For Input As #1
    

    '###################################################################
    ' set com port and open
    '###################################################################
    intPortID = 4                                                       'COM4
    
    ' Open COM port
    lngStatus = CommOpen(intPortID, "COM" & CStr(intPortID), _
        "baud=19200 parity=N data=8 stop=1")                            '19200/N/8/1
    
    '###################################################################
    ' read hex file line by line and sned
    '###################################################################
    Do Until EOF(1)
        Line Input #1, textline                                         'read a line
        strData = textline & vbCrLf
        
        'Writa data
        lngStatus = CommWrite(intPortID, strData)
        AppSleep 10                                                     'AppSleep is define in SerialComm module
    Loop
    
    '###################################################################
    ' close file #1
    '###################################################################
    Close #1

    '###################################################################
    ' close com port
    '###################################################################
    Call CommClose(intPortID)
End Sub
