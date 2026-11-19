/*
 * SID: 202530551453
 * Author: 徐克楠
 * Date: 2026.11.18
 */
#pragma once
#include "graph_io.h"
#include "ui_common.h"
#include <cmath>

// 绘图区只负责显示输入图，不参与搜索、判环和最短路计算
class GraphCanvas : public QWidget
{
    GraphData data;

  public:
    explicit GraphCanvas(const GraphData& d) : data(d)
    {
        setMinimumHeight(210);
    }

    void setGraph(const GraphData& d)
    {
        data = d;
        update();
    }

    // 根据顶点数量计算圆周坐标，再绘制边、箭头、权值和顶点名称
    void paintEvent(QPaintEvent*) override
    {
        QPainter p(this);
        p.setRenderHint(QPainter::Antialiasing);
        p.fillRect(rect(), Qt::white);
        const int n = data.names.size();
        if (!n)
        {
            return;
        }
        std::vector<QPointF> xy;
        double rx = width() * 0.36, ry = height() * 0.34;
        for (int i = 0; i < n; ++i)
        {
            double a = -1.57079632679 + 6.28318530718 * i / n;
            xy.push_back({width() / 2 + rx * std::cos(a), height() / 2 + ry * std::sin(a)});
        }
        for (auto [u, v, w] : data.edges)
        {
            QPointF a = xy[u], b = xy[v];
            p.setPen(QPen(QColor("#7890ae"), 1.8));
            if (u == v)
            {
                p.drawEllipse(a + QPointF(7, -31), 18, 18);
                p.drawText(a + QPointF(25, -37), QString::number(w));
                continue;
            }
            double len = std::hypot(b.x() - a.x(), b.y() - a.y());
            QPointF unit = (b - a) / len;
            auto end = b - unit * 25;
            auto begin = a + unit * 25;
            p.drawLine(begin, end);
            if (data.directed)
            {
                QPointF normal(-unit.y(), unit.x());
                QPolygonF arrow;
                arrow << end << (end - unit * 10 + normal * 5) << (end - unit * 10 - normal * 5);
                p.setBrush(QColor("#7890ae"));
                p.drawPolygon(arrow);
            }
            p.setPen(QColor("#243b57"));
            auto mid = (begin + end) / 2;
            QRectF label(mid.x() - 18, mid.y() - 19, 36, 20);
            p.fillRect(label, Qt::white);
            p.drawText(label, Qt::AlignCenter, QString::number(w));
        }
        for (int u = 0; u < n; ++u)
        {
            p.setPen(QPen(QColor("#2463b9"), 2));
            p.setBrush(QColor("#e5effc"));
            p.drawEllipse(xy[u], 24, 24);
            p.setPen(QColor("#192a40"));
            p.drawText(QRectF(xy[u].x() - 60, xy[u].y() - 10, 120, 20), Qt::AlignCenter,
                       data.names[u]);
        }
    }
};

// 图实验窗口按题号选择算法，共用输入、存储展示和结果核对流程
class GraphWindow : public QWidget
{
    int mode;
    QString dataDir;
    GraphData data;
    QPlainTextEdit *editor, *matrix, *list, *result;
    QComboBox* examples;
    QSpinBox* start;
    GraphCanvas* canvas;

    void load(const QString& name)
    {
        QFile f(dataDir + "/" + name);
        if (!f.open(QIODevice::ReadOnly))
        {
            throw std::runtime_error("读取示例失败");
        }
        editor->setPlainText(QString::fromUtf8(f.readAll()));
        execute();
    }

  public:
    GraphWindow(int which, const QString& dir, const QString& example) : mode(which), dataDir(dir)
    {
        setWindowTitle(QString("实验四 %1")
                           .arg(which == 1   ? "图搜索 BFS / DFS"
                                : which == 2 ? "拓扑排序与判环"
                                             : "最小费用换古筝"));
        resize(1200, 880);
        auto* box = new QVBoxLayout(this);
        title(box, windowTitle(),
              "徐克楠  |  202530551453  |  邻接矩阵与邻接链表独立计算，相互核对结果");
        auto* row = new QHBoxLayout;
        box->addLayout(row);
        examples = new QComboBox;
        examples->addItems({"traversal.txt", "dag.txt", "cycle.txt", "exchange.txt"});
        row->addWidget(examples);
        action(row, "载入示例", "example", this,
               [this]
               {
                   load(examples->currentText());
               });
        action(row, "导入文件", "open", this,
               [this]
               {
                   auto path =
                       QFileDialog::getOpenFileName(this, "选择图数据", dataDir, "Text (*.txt)");
                   if (!path.isEmpty())
                   {
                       QFile f(path);
                       if (!f.open(QIODevice::ReadOnly))
                       {
                           throw std::runtime_error("读取失败");
                       }
                       editor->setPlainText(QString::fromUtf8(f.readAll()));
                       execute();
                   }
               });
        row->addWidget(new QLabel("起点编号"));
        start = new QSpinBox;
        start->setRange(0, 99);
        row->addWidget(start);
        action(row, "执行算法", "run", this,
               [this]
               {
                   execute();
               });
        captureButton(row, this);
        row->addStretch();
        auto* upper = new QHBoxLayout;
        box->addLayout(upper, 2);
        auto* left = new QVBoxLayout;
        upper->addLayout(left, 1);
        left->addWidget(
            new QLabel("输入：首行 n m directed；顶点名；边 u v weight（编号从0开始）"));
        editor = new QPlainTextEdit;
        editor->setObjectName("input");
        left->addWidget(editor);
        canvas = new GraphCanvas(data);
        upper->addWidget(canvas, 2);
        auto* lower = new QHBoxLayout;
        box->addLayout(lower, 2);
        auto panel = [&](const QString& name)
        {
            auto* b = new QVBoxLayout;
            lower->addLayout(b);
            b->addWidget(new QLabel(name));
            auto* view = new QPlainTextEdit;
            view->setReadOnly(true);
            view->setLineWrapMode(QPlainTextEdit::NoWrap);
            view->setFont(QFont("Consolas", 10));
            b->addWidget(view);
            return view;
        };
        matrix = panel("邻接矩阵（·表示无边，0为零费用边）");
        list = panel("邻接链表（括号为边权）");
        result = new QPlainTextEdit;
        result->setObjectName("log");
        result->setReadOnly(true);
        result->setMinimumHeight(200);
        box->addWidget(result, 1);
        examples->setCurrentText(example);
        load(example);
    }

