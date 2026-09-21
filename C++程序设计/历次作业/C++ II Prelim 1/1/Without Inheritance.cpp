#include <bits/stdc++.h>
using namespace std;

class Teacher {
private:
    string name;
    string title;
    int classHours;

public:
    //Constructor
    Teacher(string n, string t, int hours)
        : name(n), title(t), classHours(hours) {
    }

    double getSalary() const {
        if (title == "教授") {
            return 5000 + classHours * 90;
        }
        else if (title == "副教授") {
            return 4000 + classHours * 80;
        }
        return 0;
    }

    string getTitle() const {
        return title;
    }

    void display() const {
        cout << setw(8) << name << setw(8) << getTitle()
            << setw(8) << classHours << setw(8) << getSalary() << endl;
    }
};

int main() {
    vector<Teacher> teachers;

    int a, b, c, d;
    cout << "依次输入张三，李四，王五，赵六的课时" << endl;
    cin >> a >> b >> c >> d;

    teachers.push_back(Teacher("Zhangsan", "教授", a));
    teachers.push_back(Teacher("Lisi", "副教授", b));
    teachers.push_back(Teacher("Wangwu", "教授", c));
    teachers.push_back(Teacher("Zhaoliu", "副教授", d));

    cout << setw(8) << "姓名" << setw(8) << "职称" << setw(8) << "上课数量" << setw(8) << "月收入" << endl;

    for (Teacher t : teachers) {
        t.display();
    }

    return 0;
}