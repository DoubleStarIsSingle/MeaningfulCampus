#pragma once
#include "List.h"
#include<iostream>
#define INF 100000 //��ʾ������
const int DefaultVertices = 30;//Ĭ�ϵ����size
using namespace std;

template<class T, class E>
class Graphmtx //�����ʾ��ͼ
{
public:
	Graphmtx(int sz = DefaultVertices);							//����
	~Graphmtx()													//����
	{
		delete[]VerticesList;
		delete[]Edge;
	}
	T getValue(int i)											//ȡ����i��ֵ����i��������NULL
	{
		if (i >= 0 && i < numVertices) return VerticesList[i];  //��ֹ����Խ��
		else
		{
			cout << "error:VerticesList[]����Խ��" << endl;
			exit(0);
		}
	}
	E getWeight(int v1, int v2)									//ȡ�ߣ�v1,v2����Ȩֵ����������0
	{
		if (v1 >= 0 && v2 >= 0 && v1 < numVertices && v2 < numVertices)//��ֹ����Խ��
			return Edge[v1][v2];
		else
			return 0;
	}
	int NumberOfVertices() { return numVertices; }	//���ص�ǰ������
	int NumberOfEdges() { return numEdges; }		//���ص�ǰ����
	int getFirstNeighbor(int v);                    //ȡ����v�ĵ�һ���ڽӶ���
	int getNextNeighbor(int v, int w);              //����v���ڽӶ���w����һ���ڽӶ���
	bool insertVertex(const T vertex);              //���붥��
	bool insertEdge(int v1, int v2, E cost);        //�����
	bool removeVertex(int v);                       //ɾ������v��������֮�����ı�
	bool removeEdge(int v1, int v2);                //ɾ���ߣ�v1,v2��
	void inputGraph();  //����ͼ
	void outputGraph(); //���ͼ
	int getVertexPos(T vertex)									//����������ͼ�е�λ��
	{
		for (int i = 0; i < numVertices; i++)
			if (VerticesList[i] == vertex)return i;
		return -1;												//�Ҳ�������-1
	}
	int DFS(int head, int tail);//�������
	void DFS(int v, int visited[]);
	void Kruskal(List<T, E>& listOfVertex);//Kruskal��С�������㷨
private:
	T* VerticesList;											//�����
	E** Edge;													//�ڽӾ���
	int maxVertices, numEdges, numVertices;

};

template<class T, class E>
Graphmtx<T, E>::Graphmtx(int sz)	//���캯��
{
	maxVertices = sz;//�������size
	numVertices = 0;
	numEdges = 0;
	int i, j;
	VerticesList = new T[maxVertices];
	Edge = new E * [maxVertices];
	for (i = 0; i < maxVertices; i++)//����forѭ����̬�ڴ濪�ٶ�ά����
		Edge[i] = new E[maxVertices];
	for (i = 0; i < maxVertices; i++)
		for (j = 0; j < maxVertices; j++)
			Edge[i][j] = (i == j) ? 0 : INF;
}

template<class T, class E>
int Graphmtx<T, E>::getFirstNeighbor(int v)//����v�ĵ�һ���ڽӶ����λ��
{
	if (v >= 0 && v < maxVertices)
	{
		for (int col = 0; col < numVertices; col++)
			if (Edge[v][col] > 0 && Edge[v][col] < INF)
				return col;
	}
	return -1;
}

template<class T, class E>
int Graphmtx<T, E>::getNextNeighbor(int v, int w)//����v���ڽӶ���w����һ���ڽӶ���
{
	if (v >= 0 && w >= 0 && v < numVertices && w < numVertices)
	{
		for (int col = w + 1; col < numVertices; col++)
		{
			if (Edge[v][col] > 0 && Edge[v][col] < INF)
				return col;
		}
	}
	return -1;
}

template<class T, class E>
bool Graphmtx<T, E>::insertVertex(const T vertex) //����һ������
{
	if (numVertices == maxVertices)return false; //���������������false
	VerticesList[numVertices++] = vertex;
	return true;
}

template<class T, class E>
bool Graphmtx<T, E>::insertEdge(int v1, int v2, E cost)//����һ����
{
	if (v1 > -1 && v1<numVertices && v2>-1 && v2 < numVertices) //�������
	{
		if (Edge[v1][v2] == INF)
		{
			Edge[v1][v2] = Edge[v2][v1] = cost;
			numEdges++;
			return true;
		}
		else
		{
			cout << "�ñ��Ѵ��ڣ����ʧ��" << endl;
			return false;
		}
	}
	else return false;
}

