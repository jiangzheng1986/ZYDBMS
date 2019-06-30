--(2)检索学习课程号为C2课程的学生学号与姓名(连接查询)
SELECT S.S#,SNAME
FROM S,SC
WHERE S.S#=SC.S# AND C#='C2'