#pragma once
#include <iostream>
using namespace std;
template <class T> class Stack;
template <class T>
class StackNode//ջ�ڵ���
{
	friend class Stack<T>;
private:
	T data;//����ڵ���Ϣ
	StackNode<T>* link;//����ڵ����ӽڵ�ĵ�ַ
public:
	StackNode(T d=NULL, StackNode<T>* L = NULL) :data(d), link(L) {}//���캯��
	StackNode<T>* GetLink() { return link; }//�������ӵĽڵ��ַ
};
template <class T>
class Stack//ջ��
{
private:
	StackNode<T>* top;//ջ��ָ��
public:
	Stack() :top(NULL) {}
	~Stack();
	void Push(const T& item);//��ջ
	T Pop();//��ջ
	T GetTop();//����ջ��Ԫ��
	int IsEmpty() const//�ж�ջ�Ƿ�Ϊ��
	{
		return top == NULL;
	}
};
template<class T>
Stack<T>::~Stack()
{
	StackNode<T>* p;
	while (top != NULL)//����������ɾ��ջ�е�����Ԫ��
	{
		p = top;
		top = top->link;
		delete p;
	}
}
template<class T>
void Stack<T>::Push(const T& item)//��ջ
{
	top = new StackNode<T>(item, top);//ͷ�����뷨���²���ڵ���Զ�������нڵ�֮ǰ������topָ���ڲ���֮��ָ���½ڵ㣬�Ӷ���������ȳ�
}
template<class T>
T Stack<T>::Pop()//��ջ
{
	if (IsEmpty())//�����ʽ����ջ�գ���ô˵�����ڱ��ʽ���淶�������ʱ����exit������������
	{
		cout << endl << "! ���ʽ���淶 ��" << endl;
		system("pause");
		exit(0);
	}
	StackNode<T>* p = top;
	T retvalue = p->data;
	top = top->link;
	delete p;     //����ɾ����ջ��Ԫ�أ���ջ��ָ��ָ���µ�ջ���ڵ㣬������ԭ��ջ��Ԫ�ص�ֵ
	return retvalue;
}
template<class T>
T Stack<T>::GetTop()//����ջ���ڵ��ֵ
{
	if (IsEmpty())//�����ʽ����ջ�գ���ô˵�����ڱ��ʽ���淶�������ʱ����exit������������
	{
		cout << endl << "! ���ʽ���淶 ��" << endl;
		system("pause");
		exit(0);
	}
	return top->data;
}