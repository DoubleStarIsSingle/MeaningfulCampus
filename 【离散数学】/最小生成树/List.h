#pragma once
#include <iostream>
using namespace std;
template <class T, class E>
class List;
template <class T, class E>
class ListNode//����ڵ���
{
	friend class List<T, E>;
private:
	T head, tail;//ͷ�ڵ㡢β�ڵ�
	E cost;//·��Ȩֵ
	ListNode<T, E>* link;
public:
	ListNode() :link(NULL) {};
	ListNode(T h, T t, E c, ListNode<T, E>* L = NULL) :head(h), tail(t), cost(c), link(L) {};
	T get_head() { return head; }//����ͷ�ڵ�
	T get_tail() { return tail; }//����β�ڵ�
	E get_cost() { return cost; }//���ر�Ȩֵ
	ListNode<T, E>* get_link() { return link; }//�������ӵ���һ���ڵ�
	void set_cost(E c) { cost = c; }//����Ȩֵ
};
template <class T, class E>
class List//������
{
private:
	ListNode<T, E>* first, * last;//ͷָ�롢βָ��
public:
	List(T h, T t, E c)//���캯��
	{
		last = first = new ListNode<T, E>(h, t, c);//��̬�����ڴ�洢�½ڵ㣬����ͷ��βָ��ָ��ýڵ�
	}
	~List();
	long long Length();//ͳ��������
	long long FindData(E value, T head, T tail);//����ֵΪvalue��ͷ�ڵ�Ϊhead��β�ڵ�Ϊtail������ڵ��λ��
	long long FindEdge(T head, T tail);//Ѱ��ͷ�ڵ�Ϊhead��β�ڵ�Ϊtail�ı�
	long long FindBigger(E value);//Ѱ�ҵ�һ����valueֵ��ı�
	ListNode<T, E>* FindNum(long long i);//���������i��λ�õĽڵ�ĵ�ַ
	ListNode<T, E>* CreateNode(T h, T t, E c, ListNode<T, E>* next = NULL);//����������ڵ�
	int Insert(T h, T t, E c, long long i);//��λ��i����������ڵ�
	int Remove(long long i);//�Ƴ�������λ��Ϊi�Ľڵ�
	ListNode<T, E>* Get(long long i);//��ȡ����iλ�õĽڵ�ֵ
};

