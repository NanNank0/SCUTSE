/*
* SID: 202530551453
* Author: 徐克楠
* Date: 2026.11.11
*/
#ifndef LAB_DYNAMIC_LIST_H
#define LAB_DYNAMIC_LIST_H
// 算法层不依赖 Qt；new[] 申请连续空间，不能用 vector 代替实验的顺序表
#include <algorithm>
#include <memory>
#include <stdexcept>
#include <utility>

template<class T> class DynamicList {
    // data_ 独占一段连续数组，析构时自动执行 delete[]
    std::unique_ptr<T[]> data_;
    // size_ 记录有效元素数，capacity_ 记录已申请的槽位数
    // 有效元素位于 [0, size_)，其余槽位只作为预留空间
    std::size_t size_ = 0, capacity_ = 4;
public:
    // 构造方法：建立最小容量为 4 的空表
    DynamicList() : data_(new T[4]) {}

    // 拷贝构造：重新申请数组并逐项复制，避免两个表共用同一块内存
    DynamicList(const DynamicList& other) : data_(new T[other.capacity_]),
        size_(other.size_), capacity_(other.capacity_) {
        for (std::size_t i=0; i<size_; ++i) data_[i]=other.data_[i];
    }
    // 赋值运算：先复制到临时表，再交换所有权
    // 临时表复制失败时当前对象保持原状；同时处理 a = a 的自赋值
    DynamicList& operator=(const DynamicList& other) {
        if (this != &other) { DynamicList copy(other); swap(copy); }
        return *this;
    }
    // 交换方法：只交换数组指针和计数，不逐项复制，因此为 O(1)
    void swap(DynamicList& other) noexcept {
        data_.swap(other.data_); std::swap(size_,other.size_); std::swap(capacity_,other.capacity_);
    }
    // init 方法：用新空表替换当前表，旧数组随临时对象销毁而释放
    void init() { DynamicList empty; swap(empty); }

    // 三个状态查询方法均只读取成员变量，时间复杂度为 O(1)
    std::size_t length() const { return size_; }
    std::size_t capacity() const { return capacity_; }
    bool isEmpty() const { return size_ == 0; }
    // resize 方法：调整容量但不改变有效元素数，也不改变元素顺序
    // 参数不得小于当前长度或最小容量，否则无法完整保存原数据
    // 复制成本为 O(n)，构造与销毁数组还与新旧容量有关
    void resize(std::size_t capacity) {
        if (capacity < size_ || capacity < 4) throw std::invalid_argument("容量不得小于长度或 4");
        // 先准备新数组，复制完成后才提交替换，避免提前释放旧数据
        auto next = std::make_unique<T[]>(capacity);
        for (std::size_t i=0; i<size_; ++i) next[i]=data_[i];
        // 交换后 next 持有旧数组，离开方法时自动回收
        data_.swap(next); capacity_=capacity;
    }

    // insert 方法：在 index 位置插入 value，原位置及后面的元素右移
    // 插入允许 index == size_，这一情况等价于向表尾追加
    // 中间插入需要移动元素，自动扩容策略下最坏时间为 O(n)
    void insert(std::size_t index, const T& value) {
        if (index > size_) throw std::out_of_range("插入下标超出范围");
        T copy=value; // value 即使引用表中元素，扩容后也仍然有效
        // 数组已满时容量翻倍，为本次及后续插入预留空间
        if (size_==capacity_) resize(capacity_*2);
        // 必须从后往前移动，否则会覆盖尚未搬移的元素
        for (std::size_t i=size_; i>index; --i) data_[i]=data_[i-1];
        data_[index]=copy; ++size_;
    }
    // append 方法：复用 insert，把当前长度作为插入下标
    // 表尾没有元素需要右移；不扩容时为 O(1)，扩容时为 O(n)
    // 容量按倍数增长，因此连续追加的摊还时间复杂度为 O(1)
    void append(const T& value) {
        insert(size_,value);
    }

    // remove 方法：删除 index 位置的元素，后续元素左移以填补空位
    // 删除下标必须小于长度；空表删除和 index == size_ 均属于越界
    void remove(std::size_t index) {
        if (index>=size_) throw std::out_of_range("删除下标超出范围");
        // 左移必须从前往后执行，保证源元素尚未被覆盖
        for (std::size_t i=index; i+1<size_; ++i) data_[i]=data_[i+1];
        // 长度减一后重置原末尾槽位，及时释放字符串等对象持有的资源
        data_[--size_]=T{};
        // 四分之一时才缩容，避免在临界位置反复扩缩容
        if (capacity_>4 && size_<=capacity_/4) resize(std::max<std::size_t>(4,capacity_/2));
    }
    // find 方法：按下标直接取值，利用连续存储实现 O(1) 访问
    // 返回引用以支持修改，扩容或删除后不应继续持有旧元素的引用
    T& find(std::size_t index) {
        if (index>=size_) throw std::out_of_range("访问下标超出范围");
        return data_[index];
    }
    // const 重载供只读对象使用，禁止通过返回值修改表中元素
    const T& find(std::size_t index) const {
        if (index>=size_) throw std::out_of_range("访问下标超出范围");
        return data_[index];
    }
    // 沿用任务书命名：at(值)查位置，find(位置)取值，与 STL 的 at 命名不同
    // at 方法：顺序比较每个有效元素，返回首次命中的下标，未命中返回 -1
    // 未排序的学号表不能直接使用二分查找，因此这里采用 O(n) 顺序检索
    int at(const T& value) const {
        for (std::size_t i=0; i<size_; ++i) if (data_[i]==value) return static_cast<int>(i);
        return -1;
    }
    // print 方法：按下标顺序遍历，由调用者提供每个元素的输出方式
    // 算法不直接依赖控制台或 Qt，窗口可用回调把结果写入操作记录
    template<class Visitor> void print(Visitor visitor) const {
        for (std::size_t i=0; i<size_; ++i) visitor(i,data_[i]);
    }
};
#endif
