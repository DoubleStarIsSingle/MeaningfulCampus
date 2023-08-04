#include <iostream>
#include <Windows.h>//��ʱ���� ��Ҫ
#include <conio.h>
#include <string.h>
#include "function.h"
using namespace std;

City city;

int control()
{
	int state = 0, x0 = -1, y0 = -1, x1 = -1, y1 = -1, x2 = -1, y2 = -1, action = -1, key_1 = -1, key_2 = -1, num = 1, judge = 1, quit = 0;
	while (1)
	{
		state = my_read_keyboard_and_mouse(x0, y0, action, key_1, key_2);//����ꡢ������Ϊ����������Ӧ 
		if (state == -1)//��ʾ�������ESC�������˳�
			break;
		else if ((state == -2 && num == 1) || (state == -2 && num == 2 && city.map[(y0 - 1) / 2][(x0 - 1) / 4] != INI))//��ʾѡ��Ҫ�ƶ������ӣ�
		{                                                                                                    //��δѡ���ƶ�Ŀ�ĵ�֮ǰ���ɵ�����������Ը���Ҫ�ƶ�������
			if (x0 < 1 || x0>4 * Y_NUM || y0 < 1 || y0 > 2 * X_NUM - 1 || y0 % 2 == 0)//������Ƿ�λ�ã��磺���������ж����Ե�λ�ã�
				continue;
			else
			{
				if (city.map[(y0 - 1) / 2][(x0 - 1) / 4] >= INI)//�����ûѡҪ�ƶ�������֮ǰ��ѡ���˿����̸���Ϊ�����ƶ���Ŀ�ĵأ�Υ����Ϸ����
					continue;
				else//���ѡ����Ҫ�ƶ�������
				{
					if (num == 2)//����ⲻ����ҵ�һ��ѡ�������
					{//��ô֮ǰ�����ѡ��Ҫ�ƶ�������ʱ���Ѿ���ͻ����ʾ�����ӣ�������Ҫ�ػ����̺�����
						city.Draw_a_board();//�ػ�����
						city.Show_all_pieces();//�ػ�����
					}
					num = 2;
					x1 = ((y0 - 1) / 2);//��������껻��ɶ�Ӧ��map����Ԫ�ص�����
					y1 = ((x0 - 1) / 4);
					city.Especially_show(x1,y1,1);//ͻ����ʾ���ѡ���Ҫ�ƶ�������
					continue;
				}
			}
		}
		else if (state == -2 && num == 2)//���ѡ��������Ҫ�ƶ�����Ŀ�ĵ�
		{
			if (x0 < 1 || x0>4 * Y_NUM || y0 < 1 || y0 > 2 * X_NUM - 1 || y0 % 2 == 0)//������Ƿ�λ�ã��磺���������ж����Ե�λ�ã�
				continue;
			else
			{
				x2 = ((y0 - 1) / 2);//��������껻��ɶ�Ӧ��map����Ԫ�ص�����
				y2 = ((x0 - 1) / 4);
				num = 3;
				city.Especially_show(x2, y2, 0);//ͻ����ʾ���ѡ�������Ҫ�ƶ�����Ŀ�ĵ�
				judge = city.shortest_way(x1, y1, x2, y2);//�������·��
				if (judge != NOT)
				{
					city.Show_way(x1,y1,x2,y2);//��·��
					city.Move(x1,y1,x2,y2);//������Ӻ��յ������ͼ�ζԵ�λ��
					city.continuity(1);//�ж����鲢�Ʒ�
					city.Current_score();//���µ�ǰ��Ϸ��ʵʱ����
					city.Kill_pieces(num);//��������
				}
				city.Draw_a_board();//�ػ�����
				if (num == 3 && judge != NOT)//��ʾ�ƶ�����δ�γ�����
					quit = city.born(GAMING_NUM);//�����������
				city.Show_all_pieces();//�ػ�����
				break;
			}
		}
	}
	if (state == -1)//����ESC�����˳���ǰ����Ϸ
		return 0;
	else if (quit)//���̱�����ռ�����˳���ǰ����Ϸ
		return 0;
	else//������ǰ����Ϸ
		return 1;
}

