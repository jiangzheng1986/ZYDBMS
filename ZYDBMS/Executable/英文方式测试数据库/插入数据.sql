DELETE FROM T     --清空T

DELETE FROM C     --清空C

DELETE FROM S     --清空S

DELETE FROM SC    --清空SC

--插入T表测试数据
INSERT INTO T(T#,TNAME,TITLE)
     VALUES ('T1','CHEN','PROFESSOR'),
            ('T2','ZHU','PROFESSOR'),
            ('T3','SUN','INSTRUCTOR'),
            ('T4','LIU','INSTRUCTOR')

--插入C表测试数据
INSERT INTO C(C#,CNAME,T#)
     VALUES ('C1','DB','T1'),
            ('C2','OS','T2'),
            ('C3','DS','T3'),
            ('C4','MIS','T4')

--插入S表测试数据
INSERT INTO S(S#,SNAME,AGE,SEX)
     VALUES ('S1','BAO',20,'M'),
            ('S2','GU',21,'M'),
            ('S3','AN',22,'F'),
            ('S4','LI',23,'M')

--插入SC表测试数据
INSERT INTO SC(SC#,S#,C#,SCORE)
     VALUES ('SC1','S1','C1',93),
            ('SC2','S1','C2',85),
            ('SC3','S1','C3',93),
            ('SC4','S1','C4',74),
            ('SC5','S2','C1',93),
            ('SC6','S2','C2',89),
            ('SC7','S3','C3',95),
            ('SC8','S4','C4',70),
            ('SC9','S4','C2',71)