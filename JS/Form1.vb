Public Class Form1
    'I'll let you make the GUI look as you need it.
    Private Sub Form1_Load(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles MyBase.Load
        'note: not all possiblilties are included here.
        'note: vb is NOT CaSe SenSiTive so assignRecur=Assignrecur=AssignRecur(and so on so forth)
        'to create a "CItem" (holds info about the assignment time, name of person, department, a description
        'date is used separate
        Dim x = CItem.Create("Sunday School", "Mr. Bob", "sunday school class", HHMMSS(7, 30, 0))
        'HHMMSS converts military time hour,minut,second into a date object
        Dim day As Date = MMDDYY(9, 1, 2013) 'MMDDYY converts month,day,year to a date object
        Dim CD = New Calander("data.xml") ' creates a calandar object
        CD.AssignRecur(day, x, Calander.RECURTYPE.NDayAnyWEEK)
        'assignRecur creats a reccuring date for someone
        'assigneOnce create a fire once date for someone
        'notice we gave an absolute date into a recurring event
        'this is on purpose so you can use a date picker.
        'consequently, though, the date MUST be absolute.
        'the event then converts it to the correct reletive format
        Dim y = CItem.Create("Janitorial", "Mr. Bob", "Sweeping up", HHMMSS(8, 30, 0))
        CD.AssignOnce(day, y)

        Dim ASNS = CD.AssignedToday(day) ' this returns all people assigned to do something today
        'if they are reccurring, they are left to be called next time
        'if they are absolute, this function DESTROYS them after being called!!!
        'so put some catch code to only allow this to be called only ONCE PER DAY
        '(store in a temp file and store a value stating last date it was run would be one way to do it)
        'for easy access to registry,filesystem,ect look in my
        'safer, but results in more files in you directory
        My.Computer.FileSystem.WriteAllText("last.txt", day.ToBinary, False) 'write all to text file
        'write to registry (more dangerous, but suggested by microsoft for some reason)
        My.Computer.Registry.CurrentUser.CreateSubKey("applications").CreateSubKey("ChurchApp").SetValue("lastran", day.ToBinary)
        'day.tobinary converts to a number. it is best to use it to store. to get later just use Date.FromBinary(number)
        'Dim day2 = Date.FromBinary(day.ToBinary)
        'now, assuming you have defined "Janitorial" and "Sunday School" as conflicting departments,
        'you will get a box saying "ERROR: are you sure you want to do this" (yes or no)
        'if, say, that sunday you have a special service and sunday school is closed, say, thanks to a
        'special breakfast, you can still assign Mr. Bob to do a quick sweep of the floor after breakfast.
        'defining conflicting departs you must do, either with code  or by hand.
        'the tag they are stored under in xml is <conflict_departments>
        'the format is


        Dim k = <data>
                    <assignments>all assignments are here using <assignment name="" date="" time="" department="" type="">
                        comments
                        </assignment>
                    </assignments>
                    <conflict_departments>
                        <cd>
                        _|deparment1|_
                        _|department2|_
                        (and so on) the _|...|_ requirement is required now
                    </cd>
                        <cd>
                        another list of deparments
                    </cd>
                    you can have all the cd element you need
                </conflict_departments>
                </data>
        'why did i define a useless variable just to give you info
        'vb has no multiline comment. so i created k with xml literal
        'to avoid doin '<tab> ... stuff
        'k is not actually part of this code!
    End Sub
End Class
Public Structure CItem
    Public Department As String
    Public name As String
    Public Description As String
    Public time As Date
    Public Shared Function Create(ByVal dp As String, ByVal nm As String, ByVal dsc As String, ByVal t As Date) As CItem
        Dim r As CItem
        r.Department = dp
        r.name = nm
        r.Description = dsc
        r.time = t
        Return r
    End Function
End Structure
Public Module DateHelper
    Public Function MMDDYY(ByVal month As Int16, ByVal day As Int16, ByVal year As Int16)
        Return New Date(year, month, day)
    End Function
    Public Function HHMMSS(ByVal Hour As Integer, ByVal minute As Integer, ByVal second As Integer)
        Return New Date(0, 0, 0, Hour, minute, second)
    End Function
End Module
Public Class Calander
    Dim XD As XDocument
    Dim ASN As XElement
    Dim CNFLCT As XElement
    Sub New(ByVal FileName As String)
        'if file exists then load else use empty document
        'if you use xdocument and xelement instead of xmldocument and xmlelement
        'you unlock special ability of vb.net to use xml definitions in code (not just string constants)
        'you can see it in action some here
        If IO.File.Exists(FileName) Then XD = XDocument.Load(FileName) Else XD = New XDocument( _
        <data><conflict_departments></conflict_departments>
            <assignments></assignments></data> _
        )
        'conflict_departmens will have lists of departments that conflict
        'such that person a cannot work in department a and b in same day
        'assignments are the actual assignment
        ASN = XD.<data>.<assignments>
        CNFLCT = XD.<data>.<conflict_departments>
    End Sub
    Private Function DayOfWeek(ByVal dt As Date) As Integer
        'first, find day of week. they use sunday=0, saturday=6. we are converting.
        'if you like monday as first day you can just add 1 instead
        'if you like monday first, and sunday as "zeroth", just use .dayofweek and ignore this function
        Select Case dt.DayOfWeek
            Case System.DayOfWeek.Monday
                Return 1
            Case System.DayOfWeek.Tuesday
                Return 2
            Case System.DayOfWeek.Wednesday
                Return 3
            Case System.DayOfWeek.Thursday
                Return 4
            Case System.DayOfWeek.Friday
                Return 5
            Case System.DayOfWeek.Saturday
                Return 6
            Case System.DayOfWeek.Sunday
                Return 7
        End Select
    End Function
    Function GT(ByVal it As String) As Date
        Dim s = Split(it, ":")
        Dim p(2) As Integer
        p(0) = Val(s(0))
        If s.Length > 1 Then p(1) = Val(s(1)) Else p(1) = 0
        If s.Length > 2 Then p(2) = Val(s(2)) Else p(2) = 0
        Return New Date(0, 0, 0, p(0), p(1), p(2))
    End Function
    Public ReadOnly Property AssignedToday(ByVal dt As Date) As CItem()
        Get
            Dim ABD As String = dt.Month & "|" & dt.Day & "|" & dt.Year 'absolute
            Dim RLDA As String = dt.Month & "|" & dt.Day 'specific day of specific month
            Dim RLDB As String = (Math.Ceiling(dt.Day / 7)) & "|" & DayOfWeek(dt)  'a day of week and week of month
            Dim RLDC As String = DayOfWeek(dt) 'a day of week each week
            Dim RLDD As String = dt.Day 'a day of monthg each month
            '3rd sunday is 3|
            Dim rt = New List(Of CItem)
            Dim CTS = XD...<assignment> 'CTS is all assignments
            For Each ITEM In CTS
                Select Case LCase(ITEM.@type)
                    Case "abs"
                        If ITEM.@date = ABD Then
                            rt.Add(CItem.Create(ITEM.@Department, ITEM.@name, ITEM.Value, GT(ITEM.@time)))
                            ITEM.Remove() 'absolute dates cannot be kept forever.
                            'as the file is saved at safe close, items are kept in case of crash
                        End If
                    Case "recur_nth_day_nth_month"
                        If ITEM.@date = RLDA Then _
                            rt.Add(CItem.Create(ITEM.@Department, ITEM.@name, ITEM.Value, GT(ITEM.@time)))
                    Case "recur_nth_day_any_month"
                        If ITEM.@date = RLDD Then _
                            rt.Add(CItem.Create(ITEM.@Department, ITEM.@name, ITEM.Value, GT(ITEM.@time)))
                    Case "recur_nth_day_nth_week"
                        If ITEM.@date = RLDB Then _
                            rt.Add(CItem.Create(ITEM.@Department, ITEM.@name, ITEM.Value, GT(ITEM.@time)))
                    Case "recur_nth_day_any_week"
                        If ITEM.@date = RLDC Then _
                            rt.Add(CItem.Create(ITEM.@Department, ITEM.@name, ITEM.Value, GT(ITEM.@time)))
                End Select
            Next
            Return rt.ToArray()
        End Get
    End Property
    Private Function DEPARTMENTCHECK(ByVal src As String, ByVal srca As String) As Boolean 'false = noconflict, true = conflict
        Dim XX = From A In XD...<cd> Where A.Value Like "*|" & src & "|*"
        Dim xy = From A In XX Where A.Value Like "*|" & srca & "|*"
        'you have defined the 2 departments conflicting, then xy will have a non-zero count
        'otherwize xy will have zero count
        If xy.Count > 0 Then Return True
        Return False
    End Function
    Public Sub AssignOnce(ByVal dt As Date, ByVal info As CItem)
        Dim RD As String
        'type time department date name
        RD = dt.Month & "|" & dt.Day & "|" & dt.Year
        Dim ABD As String = dt.Month & "|" & dt.Day & "|" & dt.Year 'absolute
        Dim RLDA As String = dt.Month & "|" & dt.Day 'specific day of specific month
        Dim RLDB As String = (Math.Ceiling(dt.Day / 7)) & "|" & DayOfWeek(dt)  'a day of week and week of month
        Dim RLDC As String = DayOfWeek(dt) 'a day of week each week
        Dim RLDD As String = dt.Day 'a day of month each month
        Dim X = XD...<assignment>
        Dim XX = From A In X Where A.@name = info.name Select A
        Dim ISCLEAR As Boolean = False
        If (XX.Count > 0) Then
            For Each ITEM In XX
                Select Case LCase(ITEM.@type)
                    Case "abs"
                        If ITEM.@date = ABD Then
                            'same day
                            ISCLEAR = ISCLEAR Or DEPARTMENTCHECK(ITEM.@Department, info.Department)
                        End If
                    Case "recur_nth_day_nth_month"
                        If ITEM.@date = RLDA Then
                            'same day
                            ISCLEAR = ISCLEAR Or DEPARTMENTCHECK(ITEM.@Department, info.Department)
                        End If
                    Case "recur_nth_day_any_month"
                        If ITEM.@date = RLDD Then
                            'same day
                            ISCLEAR = ISCLEAR Or DEPARTMENTCHECK(ITEM.@Department, info.Department)
                        End If
                    Case "recur_nth_day_nth_week"
                        If ITEM.@date = RLDB Then
                            'same day
                            ISCLEAR = ISCLEAR Or DEPARTMENTCHECK(ITEM.@Department, info.Department)
                        End If
                    Case "recur_nth_day_any_week"
                        If ITEM.@date = RLDC Then
                            'same day
                            ISCLEAR = ISCLEAR Or DEPARTMENTCHECK(ITEM.@Department, info.Department)
                        End If
                End Select
            Next
            If ISCLEAR Then
                Dim OP = <assignment type="abs" date=<%= RD %> name=<%= info.name %> department=<%= info.Department %>
                             time=<%= info.time.Hour & ":" & info.time.Minute & ":" & info.time.Second %>>
                             <%= info.Description %>
                         </assignment>
                ASN.Add(OP)
            Else
                If (MsgBox("ERROR: A conflict of dates has occured. Add anyway?", MsgBoxStyle.YesNo, "Date Pick") = MsgBoxResult.Yes) Then
                    Dim OP = <assignment type="abs" date=<%= RD %> name=<%= info.name %> department=<%= info.Department %>
                                 time=<%= info.time.Hour & ":" & info.time.Minute & ":" & info.time.Second %>>
                                 <%= info.Description %>
                             </assignment>
                    ASN.Add(OP)
                    '
                End If
            End If
        Else
            Dim OP = <assignment type="abs" date=<%= RD %> name=<%= info.name %> department=<%= info.Department %>
                         time=<%= info.time.Hour & ":" & info.time.Minute & ":" & info.time.Second %>>
                         <%= info.Description %>
                     </assignment>
            ASN.Add(OP)
        End If
        'to create different reccuring scales, use type="recur_..."
    End Sub
    Public Enum RECURTYPE
        NDayNMonth = 1
        NDayAnyMonth = 2
        NDayNWeek = 3
        NDayAnyWEEK = 4
    End Enum

    Public Sub AssignRecur(ByVal dt As Date, ByVal info As CItem, ByVal RCT As RECURTYPE)
        Dim RD As String, tp As String
        'type time department date name
        'RD = dt.Month & "|" & dt.Day & "|" & dt.Year
        Dim ABD As String = dt.Month & "|" & dt.Day & "|" & dt.Year 'absolute
        Dim RLDA As String = dt.Month & "|" & dt.Day 'specific day of specific month
        Dim RLDB As String = (Math.Ceiling(dt.Day / 7)) & "|" & DayOfWeek(dt)  'a day of week and week of month
        Dim RLDC As String = DayOfWeek(dt) 'a day of week each week
        Dim RLDD As String = dt.Day 'a day of month each month
        Select Case RCT
            Case RECURTYPE.NDayAnyMonth
                tp = "recur_nth_day_any_month"
                RD = RLDD
            Case RECURTYPE.NDayAnyWEEK
                tp = "recur_nth_day_any_week"
                RD = RLDC
            Case RECURTYPE.NDayNMonth
                tp = "recur_nth_day_nth_month"
                RD = RLDA
            Case RECURTYPE.NDayNWeek
                tp = "recur_nth_day_nth_week"
                RD = RLDB
            Case Else

        End Select
        RD = RLDA
        Dim X = XD...<assignment>
        Dim XX = From A In X Where A.@name = info.name Select A
        Dim ISCLEAR As Boolean = False
        If (XX.Count > 0) Then
            For Each ITEM In XX
                Select Case LCase(ITEM.@type)
                    Case "abs"
                        If ITEM.@date = ABD Then
                            'same day
                            ISCLEAR = ISCLEAR Or DEPARTMENTCHECK(ITEM.@Department, info.Department)
                        End If
                    Case "recur_nth_day_nth_month"
                        If ITEM.@date = RLDA Then
                            'same day
                            ISCLEAR = ISCLEAR Or DEPARTMENTCHECK(ITEM.@Department, info.Department)
                        End If
                    Case "recur_nth_day_any_month"
                        If ITEM.@date = RLDD Then
                            'same day
                            ISCLEAR = ISCLEAR Or DEPARTMENTCHECK(ITEM.@Department, info.Department)
                        End If
                    Case "recur_nth_day_nth_week"
                        If ITEM.@date = RLDB Then
                            'same day
                            ISCLEAR = ISCLEAR Or DEPARTMENTCHECK(ITEM.@Department, info.Department)
                        End If
                    Case "recur_nth_day_any_week"
                        If ITEM.@date = RLDC Then
                            'same day
                            ISCLEAR = ISCLEAR Or DEPARTMENTCHECK(ITEM.@Department, info.Department)
                        End If
                End Select
            Next
            If ISCLEAR Then
                Dim OP = <assignment type=<%= tp %> date=<%= RD %> name=<%= info.name %> department=<%= info.Department %>
                             time=<%= info.time.Hour & ":" & info.time.Minute & ":" & info.time.Second %>>
                             <%= info.Description %>
                         </assignment>
                ASN.Add(OP)
            Else
                If (MsgBox("ERROR: A conflict of dates has occured. Add anyway?", MsgBoxStyle.YesNo, "Date Pick") = MsgBoxResult.Yes) Then
                    Dim OP = <assignment type=<%= tp %> date=<%= RD %> name=<%= info.name %> department=<%= info.Department %>
                                 time=<%= info.time.Hour & ":" & info.time.Minute & ":" & info.time.Second %>>
                                 <%= info.Description %>
                             </assignment>
                    ASN.Add(OP)
                    '
                End If
            End If
        Else
            Dim OP = <assignment type=<%= tp %> date=<%= RD %> name=<%= info.name %> department=<%= info.Department %>
                         time=<%= info.time.Hour & ":" & info.time.Minute & ":" & info.time.Second %>>
                         <%= info.Description %>
                     </assignment>
            ASN.Add(OP)
        End If
        'to create different reccuring scales, use type="recur_..."
    End Sub

End Class