template <class T, class E>
List<T, E>::~List()
{
	ListNode<T, E>* q;
	while (first->link != last && first->link != NULL)//�ڱ�����ĵ�ѭ�������У������գ���һֱѭ��ִ��ɾ������
	{
		q = first->link;//ָ��ͷ������һ���ڵ㣬����q�ݴ����ַ
		first->link = q->link;//��ͷ���ָ�����¸��ڵ㣬�����ͷ������һ���ڵ��ѹ����Ա��ڶ���ִ��ɾ������
		delete q;//ɾ��qָ��Ľڵ�
	}
	last = first;//ѭ��ִ����Ϻ�������ֻ����ͷ��㣬��lastָ����firstָ����룬�Ա���ɾ��
	delete first;//ɾ��ͷ���
}
template <class T, class E>
ListNode<T, E>* List<T, E>::CreateNode(T h, T t, E c, ListNode<T, E>* next)//�����½ڵ�
{
	ListNode<T, E>* newnode = new ListNode<T, E>(h, t, c);
	newnode->link = next;//�����½ڵ㲢���½ڵ�ָ��ָ����һ���ڵ�
	return newnode;
}
template <class T, class E>
long long List<T, E>::Length() //ͳ��������
{
	ListNode<T, E>* p = first->link;//��ͷ���ָ�����һ���ڵ㿪ʼ����
	long long count = 0;
	while (p != NULL)//ֻҪ�ڵ㲻�ռ�δ������ĩβ����һֱѭ������������ָ��ָ��ǰ�ڵ����һ���ڵ�
	{
		p = p->link;
		count++;
	}
	return count;//���ؽڵ���������������
}
template <class T, class E>
long long List<T, E>::FindData(E value, T head, T tail)//����ȨֵΪvalue��ͷ���Ϊhead,β�ڵ�Ϊtail������ڵ��λ��
{
	ListNode<T, E>* p = first->link;//��ͷ���ָ�����һ���ڵ㿪ʼ
	long long count = 0;
	while (p != NULL && p->get_cost() != value && p->get_head() != head && p->get_tail() != tail)
	{
		p = p->link;
		count++;
	}
	if (p == NULL)//���p==NULL����˵����������û�ҵ�ӵ�и�ֵ�Ľڵ㣬����-1����ʾδ�ҵ�
		count = -1;
	return count;
}
template <class T, class E>
long long List<T, E>::FindEdge(T head, T tail)//����ͷ�ڵ�Ϊhead,β�ڵ�Ϊtail�ıߵ�λ��
{
	ListNode<T, E>* p = first->link;//��ͷ���ָ�����һ���ڵ㿪ʼ
	long long count = 0;
	while (p != NULL && (!(p->get_head() == head && p->get_tail() == tail)) && (!(p->get_head() == tail && p->get_tail() == head)))
	{                            //ע��˳���ı�Ϊ�����
		p = p->link;
		count++;
	}
	if (p == NULL)//δ�ҵ�����ʾ-1
		count = -1;
	return count;
}
template <class T, class E>
long long List<T, E>::FindBigger(E value)//Ѱ�ҵ�һ��Ȩֵ��value��ıߵ�λ��
{
	if (first->link == NULL)
		return 0;
	ListNode<T, E>* p = first->link;//��ͷ���ָ�����һ���ڵ㿪ʼ
	long long count = 0;
	while (p != NULL && p->get_cost() < value)//δ�ҵ���һֱѭ��
	{
		p = p->link;
		count++;
	}
	return count;
}
template <class T, class E>
ListNode<T, E>* List<T, E>::FindNum(long long i)//���������i��λ�õĽڵ�ĵ�ַ
{
	if (i < -1)//�Ƿ�����Ч���봦��
		return NULL;
	if (i == -1)//ͷ����������е�λ����-1��ʶ
		return first;
	ListNode<T, E>* p = first->link;
	int j = 0;
	while (p != NULL && j < i)//�������ȷ�Χ��һֱѭ����i��λ�û����һ���ڵ�
	{
		p = p->link;
		j = j++;
	}
	return p;//�����i��λ���������ȷ�Χ�ڣ����ص��ǽڵ��ַ����i�Ѿ������������ȣ���ʱp��ֵǡ����NULL������NULL��ʾ��λ�ò�������Ч�ڵ�
}
template <class T, class E>
int List<T, E>::Insert(T h, T t, E c, long long i)//��λ��i����ȨֵΪvalue��ͷ���Ϊh��β�ڵ�Ϊc������ڵ�
{
	ListNode<T, E>* p = FindNum(i - 1);//�ڵ�i��λ�ò���ڵ㵱Ȼ��Ҫ��λi-1λ�õĽڵ㣬��ʹ��ָ��ָ�������½ڵ�
	if (p == NULL)//�ԷǷ���Чλ�õĴ���
		return 0;//����0��ʾ����ʧ��
	ListNode<T, E>* newnode = CreateNode(h, t, c, p->link);//�ȶ�̬�����ڴ�洢�½����Ľڵ㣬�����½����Ľڵ�ָ��ԭ����iλ�õĽڵ�
	if (p->link == NULL)
		last = newnode;//���p�Ѿ���β���Ļ�����ô�²���Ľڵ��Ϊ�µ�β���
	p->link = newnode;//ԭ��i-1λ�õĽڵ�ָ��ָ���½��ڵ㣬��ʱ���½��ڵ��Ϊ��iλ�õĽڵ㣬ԭ��iλ�ü�����Ľڵ�λ��������һ��������Ȼ����1λ
	return 1;//����1��ʾ����ɹ�
}
template <class T, class E>
int List<T, E>::Remove(long long i)//�Ƴ�������λ��Ϊi�Ľڵ�
{
	ListNode<T, E>* q, * p = FindNum(i - 1);//�ڵ�i��λ��ɾ���ڵ㵱Ȼ��Ҫ��λi-1λ�õĽڵ㣬�Ը�����ָ��ָ��
	if (p == NULL || p->link == NULL)//�����iλ�ñ������ڽڵ㣬��Ȼ�Ͳ���Ҫ�����ɾ������
		return 0;
	q = p->link;
	p->link = q->link;//i-1λ�õ�ָ��ָ�����¸��ڵ㣬����Ҫɾ����iλ�ýڵ�
	if (q == last)
	{
		last = p;//���Ҫɾ���Ľڵ���β�ڵ�Ļ���β���ָ��Ӧǰ��һλ
	}
	delete q;//ɾ��iλ�ýڵ�
	return 1;//����1��ʾɾ���ɹ�
}
template <class T, class E>
ListNode<T, E>* List<T, E>::Get(long long i)//��ȡ����iλ�õĽڵ�ֵ
{
	ListNode<T, E>* p = FindNum(i);//���ҵ�iλ�õĽڵ�
	if (p == NULL || p == first)//���ڷǷ�λ�õĽڵ�Ĵ���
		return NULL;
	else
		return p;//����iλ�ýڵ��ַ
}

