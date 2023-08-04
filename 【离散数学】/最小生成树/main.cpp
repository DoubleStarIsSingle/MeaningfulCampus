#include <iostream>
#include <string>
#include "Graphmtx.h"
#include "List.h"
using namespace std;

int main()
{
	string bin, vertex;//binר�������Ե��������Ч����,vertex�����ݴ�ͼ�Ķ���
	int num_vertices, num_edges;//������������

	cout << "======================================================" << endl;
	cout << "**           Kruskal��С������ģ��ϵͳ              **" << endl;
	cout << "======================================================" << endl;

	while (1)//���붥�����
	{
		cout << "�����붥��ĸ���[2,100]:  ";
		cin >> num_vertices;
		if (cin.fail() || num_vertices < 2|| num_vertices>100)//������Ķ������Ƿ������ַ��������ֳ����������ޣ���Ҫ��������
		{
			cout << "! ��Ч���� !" << endl;
			cin.clear();
			getline(cin, bin);
			continue;
		}
		else
		{
			getline(cin, bin);
			break;
		}
	}

	Graphmtx<string, double> graph(num_vertices);//����ͼ
	List<string, double> vertex_list("*", "*", 0);//����������

	cout << "��������������������(�м��Կո����): " << endl;//���벢����������
	for (int i = 0; i < num_vertices; i++)
	{
		cin >> vertex;
		if (graph.getVertexPos(vertex) == -1)
			graph.insertVertex(vertex);
		else//����������������㣬����Ч
			i--;
	}
	getline(cin, bin);

	while (1)//����ߵ�����
	{
		cout << "������ߵ�����(Ϊ��֤�γ���ͨͼ�����ս����ı�����������" << num_vertices - 1 << "��):  ";
		cin >> num_edges;
		if (cin.fail() || num_edges < num_vertices - 1)//��Ч���룬��������
		{
			cout << "! ��Ч���� !" << endl;
			cin.clear();
			getline(cin, bin);
			continue;
		}
		else
		{
			getline(cin, bin);
			break;
		}
	}
	for (int i = 0; i < num_edges; i++)//���벢��������
	{
		string h, t;//�����˵Ķ���
		double c;//��Ȩֵ
		while (1)
		{
			cout << "�������" << i + 1 << "���ߵ����������Ȩֵ:  ";
			cin >> h >> t >> c;
			if (cin.fail() || c <= 0 || graph.getVertexPos(h) == -1 || graph.getVertexPos(t) == -1)
			{
				cout << "! ��Ч�� !" << endl;
				cin.clear();
				getline(cin, bin);
				continue;
			}
			else
			{
				long long pos = vertex_list.FindEdge(h, t);
				if (pos != -1)//������Ѿ����ڵıߣ�ֻ��Ȩֵ��ͬ������Ϊ�����бߵ�Ȩֵ���и��£������ǲ����±�
				{
					cout << "�� " << vertex_list.FindNum(pos)->get_head() << "-("
						<< vertex_list.FindNum(pos)->get_cost() << ")-"
						<< vertex_list.FindNum(pos)->get_tail() << "��Ȩֵ�Ѹ���Ϊ: " << c << endl;
					vertex_list.FindNum(pos)->set_cost(c);
					i--;
				}
				else
					vertex_list.Insert(h, t, c, vertex_list.FindBigger(c));//���±߲���ͼ��
				getline(cin, bin);
				break;
			}
		}
	}

	graph.Kruskal(vertex_list);//����Kruskal�㷨������С������
	graph.outputGraph();//�����С������
	cout << endl;

	system("pause");
	return 0;
}