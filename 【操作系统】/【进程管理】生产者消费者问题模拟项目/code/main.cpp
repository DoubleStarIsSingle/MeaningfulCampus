#include <iostream>
#include <string>
#include <queue>
#include <unordered_map>
#include <windows.h>
#include "function.h"
using namespace std;

struct Process {//����
	bool state;    //����״̬������Ϊfalse��������ִ��Ϊtrue
	string address;//���������ĸ��ź�����������
	string PID;    //���̱�ʶ��
	Process(string pid, bool a = true, string b = "\0") {
		PID = pid;
		state = a;
		address = b;
	}
};
struct Buffer {//��������Ԫ
	int num;      //��������Ԫ�洢������
	string owner; //������������Ԫ�Ľ��̱��
	Buffer(int a = 0, string b = "\0") {
		num = a;
		owner = b;
	}
};

int emptyNum = 20;    //�ź�������ʾ�������ո���Ŀ
queue<string> Empty;  //empty��Ӧ����������
int fullNum = 0;      //�ź�������ʾ�����������ĵ�Ԫ��Ŀ
queue<string> Full;   //full��Ӧ����������
int mutex = 1;        //�ź��������ڱ��ֽ��̼�Ļ����ϵ
queue<string> Mutex;  //mutex��Ӧ����������
int sindex[5] = { 0 };//�������ʾs1-s5����ź���
queue<string> S1, S2, S3, S4, S5;//�ź���s1-s5��Ӧ��5����������
                                 //5�������߽���: P1-P5,5�������߽���C1-C5
Process P1("P1"), C1("C1"), P2("P2"), C2("C2"), P3("P3"), C3("C3"), P4("P4"), C4("C4"), P5("P5"), C5("C5");
unordered_map<int, Process*> producer;//�����߽���P1-P5�ֱ�������1-5����ӳ��
unordered_map<int, Process*> consumer;//�����߽���C1-C5�ֱ�������1-5����ӳ��
unordered_map<int, queue<string>*> S; //��������S1-S5�ֱ�������1-5����ӳ��
Buffer buffer[20];     //�������ʾ������
queue<string> Ready;   //��������
queue<int> ReadyLevel; //���������и����̼���ִ��ʱ���ź����ȼ�
queue<string> copyTmp; //һ���м���У������������
int showBlue = 0;      //��¼��ǰ�����������������»��ѵĽ�������

template <typename T>
void ReSort(queue<T>* q) {   //���������򣬰Ѷ����е����һ�������ᵽ��ǰ��
	queue<T> tmp;
	if ((*q).size() < 2) {   //���������ֻ��һ����û��Ԫ�أ��ز���������Ч��һ��
		return;
	}
	else {
		tmp.push((*q).back());      //�����¶���ѹ�����һ��Ԫ��
		while ((*q).size() > 1) {   //Ȼ���ٰ���ԭ����Ԫ��˳�򽫸�Ԫ��ѹ���¶���
			tmp.push((*q).front());
			(*q).pop();
		}
		(*q).pop();
		while (!tmp.empty()) {      //�൱�ڰ��ź�����м���и��Ƹ�ԭ����
			(*q).push(tmp.front());
			tmp.pop();
		}
	}
}

string P(int* flag, queue<string>* q,string q_name,Process* process,int level) {//ģ��Pԭ��
	if (ReadyLevel.front() <= level) {      //�˴�level���������壺����:����A��������empty�ź�������������
		(*flag) -= 1;                       //��A���̱����Ѻ���һ��������Ӧ������һ��Pԭ��磺P(mutex)
	}                                       //�������ٴ���һ��P(empty)
	if ((*flag) < 0 && ReadyLevel.front()<=level) {
		(*q).push((*process).PID);          //���ź���<0���ý��̱��������ź�����Ӧ������������
		Ready.pop();                //�������̱��Ӿ��������е���
		ReadyLevel.pop();           //�������̶�Ӧ��level��Ӧ����
		(*process).state = false;   //�������̵�״̬������Ϊfalse
		(*process).address = q_name;//�������̴洢���������ź������б�ʶ����
		return (*process).PID;      //�����������̵ı�ʶ��
	}
	else { //���ź�����0,�ý��̼���ִ��
		return "\0";
	}
}

