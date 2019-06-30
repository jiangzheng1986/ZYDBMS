--(8)检索所学课程包含学号为S3学生所学课程的学生学号(使用存在量词的相关子查询)
SELECT DISTINCT S#             --在SC中找一个学生
FROM SC AS X
WHERE NOT EXISTS(SELECT SC#           --不存在S3的一门课
                 FROM SC AS Y
                 WHERE Y.S#='S3' AND NOT EXISTS(SELECT SC#      --该学生没有学
                                                FROM SC AS Z
                                                WHERE Z.S#=X.S# AND Z.C#=Y.C#))