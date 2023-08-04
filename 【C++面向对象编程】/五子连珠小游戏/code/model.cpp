#include <iostream>
#include <stdlib.h>  //�����������Ҫ
#include <time.h>   //�����������Ҫ
#include <Windows.h>//��ʱ���� ��Ҫ
#include "function.h"
using namespace std;
extern class City city;

int City::score = 0,City::score_num=0;
City::City()
{
	for (int i = 0; i < X_NUM; i++)
		for (int j = 0; j < Y_NUM; j++)
			map[i][j] = INI;
	for (int i = 0; i < MARK; i++)
		score_sort[i] = -1;
}
void City::set()//����map��������ʼ״̬
{
	for (int i = 0; i < X_NUM; i++)
		for (int j = 0; j < Y_NUM; j++)
			map[i][j] = INI;

}
void City::Draw_a_board()//������
{
	cct_gotoxy(0, 0);
	cct_setcolor(COLOR_HWHITE, COLOR_BLACK);
	for (int y = 0; y < Y_NUM * 2 + 1; y++)
	{
		for (int x = 0; x < X_NUM * 4 + 2; x += 2)
		{
			if (y == 0 && x == 0)
				cout << "��";
			else if (y == Y_NUM * 2 && x == 0)
				cout << "��";
			else if (y == 0 && x == X_NUM * 4)
				cout << "��" << endl;
			else if (y == Y_NUM * 2 && x == X_NUM * 4)
				cout << "��" << endl;
			else if (y == 0 && x % 4 == 0)
				cout << "��";
			else if (y == Y_NUM * 2 && x % 4 == 0)
				cout << "��";
			else if (y % 2 == 0 && x == 0)
				cout << "��";
			else if (y % 2 == 0 && x == X_NUM * 4)
				cout << "��" << endl;
			else if (y % 2 == 0 && x % 4 == 0)
				cout << "��";
			else if (y % 2 == 0)
				cout << "��";
			else if (x % 4 == 0)
			{
				cout << "��";
				if (x == X_NUM * 4)
					cout << endl;
			}
			else
				cout << "  ";
		}
	}
	cct_setcolor(COLOR_BLACK, COLOR_WHITE);
}
void City::Show_all_pieces()//������
{
	for (int i = 0; i < X_NUM; i++)
	{
		for (int j = 0; j < Y_NUM; j++)
		{
			if (map[i][j] < INI)
			{
				cct_setcolor(COLOR_HWHITE, city.map[i][j] + COLOR_DIS + ((map[i][j] < COLOR_MIN) ? COLOR_NUM : 0));
				cct_gotoxy(4 * (j + 1) - 2, 2 * i + 1);
				cout << "��";
				cct_setcolor(COLOR_BLACK, COLOR_WHITE);
			}
		}
	}
}
void City::Show_way(int x1, int y1, int x2, int y2)//��·��
{
	for (int i = 0; i < X_NUM; i++)
	{
		for (int j = 0; j < Y_NUM; j++)
		{
			if ((!((i == x1 && j == y1) || (i == x2 && j == y2))) &&map[i][j] > 100)
			{
				cct_setcolor(COLOR_WHITE, COLOR_WHITE);
				cct_gotoxy(4 * j + 2, 2 * i + 1);
				cout << "  ";
				cct_setcolor(COLOR_BLACK, COLOR_WHITE);
			}
		}
	}
}
void City::Kill_pieces(int& number)//��������
{
	for (int i = 0; i < X_NUM; i++)
	{
		for (int j = 0; j < Y_NUM; j++)
		{
			if (!(map[i][j] >= COLOR_MIN && map[i][j] < INI))
			{
				if (map[i][j] < COLOR_MIN)
					number = 4;
				map[i][j] = INI;
			}
		}
	}
}
void City::Move(int x1, int y1, int x2, int y2)//�Ե�������Ӻ��յ������ͼ�ε�λ��
{
	Sleep(200);
	cct_setcolor(COLOR_WHITE, COLOR_BLACK);
	cct_gotoxy(4 * y1 + 2, 2 * x1 + 1);
	cout << "  ";
	cct_setcolor(COLOR_WHITE, map[x2][y2] + COLOR_DIS);
	cct_gotoxy(4 * y2 + 2, 2 * x2 + 1);
	cout << "��";
	cct_setcolor(COLOR_BLACK, COLOR_WHITE);
}
void City::Especially_show(int x, int y, int state)//ͻ����ʾ������յ�
{
	cct_setcolor(COLOR_WHITE, COLOR_BLACK);
	cct_gotoxy(4 * y, 2 * x);
	cout << "������";
	cct_gotoxy(4 * y, 2 * x + 1);
	cout << "��  ��";
	cct_gotoxy(4 * y, 2 * x + 2);
	cout << "������";
	if (state)
	{
		cct_setcolor(COLOR_WHITE, city.map[x][y] + COLOR_DIS);
		cct_gotoxy(4 * y + 2, 2 * x + 1);
		cout << "��";
	}
	cct_setcolor(COLOR_BLACK, COLOR_WHITE);
}
void City::Current_score()//��ʾ��ǰ��Ϸ��ʵʱ����
{
	cct_gotoxy(0, 2 * X_NUM + 2);
	cout << "                                                 ";
	cct_gotoxy(0, 2 * X_NUM + 2);
	cout << "��ǰ���� : " << score * 2 << "��";
}
void City::sort()//��������
{
	int temp_1 = -1, temp_2 = -1,state=0;
	score_num++;
	temp_1 = score* MARK + score_num;
	score *= 0;
		if (score_sort[0] == -1)
			score_sort[0] = temp_1;
		else
		{
			for (int i = 0;i<MARK-1; i++)
			{
				if (score_sort[i]/ MARK <= temp_1/MARK)
				{
					for (int j = i;j<MARK-1; j++)
					{
						temp_2 = score_sort[j];
						score_sort[j] = temp_1;
						temp_1 = temp_2;
						if (score_sort[j + 1] == -1)
						{
							score_sort[j + 1] = temp_2;
							state = 1;
							break;
						}
						else
							continue;
					}
				}
				else if (score_sort[i + 1] == -1)
				{
					score_sort[i + 1] = temp_1;
					break;
				}
				else
					continue;
				if (state)
					break;
			}
		}
}
void City::show_score()//չʾ����
{
	cct_setcolor(COLOR_BLACK, COLOR_HYELLOW);
	cct_gotoxy(4*(X_NUM+1),0);
	cout << "����ص������̨���Ͻǣ�������������:ʹ�þɰ����̨";
	cct_gotoxy(4 * (X_NUM + 1), 1);
	cout<<"���رտ��ٱ༭�Ͳ���ģʽ, �����뷨�л���Ӣ��";
	cct_setcolor(COLOR_BLACK,COLOR_HGREEN);
	cct_gotoxy(4 * (X_NUM + 1), 3);
	cout << "�������а�(�����Ӹߵ�������,ֻ��ʾ��ߵ�12������)" << endl;
	cct_setcolor(COLOR_BLACK, COLOR_HGREEN);
	for (int i = 0; i < MARK && score_sort[i] != -1; i++)
	{
		cct_gotoxy(4 * (X_NUM+1), i+4);
		cout << "��" << score_sort[i] % MARK << "�� :  " << score_sort[i] / MARK * 2 << " ��" << endl;
	}
	cct_setcolor(COLOR_BLACK, COLOR_WHITE);
}
int City::Wall(int i, int j,int kind,int x_start,int y_start,int x_finish,int y_finish)//�ж����̸��Ƿ����Ӱ�Χ���ж��߼�Ϊ���Ľǡ��ٱ߽硢���м�
{
	if ((i == 0 && j == 0 && map[i][j + 1] < INI && map[i + 1][j] < INI)
        && (!(kind == 1 && ((x_start - 1 == x_finish && y_start == y_finish) || (x_start + 1 == x_finish && y_start == y_finish) || (x_start == x_finish && y_start - 1 == y_finish) || (x_start == x_finish && y_start + 1 == y_finish)))))
	//&&(!(kind==1&&((x_start-1==x_finish&&y_start==y_finish)||(x_start+1==x_finish&&y_start==y_finish)||(x_start==x_finish&&y_start-1==y_finish)||(x_start==x_finish&&y_start+1==y_finish))))
		return NOT;
	else if ((i == 0 && j == Y_NUM - 1 && map[i][j - 1] < INI && map[i + 1][j] < INI)
		&& (!(kind == 1 && ((x_start - 1 == x_finish && y_start == y_finish) || (x_start + 1 == x_finish && y_start == y_finish) || (x_start == x_finish && y_start - 1 == y_finish) || (x_start == x_finish && y_start + 1 == y_finish)))))
		return NOT;
	else if ((i == X_NUM - 1 && j == 0 && map[i][j + 1] < INI && map[i - 1][j] < INI)
		&& (!(kind == 1 && ((x_start - 1 == x_finish && y_start == y_finish) || (x_start + 1 == x_finish && y_start == y_finish) || (x_start == x_finish && y_start - 1 == y_finish) || (x_start == x_finish && y_start + 1 == y_finish)))))
		return NOT;
	else if ((i == X_NUM - 1 && j == Y_NUM - 1 && map[i][j - 1] < INI && map[i - 1][j] < INI)
		&& (!(kind == 1 && ((x_start - 1 == x_finish && y_start == y_finish) || (x_start + 1 == x_finish && y_start == y_finish) || (x_start == x_finish && y_start - 1 == y_finish) || (x_start == x_finish && y_start + 1 == y_finish)))))
		return NOT;
	else if ((j >= 1 && j <= Y_NUM - 2 && i == 0 && map[i][j - 1] < INI && map[i][j + 1] < INI && map[i + 1][j] < INI)
		&& (!(kind == 1 && ((x_start - 1 == x_finish && y_start == y_finish) || (x_start + 1 == x_finish && y_start == y_finish) || (x_start == x_finish && y_start - 1 == y_finish) || (x_start == x_finish && y_start + 1 == y_finish)))))
		return NOT;
	else if ((j >= 1 && j <= Y_NUM - 2 && i == X_NUM - 1 && map[i][j - 1] < INI && map[i][j + 1] < INI && map[i - 1][j] < INI)
		&& (!(kind == 1 && ((x_start - 1 == x_finish && y_start == y_finish) || (x_start + 1 == x_finish && y_start == y_finish) || (x_start == x_finish && y_start - 1 == y_finish) || (x_start == x_finish && y_start + 1 == y_finish)))))
		return NOT;
	else if ((i >= 1 && i <= X_NUM - 2 && j == 0 && map[i - 1][j] < INI && map[i + 1][j] < INI && map[i][j + 1] < INI)
		&& (!(kind == 1 && ((x_start - 1 == x_finish && y_start == y_finish) || (x_start + 1 == x_finish && y_start == y_finish) || (x_start == x_finish && y_start - 1 == y_finish) || (x_start == x_finish && y_start + 1 == y_finish)))))
		return NOT;
	else if ((i >= 1 && i <= X_NUM - 2 && j == Y_NUM - 1 && map[i - 1][j] < INI && map[i + 1][j] < INI && map[i][j - 1] < INI)
		&& (!(kind == 1 && ((x_start - 1 == x_finish && y_start == y_finish) || (x_start + 1 == x_finish && y_start == y_finish) || (x_start == x_finish && y_start - 1 == y_finish) || (x_start == x_finish && y_start + 1 == y_finish)))))
		return NOT;
	else if ((i >= 1 && i <= X_NUM - 2 && j >= 1 && j <= Y_NUM - 2 && map[i][j + 1] < INI && map[i][j - 1] < INI && map[i - 1][j] < INI && map[i + 1][j] < INI)
		&& (!(kind == 1 && ((x_start - 1 == x_finish && y_start == y_finish) || (x_start + 1 == x_finish && y_start == y_finish) || (x_start == x_finish && y_start - 1 == y_finish) || (x_start == x_finish && y_start + 1 == y_finish)))))
		return NOT;
	else
		return INI;
}
int City::shortest_way(int x_start, int y_start, int x_finish, int y_finish)//�������·��
{
	//���ж�·���ܷ���ͨ
	if (x_start<0 || x_start>X_NUM - 1 || y_start<0 || y_start>Y_NUM - 1 || x_finish<0 || x_finish>X_NUM - 1 || y_finish<0 || y_finish>Y_NUM - 1)
		return -1;
	else if (map[x_start][y_start] >= INI || map[x_finish][y_finish] != INI)
		return -1;
	else if (Wall(x_start, y_start,0,x_start,y_start,x_finish,y_finish) == NOT || Wall(x_finish, y_finish,1, x_start, y_start, x_finish, y_finish) == NOT)
		return NOT;
	//�ټ�������·�����߼�Ϊ���յ㿪ʼ���ƣ�������յ㵽����ÿһ������·����
	//˼·������Dijkstra�㷨������ڹ��������㷨���ŵ����������ָ��ӵ��������������С�ұ�֤��·�����
	//���ҿ���չ�Ը�ǿ��ͨ�����ڵ���·����Ȩ�Լ��ɳڲ�������ʵ������ͼ���㡢�滮·�ߵĹ���
	else
	{
		int temp = map[x_start][y_start];
		map[x_finish][y_finish] = 0;//���յ㷴�ƣ�����Ϊ0
		map[x_start][y_start] = INI;
		//��������յ�Ϊ���ķ���������·�������������ڵ�������£������޷�ֱ�Ӽ���ĵ�
		for (int grow = 1; !(x_finish - grow < 0 && x_finish + grow >X_NUM - 1 && y_finish - grow < 0 && y_finish + grow > Y_NUM - 1); grow++)
		{
			for (int i = 0; i < Y_NUM; i++)//���ϡ�����չ
			{
				if (!(x_finish - grow >= 0 && x_finish - grow <= X_NUM - 1))//�ж��Ƿ����̷�Χ
					;
				else if (Wall(x_finish - grow, i,1, x_start, y_start, x_finish, y_finish) == NOT&& map[x_finish - grow][i]>=INI)//�ж��Ƿ��������Ӱ�Χ�������˷�֧�����ȼ�һ��Ҫ������һ��֧������BUG
					map[x_finish - grow][i] = NOT;
				else if (map[x_finish - grow + 1][i] < 0 || map[x_finish - grow][i] < INI)//�ж��Ƿ������ӵ���
					;
				else//��չ��̾���
					map[x_finish - grow][i] = map[x_finish - grow + 1][i] + 1;

				if (!(x_finish + grow >= 0 && x_finish + grow <= X_NUM - 1))//�ж��Ƿ����̷�Χ
					;
				else if (Wall(x_finish + grow, i,1, x_start, y_start, x_finish, y_finish) == NOT && map[x_finish + grow][i] >= INI)//�ж��Ƿ��������Ӱ�Χ�������˷�֧�����ȼ�һ��Ҫ������һ��֧������BUG
					map[x_finish + grow][i] = NOT;
				else if (map[x_finish + grow - 1][i] < 0 || map[x_finish + grow][i] < INI)//�ж��Ƿ������ӵ���
					;
				else//��չ��̾���
					map[x_finish + grow][i] = map[x_finish + grow - 1][i] + 1;
			}
			for (int i = 0; i < X_NUM; i++)//��������չ
			{
				if (!(y_finish - grow >= 0 && y_finish - grow <= Y_NUM - 1))//�ж��Ƿ����̷�Χ
					;
				else if (Wall(i, y_finish - grow,1, x_start, y_start, x_finish, y_finish) == NOT&& map[i][y_finish - grow]>=INI)//�ж��Ƿ��������Ӱ�Χ�������˷�֧�����ȼ�һ��Ҫ������һ��֧������BUG
					map[i][y_finish - grow] = NOT;
				else if (map[i][y_finish - grow + 1] < 0 || map[i][y_finish - grow] < INI)//�ж��Ƿ������ӵ���
					;
				else//��չ��̾���
					map[i][y_finish - grow] = map[i][y_finish - grow + 1] + 1;

				if (!(y_finish + grow >= 0 && y_finish + grow <= Y_NUM - 1))//�ж��Ƿ����̷�Χ
					;
				else if (Wall(i, y_finish + grow,1, x_start, y_start, x_finish, y_finish) == NOT && map[i][y_finish + grow] >= INI)//�ж��Ƿ��������Ӱ�Χ�������˷�֧�����ȼ�һ��Ҫ������һ��֧������BUG
					map[i][y_finish + grow] = NOT;
				else if (map[i][y_finish + grow - 1] < 0 || map[i][y_finish + grow] < INI)//�ж��Ƿ������ӵ���
					;
				else//��չ��̾���
					map[i][y_finish + grow] = map[i][y_finish + grow - 1] + 1;
			}
		}
		for (int k=1;;k++)//������һѭ����Ϊ�����ڵ�δ�ܼ������̾���
		{//���˴���Ƭ�Σ���̾���������һ���Ϳ��ܲ�����̾��룬����ÿ����һ������Ҫ���ڱȽϣ�ѡ�����м���������̵ģ��������·��
			for (int i = 0; i < X_NUM; i++)
			{
				for (int j = 0; j < Y_NUM; j++)
				{
					if (map[i][j]==k)
					{//�ϡ��¡�������չ���㣬ֱ���յ㷴�Ƶ��������̸�ľ���ȷ����ѭ������
						if (map[i - ((i >= 1 && i < X_NUM) ? 1 : 0)][j] > map[i][j] + 1 || map[i - ((i >= 1 && i < X_NUM) ? 1 : 0)][j] == INI)
							map[i - ((i >= 1 && i < X_NUM) ? 1 : 0)][j] = map[i][j] + 1;
						if (map[i + ((i >= 0 && i < X_NUM - 1) ? 1 : 0)][j] > map[i][j] + 1 || map[i + ((i >= 0 && i < X_NUM - 1) ? 1 : 0)][j] == INI)
							map[i + ((i >= 0 && i < X_NUM - 1) ? 1 : 0)][j] = map[i][j] + 1;
						if (map[i][j - ((j >= 1 && j < Y_NUM) ? 1 : 0)] > map[i][j] + 1 || map[i][j - ((j >= 1 && j < Y_NUM) ? 1 : 0)] == INI)
							map[i][j - ((j >= 1 && j < Y_NUM) ? 1 : 0)] = map[i][j] + 1;
						if (map[i][j + ((j >= 0 && j < Y_NUM - 1) ? 1 : 0)] > map[i][j] + 1 || map[i][j + ((j >= 0 && j < Y_NUM - 1) ? 1 : 0)] == INI)
							map[i][j + ((j >= 0 && j < Y_NUM - 1) ? 1 : 0)] = map[i][j] + 1;
					}
				}
			}
			if (count(k, 360, 0, 0, 0) == 0)
			{
				for (int i = 0; i < X_NUM; i++)
				{
					for (int j = 0; j < Y_NUM; j++)
					{
						if (map[i][j] == INI)
							map[i][j] = NOT;
					}
				}
				break;
			}
		}
		if (map[x_start][y_start] == NOT)
		{
			map[x_start][y_start] = temp;
			for (int i = 0; i < X_NUM; i++)
			{
				for (int j = 0; j < Y_NUM; j++)
				{
					if(!(map[i][j]<INI&&map[i][j]>=COLOR_MIN))
						map[i][j] = INI;
				}
			}
			return NOT;
		}
		else
		{
			//�����Ѽ���õ���㵽�յ����̾��룬��ô����㿪ʼ�Ҿ����յ���Լ�Сһ�����̸��Դ����ƣ�ֱ�������յ㣬�������·��
			for (int i = x_start, j = y_start; !(i == x_finish && j == y_finish); )
			{//˳ʱ�뷽��Ѱ�ң�Ѱ�ҷ���Ӱ���Ƿ�Ϊ��̾���
				if (i - 1 >= 0 && i - 1 <= X_NUM - 1 && map[i - 1][j] == map[i][j] - 1 && map[i - 1][j]>=0)
				{
					map[i][j] += 100;
					i -= 1;
				}
				else if (j + 1 >= 0 && j + 1 <= Y_NUM - 1 && map[i][j + 1] == map[i][j] - 1 && map[i][j + 1]>=0)
				{
					map[i][j] += 100;
					j += 1;
				}
				else if (i + 1 >= 0 && i + 1 <= X_NUM - 1 && map[i + 1][j] == map[i][j] - 1 && map[i + 1][j]>=0)
				{
					map[i][j] += 100;
					i += 1;
				}
				else if (j - 1 >= 0 && j - 1 <= Y_NUM - 1 && map[i][j - 1] == map[i][j] - 1 && map[i][j - 1] >=0)
				{
					map[i][j] += 100;
					j -= 1;
				}
				else
					continue;
			}
			map[x_finish][y_finish] = temp;
			return -2;
		}
	}
}
int City::count(int target, int angle, int x, int y, int state)//����������Ŀ�����ӵ���Ŀ���ֵ��ж�������ƷֵĹ���
{                                                              //stateΪ0��ʾ�ǼƷ���;��1Ϊ�Ʒ���;��score����
	int number_same = 0, number_score = 0, kill_x = 0, kill_y = 0;
	if (angle == 360)//ȫ������Ŀ�����ӵ�����
	{
		for (int x = 0; x < X_NUM; x++)
			for (int y = 0; y < Y_NUM; y++)
			{
				if (map[x][y] == target)
					number_same++;
			}
		return number_same;
	}
	else if (angle == 0)//0�㷽�������ж�
	{
		for (int y = 0; y < Y_NUM - 1; y++)
		{
			if ((map[x][y] == target || map[x][y] == target - COLOR_NUM) && (map[x][y + 1] == target || map[x][y + 1] == target - COLOR_NUM))
			{
				number_same++;
				kill_y = y + 1;
			}
			else
			{
				if (number_same + 1 < SCORE_NUM)
					number_same = 0;
				else
					break;
			}
		}
		if (number_same + 1 >= SCORE_NUM)
		{
			number_same++;
			for (int i = 0; i < number_same; i++)
			{
				if (map[x][kill_y - i] == target)
					number_score++;
				map[x][kill_y - i] = target - COLOR_NUM;
			}
		}
		if (state)
			score += number_score;
		return number_same;
	}
	else if (angle == 90)//90�㷽�������ж�
	{
		for (int x = 0; x < X_NUM - 1; x++)
		{
			if ((map[x][y] == target || map[x][y] == target - COLOR_NUM) && (map[x + 1][y] == target || map[x + 1][y] == target - COLOR_NUM))
			{
				number_same++;
				kill_x = x + 1;
			}
			else
			{
				if (number_same + 1 < SCORE_NUM)
					number_same = 0;
				else
					break;
			}
		}
		if (number_same + 1 >= SCORE_NUM)
		{
			number_same++;
			for (int i = 0; i < number_same; i++)
			{
				if (map[kill_x - i][y] == target)
					number_score++;
				map[kill_x - i][y] = target - COLOR_NUM;
			}
		}
		if (state&&number_same>= SCORE_NUM)
			score += number_score;
		return number_same;
	}
	else//45�㡢135�㷽�������ж�
	{

		int x_add = -1, y_add = 1;
		if (x == X_NUM - 1 && angle == 135)
		{
			x_add = -1;
			y_add = -1;
		}
		else if (x == X_NUM - 1 && angle == 45)
		{
			x_add = -1;
			y_add = 1;
		}
		else if (x == 0 && angle == 45)
		{
			x_add = 1;
			y_add = -1;
		}
		else
		{
			x_add = 1;
			y_add = 1;
		}
		for (; x >= 0 && x <= X_NUM - 1 && y >= 0 && y <= Y_NUM - 1; x += x_add, y += y_add)
		{
			int x_next = x + x_add, y_next = y + y_add;
			if (x_next < 0)
				x_next = 0;
			else if (x_next > X_NUM - 1)
				x_next = X_NUM - 1;
			else if (y_next < 0)
				y_next = 0;
			else if (y_next > Y_NUM - 1)
				y_next = Y_NUM - 1;
			if ((map[x][y] == target || map[x][y] == target - COLOR_NUM) && (map[x_next][y_next] == target
				|| map[x_next][y_next] == target - COLOR_NUM))
			{
				number_same++;
				kill_x = x_next;
				kill_y = y_next;
			}
			else
			{
				if (number_same + 1 < SCORE_NUM)
					number_same = 0;
				else
					break;
			}
		}
		if (number_same + 1 >= SCORE_NUM)
		{
			number_same++;
			for (int i = 0; i < number_same; i++)
			{
				if (map[kill_x - x_add * i][kill_y - y_add * i] == target)
					number_score++;
				map[kill_x - x_add * i][kill_y - y_add * i] = target - COLOR_NUM;
			}
		}
		if (state)
			score += number_score;
		return number_same;
	}
}
int City::born(int num)//���������Ŀ�������ɫ������
{
	srand(static_cast<int>(time(0)));
	for (int i = 0; i < num;)
	{
		int x_rand = rand() % 9, y_rand = rand() % 9, original = score, result = 0;
		if (map[x_rand][y_rand] < INI)
			continue;
		else
		{
			map[x_rand][y_rand] = (rand() % COLOR_NUM + 5) * (-1);
			continuity(1);
			result = score;
			if (result != original)
			{
				score -= (result - original);
				if (num == BEGIN_NUM)
				{
					map[x_rand][y_rand] = INI;
					result = original;
					continue;
				}
				else
					i++;
			}
			else
				i++;
		}
		if (result != original)
		{
			for (int i = 0; i < X_NUM; i++)
			{
				for (int j = 0; j < Y_NUM; j++)
				{
					if (map[i][j] < INI)
					{
						cct_setcolor(COLOR_HWHITE, map[i][j] + COLOR_DIS + ((map[i][j] < COLOR_MIN) ? COLOR_NUM : 0));
						cct_gotoxy(4 * (j + 1) - 2, 2 * i + 1);
						cout << "��";
						cct_setcolor(COLOR_BLACK, COLOR_WHITE);
						if (map[i][j] < COLOR_MIN)
						    map[i][j] = INI;
					}
				}
			}
			result = original;
			Sleep(700);
			Draw_a_board();
		}
		if (count(INI, 360, 0, 0, 0) == 0)
			break;
		else
			continue;
	}
	if (count(INI, 360, 0, 0, 0) == 0)
		return 1;
	else
		return 0;
}
void City::continuity(int state)//ɨ��ȫ���̡��ж�����
{
	for (int color = -5; color >= -10; color--)
	{
		for (int x = 0; x <= X_NUM - 1; x++)
			count(color, 0, x, 0, state);
		for (int y = 0; y <= Y_NUM - 1; y++)
			count(color, 90, 0, y, state);
		for (int y = SCORE_NUM - 1; y < Y_NUM; y++)
			count(color, 45, 0, y, state);
		for (int y = 0; y <= Y_NUM - SCORE_NUM; y++)
			count(color, 45, X_NUM - 1, y, state);
		for (int y = SCORE_NUM - 1; y < X_NUM; y++)
			count(color, 135, X_NUM - 1, y, state);
		for (int y = 0; y <= Y_NUM - SCORE_NUM; y++)
			count(color, 135, 0, y, state);
	}
}