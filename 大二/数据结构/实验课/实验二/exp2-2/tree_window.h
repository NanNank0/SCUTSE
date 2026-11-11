/*
* SID: 202530551453
* Author: 徐克楠
* Date: 2026.11.11
*/
#pragma once
#include "search_tree.h"
#include "ui_common.h"
#include <map>
#include <set>
// 树窗口复用同一套界面，通过构造参数决定使用普通 BST 还是 AVL
class TreeWindow:public QWidget {
    SearchTree tree;
    QLineEdit* sequence; QSpinBox* key; QComboBox* samples;
    QGraphicsScene* scene; QGraphicsView* view; QPlainTextEdit* log; QLabel *stats,*orders;
    // 有效键值为 0–100，使用 -1 表示没有需要高亮的结点
    int selected=-1;
    static QString join(const std::vector<int>& values) {QStringList s;for(int k:values)s<<QString::number(k);return s.join("  ");}
    // refresh 方法：重新计算坐标并绘制当前树，随后更新遍历和统计结果
    void refresh() {
        scene->clear(); std::map<const SearchTree::Node*,QPointF> pos;int x=0;
        // 中序排名作为横坐标，递归深度作为纵坐标，保证左右子树顺序正确
        std::function<void(const SearchTree::Node*,int)> locate=[&](const auto* p,int depth){
            if(!p)return;locate(p->left.get(),depth+1);pos[p]={double(60+(x++)*72),double(50+depth*90)};locate(p->right.get(),depth+1);
        };locate(tree.root(),0);
        // 先画边，再画结点。中序下标决定横坐标，因此不同结点不会重叠
        for(auto [p,point]:pos)for(const auto* child:{p->left.get(),p->right.get()})if(child)scene->addLine(QLineF(point,pos[child]),QPen(QColor("#91a5bf"),2));
        for(auto [p,point]:pos) {
            scene->addEllipse(QRectF(point.x()-23,point.y()-23,46,46),QPen(Qt::white,2),QBrush(p->key==selected?QColor("#db7a15"):QColor("#2463b9")));
            auto* text=scene->addText(QString::number(p->key),QFont("Microsoft YaHei UI",11,QFont::Bold));text->setDefaultTextColor(Qt::white);text->setPos(point.x()-text->boundingRect().width()/2,point.y()-text->boundingRect().height()/2);
            auto* height=scene->addText(QString("h=%1").arg(p->height));height->setPos(point.x()-20,point.y()+23);
        }
        if(!tree.root())scene->addText("空树：输入序列后点“构建”，或插入一个结点。");
        // 场景范围包含全部结点，视图按比例缩放，避免深树超出可见区域
        scene->setSceneRect(scene->itemsBoundingRect().adjusted(-25,-20,25,20));
        const auto leaves=tree.leaves();stats->setText(QString("结点数：%1    树高：%2    叶子数：%3    叶子值：%4")
            .arg(tree.size()).arg(tree.root()?tree.root()->height:0).arg(leaves.size()).arg(join(leaves)));
        orders->setText("先序："+join(tree.traverse(0))+"\n中序："+join(tree.traverse(1))+"\n后序："+join(tree.traverse(2)));
        view->fitInView(scene->sceneRect(),Qt::KeepAspectRatio);
    }
public:
    explicit TreeWindow(bool avl):tree(avl) {
        setWindowTitle(avl?"实验二 · AVL 树":"实验二 · 二叉搜索树");resize(1160,850);
        auto* box=new QVBoxLayout(this);title(box,avl?"AVL 平衡二叉搜索树":"二叉搜索树 BST","结点值范围为 0–100，橙色表示当前查找或插入的结点");
        auto* row=new QHBoxLayout;box->addLayout(row);
        samples=new QComboBox;samples->setObjectName("samples");samples->addItems({"序列 A · 较均衡","序列 B · 递增","序列 C · 混合"});row->addWidget(samples);
        sequence=new QLineEdit;sequence->setObjectName("sequence");row->addWidget(sequence,1);
        QStringList sets={"50 30 70 20 40 60 80 10 25 65","10 20 30 40 50 60 70 80 90","60 20 80 10 40 70 90 30 50 35"};
        sequence->setText(sets[0]);connect(samples,&QComboBox::currentIndexChanged,this,[=](int i){sequence->setText(sets[i]);});
        action(row,"构建（至少 8 个）","build",this,[=]{buildSequence();});
        auto* controls=new QHBoxLayout;box->addLayout(controls);key=new QSpinBox;key->setObjectName("key");key->setRange(0,100);key->setValue(55);controls->addWidget(new QLabel("结点值 0–100"));controls->addWidget(key);
        // 插入与删除均保存操作前的中序序列，满足实验的前后结果对照要求
        action(controls,"插入 Insert","insert",this,[=]{auto before=join(tree.traverse());bool ok=tree.insert(key->value());selected=key->value();refresh();log->appendPlainText(QString("Insert %1：%2\n操作前中序：%3\n操作后中序：%4").arg(key->value()).arg(ok?"成功":"重复值，不重复插入").arg(before,join(tree.traverse())));});
        // 查找只改变高亮状态，不修改树的结构
        action(controls,"查找 Find","find",this,[=]{selected=tree.find(key->value())?key->value():-1;refresh();log->appendPlainText(selected<0?"Find：未找到":"Find：已找到，橙色结点为查询结果");});
        action(controls,"删除 Remove","remove",this,[=]{auto before=join(tree.traverse());bool ok=tree.remove(key->value());selected=-1;refresh();log->appendPlainText(QString("Remove %1：%2\n操作前中序：%3\n操作后中序：%4").arg(key->value()).arg(ok?"成功":"值不存在").arg(before,join(tree.traverse())));});
        action(controls,"清空","clear",this,[=]{tree.clear();selected=-1;refresh();log->appendPlainText("树已清空");});
        captureButton(controls,this);
        stats=new QLabel;stats->setObjectName("stats");stats->setWordWrap(true);box->addWidget(stats);
        scene=new QGraphicsScene(this);view=new QGraphicsView(scene);view->setRenderHint(QPainter::Antialiasing);view->setObjectName("treeView");box->addWidget(view,1);
        auto* zoom=new QHBoxLayout;box->addLayout(zoom);
        action(zoom,"适应窗口","fit",this,[=]{view->fitInView(scene->sceneRect(),Qt::KeepAspectRatio);});
        action(zoom,"放大","zoomIn",this,[=]{view->scale(1.2,1.2);});action(zoom,"缩小","zoomOut",this,[=]{view->scale(1/1.2,1/1.2);});zoom->addStretch();
        orders=new QLabel;orders->setObjectName("orders");orders->setWordWrap(true);orders->setTextInteractionFlags(Qt::TextSelectableByMouse);box->addWidget(orders);
        log=makeLog(box);buildSequence();
        QTimer::singleShot(0,this,[=]{view->fitInView(scene->sceneRect(),Qt::KeepAspectRatio);});
    }
    // buildSequence 方法：先验证整组输入，再清空旧树并按输入顺序逐一插入
    void buildSequence() {
        auto tokens=sequence->text().split(QRegularExpression("[,，;；\\s]+"),Qt::SkipEmptyParts);
        if(tokens.size()<8||tokens.size()>101)throw std::invalid_argument("构建时需输入 8 到 101 个不同整数");
        // 保留模式、整批验证后替换：输入错误时旧树仍在
        // set 只用于检查输入是否重复，实际树结点仍由 SearchTree 的指针结构保存
        std::vector<int> values;std::set<int> seen;
        for(auto s:tokens){bool ok=false;int k=s.toInt(&ok);if(!ok||k<0||k>100||!seen.insert(k).second)throw std::invalid_argument("结点必须是 0 到 100 的不重复整数");values.push_back(k);}
        // 所有输入验证通过后再替换，避免输入错误时破坏原树
        tree.clear();for(int k:values)tree.insert(k);selected=-1;view->resetTransform();refresh();
        log->appendPlainText("已构建："+sequence->text()+"\n中序（递增）："+join(tree.traverse()));
    }
};
