--(3)检索至少选修LIU老师所授课程中一门课程的学生学号与姓名(连接查询)
SELECT S.S#,SNAME
FROM S,SC,C,T
WHERE S.S#=SC.S# AND SC.C#=C.C#
      AND C.T#=T.T# AND TNAME='LIU'