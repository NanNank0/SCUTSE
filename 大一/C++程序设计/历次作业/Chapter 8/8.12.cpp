#include <bits/stdc++.h>
using namespace std;

void moveTortoise(int*);//乌龟走法
void moveHare(int*);//兔子走法
void printmap(int, int);//绘制地图方法

int main() {
	//Ciallo～ (∠・ω< )⌒★
	//本题是龟兔赛跑
	srand(time(0));//重置随机数
	int t_pos = 1, r_pos = 1;//初始化坐标
	cout << "BANG !!!!!" << endl << "AND THEY'RE OFF !!!!!" << endl;//表示比赛开始
	//因为每秒需要更新一次赛道
	int current_time = time(0);
	while (true) {
		if (time(0) - current_time >= 1) {
			current_time = time(0);
			moveTortoise(&t_pos);
			moveHare(&r_pos);
			if (t_pos >= 70 && r_pos >= 70) {//出现胜者的三种情况
				cout << "It's a tie";
				break;
			}
			else if (t_pos >= 70) {
				cout << "TORTOISE WINS!!! YAY!!!";
				break;
			}
			else if (r_pos >= 70) {
				cout << "Hare wins. Yuch.";
				break;
			}
			else if (r_pos == t_pos && r_pos != 1) {
				cout << "OUCHH!!!!!!!" << endl;
			}
			else {//否则打印地图
				printmap(t_pos, r_pos);
			}
		}
	}
	return 0;
}

void moveTortoise(int *t_pos) {
	int rand_num = rand() % 10 + 1;
	switch (rand_num) {
		case 1:case 2:case 3:case 4:case 5: //快爬
			*t_pos += 3;
			break;
		case 6:case 7://摔倒
			*t_pos -= 6;
			break;
		default://慢爬 
			*t_pos += 1;
	}
	if (*t_pos < 1) {//修正
		*t_pos = 1;
	}
}

void moveHare(int *r_pos) {
	int rand_num = rand() % 10 + 1;
	switch (rand_num) {
	case 1:case 2://睡觉
		break;
	case 3:case 4://大跳
		*r_pos += 9;
		break;
	case 5://大摔
		*r_pos -= 12;
		break;
	case 6:case 7:case 8://小跳
		*r_pos += 1;
		break;
	default://小摔 
		*r_pos -= 2;
	}
	if (*r_pos < 1) {//修正
		*r_pos = 1;
	}
}

void printmap(int t_pos, int r_pos) {
	for (int i = 1; i <= 70; i++) {
		if (i == t_pos) {
			cout << "T";
		}
		else if (i == r_pos) {
			cout << "H";
		}
		else {
			cout << "□";
		}
	}
	cout << endl;
}