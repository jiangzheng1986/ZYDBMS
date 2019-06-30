--(5)检索至少选修课程号为C2和C4课程的学生学号(连接查询)
SELECT X.S#
FROM SC AS X,SC AS Y
WHERE X.S#=Y.S# AND X.C#='C2' AND Y.C#='C4'