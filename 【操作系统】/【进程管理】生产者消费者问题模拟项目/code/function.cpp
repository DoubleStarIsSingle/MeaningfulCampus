#include <iostream>
#include "function.h"
using namespace std;

extern int fullNum;
extern int emptyNum;
void input_headline() { //�˺������ܣ������ͷһ����ʾ��Ϣ
	cout << "�����������߲���ϵͳģ����Ŀ:" << endl;
	cout << "���������:P1��P2��P3��P4��P5�����������:C1��C2��C3��C4��C5" << endl;
	cout << "C1ֻ����P1���������֣�C2-C5��P2-P5�Ķ�Ӧ��ϵͬ��" << endl;
	cct_setcolor(COLOR_BLACK, COLOR_HYELLOW);
	cout << "��һ�λس�����ģ�������ʾִ��һ��,��ɫ״̬������ǰ�����������̣���ɫ״̬��ʾ��ǰ�޽�������"<<endl;
	cout << "Ϊ�˷�������֤�����PV���ƵĹ���ģ�⣬��Ԥ����һ����̶��У���Ȼ��Ҳ������ʱ�޸Ľ��̶���" << endl;
	cct_setcolor(COLOR_BLACK, COLOR_WHITE);
}

void draw_buffer() { //�˺�����������Ļ�ϻ���buffer��
	cct_gotoxy(2, 7);
	cct_setcolor(COLOR_HWHITE, COLOR_BLACK);
	cout << "��";
	for (int i = 0; i < MAX_BUFFER-1; ++i) {
		cout << " �� "; 
		cout << "��";
	}
	cout << " �� ";
	cout << "�� " << endl;
	cct_gotoxy(2, 8);
	cct_setcolor(COLOR_HWHITE, COLOR_BLACK); 
	for (int i = 0; i < MAX_BUFFER; ++i) {
		cout << "��   ";
	}
	cout << "�� " << endl;
	cct_gotoxy(2, 9);
	cct_setcolor(COLOR_HWHITE, COLOR_BLACK);
	cout << "��";
	for (int i = 0; i < MAX_BUFFER-1; ++i) {
		cout << " �� ";
		cout << "��";
	}
	cout << " �� ";
	cout << "�� " << endl << endl;
	cct_setcolor(COLOR_BLACK, COLOR_WHITE);
}