string V(int* flag, queue<string>* q, queue<string>* qnext,int level) { //ģ��Vԭ��
	(*flag) += 1;
	if ((*flag) <= 0) {            //����ź�����0
		string tmp = (*q).front(); //���ź�����Ӧ�����������е�ͷһ�����̵�������
		(*qnext).push(tmp);        //�����ѵĽ��̽����������
		ReadyLevel.push(level + 1);
		ReSort<string>(&Ready);    //���»��ѵĽ�������ִ��
		ReSort<int>(&ReadyLevel);
		++showBlue;                //�����ѵĽ�����Ŀ��һ
		(*q).pop();
		for (int i = 0; i < 5; ++i) {        
			if (tmp == producer[i]->PID) {
				producer[i]->address = "\0"; //�����ѵĽ��̲�����֮ǰ�������еı�ʶ��
				producer[i]->state = true;   //���ñ����ѵĽ���״̬Ϊtrue
				return tmp;
			}
		}
		for (int i = 0; i < 5; ++i) {
			if (tmp == consumer[i]->PID) {
				consumer[i]->address = "\0";  //�����ѵĽ��̲�����֮ǰ�������еı�ʶ��
				consumer[i]->state = true;    //���ñ����ѵĽ���״̬Ϊtrue
				return tmp;
			}
		}
		return tmp;
	}
	else { //���ź���>0���ý��̼���ִ��
		return "\0";
	}
}

void PAction(int index,int* si) {//ִ�������߽���
	if (P(&emptyNum, &Empty, "Empty", producer[index],0) == "\0") {  //�൱��P(empty)
		if (P(&mutex, &Mutex, "Mutex", producer[index],2) == "\0") { //�൱��P(mutex)
			for (int i = 0; i < 20; ++i) {//��buffer�����е����ݽ��и���
				if (buffer[i].num == 0) {
					buffer[i].num = i + 1;
					buffer[i].owner = producer[index]->PID;
					//******************************************��С�Ƶ�ָʾһ�µ�ǰ���̲�����buffre��Ԫλ��
					cct_setcursor(CURSOR_INVISIBLE);
					cct_gotoxy(4 * (i + 1), 10);
					cct_setcolor(COLOR_BLACK, COLOR_HYELLOW);
					cout << "��";
					Sleep(350);
					cct_gotoxy(4 * (i + 1), 10);
					cct_setcolor(COLOR_BLACK, COLOR_WHITE);
					cout << "  ";
					//*****************************************
					Ready.pop();      //һ�����̱�ִ����
					ReadyLevel.pop();
					break;
				}
			}
			V(&mutex, &Mutex, &Ready,2);//�൱��V(mutex)
			V(si, S[index], &Ready, 1); //�൱��V(Si)
			V(&fullNum, &Full,&Ready,0);//�൱��V(full)
		}
	}
}

void CAction(int index, int* si) {//ִ�������߽���
	if (P(&fullNum, &Full, "Full", consumer[index],0) == "\0") {  //�൱��P(full)
		if (P(si, S[index], "S" + to_string(index + 1), consumer[index], 1) == "\0") {  //�൱��P(Si)
			if (P(&mutex, &Mutex, "Mutex", consumer[index], 2) == "\0") {   //�൱��P(mutex)
				for (int i = 0; i < 20; ++i) {  //��buffer�����е����ݽ��и���
					if (buffer[i].owner == "P"+to_string(index+1)) {
						buffer[i].num = 0;
						buffer[i].owner = "\0";
						//******************************************��С�Ƶ�ָʾһ�µ�ǰ���̲�����buffre��Ԫλ��
						cct_setcursor(CURSOR_INVISIBLE);
						cct_gotoxy(4 * (i + 1), 10);
						cct_setcolor(COLOR_BLACK, COLOR_HYELLOW);
						cout << "��";
						Sleep(350);
						cct_gotoxy(4 * (i + 1), 10);
						cct_setcolor(COLOR_BLACK, COLOR_WHITE);
						cout << "  ";
						//*****************************************
						Ready.pop();       //һ�����̱�ִ����
						ReadyLevel.pop();
						break;
					}
				}
				V(&mutex, &Mutex,&Ready,2);    //�൱��V(mutex)
				V(&emptyNum, &Empty,&Ready,0); //�൱��V(empty)
			}
		}
	}
}

