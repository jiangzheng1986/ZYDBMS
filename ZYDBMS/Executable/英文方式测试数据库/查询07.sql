--(7)检索学习全部课程的学生姓名(使用存在量词的相关子查询)
SELECT SNAME                   --在S中找一个学生
FROM S
WHERE NOT EXISTS(SELECT C#           --C表中不存在一门课程
                 FROM C
                 WHERE NOT EXISTS(SELECT SC#     --该学生没有学
                                  FROM SC
                                  WHERE SC.S#=S.S# AND SC.C#=C.C#))