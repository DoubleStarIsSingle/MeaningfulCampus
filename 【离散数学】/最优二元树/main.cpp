#include <iostream>
#include <string>
using namespace std;
#define N 1000
int num=0;
struct tree
{
	int num;
	struct tree* Lnode;
	struct tree* Rnode;
}*fp[N];     //������ 
char s[N] = { '\0' };  //��ǰ׺��

void init_node(int f[], int n)  //����Ҷ�ӽ��
{
	int i;
	struct tree* pt;
	for (i = 0; i < n; i++)
	{
		pt = new tree;  //����Ҷ�ӽ��
		pt->num = f[i];
		pt->Lnode = NULL;
		pt->Rnode = NULL;
		fp[i] = pt;
	}
}

void sort(struct tree* array[], int n)  //����N-n������뵽���ź����������
{
	int i;
	struct tree* temp;
	for (i = n; i < num - 1; i++)
		if (array[i]->num > array[i + 1]->num)
		{
			temp = array[i + 1];
			array[i + 1] = array[i];
			array[i] = temp;
		}
}

struct tree* construct_tree(int f[], int n)  //������ 
{
	int i;
	struct tree* pt;
	for (i = 1; i < n; i++)
	{
		pt = new tree;  //���ɷ�Ҷ�ӽ��
		pt->num = fp[i - 1]->num + fp[i]->num;
		pt->Lnode = fp[i - 1];
		pt->Rnode = fp[i];
		fp[i] = pt;  //wl+w2
		sort(fp,i);
	}
	return fp[n - 1];
}

void preorder(struct tree* p, int k, char c)//ǰ�������
{
	int j;
	if (p != NULL)
	{
		if (c == 'L')
			s[k] = '0';
		else 
			s[k] = '1';
		if (p->Lnode == NULL)
		{   //P ָ��Ҷ�� 
			cout << p->num<<" : ";
			for (j = 0; j <= k; j++)
				cout<<s[j];
			cout<<endl;
		}
		preorder(p->Lnode, k + 1, 'L');
		preorder(p->Rnode, k + 1, 'R');
	}
}
void free_tree(struct tree* p)//�ͷ����Ĵ洢�ռ�
{
	if (p->Lnode == NULL&&p->Rnode==NULL)
		delete p;
	else
	{
		free_tree(p->Lnode);
		free_tree(p->Rnode);
		delete p;
	}
}
void quicksort(int a[], int left, int right)//���������㷨����Ȩֵ��С�������򣬱��ڽ���
{
	int i, j, t, temp;
	if (left > right)
		return;

	temp = a[left];
	i = left;
	j = right;
	while (i != j) {
		while (a[j] >= temp && j > i)
			j--;
		while (a[i] <= temp && j > i)
			i++;
		if (i < j) {
			t = a[i];
			a[i] = a[j];
			a[j] = t;
		}
	}
	a[left] = a[i];
	a[i] = temp;

	quicksort(a,left, i - 1);
	quicksort(a,i + 1, right);
}
int main()
{
	string bin;
	cout << "������ڵ����(�����ǲ�����"<<N<<"��������):  ";
	while (1)//����ڵ���Ŀ��������������ַ��������ִ�С����Χ������������
	{
		cin >> num;
		if (cin.fail() || num<1 || num>N)
		{
			cout << "�ڵ���Ŀ�Ƿ�������������:  ";
			cin.clear();
			getline(cin, bin);
			continue;
		}
		else
			break;
	}
	int f[N];
	for (int i = 0; i < N; i++)//��ʼ������
		f[i]=-1;
	cout << "������ڵ�(�Կո�ָ�):  ";
	for (int i = 0; i < num; i++)//������ڵ�Ȩֵ
		cin >> f[i];
	quicksort(f, 0, num - 1);//��Ƶ��Ȩֵ��С��������
	struct tree* head;
	init_node(f, num);  //��ʼ����� 
	head = construct_tree(f, num);  //���������� 
	preorder(head, 0, 'L');  //������ 
	free_tree(head);//ɾ����
	system("pause");
	return 0;
}
//2 3 5 7 11 13 17 19 23 29 31 37 41
//3 5 7 11 2 13 17 19 41 23 29 31 37