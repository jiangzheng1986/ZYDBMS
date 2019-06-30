--��ʦ��
CREATE TABLE T
    (T# CHAR(4),
     TNAME CHAR(8),
     TITLE CHAR(10),
     PRIMARY KEY(T#))

--�γ̱�
CREATE TABLE C
    (C# CHAR(4),
     CNAME CHAR(10),
     T# CHAR(4),
     PRIMARY KEY(C#),
     FOREIGN KEY(T#) REFERENCES T(T#))

--ѧ����
CREATE TABLE S
    (S# CHAR(4),
     SNAME CHAR(8),
     AGE INTEGER,
     SEX CHAR(1),
     PRIMARY KEY(S#))

--ѡ�ޱ�
CREATE TABLE SC
    (SC# CHAR(8),
     S# CHAR(4),
     C# CHAR(4),
     SCORE INTEGER,
     PRIMARY KEY(SC#),
     FOREIGN KEY(S#) REFERENCES S(S#),
     FOREIGN KEY(C#) REFERENCES C(C#))
