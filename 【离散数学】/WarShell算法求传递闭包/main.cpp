#include <iostream>
#include <string>
using namespace std;

int main()
{
    int n_dim = 0;
    string bin;
    while (1)//�����ϵ����ά�����ж��Ƿ�Ƿ����������������ֳ���Χ����������Ч������������
    {
        cout << "Input number of dimensions [1,100]:";
        cin >> n_dim;
        if (cin.fail() || n_dim < 1 || n_dim>100)
        {
            cin.clear();
            getline(cin, bin);
            cout << "! Illegal Input !" << endl;
            continue;
        }
        else
        {
            getline(cin, bin);
            break;
        }
    }
    char** array = new char* [n_dim];   //�����Ƕ�̬����һ�� n_dim*n_dim ����
    char ch;
    for (int k = 0; k < n_dim; k++)//��ʼ��ָ�����飬��ֹ����Ұָ��
        array[k] =NULL;


    array[0] = new char[n_dim];          //���Ҳ�Ƕ�̬����һ�����飬������濪�ٵ����飬���ջ��γ�n_dim*n_dim�Ķ�ά����
    for (int k = 0; k < n_dim; k++)
        array[0][k] = '\0';

    for (int i = 0; i < n_dim;)
    {
        cout << "�������" << i+1 << "�е�" << n_dim << "��Ԫ�أ�ÿ������֮�����һ���ո����" << endl;
        for (int j = 0; j < n_dim; j++)
        {
            cin >> ch;
            if (cin.fail() || (ch != '1' && ch != '0'))
            {
                cin.clear();
                getline(cin, bin);
                cout << "! ���ڳ�0��1֮��������Ƿ����� !" << endl;//��ֹ�Ƿ����룬��ϵ������ֻ��0��1��������
                break;
            }
            else
            {
                array[i][j] = ch;
                if (j == n_dim - 1)
                {
                    getline(cin, bin);//�Ե����ܴ��ڵĶ������Ч����
                    i++;
                    if (i < n_dim)
                    {
                        array[i] = new char[n_dim];          //���Ҳ�Ƕ�̬����һ�����飬������濪�ٵ����飬���ջ��γ�n_dim*n_dim�Ķ�ά����
                        for (int k = 0; k < n_dim; k++)//��ʼ��ָ�룬��ֹ����Ұָ��
                            array[i][k] = '\0';
                    }
                }
                continue;
            }
        }
    }
    //******************************************************************************
    //WarShell�㷨����******************************************************************************
    for (int i = 0; i < n_dim; i++)
        for (int j = 0; j < n_dim; j++)
            for (int k = 0; k < n_dim; k++)
                if (array[i][j] == '1' && array[k][i] == '1')
                    array[k][j] = '1';
    //******************************************************************************
    //******************************************************************************

    cout << "The trasitive closure of a relation R represented:" << endl;//������
    for (int i = 0; i < n_dim; i++)
    {
        for (int j = 0; j < n_dim; j++)
            cout << array[i][j];
        cout << endl;
    }
    cout << endl;

    for (int i = 0; i < n_dim; i++)//ɾ�����ٵ����ڴ��������ڴ�
        delete array[i];

    system("pause");
    return 0;
}