/*
* SID: 202530551453
* Author: 徐克楠
* Date: 2026.11.11
*/
#pragma once
#include <algorithm>
#include <functional>
#include <memory>
#include <stdexcept>
#include <vector>

class SearchTree {
public:
    struct Node {
        // 单结点高度为 1，空孩子高度为 0
        int key,height=1;
        // 左右孩子的所有权属于当前结点，整棵树销毁时自动递归释放
        std::unique_ptr<Node> left,right;
        explicit Node(int value):key(value){}
    };
private:
    std::unique_ptr<Node> root_;
    // false 使用普通 BST，true 在更新后执行 AVL 平衡修复
    bool avl_;
    std::size_t size_=0;
    // h 方法统一处理空指针，避免更新高度和求平衡因子时重复判空
    static int h(const std::unique_ptr<Node>& p) { return p?p->height:0; }
    // update 方法根据两个孩子的高度重新计算当前结点高度
    static void update(Node* p) { p->height=1+std::max(h(p->left),h(p->right)); }
    // rotateLeft 方法：右孩子 q 上升为新根，原根 p 成为 q 的左孩子
    // q 原来的左子树转移给 p 的右侧，旋转前后中序序列保持不变
    // unique_ptr 不允许复制，通过 std::move 转移结点所有权
    static void rotateLeft(std::unique_ptr<Node>& p) {
        auto q=std::move(p->right); p->right=std::move(q->left); update(p.get());
        q->left=std::move(p); update(q.get()); p=std::move(q);
    }
    // rotateRight 方法与左旋对称，适用于左侧过高的情况
    // 必须先更新下降结点的高度，再更新上升结点的高度
    static void rotateRight(std::unique_ptr<Node>& p) {
        auto q=std::move(p->left); p->left=std::move(q->right); update(p.get());
        q->right=std::move(p); update(q.get()); p=std::move(q);
    }
    // rebalance 方法：在递归返回时自底向上维护高度和 AVL 平衡性
    // 参数使用 unique_ptr 的引用，使旋转能够直接替换父结点持有的子树根
    void rebalance(std::unique_ptr<Node>& p) {
        if (!p) return;
        update(p.get()); if (!avl_) return;
        // 平衡因子 b > 1 表示左侧过高，b < -1 表示右侧过高
        int b=h(p->left)-h(p->right);
        if (b>1) {
            if (h(p->left->left)<h(p->left->right)) rotateLeft(p->left); // LR
            rotateRight(p); // LL，含删除时子树平衡因子为 0 的情形
        } else if (b < -1) {
            if (h(p->right->right)<h(p->right->left)) rotateRight(p->right); // RL
            rotateLeft(p); // RR
        }
    }
    // insertAt 方法：沿 BST 的大小关系寻找空指针，建立新结点
    // 返回 true 表示新增成功，false 表示遇到重复值
    // 每层递归返回前调用 rebalance，普通 BST 仅更新高度
    bool insertAt(std::unique_ptr<Node>& p,int key) {
        if (!p) { p=std::make_unique<Node>(key); return true; }
        if (key==p->key) return false;
        bool changed=key<p->key?insertAt(p->left,key):insertAt(p->right,key);
        rebalance(p); return changed;
    }
    // removeAt 方法：先按大小关系定位，再分零个、一个、两个孩子处理
    // 无左孩子时让右孩子接替，无右孩子时让左孩子接替
    // 两个孩子时用右子树最小值替换当前键，再删除原来的后继结点
    bool removeAt(std::unique_ptr<Node>& p,int key) {
        if (!p) return false;
        bool changed=true;
        if (key<p->key) changed=removeAt(p->left,key);
        else if (key>p->key) changed=removeAt(p->right,key);
        else if (!p->left) { auto next=std::move(p->right); p=std::move(next); }
        else if (!p->right) { auto next=std::move(p->left); p=std::move(next); }
        else {
            // 右子树一路向左即可找到中序后继，它至多只有一个右孩子
            Node* successor=p->right.get();
            while (successor->left) successor=successor->left.get();
            p->key=successor->key; removeAt(p->right,successor->key);
        }
        // AVL 删除可能使多层祖先失衡，不能只在被删结点附近修复一次
        rebalance(p); return changed;
    }
public:
    explicit SearchTree(bool avl=false):avl_(avl){}
    // root 返回只读观察指针供绘图使用，不转移树的所有权
    const Node* root() const { return root_.get(); }
    std::size_t size() const { return size_; }
    // clear 方法释放根指针，其左右子树随所有权链自动销毁
    void clear() { root_.reset(); size_=0; }
    // insert 方法负责输入范围检查，并只在真正新增结点时增加计数
    bool insert(int key) {
        if (key<0||key>100) throw std::invalid_argument("结点值必须在 0 到 100 之间");
        bool changed=insertAt(root_,key); if (changed) ++size_; return changed;
    }
    // remove 方法只在实际删除成功时减少计数，内部删除后继不重复计数
    bool remove(int key) { bool changed=removeAt(root_,key); if(changed) --size_; return changed; }
    // find 方法使用迭代，沿单一路径下降，查找时间为 O(h)
    const Node* find(int key) const {
        auto p=root_.get();
        while (p && p->key!=key) p=key<p->key?p->left.get():p->right.get();
        return p;
    }
    // traverse 方法：0 表示先序，1 表示中序，2 表示后序
    // 三种遍历的差别仅在于访问当前结点发生在两次子树递归的哪个位置
    // vector 只收集遍历结果，树的结点本身仍以指针连接
    std::vector<int> traverse(int order=1) const {
        std::vector<int> values;
        std::function<void(const Node*)> walk=[&](const Node* p) {
            if (!p) return;
            if (order==0) values.push_back(p->key);
            walk(p->left.get()); if (order==1) values.push_back(p->key);
            walk(p->right.get()); if (order==2) values.push_back(p->key);
        }; walk(root_.get()); return values;
    }
    // leaves 方法：左右孩子都为空的结点才是叶子，只有一个孩子不属于叶子
    // 访问所有结点一次，时间为 O(n)，空树返回空结果
    std::vector<int> leaves() const {
        std::vector<int> values;
        std::function<void(const Node*)> walk=[&](const Node* p) {
            if (!p) return;
            if (!p->left && !p->right) values.push_back(p->key);
            walk(p->left.get()); walk(p->right.get());
        }; walk(root_.get()); return values;
    }
};
