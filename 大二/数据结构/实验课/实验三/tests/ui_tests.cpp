/*
 * SID: 202530551453
 * Author: 徐克楠
 * Date: 2026.11.18
 */
#include "../common/sort_window.h"
#include <iostream>

// 使用实际控件和按钮检查界面反馈，避免只验证算法而遗漏交互流程
int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    styleApp(app);
    try
    {
        SortWindow window(-1, locateData(), {});
        auto* table = window.findChild<QTableWidget*>("timings");
        auto* run = window.findChild<QPushButton*>("run");
        if (!table || !run || table->rowCount() != 21)
        {
            throw std::runtime_error("initial table");
        }
        run->click();
        if (table->rowCount() != 21 ||
            !window.findChild<QPlainTextEdit*>("log")->toPlainText().contains("通过"))
        {
            throw std::runtime_error("run button");
        }
        for (int row = 0; row < 21; ++row)
        {
            for (int column = 2; column < 6; ++column)
            {
                if (table->item(row, column)->text().toDouble() < 0)
                {
                    throw std::runtime_error("negative time");
                }
            }
        }
        std::cout << "PASS: sort GUI initial results, actual Run button, all 21 timing rows and "
                     "verification message\n";
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return 1;
    }
}
