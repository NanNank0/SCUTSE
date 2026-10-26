#include <bits/stdc++.h>
using namespace std;

bool visited(vector<vector<int>>&, int, int);
bool out(int, int);
bool search(vector<vector<int>>&, vector<vector<int>>&, int, int, int);

int dx[8] = { 2, 1, -1, -2, -2, -1, 1, 2 };//马能走的八个方向，在这里声明成全局变量这样哪都能用
int dy[8] = { 1, 2, 2, 1, -1, -2, -2, -1 };

int main() {
	//Ciallo～ (∠・ω< )⌒★
	//本题要求在一个8*8的国际象棋棋盘上用马走完64个格子并判断这条路径是不是闭合的
	vector<vector<int>> chessboard(8, vector<int>(8,0));
	vector<vector<int>> path;
	int step = 0;

    if (search(chessboard, path, 0, 0, 1)) {
        cout << "Path: " << endl;
        for (int i = 0; i < path.size();i++) {
            cout << "(" << path[i][0] << "," << path[i][1] << ") ";
        }
        cout << endl;
        if (path.back()[0] + path.back()[1] == 3) {
            cout << "路径闭合" << endl;
        }
        else {
            cout << "路径不闭合" << endl;
        }
    }
	return 0;
}

bool visited(vector<vector<int>> &chessboard, int x, int y) {//用于判断是否被走过
	if (chessboard[x][y] == 0) {//如果这个位置是0则没走过
		return false;
	}
	return true;
}

bool out(int x, int y) {//用于判断是否走出棋盘
	if (x < 0 || x > 7 || y < 0 || y > 7) {
        return true;
	}
	return false;
}

bool search(vector<vector<int>> &chessboard, vector<vector<int>> &path, int x, int y, int step) {
    chessboard[x][y] = step;
    path.push_back({ x, y });
    if (step == 64) {
        return true;
    }
    for (int i = 0; i < 8; i++) {//分别朝着八个方向出发探索
        int nx = x + dx[i];
        int ny = y + dy[i];
        if (!out(nx, ny)) {
            if (!visited(chessboard, nx, ny)) {//如果这八个方向都不会到达不能去的地方（这里拆成两个if是因为没在visit写越界检查）
                if (search(chessboard, path, nx, ny, step + 1)) {
                    return true;
                }
            }
        }
    }
    chessboard[x][y] = 0;//如果没有找到结果就回溯
    path.pop_back();
    return false;
}