    // 每次执行都重新解析输入，并分别构建两种图表示
    void execute()
    {
        data = parseGraph(editor->toPlainText());
        if (start->value() >= data.names.size())
        {
            throw std::runtime_error("起点编号超出顶点范围");
        }
        matrix->setPlainText(matrixText(data));
        list->setPlainText(listText(data));
        canvas->setGraph(data);
        auto m = data.build<MatrixGraph>();
        auto l = data.build<ListGraph>();
        QString text;
        // 第一题同时运行 BFS 和 DFS，并核对矩阵与链表的完整遍历结果
        if (mode == 1)
        {
            auto bm = bfs(m, start->value()), bl = bfs(l, start->value()),
                 dm = dfs(m, start->value()), dl = dfs(l, start->value());
            if (bm != bl || dm != dl)
            {
                throw std::runtime_error("两种存储结果不一致");
            }
            text = "邻接矩阵 BFS：" + namesFor(data, bm) + "\n邻接链表 BFS：" + namesFor(data, bl) +
                   "\n邻接矩阵 DFS：" + namesFor(data, dm) + "\n邻接链表 DFS：" +
                   namesFor(data, dl) +
                   "\n验证通过：按编号递增扫描邻居；起点所在分量完成后继续遍历其他分量。";
        }
        else if (mode == 2)
        {
            // 第二题要求有向图，使用输出顶点数量判断是否存在有向环
            if (!data.directed)
            {
                throw std::runtime_error("拓扑排序要求有向图");
            }
            auto a = topological(m), b = topological(l);
            if (a.order != b.order || a.acyclic != b.acyclic)
            {
                throw std::runtime_error("两种存储结果不一致");
            }
            text = "邻接矩阵：" +
                   QString(a.acyclic ? "无环，拓扑序：" : "存在环，已输出的非完整序列：") +
                   namesFor(data, a.order) + "\n邻接链表：" +
                   QString(b.acyclic ? "无环，拓扑序：" : "存在环，已输出的非完整序列：") +
                   namesFor(data, b.order) +
                   QString("\n已输出 %1 / %2 个顶点。判环依据：输出顶点数是否等于总顶点数。")
                       .arg(a.order.size())
                       .arg(data.names.size());
        }
        else
        {
            int s = start->value(), target = data.names.size() - 1;
            auto a = dijkstra(m, s), b = dijkstra(l, s);
            if (a.distance != b.distance)
            {
                throw std::runtime_error("两种存储结果不一致");
            }
            // 目标不可达时只显示提示，不构造不存在的交换路径
            if (a.distance[target] == INF)
            {
                text = "终点不可达，两种存储结果一致。";
            }
            else
            {
                text = "邻接矩阵最短路径：" + namesFor(data, shortestPath(a, target)) +
                       "\n邻接链表最短路径：" + namesFor(data, shortestPath(b, target)) +
                       QString("\n最小补款：%1 元\n各顶点最短费用：").arg(a.distance[target]);
                for (int u = 0; u < m.size(); ++u)
                {
                    text += data.names[u] + "=" +
                            (a.distance[u] == INF ? "不可达" : QString::number(a.distance[u])) +
                            "  ";
                }
                text += "\n边的方向表示用当前物品换下一件物品；边权是额外支付的钱。";
            }
        }
        result->setPlainText(text);
    }
};

// 创建应用与窗口，并按需导出算法结果或保存实际窗口截图
inline int runGraphApp(int argc, char** argv, int mode)
{
    QApplication app(argc, argv);
    styleApp(app);
    auto args = app.arguments();
    auto value = [&](const QString& flag, const QString& fallback)
    {
        int i = args.indexOf(flag);
        return i >= 0 && i + 1 < args.size() ? args[i + 1] : fallback;
    };
    try
    {
        GraphWindow w(mode, value("--data", locateData()),
                      value("--example", mode == 1   ? "traversal.txt"
                                         : mode == 2 ? "dag.txt"
                                                     : "exchange.txt"));
        w.show();
        int exportIndex = args.indexOf("--export");
        if (exportIndex >= 0 && exportIndex + 1 < args.size())
        {
            QFile file(args[exportIndex + 1]);
            if (!file.open(QIODevice::WriteOnly))
            {
                throw std::runtime_error("导出失败");
            }
            QTextStream out(&file);
            out << w.findChild<QPlainTextEdit*>("log")->toPlainText() << '\n';
        }
        setupCapture(app, w);
        return app.exec();
    }
    catch (const std::exception& e)
    {
        qCritical() << e.what();
        return 1;
    }
}
