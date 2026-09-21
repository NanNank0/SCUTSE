#include <bits/stdc++.h>
using namespace std;

//Defined basic class Teacher
class Teacher {
protected:
    string name;
    int classHours;
public:
    //Constructor
    Teacher(string n, int hours) : name(n), classHours(hours) {}
    virtual ~Teacher() {}
    
    virtual double getSalary() const = 0; 
    
    virtual string getTitle() const = 0;
    
    void display() const {
        cout << setw(8) << name << setw(8) << getTitle() << setw(8) << classHours << setw(8) << getSalary() << endl;
    }
};

//Define Professor class
class Professor : public Teacher {
private:
    static const double FIXED_SALARY;
    static const double HOURLY_RATE;
public:
    Professor(string n, int hours) : Teacher(n, hours) {}
    
    double getSalary() const override {
        return FIXED_SALARY + classHours * HOURLY_RATE;
    }
    
    string getTitle() const override {
        return "教授";
    }
};

//Define FuProfessor class
class FuProfessor : public Teacher{
private:
    static const double FIXED_SALARY;
    static const double HOURLY_RATE;
public:
    FuProfessor(string n, int hours) : Teacher(n, hours){}

    double getSalary() const override {
        return FIXED_SALARY + classHours * HOURLY_RATE;
    }

    string getTitle() const override{
        return "副教授";
    }
};

//Initialize
const double Professor::FIXED_SALARY = 5000;
const double Professor::HOURLY_RATE = 90;
const double FuProfessor::FIXED_SALARY = 4000;
const double FuProfessor::HOURLY_RATE = 80;

int main() {
	//Ciallo～ (∠・ω< )⌒★
    //Storing Teachers with array
    vector<Teacher*> teachers;
    
    int a, b, c, d;
    cout << "依次输入张三，李四，王五，赵六的课时" << endl;
    cin >> a >> b >> c >> d;
    teachers.push_back(new Professor("Zhangsan", a));
    teachers.push_back(new FuProfessor("Lisi", b));
    teachers.push_back(new Professor("Wangwu", c));
    teachers.push_back(new FuProfessor("Zhaoliu", d));

    cout << setw(8) << "姓名" << setw(8) << "职称" << setw(8) << "上课数量" << setw(8) << "月收入" << endl;

    for (Teacher* t : teachers) {
        t->display();
        delete t;
    }

	return 0;
}