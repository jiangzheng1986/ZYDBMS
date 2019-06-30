--(4)检索选修课程号为C2或C4课程的学生学号(连接查询)
SELECT DISTINCT S.S#,SNAME
FROM SC,S
WHERE (C#='C2' OR C#='C4') AND SC.S#=S.S#