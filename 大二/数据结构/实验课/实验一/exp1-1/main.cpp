/*
* SID: 202530551453
* Author: 徐克楠
* Date: 2026.11.11
*/
#include "dynamic_list.h"
#include "ui_common.h"
#include <string>

// 顺序表主窗口：负责收集输入、调用算法并显示结果
class ListWindow:public QWidget {
    DynamicList<std::string> list;
    QLineEdit* value; QSpinBox *position,*capacity;
    QTableWidget* table; QLabel* stats; QPlainTextEdit* log;
    // refresh 方法：按当前容量重建两行表格，并同步长度、容量和空表状态
    // 表格只负责显示，实际数据始终由 list 保存
    void refresh(const QString& message) {
        table->setColumnCount(static_cast<int>(list.capacity())); table->setRowCount(2);
        table->setVerticalHeaderLabels({"下标（从 0 开始）","学号 / 空闲空间"});
        for(std::size_t i=0;i<list.capacity();++i) {
            table->setItem(0,i,cell(QString::number(i)));
            auto* item=cell(i<list.length()?QString::fromStdString(list.find(i)):"空闲");
            item->setBackground(i<list.length()?QColor("#dbeafe"):QColor("#f1f5f9")); table->setItem(1,i,item);
            table->setColumnWidth(i,138);
        }
        // 插入允许下标等于当前长度，删除和取值的更严格边界由算法层检查
        position->setMaximum(static_cast<int>(list.length()));
        capacity->setValue(static_cast<int>(list.capacity()));
        stats->setText(QString("length = %1    capacity = %2    isEmpty = %3")
            .arg(list.length()).arg(list.capacity()).arg(list.isEmpty()?"true":"false"));
        log->appendPlainText(message);
    }
public:
    ListWindow() {
        setWindowTitle("实验一 · 变长顺序表"); resize(1100,680);
        auto* box=new QVBoxLayout(this);
        title(box,"变长顺序表","下标从 0 开始；蓝色表示有效元素，灰色表示空闲空间");
        auto* row=new QHBoxLayout; box->addLayout(row);
        row->addWidget(new QLabel("学号")); value=new QLineEdit("202530550011"); value->setObjectName("value"); row->addWidget(value);
        row->addWidget(new QLabel("下标")); position=new QSpinBox; position->setObjectName("position"); row->addWidget(position);
        // read 回调统一读取学号，去除首尾空白并拒绝空值
        auto read=[this]{auto s=value->text().trimmed(); if(s.isEmpty()) throw std::invalid_argument("学号不能为空"); return s.toStdString();};
        // 追加按钮：读取学号后调用 append，成功时刷新表格和操作记录
        action(row,"追加 append","append",this,[=]{list.append(read());refresh("追加成功");});
        // 插入按钮：使用下标输入框定位插入位置，元素右移由算法层完成
        action(row,"插入 insert","insert",this,[=]{list.insert(position->value(),read());refresh("插入成功");});
        // 删除按钮：算法可能在删除后自动缩容，refresh 会显示新的容量
        action(row,"删除 remove","remove",this,[=]{list.remove(position->value());refresh("删除成功");});
        auto* row2=new QHBoxLayout; box->addLayout(row2);
        // 两个查询入口分别按值查下标、按下标取值，不修改顺序表
        action(row2,"查位置 at","at",this,[=]{log->appendPlainText(QString("at：下标 %1（-1 表示未找到）").arg(list.at(read())));});
        action(row2,"取元素 find","find",this,[=]{log->appendPlainText("find："+QString::fromStdString(list.find(position->value())));});
        // print 的回调把每个学号加入 QStringList，最后一次性写入结果区
        action(row2,"遍历 print","print",this,[=]{QStringList s; list.print([&](auto,auto& x){s<<QString::fromStdString(x);}); log->appendPlainText("print：["+s.join(", ")+"]");});
        capacity=new QSpinBox; capacity->setRange(4,4096); capacity->setObjectName("capacity"); row2->addWidget(new QLabel("容量")); row2->addWidget(capacity);
        action(row2,"调整 resize","resize",this,[=]{list.resize(capacity->value());refresh("容量调整成功");});
        action(row2,"清空 init","clear",this,[=]{list.init();refresh("顺序表已清空");});
        auto* row3=new QHBoxLayout; box->addLayout(row3);
        action(row3,"载入示例","sample",this,[=]{list.init();for(int i=1;i<=8;++i)list.append("20253055000"+std::to_string(i));refresh("已载入 8 个示例学号（虚构数据）");});
        action(row3,"从文件初始化","load",this,[=]{
            auto path=QFileDialog::getOpenFileName(this,"每行一个学号",{},"文本文件 (*.txt)");if(path.isEmpty())return;
            QFile file(path);if(!file.open(QIODevice::ReadOnly))throw std::runtime_error("无法读取文件");
            // 在临时表中完成读取和数量检查，成功后才交换，防止坏文件清空原表
            DynamicList<std::string> next; const auto lines=QString::fromUtf8(file.readAll()).split(QRegularExpression("\\s+"),Qt::SkipEmptyParts);
            if(lines.size()>1000)throw std::invalid_argument("最多可载入 1000 个学号");
            for(const auto& s:lines)next.append(s.toStdString()); list.swap(next);refresh("已从文件初始化");
        });
        captureButton(row3,this); row3->addStretch();
        stats=new QLabel; stats->setObjectName("stats"); box->addWidget(stats);
        // 禁止直接编辑表格，所有数据修改通过经过检查的算法接口完成
        table=new QTableWidget; table->setObjectName("table"); table->setEditTriggers(QAbstractItemView::NoEditTriggers);
        table->horizontalHeader()->hide(); table->verticalHeader()->setMinimumWidth(160); table->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        box->addWidget(table,1); log=makeLog(box);
        for(int i=1;i<=4;++i)list.append("20253055000"+std::to_string(i));
        refresh("初始化完成：4 个元素，容量为 4");
    }
};
#ifndef LAB_NO_MAIN
// 应用入口：创建事件循环和主窗口，exec 等待并分发用户操作
int main(int argc,char** argv) { QApplication app(argc,argv);styleApp(app);ListWindow w;w.show();setupCapture(app,w);return app.exec(); }
#endif