void Push(string pname) { //������״̬�Ľ���ѹ���������
	Ready.push(pname);
	ReadyLevel.push(0);
}
void InterAction() {  //�˺�������ʹ���û��ܹ�ͨ��������̱�ʶ���ھ�����������������
	while (1) {       //ͬʱ����û��Ƿ���������Ч�Ϸ��Ľ��̱�ʶ��
		//******************************************����Ļ�ϲ�����һ�ε���Ϣ
		cct_gotoxy(0,23 + Ready.size() / 18);
		for (int i = 0; i < 5; ++i) {
			for (int j = 0; j < 100; ++j) {
				cout << ' ';
			}
		}
		//******************************************����Ļ��д����һ�ε���Ϣ
		cct_gotoxy(0,23 + Ready.size() / 18);
		cct_setcursor(CURSOR_VISIBLE_NORMAL);
		cout << "������һ������Ҫ�����Ľ�����(P1-P5��C1-C5)����������Ҫ������ֱ���û��س���" << endl;
		cout << "���������˲����ڵĽ�����,����򲻻���Ӧ,����������Ϊ��ʱ�������:";
		//******************************************
		string tmpName = "\0";
		getline(cin, tmpName);
		if (tmpName.length() == 0) {//�����˵���û�����Ҫ���������ݣ�ֱ���û��˻س���
			return;
		}
		else {  //������û���������Ч�Ľ��̱�ʶ��
            if (tmpName.length() != 2 || (tmpName[0] != 'P' && tmpName[0] != 'C') || tmpName[1] < '1' || tmpName[1] > '5') {
				continue;
			}
			else { //������û���������ȷ��Ч�ı�ʶ��
				Ready.push(tmpName); //���û������Ľ���ѹ���������ĩβ
				ReadyLevel.push(0);  //δ��ִ�й��Ľ���levelΪ0
				update_buffer();     //������Ļ�ϵ�buffer����
				continue;
			}
		}
	}
}
void update_buffer() { //������Ļ�ϵ�buffer���棬211-326��ֻ�漰��������ݽ��и�ʽ��������������߼��޹�
	for (int i = 0; i < 20; ++i) {               //��Ӱ�������⣬���Ժ���
		if (emptyNum >= 0 && fullNum >= 0) {
			cct_setcolor(COLOR_HWHITE, COLOR_HGREEN);
		}
		else {
			cct_setcolor(COLOR_HWHITE, COLOR_HRED);
		}
		cct_gotoxy(4 * (i + 1), 8);
		if (buffer[i].num == 0) {
			cout << "  ";
		}
		else {
			cout << buffer[i].num;
		}
		if (emptyNum >= 0 && fullNum >= 0) {
			cct_setcolor(COLOR_BLACK, COLOR_HGREEN);
		}
		else {
			cct_setcolor(COLOR_BLACK, COLOR_HRED);
		}
		cct_gotoxy(4 * (i + 1), 6);
		if (buffer[i].owner == "\0") {
			cout << "  ";
		}
		else {
			cout << buffer[i].owner;
		}
	}
	cct_setcolor(COLOR_BLACK, COLOR_WHITE);
	cct_gotoxy(0, 11);
	for (unsigned int i = 0; i < 14+Ready.size()/18; ++i) {
		for (int j = 0; j < 100; ++j) {
			cout << ' ';
		}
		cout << endl;
	}
	cct_gotoxy(0, 11);
	cout << "���ź��������������:";
	if (showBlue>0) {
		cct_setcolor(COLOR_BLACK, COLOR_HBLUE);
		cout << "(�����ѵĽ��̽�����һ������ִ�У�δ������ź������ڴ˽�����ɺ����)";
		cct_setcolor(COLOR_BLACK, COLOR_WHITE);
		--showBlue;
	}
	cout<< endl;
	cout << "full: " << fullNum << "   ��������: ";
	copyTmp = Full;
	while (!copyTmp.empty()) {
		cout << copyTmp.front() << "  ";
		copyTmp.pop();
	}
	cout << endl;
	cout << "empty: " << emptyNum << "   ��������: ";
	copyTmp = Empty;
	while (!copyTmp.empty()) {
		cout << copyTmp.front() << "  ";
		copyTmp.pop();
	}
	cout << endl;
	cout << "mutex: " << mutex << "   ��������: ";
	copyTmp = Mutex;
	while (!copyTmp.empty()) {
		cout << copyTmp.front() << "  ";
		copyTmp.pop();
	}
	cout << endl;
	cout << "s1: " << sindex[0] << "   ��������: ";
	copyTmp = (*S[0]);
	while (!copyTmp.empty()) {
		cout << copyTmp.front() << "  ";
		copyTmp.pop();
	}
	cout << endl;
	cout << "s2: " << sindex[1] << "   ��������: ";
	copyTmp = (*S[1]);
	while (!copyTmp.empty()) {
		cout << copyTmp.front() << "  ";
		copyTmp.pop();
	}
	cout << endl;
	cout << "s3: " << sindex[2] << "   ��������: ";
	copyTmp = (*S[2]);
	while (!copyTmp.empty()) {
		cout << copyTmp.front() << "  ";
		copyTmp.pop();
	}
	cout << endl;
	cout << "s4: " << sindex[3] << "   ��������: ";
	copyTmp = (*S[3]);
	while (!copyTmp.empty()) {
		cout << copyTmp.front() << "  ";
		copyTmp.pop();
	}
	cout << endl;
	cout << "s5: " << sindex[4] << "   ��������: ";
	copyTmp = (*S[4]);
	while (!copyTmp.empty()) {
		cout << copyTmp.front() << "  ";
		copyTmp.pop();
	}
	cout << endl;
	cout << "�������̶���:"<<endl;
	copyTmp = Ready;
	int endline = 1;
	while (!copyTmp.empty()) {
		cout << copyTmp.front() << "  ";
		copyTmp.pop();
		if (endline % 18 == 0) {
			cout << endl;
		}
		++endline;
	}
	endline = 1;
	cout << endl;
}

