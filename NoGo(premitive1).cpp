#include <easyx.h>
#include <graphics.h>
#include <iostream>
#include <stdlib.h>
#include <Windows.h>
#include<string>
#include<cstdio>
#include<vector>
using namespace std;
int board[10][10] = { 0 };//0表无子，1表黑子，2表白子
int check_board[10][10] = { 0 };//是否要归零还不确定
int edge = 9;
int MOVE[4][2] = { {1,0},{-1,0},{0,1},{0,-1} };
int cnt = 0;
int flag_side = 0;
int MININT = -2147483648;
int evaluation(int i, int j)
{
	srand((unsigned int)time(0));
	return (rand() % 9);
}
int check(int r, int c)//返回1表示在棋盘内
{
	if (r >= 0 && r < 9 && c >= 0 && c < 9)
		return 1;
	else
		return 0;
}
bool judge(int r, int c)//返回1表示能下(与上面保持一致)
{
	check_board[r][c] = 1;
	int flag = 0;
	for (int i = 0; i < 4; i++) {
		int r1 = r + MOVE[i][0];
		int c1 = c + MOVE[i][1];
		if (check(r1, c1)) {
			if (board[r1][c1] == 0)
				flag = 1;
			if (board[r1][c1] == board[r][c] && !check_board[r1][c1]) {
				if (judge(r1, c1))
					flag = 1;
			}
		}
	}
	return flag;
}
bool judgement(int r, int c, int flag_side)//返回1表示可下
{
	if (board[r][c]==1||board[r][c]==2)
		return 0;
	board[r][c] = flag_side;
	memset(check_board, 0, sizeof(check_board));
	//check_board[r][c] = 1;
	if (judge(r, c)==0) {
		board[r][c] = 0;
		return 0;
	}
	for (int i = 0; i < 4; i++) {
		int r1 = r + MOVE[i][0];
		int c1 = c + MOVE[i][1];
		if (check(r1, c1)) {
			if (board[r1][c1] && !check_board[r1][c1]) {
				if (!judge(r1, c1)) {
					board[r][c] = 0;
					return 0;
				}
			}
		}
	}
	board[r][c] = 0;
	return 1;
}
int row = 0, col = 0, best_row, best_col, best_value = MININT, value = 0;
/*int nega_max(int depth, int flag_side)//flag_side==1表示AI为黑方，flagSide==2表示AI为白方
{
	if (depth == 0)
		return evaluation(row, col);
	for (row = 0; row < edge; row++) {
		for (col = 0; col < edge; col++) {
			if (board[row][col] == 0) {
				if (judgement(row, col, flag_side) == 0) {
					continue;
				}
				else {
					value = -nega_max(depth - 1, flag_side);
				}
				if (value > best_value) {
					best_value = value;
					best_row = row;
					best_col = col;
				}
				//以下用来清空初始数据
				col = 0; row = 0;
			}
			else
				continue;
		}
	}
	return best_value;
}*/
void print_game_over_AI(int flag_side)
{
	if (flag_side == 2) {//AI执白，AI输
		outtextxy(600, 360, L"BLACK SIDE WINS");
	}
	else if (flag_side == 1) {//AI执黑，AI输
		outtextxy(600, 360, L"WHITE SIDE WINS");
	}
	return;
}
void print_game_over_human(int human_flag_side)
{
	if (human_flag_side == 1) {//AI执白，AI赢
		outtextxy(600, 360, L"WHITE SIDE WINS");
	}
	else if (human_flag_side == 2) {//AI执黑，AI赢
		outtextxy(600, 360, L"BLACK SIDE WINS");
	}
	return;
}
//x横，y竖
//黑棋第一手禁止走棋盘中心位置

