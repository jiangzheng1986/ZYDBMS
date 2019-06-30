--(10)检索学习课程名为数据结构的女学生学号和姓名(查询优化)
SELECT S.S#,SNAME
FROM C,SC,S
WHERE CNAME='DS' AND SEX='F'
   AND C.C#=SC.C# AND SC.S#=S.S#














