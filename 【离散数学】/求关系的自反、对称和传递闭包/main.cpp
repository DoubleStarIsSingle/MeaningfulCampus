#include <iostream>
#include <string>
using namespace std;
#define SIZE 100//�궨���ϵ��������ߴ�
void output(int s[][SIZE]);//�����ϵ����
void zifan(int s2[][SIZE]);//���Է��հ�
void duichen(int s2[][SIZE]);//��ԳƱհ�
void chuandi(int s2[][SIZE]);//�󴫵ݱհ�
void select();//ѡ���Ƿ����ִ�г����Լ�������ֱհ�
int s[SIZE][SIZE], s1[SIZE][SIZE], choice;
int num, i, j;
string bin;
int main()
{
	select();
	system("pause");
	return 0;
}

void select()
{
	cout << "��ѡ���Ƿ�ִ�м�����򣬼���������Y,��������N:  ";
	while (1)//�˴���whileѭ�������ж��Ƿ���ڷǷ����룬�����д���
	{
		cin >> bin;
		if (bin!="Y"&&bin!="N")//��Ч���룬��Ҫ��������
		{
			cin.clear();
			getline(cin, bin);
			cout << "���벻�淶������������:  ";
			continue;
		}
		else
		{
			if (bin == "Y")
			{
				getline(cin, bin);//�˾����ڳԵ����ܴ��ڵĶ�����Ч����
				break;
			}
			else
			{
				system("pause");
				exit(0);//�˳�����
			}
		}
	}
	cout << "�����뷽���ά��("<< SIZE<<"ά����): ";
	while (1)//�˴���whileѭ�������ж��Ƿ���ڷǷ����룬�����д���
	{
		cin >> num;
		if (cin.fail() || num > SIZE||num<1)//��Ч���룬��Ҫ��������
		{
			cin.clear();
			getline(cin, bin);
			cout << "���벻�淶������������:  ";
			continue;
		}
		else
		{
			getline(cin, bin);
			break;
		}
	}
	cout << "�������ϵ����:" << endl<<endl;
	for (i = 0; i < num; i++)
	{
		cout << "���������ĵ�" << i << "��Ԫ��(Ԫ���Կո�ָ�) :";
		for (j = 0; j < num; j++)
			cin >> s[i][j];
		cout << endl;
	}
	cout << "�����Ӧ���ѡ���㷨\nl:�Է��հ�\n2:���ݱհ�\n3:�ԳƱհ�\n4:�˳�\n";
	cin >> choice;
	switch (choice)
	{
	    case 1:
			zifan(s); //���Է��հ�
			break;
	    case 2:
			chuandi(s); //�󴫵ݱհ�
			break;
	    case 3:
			duichen(s); //��ԳƱհ�
			break;
	    default:
			system("pause");
			exit(0); //�˳�����
	}
}
void clear(int s[][SIZE])//���þ���
{
	for (int i = 0; i < num; i++)
		for (int j = 0; j < num; j++)
			s[i][j] = 0;
}
void output(int s[][SIZE])//������ά���飬�����ϵ����
{
	cout << "�����ϵ����Ϊ: "<<endl;
	for (i = 0; i < num; i++)
	{
		for (j = 0; j < num; j++)
			cout << s[i][j];
		cout << endl;
	}
	cout << endl << endl;
}

void zifan(int s2[][SIZE])//�����Է��հ�ֻ��Ҫ�����Խ���Ԫ���߼���1
{
	for (i = 0; i < num; i++)
		s2[i][i] = 1;
	output(s2);//������
	clear(s2);//���þ���
	select();
}

void duichen(int s2[][SIZE])
{
	for (i = 0; i < num; i++)//����ת�þ���
		for (j = 0; j < num; j++)
			s1[j][i] = s2[i][j];
	for (i = 0; i < num; i++)//ԭ�����ת�þ���Ķ�Ӧλ��Ԫ�ؽ����߼���
		for (j = 0; j < num; j++)
		{
			s2[i][j] = s2[i][j] + s1[i][j];
			if (s2[i][j] > 1)
				s2[i][j] = 1;
		}
	output(s2);//������
	clear(s1);//���þ���
	clear(s2);
	select();
}

void chuandi(int s2[][SIZE])//���촫�ݱհ�
{
	int k, h;
	for (h = 0; h < num; h++)//nά����Ĵ���ֻ��n�������Ҳ����˵��n+1����һ����1-n�����е�һ����ͬ
	{                        //����Ϊ�߼���������ͬ����������ԭ���ľ��������ۼ�n���ݱ�Ȼ�н��
		for (i = 0; i < num; i++)//�Ծ���n������ǰn-1�����ۼӵ�ģ��
			for (j = 0; j < num; j++)
				if (s2[i][j] == 1)
				{
					for (k = 0; k < num; k++)
						if (s2[j][k] == 1)
							s2[i][k] = 1;
				}
	}
	output(s2);//������
	clear(s2);//���þ���
	select();
}
