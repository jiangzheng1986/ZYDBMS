--(3)��������ѡ��LIU��ʦ���ڿγ���һ�ſγ̵�ѧ��ѧ��������(���Ӳ�ѯ)
SELECT S.S#,SNAME
FROM S,SC,C,T
WHERE S.S#=SC.S# AND SC.C#=C.C#
      AND C.T#=T.T# AND TNAME='LIU'