template<class T, class E>
bool Graphmtx<T, E>::removeVertex(int v)				//ɾ��һ������
{
	if (v < 0 || v >= numVertices)	return false;			//v����ͼ��
	if (numVertices == 1)	return false;				//ֻʣһ�����㣬��ɾ��
	int i, j;
	VerticesList[v] = VerticesList[numVertices - 1];	//�������ɾ��
	for (i = 0; i < numVertices; i++)						//��������
		if (Edge[i][v] > 0 && Edge[i][v] < INF)
			numEdges--;
	for (i = 0; i < numVertices; i++)
		Edge[i][v] = Edge[i][numVertices - 1];
	numVertices--;									//����������
	for (j = 0; j < numVertices; j++)
		Edge[v][j] = Edge[numVertices][j];
	return true;
}

template<class T, class E>
bool Graphmtx<T, E>::removeEdge(int v1, int v2)		//ɾ����
{
	if (v1 > -1 && v1<numVertices && v2>-1 && v2 < numVertices && Edge[v1][v2]>0 && Edge[v1][v2] < INF)
	{
		Edge[v1][v2] = Edge[v1][v2] = INF;
		numEdges--;
		return true;
	}
	else return false;
};

template<class T, class E>
void Graphmtx<T, E>::inputGraph()
{
	//ͨ��������������in����n�Ķ����e������ߵ���Ϣ�����ڽӾ����ʾ��ͼG���ڽӾ����ʼ�������ڹ��캯�����
	int i, j, k, m, n;
	T e1, e2;
	E weight;
	cout << "�����붥�����ͱ���(�ո����):" << endl;
	cin >> n >> m;	//�������n������m
	cout << "���������붥��:" << endl;
	for (i = 0; i < n; i++)//���붥�㣬���������
	{
		cin >> e1;
		this->insertVertex(e1);
		//G.insertVertex(e1);
	}
	cout << "����������ߣ����� v1 v2 weight ��" << endl;
	i = 0;
	while (i < m)
	{
		cin >> e1 >> e2 >> weight;
		j = this->getVertexPos(e1);//�鶥���
		k = this->getVertexPos(e2);
		if (j == -1 || k == -1)
		{
			cout << "�����˵���Ϣ�����������룡" << endl;
		}
		else
		{
			if (this->insertEdge(j, k, weight))
				i++;
		}
	}

}
template<class T, class E>
void Graphmtx<T, E>::outputGraph()
{
	//���ͼ�����ж���ͱ���Ϣ
	int i, j, n, m;
	T e1, e2;
	E weight,cost=0;
	n = this->NumberOfVertices();	 //����
	m = this->NumberOfEdges();		//����
	cout << "�������ͱ����ֱ�Ϊ��";
	cout << n << "," << m << endl;		//��������ͱ���
	cout << "��������Ϊ��" << endl;
	for (i = 0; i < n; i++)//������ά����
	{
		for (j = i + 1; j < n; j++)
		{
			weight = this->getWeight(i, j);
			if (weight > 0 && weight < INF)
			{
				e1 = this->getValue(i);
				e2 = this->getValue(j);
				cout << "(" << e1 << "," << e2 << "," << weight << ")" << endl;
				cost += weight;
			}
		}
	}
	cout << endl << "�ܷ���Ϊ: " << cost << endl;
}
template<class T, class E>
int Graphmtx<T, E>::DFS(int head, int tail)//����������ݹ�ʵ��
{
	int* visited = new int[numVertices];
	for (int i = 0; i < numVertices; i++)//��visited�������Ƿ����
		visited[i] = 0;
	DFS(head, visited);
	int exist = visited[tail];
	delete[]visited;
	return !exist;
}
template<class T, class E>
void Graphmtx<T, E>::DFS(int v, int visited[])
{
	visited[v] = 1;
	int w = getFirstNeighbor(v);
	while (w != -1)//��������ڽӶ���Ļ�
	{
		if (!visited[w])//���û���ʹ����ͷ���
			DFS(w, visited);
		w = getNextNeighbor(v, w);
	}
}
template<class T, class E>
void Graphmtx<T, E>::Kruskal(List<T, E>& listOfVertex)//Keuskal��С�������㷨
{
	while (listOfVertex.Length())
	{
		long long len = listOfVertex.Length();//ע�⣬�������д�ͷ��λ�Ѿ��ǰ�Ȩֵ��С�����ź����
		int q = 0;
		for (int i = 1; i < numVertices; i++)
			q += DFS(0, i);
		if (q == 0)//���0�ڵ㵽���������ڵ�ͨ������������ֶ���ͨ·���������Ѿ�������ͨͼ��ѭ������
			break;
		else
		{
			int x = getVertexPos(listOfVertex.FindNum(0)->get_head());
			int y = getVertexPos(listOfVertex.FindNum(0)->get_tail());
			for (int i = 0; i < numVertices; i++)
			{
				if (Edge[x][i] > 0 && Edge[x][i] < INF && (!DFS(i, y)))//������ҵı߻��γ�������ɾ���ñ�
				{
					listOfVertex.Remove(0);
					break;
				}

			}
			if (len == listOfVertex.Length())//�������γ�����������ñ�
			{
				insertEdge(x, y, listOfVertex.FindNum(0)->get_cost());
				listOfVertex.Remove(0);
			}
		}
	}
}
