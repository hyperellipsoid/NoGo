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
int row = 0, col = 0, best_row, best_col, best_value = MININT, value = 0;
void print_board()
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
	outtextxy(632, 423, L"RETURN");
	setcolor(RGB(0, 0, 0));
	rectangle(600, 420, 720, 440);
	outtextxy(643, 483, L"SAVE");
	setcolor(RGB(0, 0, 0));
	rectangle(600, 480, 720, 500);
	return;
}
void print_menu()
{
	initgraph(800, 600); //初始化图形系统
	setbkcolor(RGB(255, 239, 213)); //设置图形窗口的背景颜色
	cleardevice(); //清屏幕
	setcolor(RGB(0, 0, 0));
	rectangle(60, 60, 300, 120);
	rectangle(60, 180, 300, 240);
	rectangle(60, 300, 300, 360);
	rectangle(60, 420, 300, 480);
	settextstyle(30, 0, _T("Consolas"));
	outtextxy(145, 193, _T("BEGIN"));
	while (1)
	{
		MOUSEMSG msg = GetMouseMsg();
		if (msg.mkLButton) {
			int x = msg.x;
			int y = msg.y;
			if (x >= 60 && x <= 300 && y >= 180 && y <= 240)
			{
				break;
			}
		}
	}
	return;
}
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
	if (board[r][c] == 1 || board[r][c] == 2)
		return 0;
	board[r][c] = flag_side;
	memset(check_board, 0, sizeof(check_board));
	//check_board[r][c] = 1;
	if (judge(r, c) == 0) {
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
int nega_max(int depth, int flag_side)//flag_side==1表示AI为黑方，flagSide==2表示AI为白方
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
}
int judge_eye(int r,int c,int flag_side)//返回1表示优先下此处
{
	int Qi = 4;
	if (check(r, c)) {
		for (int i = 0; i < 4; i++) {
			int r1 = r + MOVE[i][0];
			int c1 = c + MOVE[i][1];
			if (check(r1, c1)) {
				if (board[r1][c1] != 0) {
					Qi--;
				}
			}
			else {
				Qi--;
			}
		}
	}
	if (Qi == 1)
		return 1;
	else
		return 0;
}
int eye_maker(int r, int c, int flag_side)
{
	return 0;
}
void choose_side()
{
	while (1) {//选择对弈方
		MOUSEMSG msg = GetMouseMsg();
		if (msg.mkLButton)
		{
			int x = msg.x;
			int y = msg.y;
			//flag_side = choose_side(x, y);
			if (x >= 600 && x <= 660 && y >= 120 && y <= 180)//600, 120, 660, 180
			{
				setfillcolor(RGB(255, 239, 213));
				fillrectangle(600, 240, 660, 300);
				flag_side = 2;//AI执白
				return;
			}
			else if (x >= 600 && x <= 660 && y >= 240 && y <= 300)//600, 240, 660, 300
			{
				setfillcolor(RGB(255, 239, 213));
				fillrectangle(600, 120, 660, 180);
				flag_side = 1;//AI执黑
				return;
			}
			else if (x >= 600 && x <= 720 && y >= 420 && y <= 440) {
				print_menu();
			}
		}
		FlushMouseMsgBuffer();
	}//选择对弈方
}
void stop()
{
	while (1)
	{
		MOUSEMSG msg = GetMouseMsg();
		if (msg.mkLButton) {
			int x = msg.x;
			int y = msg.y;
			if (x >= 600 && x <= 720 && y >= 420 && y <= 440) {
				break;
			}
		}
	}
	return;
}
void first_move_AI(int flag_side)
{
	if (flag_side == 1)//人选白棋时AI的第一手棋
	{
		int x0 = 60; int y0 = 60;
		int initial_x = 7, initial_y = 1;
		board[initial_y][initial_x] = flag_side;
		setfillcolor(BLACK);
		fillcircle(x0 + initial_x * 60, y0 + initial_y * 60, 25);
		cnt++;
	}//人选白棋时AI的第一手棋
	return;
}
//以下图形界面相关

