--(6)检索不学C4课程的学生姓名与年龄(嵌套查询)
SELECT SNAME,AGE
FROM S
WHERE S# NOT IN(SELECT S#
                FROM SC
                WHERE C#='C4')