IMAGE white;
IMAGE black;
IMAGE cundang;
int main()
{
	initgraph(800, 600); //初始化图形系统
	setbkcolor(RGB(255, 239, 213)); //设置图形窗口的背景颜色
	cleardevice(); //清屏幕
	//下面行读取图像文件
	//loadimage(&black, _T("hei.jpg"), 54, 54, FALSE);
	//loadimage(&white, _T("bai.jpg"), 54, 54, FALSE);
	//loadimage(&cundang, _T("cundang.jpg"), 141, 50, FALSE);
	//下面行显示图像文件
	//putimage(600, 50, &black);
	//putimage(600, 380, &white);
	//putimage(600, 490, &cundang);
	//下面代码网格
	setcolor(RGB(0, 0, 0));
	rectangle(60, 60, 540, 540);
	setfillcolor(RGB(255, 239, 213));
	fillrectangle(60, 60, 540, 540);
	setcolor(RGB(0, 0, 0));
	line(60, 120, 540, 120);
	line(60, 180, 540, 180);
	line(60, 240, 540, 240);
	line(60, 300, 540, 300);
	line(60, 360, 540, 360);
	line(60, 420, 540, 420);
	line(60, 480, 540, 480);
	line(120, 60, 120, 540);
	line(180, 60, 180, 540);
	line(240, 60, 240, 540);
	line(300, 60, 300, 540);
	line(360, 60, 360, 540);
	line(420, 60, 420, 540);
	line(480, 60, 480, 540);
	setcolor(RGB(0, 0, 0));//画黑棋选项
	rectangle(600, 120, 660, 180);
	setfillcolor(RGB(0, 0, 0));
	fillrectangle(600, 120, 660, 180);
	setcolor(BLACK);//画白棋选项
	rectangle(600, 240, 660, 300);
	setfillcolor(WHITE);
	fillrectangle(600, 240, 660, 300);
	//下面代码鼠标功能
	//int cnt=0;//用来统计总共走了多少步
	while (1) {
		MOUSEMSG msg = GetMouseMsg();
		if (msg.mkLButton)//选择对弈方，暂已停用
		{
			int x = msg.x;
			int y = msg.y;
			//flag_side = choose_side(x, y);
			if (x >= 600 && x <= 660 && y >= 120 && y <= 180)//600, 120, 660, 180
			{
				setfillcolor(RGB(255, 239, 213));
				fillrectangle(600, 240, 660, 300);
				flag_side = 2;//AI执白
				break;
			}
			else if (x >= 600 && x <= 660 && y >= 240 && y <= 300)//600, 240, 660, 300
			{
				setfillcolor(RGB(255, 239, 213));
				fillrectangle(600, 120, 660, 180);
				flag_side = 1;//AI执黑
				break;
			}
		}
	}
	if (flag_side == 1)
	{
		int x0 = 60; int y0 = 60;
		int initial_x = 7, initial_y = 1;
		board[initial_y][initial_x] = flag_side;
		setfillcolor(BLACK);
		fillcircle(x0 +  initial_x* 60, y0 + initial_y * 60, 25);
		cnt++;
	}
	while (1)
	{
		MOUSEMSG msg = GetMouseMsg();
		if (msg.mkLButton && flag_side == 2)//人执黑，AI执白
		{
			int finish_loop = 0;
			int x = msg.x;
			int y = msg.y;
			//以下可改写为一个函数
			for (int j = 0; j < edge; j++) {
				for (int i = 0; i < edge; i++) {
					int x0 = 60, y0 = 60;
					if (((x - (x0 + i * 60)) * (x - (x0 + i * 60)) + (y - (y0 + j * 60)) * (y - (y0 + j * 60)) <= 30 * 30) && (board[j][i] == 0)) {
						if (judgement(j, i, 1)) {
							board[j][i] = 1;
							setfillcolor(BLACK);
							fillcircle(x0 + i * 60, y0 + j * 60, 25);
							cnt++;
							//人走后，马上进AI判断
							vector<int> available_list; //合法位置表 

							for (int i = 0; i < 9; i++)
								for (int j = 0; j < 9; j++)
									if (judgement(i, j, flag_side)) {
										available_list.push_back(i * 9 + j);
									}
							if (available_list.size() == 0)
								print_game_over_AI(flag_side);
							else {
								int result = available_list[rand() % available_list.size()];
								best_row = result / 9; best_col = result % 9;
								board[best_row][best_col] = flag_side;
								setfillcolor(WHITE);
								fillcircle(x0 + best_col * 60, y0 + best_row * 60, 25);
								cnt++;
							}
						}
						else {
							print_game_over_human(1);
						}
						finish_loop = 1;
						best_row = 0; best_col = 0;
						break;
					}
				}
				if (finish_loop)
					break;
			}
		}
		else if (msg.mkLButton && flag_side == 1)//人执白，AI执黑
		{
			int x = msg.x;
			int y = msg.y;
			//以下可改写为一个函数
			for (int j = 0; j < edge; j++) {
				int finish_loop = 0;
				for (int i = 0; i < edge; i++) {
					int x0 = 60, y0 = 60;
					if (((x - (x0 + i * 60)) * (x - (x0 + i * 60)) + (y - (y0 + j * 60)) * (y - (y0 + j * 60)) <= 30 * 30) && board[j][i] == 0) {
						if (judgement(j, i, 2)) {
							board[j][i] = 2;
							setfillcolor(WHITE);
							fillcircle(x0 + i * 60, y0 + j * 60, 25);
							cnt++;
							vector<int> available_list; //合法位置表 

							for (int i = 0; i < 9; i++)
								for (int j = 0; j < 9; j++)
									if (judgement(i, j, flag_side)) {
										available_list.push_back(i * 9 + j);
									}
							if (available_list.size() == 0)
								print_game_over_AI(flag_side);
							else {
								int result = available_list[rand() % available_list.size()];
								best_row = result / 9; best_col = result % 9;
								board[best_row][best_col] = flag_side;
								setfillcolor(BLACK);
								fillcircle(x0 + best_col * 60, y0 + best_row * 60, 25);
								cnt++;
							}
						}
						else {
							print_game_over_human(2);
						}
						finish_loop = 1;
						best_row = 0; best_col = 0;
						break;
					}
				}
				if (finish_loop)
					break;
			}
		}
		FlushMouseMsgBuffer();
	}
	system("pause");
	return 0;
}
