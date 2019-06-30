--(9)求男学生的总人数和平均年龄(聚合函数)
SELECT COUNT(AGE) AS COUNT_AGE,
       SUM(AGE) AS SUM_AGE,
       AVG(AGE) AS AVG_AGE,
       MAX(AGE) AS MAX_AGE,
       MIN(AGE) AS MIN_AGE
FROM S
WHERE SEX='M'