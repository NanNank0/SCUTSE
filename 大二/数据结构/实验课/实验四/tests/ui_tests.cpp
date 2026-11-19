/*
 * SID: 202530551453
 * Author: 徐克楠
 * Date: 2026.11.18
 */
#include "../common/graph_window.h"
#include <iostream>

void require(bool b)
{
    if (!b)
    {
        throw std::runtime_error("GUI verification failed");
    }
}

// 使用实际控件和按钮检查界面反馈，避免只验证算法而遗漏交互流程
int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    styleApp(app);
    try
    {
        QString dir = locateData();
        GraphWindow search(1, dir, "traversal.txt");
        require(search.findChild<QPlainTextEdit*>("log")->toPlainText().contains(
            "A → B → C → D → E → F → G → H"));
        search.findChild<QPushButton*>("run")->click();
        require(search.findChild<QPlainTextEdit*>("log")->toPlainText().contains(
            "A → B → D → G → E → C → F → H"));
        GraphWindow topo(2, dir, "dag.txt");
        auto* result = topo.findChild<QPlainTextEdit*>("log");
        require(result->toPlainText().contains("无环"));
        QFile cycle(dir + "/cycle.txt");
        require(cycle.open(QIODevice::ReadOnly));
        topo.findChild<QPlainTextEdit*>("input")->setPlainText(QString::fromUtf8(cycle.readAll()));
        topo.findChild<QPushButton*>("run")->click();
        require(result->toPlainText().contains("存在环"));
        topo.findChild<QPlainTextEdit*>("input")->setPlainText("bad input");
        topo.findChild<QPushButton*>("run")->click();
        require(result->toPlainText().contains("操作未完成"));
        GraphWindow exchange(3, dir, "exchange.txt");
        require(
            exchange.findChild<QPlainTextEdit*>("log")->toPlainText().contains("最小补款：300 元"));
        std::cout << "PASS: graph GUI BFS/DFS, Run button, DAG-to-cycle input edit, invalid input "
                     "feedback and 300-yuan exchange\n";
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return 1;
    }
}
