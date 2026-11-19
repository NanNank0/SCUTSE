/*
* SID: 202530551453
* Author: 徐克楠
* Date: 2026.11.18
*/
#pragma once
// Qt 界面公共帮助函数
#include <QtWidgets>
#include <functional>
#include <exception>

// action 方法：统一创建按钮、设置对象名并连接点击事件
// owner 作为连接上下文，窗口销毁时连接自动失效，避免访问已释放的窗口
// 按钮事件在统一入口捕获异常，将失败原因写入结果区
inline QPushButton* action(QHBoxLayout* row, const QString& text, const QString& name,
                           QWidget* owner, std::function<void()> fn)
{
    auto* b = new QPushButton(text, owner);
    b->setObjectName(name);
    row->addWidget(b);
    // clicked 是信号；lambda 是点击后执行的槽。context owner 管理连接生命周期
    QObject::connect(b, &QPushButton::clicked, owner,
                     [owner, fn]
                     {
                         try
                         {
                             fn();
                         }
                         catch (const std::exception& e)
                         {
                             auto* log = owner->findChild<QPlainTextEdit*>("log");
                             if (log)
                             {
                                 log->appendPlainText("操作未完成：" + QString::fromUtf8(e.what()));
                             }
                         }
                     });
    return b;
}

// makeLog 方法：创建只读操作记录区，限制记录数量以控制长时间运行的占用
inline QPlainTextEdit* makeLog(QVBoxLayout* layout)
{
    auto* log = new QPlainTextEdit;
    log->setObjectName("log");
    log->setReadOnly(true);
    log->setMaximumBlockCount(500);
    log->setMaximumHeight(150);
    layout->addWidget(log);
    return log;
}

// styleApp 方法：统一字体、颜色和控件样式，不参与数据结构运算
// 统一设置中文字体和控件外观，让两个实验的界面风格保持一致
inline void styleApp(QApplication& app)
{
#ifdef Q_OS_WIN
    // offscreen 测试平台不枚举 Windows 字体，显式注册也让测试截图正确显示中文
    const auto fontDir = qEnvironmentVariable("WINDIR", "C:/Windows") + "/Fonts/";
    QFontDatabase::addApplicationFont(fontDir + "msyh.ttc");
    QFontDatabase::addApplicationFont(fontDir + "segoeui.ttf");
#endif
    app.setStyle("Fusion");
    app.setFont(QFont("Microsoft YaHei UI", 10));
    app.setStyleSheet(
        "QWidget { background:#f4f7fc; color:#192a40; }"
        "QLineEdit,QSpinBox,QDoubleSpinBox,QTableWidget,QPlainTextEdit,QGraphicsView {"
        " background:white; border:1px solid #cbd5e1; border-radius:5px; padding:5px; }"
        "QPushButton { background:#2463b9; color:white; border:0; border-radius:5px; padding:8px "
        "13px; }"
        "QPushButton:hover { background:#174a8f; }"
        "QHeaderView::section { background:#e2eaf6; padding:7px; border:0; }"
        "QLabel#title { font-size:23px; font-weight:600; }");
}

inline void title(QVBoxLayout* box, const QString& heading, const QString& subtitle)
{
    auto* h = new QLabel(heading);
    h->setObjectName("title");
    box->addWidget(h);
    auto* sub = new QLabel(subtitle);
    sub->setWordWrap(true);
    box->addWidget(sub);
}

// captureButton 方法：允许用户选择路径，将当前窗口保存为 PNG
inline void captureButton(QHBoxLayout* row, QWidget* window)
{
    action(row, "保存界面截图", "capture", window,
           [window]
           {
               const auto path =
                   QFileDialog::getSaveFileName(window, "保存截图", {}, "PNG 图片 (*.png)");
               if (!path.isEmpty() && !window->grab().save(path))
               {
                   throw std::runtime_error("截图保存失败");
               }
           });
}

// setupCapture 方法：仅在传入 --capture 时自动保存窗口，供结果截图验证使用
// 延迟截图以等待窗口完成布局和绘制，保存后自动退出
inline void setupCapture(QApplication& app, QWidget& window)
{
    const auto args = app.arguments();
    const int i = args.indexOf("--capture");
    if (i >= 0 && i + 1 < args.size())
    {
        QTimer::singleShot(350, &window,
                           [&, path = args[i + 1]]
                           {
                               app.exit(window.grab().save(path) ? 0 : 2);
                           });
    }
}

// cell 方法：统一生成居中显示的只读表格项
inline QTableWidgetItem* cell(const QString& text)
{
    auto* item = new QTableWidgetItem(text);
    item->setTextAlignment(Qt::AlignCenter);
    return item;
}

// 支持从源码根目录、构建目录或可执行文件目录启动
// 依次尝试常见启动目录，便于在 Qt Creator 和本地程序中找到测试数据
inline QString locateData()
{
    const QString source = QFileInfo(QString::fromUtf8(__FILE__)).absolutePath() + "/../测试数据";
    for (const auto& path :
         QStringList{QDir::currentPath() + "/测试数据",
                     QCoreApplication::applicationDirPath() + "/测试数据",
                     QCoreApplication::applicationDirPath() + "/../测试数据", source})
    {
        if (QDir(path).exists())
        {
            return QDir(path).absolutePath();
        }
    }
    return source;
}