void Run() { //����ִ�о��������еĽ���
	while (!Ready.empty()) {  //ֻҪ�������в��գ���һֱִ��
		update_buffer();    //����buffer����
		InterAction();      //���û�����
		string tmp = Ready.front();
		if (tmp[0] == 'P') {     //ִ�������߽���
			for (int i = 0; i < 5; ++i) {
				if (tmp == producer[i]->PID) {
					PAction(i, &sindex[i]);
					break;
				}
			}
		}
		else if (tmp[0] == 'C') { //ִ�������߽���
			for (int i = 0; i < 5; ++i) {
				if (tmp == consumer[i]->PID) {
					CAction(i, &sindex[i]);
					break;
				}
			}
		}
		else {
			cout << "δ��������" << endl;
			break;
		}
	}
	update_buffer(); //����buffer����
	InterAction();   //���û�����
}

int main() {
	//����ӳ���ϵ�������߼���Ҫ
	producer[0] = &P1;
	producer[1] = &P2;
	producer[2] = &P3;
	producer[3] = &P4;
	producer[4] = &P5;
	consumer[0] = &C1;
	consumer[1] = &C2;
	consumer[2] = &C3;
	consumer[3] = &C4;
	consumer[4] = &C5;
	S[0] = &S1;
	S[1] = &S2;
	S[2] = &S3;
	S[3] = &S4;
	S[4] = &S5;
	//�����ʾ��Ϣ
	input_headline();
	draw_buffer();

	//Ϊ������֤���ź�����Ӧ���������й��ܣ�Ԥ����һ��������������
	//c1 c2 p2 p1 20��p1 p2 c1 p1 c1 c1 c1 c1 p1 c1
	Push("C1");
	Push("C2");
	Push("P2");
	Push("P1");
	for (int i = 0; i < 20; ++i) {
		Push("P1");
	}
	Push("P2");
	Push("C1");
	Push("P1");
	Push("C1");
	Push("C1");
	Push("C1");
	Push("C1");
	Push("P1");
	Push("C1");
	//Push("P1");

	Run();//����PV����ִ�о��������еĽ���
	
	system("pause");
	return 0;
}