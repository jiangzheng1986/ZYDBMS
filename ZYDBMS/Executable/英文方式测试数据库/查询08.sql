--(8)������ѧ�γ̰���ѧ��ΪS3ѧ����ѧ�γ̵�ѧ��ѧ��(ʹ�ô������ʵ�����Ӳ�ѯ)
SELECT DISTINCT S#             --��SC����һ��ѧ��
FROM SC AS X
WHERE NOT EXISTS(SELECT SC#           --������S3��һ�ſ�
                 FROM SC AS Y
                 WHERE Y.S#='S3' AND NOT EXISTS(SELECT SC#      --��ѧ��û��ѧ
                                                FROM SC AS Z
                                                WHERE Z.S#=X.S# AND Z.C#=Y.C#))