--(6)������ѧC4�γ̵�ѧ������������(Ƕ�ײ�ѯ)
SELECT SNAME,AGE
FROM S
WHERE S# NOT IN(SELECT S#
                FROM SC
                WHERE C#='C4')