//x横，y竖
IMAGE white;
IMAGE black;
IMAGE cundang;
int main()
{
	while (1) {
		print_menu();
		print_board();
		//下面代码鼠标功能
		//int cnt=0;//用来统计总共走了多少步
		choose_side();
		first_move_AI(flag_side);
		//结束之后无法再次begin,可以通过将落子，AI判断那一块写成函数解决
		//未落子情况下无法return
		int flag_game_over = 0;
		while (1)
		{
			MOUSEMSG msg = GetMouseMsg();
			if (msg.mkLButton && flag_side == 2)//人执黑，AI执白
			{
				int finish_loop = 0;//用来让游戏结束
				int x = msg.x;
				int y = msg.y;
				if (x >= 600 && x <= 720 && y >= 420 && y <= 440) {
					break;
				}
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
								int suspension = 0;
								for (int i = 0; i < 9; i++) {
									for (int j = 0; j < 9; j++) {
										if (judgement(i, j, flag_side)) {
											if (!judge_eye(i, j, flag_side)) {
												available_list.push_back(i * 9 + j);
											}
											else {
												suspension = 1;
												board[i][j] = flag_side;
												setfillcolor(WHITE);
												fillcircle(x0 + j * 60, y0 + i * 60, 25);
												cnt++;
												break;
											}
										}
									}
									if (suspension)
										break;
								}
								if (suspension == 0) {
									if (available_list.size() == 0) {//若AI没位置下，判AI输
										print_game_over_AI(flag_side);
										flag_game_over = 1;
									}
									else {
										int result = available_list[rand() % available_list.size()];
										best_row = result / 9; best_col = result % 9;
										board[best_row][best_col] = flag_side;
										setfillcolor(WHITE);
										fillcircle(x0 + best_col * 60, y0 + best_row * 60, 25);
										cnt++;
									}
								}
								suspension = 0;
							}
							else {
								print_game_over_human(1);
								flag_game_over = 1;
							}
							finish_loop = 1;
							best_row = 0; best_col = 0;
							break;
						}
					}
					if (finish_loop)
						break;
				}
				if (flag_game_over)
					break;
			}
			else if (msg.mkLButton && flag_side == 1)//人执白，AI执黑
			{
				int flag_game_over = 0;//用来让游戏结束
				int x = msg.x;
				int y = msg.y;
				if (x >= 600 && x <= 720 && y >= 420 && y <= 440) {
					break;
				}
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
								int suspension = 0;
								for (int i = 0; i < 9; i++) {
									for (int j = 0; j < 9; j++) {
										if (judgement(i, j, flag_side)) {
											if (!judge_eye(i, j, flag_side)) {
												available_list.push_back(i * 9 + j);
											}
											else {
												suspension = 1;
												board[i][j] = flag_side;
												setfillcolor(BLACK);
												fillcircle(x0 + j * 60, y0 + i * 60, 25);
												cnt++;
												break;
											}
										}
									}
									if (suspension)
										break;
								}
								if (suspension == 0) {
									if (available_list.size() == 0) {
										print_game_over_AI(flag_side);
										flag_game_over = 1;
									}
									else {
										int result = available_list[rand() % available_list.size()];
										best_row = result / 9; best_col = result % 9;
										board[best_row][best_col] = flag_side;
										setfillcolor(BLACK);
										fillcircle(x0 + best_col * 60, y0 + best_row * 60, 25);
										cnt++;
									}
								}
								suspension = 0;
							}
							else {
								print_game_over_human(2);
								flag_game_over = 1;
							}
							finish_loop = 1;
							best_row = 0; best_col = 0;
							break;
						}
					}
					if (finish_loop)
						break;
				}
				if (flag_game_over)
					break;
			}
			FlushMouseMsgBuffer();
		}
		if (flag_game_over) {
			stop();
		}
		for (int i = 0; i < edge; i++) {
			for (int j = 0; j < edge; j++) {
				board[i][j] = 0;
			}
		}
		cnt = 0; flag_side = 0; flag_game_over = 0;
		row = 0, col = 0, best_row, best_col, best_value = MININT, value = 0;
	}
	return 0;
}
