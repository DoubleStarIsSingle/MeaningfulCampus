#include <iostream>
#include <string>
using namespace std;

int input(int value,char value_name)//���뺯��
{
	string bin;
	cout << endl << "������" << value_name << "��ֵ��0��1��, �Իس�����: ";
	while (1)
	{
		cin >> value;
		if ((value != 0 && value != 1) || cin.fail())//����������ַ���������0��1֮��������ַ�Ϊ��Ч���룬��Ҫ��������
		{
			cin.clear();
			getline(cin, bin);//�Ե����ܴ��ڵĶ�����Ч����
			cout << endl << value_name<<"��ֵ��������, ����������: ";
			continue;
		}
		else
			break;
	}
	return value;
}

int main()
{
	int p=-1, q=-1, result[4];
	char choice;
	string bin;
	while (1)
	{
		cout << "***************************************" << endl;
		cout << "**                                   **" << endl;
		cout << "**        ��ӭ�����߼��������       **" << endl;
		cout << "**                                   **" << endl;
		cout << "***************************************" << endl;
		p = input(p, 'P');//����
		q = input(q, 'Q');
		result[0] = p && q;  //������
		result[1] = p || q;  //������
		result[2] = (!p) || q;  //�̺����㣬����ת��Ϊ������ʽ
		result[3] = ((!p) || q) && ((!q) || p);  //��ֵ���㣬����ת��Ϊ������ʽ
		cout << endl;
		cout << "��ȡ����:" << endl << "P��Q = " << result[0] << endl;
		cout << "��ȡ����:" << endl << "P��Q = " << result[1] << endl;
		cout << "�̺�����:" << endl << "P��Q = " << result[2] << endl;
		cout << "��ֵ����:" << endl << "P <-> Q = " << result[3] << endl;

		cout << endl << "�Ƿ��������?��y/n��: ";
		while (1)//����Ƿ�����
		{
			cin >> choice;
			if (choice != 'y' && choice != 'n')
			{
				getline(cin, bin);
				cout << endl << "�������,����������y��n : ";
				continue;
			}
			else
				break;
		}
		if (choice == 'y')//��������
		{
			system("cls");  //����
			continue;
		}
		else if (choice == 'n')//��������
		{
			cout << "��ӭ�´��ٴ�ʹ��!" << endl;
			break;
		}
	}
	system("pause");
	return 0;
}