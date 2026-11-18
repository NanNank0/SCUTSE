/*
 * SID: 202530551453
 * Author: 徐克楠
 * Date: 2026.11.18
 */
#pragma once
#include "benchmark.h"
#include "ui_common.h"

// 排序窗口负责用户输入和结果展示，具体排序仍由独立算法函数完成
class SortWindow : public QWidget
{
    int selected;
    QString dataDir;
    std::vector<Timing> timings;
    QTableWidget* table;
    QPlainTextEdit* details;

  public:
    SortWindow(int which, const QString& dir, const QString& saved) : selected(which), dataDir(dir)
    {
        setWindowTitle(which < 0 ? "实验三 排序算法比较"
                                 : QString("实验三 %1").arg(sortAlgorithms[which].chinese));
        resize(which < 0 ? 1160 : 1060, which < 0 ? 980 : 500);
        auto* box = new QVBoxLayout(this);
        title(box, windowTitle(),
              "徐克楠  |  202530551453  |  相同输入 · 每组3次 · steady_clock 纳秒计时");
        auto* row = new QHBoxLayout;
        box->addLayout(row);
        action(row, "选择数据目录", "data", this,
               [this]
               {
                   auto p =
                       QFileDialog::getExistingDirectory(this, "选择含三组数据的目录", dataDir);
                   if (!p.isEmpty())
                   {
                       dataDir = p;
                   }
               });
        action(row, "运行三组测试", "run", this,
               [this]
               {
                   timings = benchmark(dataDir, selected);
                   refresh();
               });
        action(row, "导出 CSV", "export", this,
               [this]
               {
                   auto p = QFileDialog::getExistingDirectory(this, "导出目录");
                   if (!p.isEmpty())
                   {
                       saveTimings(p, timings);
                   }
               });
        captureButton(row, this);
        row->addStretch();
        table = new QTableWidget;
        table->setObjectName("timings");
        table->setColumnCount(6);
        table->setHorizontalHeaderLabels(
            {"算法", "规模 n", "第1次 / ns", "第2次 / ns", "第3次 / ns", "平均 / ns"});
        table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        table->setEditTriggers(QAbstractItemView::NoEditTriggers);
        table->verticalHeader()->hide();
        box->addWidget(table, 3);
        details = new QPlainTextEdit;
        details->setObjectName("log");
        details->setReadOnly(true);
        details->setMinimumHeight(170);
        box->addWidget(details, 1);
        timings = saved.isEmpty() ? benchmark(dataDir, selected) : loadTimings(saved);
        refresh();
    }

    // 刷新只读表格与序列预览，不把界面绘制计入排序时间
    void refresh()
    {
        table->setRowCount(0);
        for (const auto& t : timings)
        {
            if (selected < 0 || t.algorithm == selected)
            {
                int r = table->rowCount();
                table->insertRow(r);
                QStringList values{
                    sortAlgorithms[t.algorithm].name, QString::number(t.n),
                    QString::number(t.ns[0]),         QString::number(t.ns[1]),
                    QString::number(t.ns[2]),         QString::number(t.average(), 'f', 2)};
                for (int c = 0; c < 6; ++c)
                {
                    table->setItem(r, c, cell(values[c]));
                }
            }
        }
        table->resizeRowsToContents();
        auto a = readNumbers(dataDir + "/100_data.txt"), b = a;
        sortAlgorithms[selected < 0 ? 6 : selected].sort(b);
        details->setPlainText(
            "输入文件：" + dataDir + "\n100个数的原始序列（前24项）：\n" + preview(a) +
            "\n排序后（前24项）：\n" + preview(b) +
            "\n每次结果均与 std::sort "
            "完整序列比较：通过。复制、校验、界面刷新均不计时；算法内部临时空间分配计时。");
    }
};

// 统一处理窗口启动、命令行计时和截图参数
inline int runSortApp(int argc, char** argv, int which)
{
    QApplication app(argc, argv);
    styleApp(app);
    auto args = app.arguments();
    auto value = [&](const QString& flag, const QString& fallback = QString())
    {
        int i = args.indexOf(flag);
        return i >= 0 && i + 1 < args.size() ? args[i + 1] : fallback;
    };
    try
    {
        QString data = value("--data", locateData());
        if (args.contains("--generate"))
        {
            generateData(data);
        }
        if (args.contains("--benchmark"))
        {
            saveTimings(value("--out", "运行结果"), benchmark(data, which));
            return 0;
        }
        SortWindow w(which, data, value("--results"));
        w.show();
        setupCapture(app, w);
        return app.exec();
    }
    catch (const std::exception& e)
    {
        qCritical() << e.what();
        return 1;
    }
}
