--(7)����ѧϰȫ���γ̵�ѧ������(ʹ�ô������ʵ�����Ӳ�ѯ)
SELECT SNAME                   --��S����һ��ѧ��
FROM S
WHERE NOT EXISTS(SELECT C#           --C���в�����һ�ſγ�
                 FROM C
                 WHERE NOT EXISTS(SELECT SC#     --��ѧ��û��ѧ
                                  FROM SC
                                  WHERE SC.S#=S.S# AND SC.C#=C.C#))