#include <iostream>
#include <string>
#include <cmath>
#include "stack.h"
using namespace std;

string op_set = "!&|^~()#";
char op_level[8][8] =
{
	// !   &   |   ^   ~   (   )   #
	  '>','>','>','>','>','<','>','>',//!
	  '<','>','>','>','>','<','>','>',//&
	  '<','<','>','>','>','<','>','>',//|
	  '<','<','<','>','>','<','>','>',//^
	  '<','<','<','<','>','<','>','>',//~
	  '<','<','<','<','<','<','=','>',//(
	  '>','>','>','>','>','<','>','>',//)
	  '<','<','<','<','<','<','>','=',//#
};

int cal_rule(int x, int y, char op)//�������
{
	if (op == '&')
		return x && y;
	else if (op == '|')
		return x || y;
	else if (op == '^')
		return (!x) || y;
	else 
		return ((!x) || y)&& ((!y) || x);
}
int calculator(string s)//����
{
	int len = s.length();
	s += "#"; //��־s��ĩβ
	int x, y;
	char op;
	string mid;
	Stack<int>P;
	Stack<char>Q;
	Q.Push('#');//��־Q��ĩβ

	//sû��β�����ܼ�����P�зţ�Qû��β�����ܼ�������
	for(int i=0;s[i] != '#' || Q.GetTop() != '#';)
	{
		if (s[i] == '1' || s[i] == '0')
		{
			mid = s[i];
			P.Push(stoi(mid));//�������־���P���
			i++;
		}
		else
		{
			switch (op_level[op_set.find(Q.GetTop())][op_set.find(s[i])])//������������Qջ��ջ��������Ƚ����ȼ�
			{
			case '<':  // ջ��Ԫ�����ȼ��ͣ���ǰԪ����ջ
				Q.Push(s[i]);
				i++;
				break;
			case '=': //��һ����������ʱ��ʾ��������������ɣ������Ų�������һ�ַ�
				Q.Pop();
				i++;
				break;
			case '>':  // ��ջ������������ջ
				if (Q.GetTop() == '!')
				{   // ջ��Ԫ��Ϊ ������Ŀ������
					x = P.GetTop();
					P.Pop();
					P.Push(!x);
					Q.Pop();
				}
				else
				{   // ջ��Ԫ�ز��� ��
					x = P.GetTop();
					P.Pop();
					y = P.GetTop();
					P.Pop();
					op = Q.GetTop();
					Q.Pop();//����������ջ
					P.Push(cal_rule(y, x, op));
				}
				break;
			}
		}
	}
	return P.GetTop();
}

