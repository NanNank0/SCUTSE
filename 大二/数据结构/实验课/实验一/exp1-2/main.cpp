/*
* SID: 202530551453
* Author: 徐克楠
* Date: 2026.11.11
*/
#include "grade_book.h"
#include "ui_common.h"
// 学生成绩管理窗口，数据校验与查询排序统一交给 GradeBook
class GradeWindow:public QWidget {
    GradeBook book;
    QLineEdit *id,*name,*course,*queryStudent,*queryCourse;
    QDoubleSpinBox* score; QTableWidget *table,*students; QPlainTextEdit* log;
    // refresh 方法：先显示学生基本信息，再按筛选条件取得降序成绩表
    void refresh() {
        students->setRowCount(static_cast<int>(book.students().length()));
        for(std::size_t i=0;i<book.students().length();++i) {
            const auto& s=book.students().find(i);students->setItem(i,0,cell(QString::fromStdString(s.id)));students->setItem(i,1,cell(QString::fromStdString(s.name)));
        }
        // 查询结果是独立顺序表，因此显示时不会重排原始成绩记录
        auto rows=book.query(queryStudent->text().trimmed().toStdString(),queryCourse->text().trimmed().toStdString());
        table->setRowCount(static_cast<int>(rows.length()));
        for(std::size_t i=0;i<rows.length();++i) {
            const auto& g=rows.find(i);table->setItem(i,0,cell(QString::fromStdString(g.id)));
            table->setItem(i,1,cell(QString::fromStdString(book.student(g.id)->name)));
            table->setItem(i,2,cell(QString::fromStdString(g.course)));table->setItem(i,3,cell(QString::number(g.score,'f',1)));
        }
    }
    static QLineEdit* edit(QFormLayout* f,const QString& text,const QString& object,const QString& initial={}) {
        auto* e=new QLineEdit(initial);e->setObjectName(object);f->addRow(text,e);return e;
    }
public:
    GradeWindow() {
        setWindowTitle("实验一 · 学生成绩管理"); resize(1120,780);
        auto* box=new QVBoxLayout(this);title(box,"学生成绩管理","支持学生信息与课程成绩管理，查询结果按分数降序显示");
        auto* form=new QFormLayout;box->addLayout(form);
        id=edit(form,"学号","id","202530550001");name=edit(form,"姓名","name","示例学生01");course=edit(form,"课程","course","数据结构");
        score=new QDoubleSpinBox;score->setObjectName("score");score->setRange(0,100);score->setDecimals(1);score->setValue(90);form->addRow("分数",score);
        auto* row=new QHBoxLayout;box->addLayout(row);
        // 业务层抛出的非空、重复等异常由公共 action 函数捕获并显示
        action(row,"添加学生","addStudent",this,[=]{book.addStudent(id->text().trimmed().toStdString(),name->text().trimmed().toStdString());refresh();log->appendPlainText("已添加学生");});
        action(row,"添加课程成绩","addGrade",this,[=]{book.addGrade(id->text().trimmed().toStdString(),course->text().trimmed().toStdString(),score->value());refresh();log->appendPlainText("已添加成绩");});
        action(row,"删除课程成绩","removeGrade",this,[=]{book.removeGrade(id->text().trimmed().toStdString(),course->text().trimmed().toStdString());refresh();log->appendPlainText("已删除成绩");});
        captureButton(row,this);
        auto* filters=new QHBoxLayout;box->addLayout(filters);
        queryStudent=new QLineEdit;queryStudent->setObjectName("queryStudent");queryStudent->setPlaceholderText("学号（精确）或姓名（包含）；留空显示全部");filters->addWidget(queryStudent);
        queryCourse=new QLineEdit;queryCourse->setObjectName("queryCourse");queryCourse->setPlaceholderText("课程名（精确）；留空显示所有课程");filters->addWidget(queryCourse);
        action(filters,"查找成绩","query",this,[=]{refresh();log->appendPlainText(QString("查找到 %1 条成绩，已按分数降序排列").arg(table->rowCount()));});
        action(filters,"显示全部","all",this,[=]{queryStudent->clear();queryCourse->clear();refresh();});
        auto* split=new QSplitter;box->addWidget(split,1);
        students=new QTableWidget(0,2);students->setObjectName("students");students->setHorizontalHeaderLabels({"学生学号","姓名"});split->addWidget(students);
        table=new QTableWidget(0,4);table->setObjectName("grades");table->setHorizontalHeaderLabels({"学号","姓名","课程","分数 ↓"});split->addWidget(table);
        for(auto* t:{students,table}) {t->setEditTriggers(QAbstractItemView::NoEditTriggers);t->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);t->setSelectionBehavior(QAbstractItemView::SelectRows);}
        // 点击表格行只回填表单，不直接修改数据，避免绕过业务约束
        connect(students,&QTableWidget::cellClicked,this,[=](int r,int){id->setText(students->item(r,0)->text());name->setText(students->item(r,1)->text());});
        connect(table,&QTableWidget::cellClicked,this,[=](int r,int){id->setText(table->item(r,0)->text());name->setText(table->item(r,1)->text());course->setText(table->item(r,2)->text());score->setValue(table->item(r,3)->text().toDouble());});
        auto* files=new QHBoxLayout;box->addLayout(files);
        action(files,"导入 JSON 数据","load",this,[=]{
            auto path=QFileDialog::getOpenFileName(this,"导入学生和成绩",{},"JSON (*.json)");if(!path.isEmpty())loadFile(path);
        });
        action(files,"保存 JSON 数据","save",this,[=]{
            auto path=QFileDialog::getSaveFileName(this,"保存学生和成绩",{},"JSON (*.json)");if(!path.isEmpty())saveFile(path);
        });
        files->addWidget(new QLabel("点击表格行可填入上方表单；修改成绩时先删除该条，再添加。"));
        log=makeLog(box);
        box->addWidget(new QLabel("关闭程序前，请保存学生与成绩数据"));
        // 初始化测试记录，外部 JSON 文件可完整替换这些记录
        for(int i=1;i<=10;++i) {
            auto sid=QString("202530550%1").arg(i,3,10,QChar('0')).toStdString();
            book.addStudent(sid,QString("示例学生%1").arg(i,2,10,QChar('0')).toStdString());
            book.addGrade(sid,"数据结构",70+i*2);book.addGrade(sid,"大学物理",65+i*3);
        }
        refresh();log->appendPlainText("已加载测试数据：10 名学生，20 条课程成绩");
    }
    // 先读入临时对象，全部验证通过后才替换，避免坏文件破坏当前数据
    // loadFile 方法：依次验证 JSON 语法、数组结构、字段类型和数据关联
    void loadFile(const QString& path) {
        QFile f(path);if(!f.open(QIODevice::ReadOnly))throw std::runtime_error("文件无法读取");
        QJsonParseError error;auto doc=QJsonDocument::fromJson(f.readAll(),&error);
        if(error.error!=QJsonParseError::NoError||!doc.isObject())throw std::invalid_argument("JSON 格式错误");
        auto o=doc.object();if(!o["students"].isArray()||!o["grades"].isArray())throw std::invalid_argument("缺少 students 或 grades 数组");
        GradeBook next;
        for(auto v:o["students"].toArray()) {auto s=v.toObject();if(!s["id"].isString()||!s["name"].isString())throw std::invalid_argument("学生字段错误");next.addStudent(s["id"].toString().trimmed().toStdString(),s["name"].toString().trimmed().toStdString());}
        for(auto v:o["grades"].toArray()) {auto g=v.toObject();if(!g["id"].isString()||!g["course"].isString()||!g["score"].isDouble())throw std::invalid_argument("成绩字段错误");next.addGrade(g["id"].toString().trimmed().toStdString(),g["course"].toString().trimmed().toStdString(),g["score"].toDouble());}
        // 所有检查成功后统一提交，失败时保留原有学生、成绩和查询结果
        book=next;queryStudent->clear();queryCourse->clear();refresh();log->appendPlainText("数据已导入："+path);
    }
    // saveFile 方法：分别序列化学生与成绩，QSaveFile 使用临时文件提交
    // 写入或提交失败时抛出异常，避免向用户报告虚假的保存成功
    void saveFile(const QString& path) {
        QJsonArray ss,gg;
        for(std::size_t i=0;i<book.students().length();++i){const auto& s=book.students().find(i);ss.append(QJsonObject{{"id",QString::fromStdString(s.id)},{"name",QString::fromStdString(s.name)}});}
        for(std::size_t i=0;i<book.grades().length();++i){const auto& g=book.grades().find(i);gg.append(QJsonObject{{"id",QString::fromStdString(g.id)},{"course",QString::fromStdString(g.course)},{"score",g.score}});}
        QSaveFile f(path);if(!f.open(QIODevice::WriteOnly))throw std::runtime_error("文件无法写入");
        auto data=QJsonDocument(QJsonObject{{"students",ss},{"grades",gg}}).toJson();
        if(f.write(data)!=data.size()||!f.commit())throw std::runtime_error("保存失败");log->appendPlainText("数据已保存："+path);
    }
};
#ifndef LAB_NO_MAIN
// 应用入口：启动 Qt 事件循环并显示成绩管理窗口
int main(int argc,char** argv) {QApplication app(argc,argv);styleApp(app);GradeWindow w;w.show();setupCapture(app,w);return app.exec();}
#endif
