--(3)检索至少选修刘老师所授课程中一门课程的学生学号与姓名(连接查询)
选择 学生.学生号,学生名
从 学生,选修,课程,教师
当 学生.学生号=选修.学生号 且 选修.课程号=课程.课程号
      且 课程.教师号=教师.教师号 且 教师名='刘'