void first_instruction()//�����Ϸ�淨˵��
{
	cct_gotoxy(0, 2 * X_NUM + 2);
	cout << "��ǰ���� : 0 ��";
	cct_gotoxy(0, 2 * X_NUM + 4);
	cout << "�淨˵��(~��Ϸѡ�����ӻ����̸�ʱ��";
	cct_setcolor(COLOR_BLACK, COLOR_HYELLOW);
	cout << "�������������ӻ����̸�������";
	cct_setcolor(COLOR_BLACK, COLOR_WHITE);
	cout<<"Ŷ~)" << endl;
	cout << "ͬɫ��������������Ϸ,���ӹ�������ɫ,��������Խ��,�÷�Խ��,ÿ��2��,�ᡢ����б����������,��ESC�����˳���ǰ��֡�" << endl;
	cout << "���������ѡ��Ҫ�ƶ���Ŀ�����ӣ����������ѡ������̸���Ϊ�ƶ��յ�,���ڿ��ƶ�·��ʱ�����ƶ�,�һ���ʾ����ƶ�·����" << endl;
	cout << "ÿ���ƶ�������ʵ����������,���̻��������������ɫ������,��������ɵ�����ǡ���������Զ���ʧ,����Ϸ�߲��÷֡�";
}
void last_instruction()//�����ǰ����Ϸ���������ؿ�һ�ֻ����˳���Ϸ��ѡ����ʾ
{
	cct_gotoxy(0, 2 * X_NUM + 4);
	cout << "                                                                                                                  " << endl;
	cout << "                                                                                                                  " << endl;
	cout << "                                                                                                                  " << endl;
	cout << "                                                                                                                  " << endl;
	cct_gotoxy(0, 2 * X_NUM + 4);
	cout << "GAME OVER !!!  Dear gamer : " << endl;
	cout << "ѡ�����¿��������� 0,ѡ���˳���Ϸ������ 1 (���ѡ���˳���Ϸ,�����ٱ������ķ������а�����) ";
}

int main()
{
	int x = 0, y = 0, xx = 0, yy = 0;
	char back = '0';//��ֵ�����ж��Ƿ��������ѭ��
	cct_getconsoleborder(x, y, xx, yy);//��ȡ��ҳ�ʼʱcmd���ڵĴ�С���ã��Ա�ı䴰�ڴ�С��Ϊ��һָ�ԭ״
	cct_setconsoleborder(X_SET, Y_SET, XX_SET,YY_SET);//�ı�cmd���ڵĴ�С���������Ĵ�С����ʹ���ڴ�С��Ӧ��Ϸ������ʾ
	while (1)
	{
		int judge = 1;//��ֵ�����ж��Ƿ������ڲ�ѭ��
		cct_setcursor(CURSOR_INVISIBLE);//���ù��״̬Ϊ����ʾ����ʹ��������
		city.set();//��ʼ�����ݲ�����
		city.Draw_a_board();//������
		city.born(BEGIN_NUM);//���������Ŀ�������ɫ������
		city.Show_all_pieces();//������
		city.show_score();//չʾ������
		first_instruction();//�����Ϸ�淨˵��
		cct_enable_mouse();//����ʹ�����
		while (1)
		{
			judge = control();
			if (judge)
				continue;
			else
				break;
		}
		last_instruction();//�����ǰ����Ϸ���������ؿ�һ�ֻ����˳���Ϸ��ѡ����ʾ
		cct_setcursor(CURSOR_VISIBLE_NORMAL);//���ù��״̬Ϊͨ����ʾ����ʹ������ʾ����
		while (1)
		{
			back = _getche();
			if (back == '1')
				break;
			else if (back == '0')
			{
				cct_cls();//����
				city.sort();//������ҵ����η���
				break;
			}
			else//����0��1֮��������Ƿ�����
			{
				cct_gotoxy(strlen("ѡ�����¿��������� 0,ѡ���˳���Ϸ������ 1,ѡ���˳������������ʷ������¼ "), 2 * X_NUM + 9);
				cout << "                            ";
				cct_gotoxy(strlen("ѡ�����¿��������� 0,ѡ���˳���Ϸ������ 1,ѡ���˳������������ʷ������¼ "), 2 * X_NUM + 9);
				continue;
			}
		}
		if (back == '1')
			break;
		else
			continue;
	}
	cct_setconsoleborder(x, y, xx, yy);//Ϊ��һָ��Լ��Ĵ��ڴ�С
	return 0;
}
