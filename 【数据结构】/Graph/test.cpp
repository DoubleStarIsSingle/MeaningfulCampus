#include "Graphmtx.h"
#include "Graphlnk.h"

void test_Graphmtx()
{
	char ch1, ch2;
	int weight;
	Graphmtx<char, int> g(30);
	g.inputGraph();		//����ͼ
	g.outputGraph();	//��ʾͼ

	cout << "�������ͱ�����" << g.NumberOfVertices() << " " << g.NumberOfEdges() << endl;
	cout << "�鿴��һ���ڽӶ���:";
	cin >> ch1;
	cout << g.getValue(g.getFirstNeighbor(g.getVertexPos(ch1))) << endl;
	cout << "�鿴��һ���ڽӶ���:";
	cin >> ch1 >> ch2;
	cout << g.getValue(g.getNextNeighbor(g.getVertexPos(ch1), g.getVertexPos(ch2))) << endl;

	cout << "���붥��:";
	cin >> ch1;
	g.insertVertex(ch1);  //�����
	cout << "����ߣ�" << endl;
	cin >> ch1 >> ch2 >> weight;
	g.insertEdge(g.getVertexPos(ch1), g.getVertexPos(ch2), weight);//�����
	g.outputGraph();

	cout << "ɾ���ߣ�";
	cin >> ch1 >> ch2;
	g.removeEdge(g.getVertexPos(ch1), g.getVertexPos(ch2)); //ɾ����

	cout << "ɾ���㣺";
	cin >> ch1;
	g.removeVertex(g.getVertexPos(ch1));
	g.outputGraph();

}

void test_Graphlnk()
{
	cout << "-----------Grapglnk Test-----------" << endl;
	char ch1, ch2;
	int weight;
	Graphmtx<char, int> g(30);
	g.inputGraph();		//����ͼ
	g.outputGraph();	//��ʾͼ

	cout << "�������ͱ�����" << g.NumberOfVertices() << " " << g.NumberOfEdges() << endl;
	cout << "�鿴��һ���ڽӶ���:";
	cin >> ch1;
	cout << g.getValue(g.getFirstNeighbor(g.getVertexPos(ch1))) << endl;
	cout << "�鿴��һ���ڽӶ���:";
	cin >> ch1 >> ch2;
	cout << g.getValue(g.getNextNeighbor(g.getVertexPos(ch1), g.getVertexPos(ch2))) << endl;

	cout << "���붥��:";
	cin >> ch1;
	g.insertVertex(ch1);  //�����
	cout << "����ߣ�" << endl;
	cin >> ch1 >> ch2 >> weight;
	g.insertEdge(g.getVertexPos(ch1), g.getVertexPos(ch2), weight);//�����
	g.outputGraph();

	cout << "ɾ���ߣ�";
	cin >> ch1 >> ch2;
	g.removeEdge(g.getVertexPos(ch1), g.getVertexPos(ch2)); //ɾ����

	cout << "ɾ���㣺";
	cin >> ch1;
	g.removeVertex(g.getVertexPos(ch1));
	g.outputGraph();
}