#include <iostream>
#include <vector>
#include <conio.h>
#include <windows.h>
#include <algorithm>
#include <random>
#include <string>

using namespace std;

const int length = 50, height = 50, pass = 12, bird_line = 15, step = 3, block_line_dis = 12, pause_stop_move = 125;
const char bg = char(32), cur_up = char(210), cur_down = char(208), block = char(219);

void setcur(int x, int y)
{
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void stopp(string message, int i) {
	cout << message;
	system("pause >nul");
	if (i == 1)
		exit(0);
}

class objekt {
public:
	int speed_y, y, acceleration_y;

	objekt() : speed_y(0), y(0), acceleration_y(0) {}
	objekt(int speedy, int posy, int accelerationy) : speed_y(speedy), y(posy), acceleration_y(accelerationy) {}
	void _move(int gap_time) { // gap_time in milliseconds
		y = y + ((speed_y / 1000.0) * gap_time) + round(((acceleration_y / 1000000.0) * pow(gap_time, 2)) / 2.0);
		speed_y += round(((acceleration_y / 1000000.0) * gap_time) * 1000.0);
	}
};

void show(vector<vector<char>> v, const objekt& bird) {
	string res = "", horizontal_line(length, char(205));
	int y = round(bird.y / 1000.0);
	char symbol_bird = ' ';
	symbol_bird = bird.speed_y > 0 ? cur_up : cur_down;
	res = res + char(201) + horizontal_line + char(187) + '\n';
	for (int j = height - 1; j > -1; --j) {
		res.push_back(char(186));
		for (int i = 0; i < length; ++i) {
			if (i == bird_line && j == y)
				res.push_back(symbol_bird);
			else
				res.push_back(v[i][j]);
		}
		res = res + char(186) + '\n';
	}
	res = res + char(200) + horizontal_line + char(188);
	cout << res;
}

void move_blocks(vector<vector<char>>& play_place) {
	vector<char> v(height, bg);
	for (int i = 0; i < length - 1; ++i)
		play_place[i] = play_place[i + 1];
	play_place[length - 1] = v;
}

void set_blocks(vector<vector<char>>& play_place, int& level, const int mov) {
	for (int j = 0; j < level; ++j)
		play_place[length - 1][j] = block;

	for (int j = level + pass; j < height; ++j)
		play_place[length - 1][j] = block;

	level += mov;
	if (level < 1)
		level = 1;
	else if (level > height - 1 - pass)
		level = height - pass - 1;
}

int main() {
	mt19937 gen{ random_device()() };
	uniform_int_distribution<int> dist(-15, 15);
	Sleep(2000);
	while (true) {
		vector<vector<char>> play_place(length, vector<char>(height, bg));
		objekt bird(3000, round((height / 2) * 1000), -2000);

		int counter = 0, distance = 0, level = height / 2;
		while (true) {
			setcur(0, 0);
			show(play_place, bird);
			if (++counter > pause_stop_move) {
				move_blocks(play_place);
				counter = 0;
				++distance;
			}
			if (distance > block_line_dis) {
				set_blocks(play_place, level, dist(gen));
				distance = 0;
			}

			if (_kbhit()) {
				_getch();
				bird.speed_y = 6600;
			}
			bird._move(step);

			int y = round(bird.y / 1000.0);
			if ((y <= -1) || (y >= height) || (play_place[bird_line][y] == block)) {
				setcur(length / 2 - 5, height / 2);
				stopp("Game over!", 0);
				break;
			}
		}
	}
}