int main()
{
	string sz, value = "#", sz_copy, value_copy = "#", hequ = "#", xiqu = "#";
	int len = 0, result = -1;
	cout << "***************************************" << endl;
	cout << "**                                   **" << endl;
	cout << "**         ��ӭ�����߼��������      **" << endl;
	cout << "**   (��������ֵ��,����ʽ,֧������)  **" << endl;
	cout << "** ���·����������Ӣ�����뷨������! **" << endl;
	cout << "**              ��!��ʾ��            **" << endl;
	cout << "**              ��&��ʾ��            **" << endl;
	cout << "**              ��|��ʾ��            **" << endl;
	cout << "**             ��^��ʾ�̺�           **" << endl;
	cout << "**             ��~��ʾ��ֵ           **" << endl;
	cout << "**                                   **" << endl;
	cout << "***************************************" << endl << endl;
	cout << "������һ���Ϸ������⹫ʽ:" << endl;
	while (1)//�ж����⹫ʽ�Ƿ�Ϸ��������⹫ʽ�Ƿ�ʱ��ʾ�û���������
	{
		int left = 0, right = 0;
		cin >> sz;//������ʽ
		len = sz.length();
		for (int i = 0; i < len; i++)
		{
			if (sz[i] == '(')//ͳ������������
				left++;
			else if (sz[i] == ')')//ͳ������������
				right++;
			else if ((sz[i] >= 'a' && sz[i] <= 'z') || (sz[i] >= 'A' && sz[i] <= 'Z'))
			{
				if (i > 0)
				{
					if ((sz[i - 1] >= 'a' && sz[i - 1] <= 'z') || (sz[i - 1] >= 'A' && sz[i - 1] <= 'Z'))//��������������Ԫ���м�û�����������
					{
						len = 0;
						break;
					}
				}
				if (i < len - 1)
				{
					if ((sz[i + 1] >= 'a' && sz[i + 1] <= 'z') || (sz[i + 1] >= 'A' && sz[i + 1] <= 'Z'))//��������������Ԫ���м�û�����������
					{
						len = 0;
						break;
					}
				}
				if (value.find(sz[i]) == -1)
					value += sz[i];
			}
			else if (op_set.find(sz[i])<0|| op_set.find(sz[i])>=op_set.length()-1)//���ַǷ��������
			{
				len = 0;
				break;
			}
		}
		if (len == 0 || left != right)//�����������˵�����⹫ʽ�Ƿ�����Ҫ��������
		{
			cout << endl << "���⹫ʽ�Ƿ�,����������: ";
			value = "#";
			continue;
		}
		else
			break;
	}
	len = value.length();
	value_copy = value;
	cout << "��ʽ���еı�������Ϊ: " << len - 1 << endl;
	cout << "�����ֵ������:" << endl << endl;
	for (int i = 1; i < len; i++)
		cout << value[i] << ' ';
	cout << sz << endl;
	for (int i = 0; i < static_cast<int>(pow(2, len - 1)); i++)//ģ����ֵ�����ж��������
	{
		sz_copy = sz;
		if (i == 0)//��ֵ���е�һ�У����б�Ԫ�ĸ�ֵ��Ϊ0���˺�ֻҪ�Զ�����0��1���в��ϼ�һ������ִ�д�0��1����1��0�ķ�ת����ģ����ֵ��
		{
			for (int j = 1; j < len; j++)
				value_copy[j] = '0';
		}
		else
		{
			for (int j = len - 1; j >= 1; j--)//����ִ�ж���������1��ģ����ֵ��
			{
				if (value_copy[j] == '0')
				{
					value_copy[j] = '1';
					break;
				}
				else
				{
					value_copy[j] = '0';
					continue;
				}
			}
		}
		for (int j = 1; j < len; j++)
		{
			while (1)
			{
				int k = sz_copy.find(value[j]);//��0��1ֵ��ֵ����Ӧ�ı���
				if (k == -1)
					break;
				else
				{
					sz_copy[k] = value_copy[j];
					continue;
				}
			}
		}
		result = calculator(sz_copy);//�ж��������
		if (result == 0)//������ٽ����Ӻ�ȡ���ȡ����ս��
		{
			hequ += to_string(i);
			hequ += "#";
		}
		else if (result == 1)
		{
			xiqu += to_string(i);
			xiqu += "#";
		}
		for (int j = 1; j < len; j++)
			cout << value_copy[j] << ' ';
		cout << result << endl;
	}
	if (hequ.length() == 1)
		cout << endl << "�����⹫ʽ����������ȡ��ʽ" << endl;
	else
	{
		cout << endl << "�����⹫ʽ������ȡ��ʽΪ:" << endl;//�������ȡ��ʽ
		for (unsigned int j = 1; j < hequ.length(); j++)
		{
			if (j > 1)
				cout << "��";
			cout << "M(";
			while (hequ[j] != '#')
			{
				cout << hequ[j];
				j++;
			}
			cout << ")";
		}
		cout << endl;
		hequ = "#";
	}
	if (xiqu.length() == 1)
		cout << endl << "�����⹫ʽ����������ȡ��ʽ" << endl;
	else
	{
		cout << endl << "�����⹫ʽ������ȡ��ʽΪ:" << endl;//�������ȡ��ʽ
		for (unsigned int j = 1; j < xiqu.length(); j++)
		{
			if (j > 1)
				cout << "��";
			cout << "m(";
			while (xiqu[j] != '#')
			{
				cout << xiqu[j];
				j++;
			}
			cout << ")";
		}
		cout << endl;
		xiqu = "#";
	}
	cout << endl << "�����������,��ӭ�´�ʹ��!" << endl;

	system("pause");
	return 0;
}