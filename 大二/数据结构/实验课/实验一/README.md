# 实验一：顺序表与成绩管理学习说明

本文件解释实验一使用的算法、C++ 和 Qt，配合源码和程序界面阅读。简要的构建要求另见 [运行说明.md](运行说明.md)。

| 目录或文件 | 内容 | 建议阅读顺序 |
|---|---|---|
| [exp1-1/dynamic_list.h](exp1-1/dynamic_list.h) | 可变容量顺序表 | 先理解成员变量，再看 append、insert、resize 和 remove |
| [exp1-1/main.cpp](exp1-1/main.cpp) | 顺序表窗口及按钮回调 | 对照一次追加操作追踪调用过程 |
| [exp1-2/grade_book.h](exp1-2/grade_book.h) | 学生、成绩和查询排序 | 理解顺序表如何用于业务记录 |
| [exp1-2/main.cpp](exp1-2/main.cpp) | 成绩管理窗口及 JSON 读写 | 看输入如何进入业务类、结果如何显示 |
| [exp1-1/ui_common.h](exp1-1/ui_common.h) | 两题采用的公共界面辅助函数 | 看 action 如何连接按钮和处理异常 |
| [测试数据](测试数据/) | 供程序导入和验证的数据 | 配合手工检查使用 |

两个题目各自保留一份 `dynamic_list.h` 和 `ui_common.h`，方便独立构建。所有顺序表下标从 0 开始。

# append方法
对应源码：`exp1-1/dynamic_list.h`，成绩管理中的同名文件采用相同实现。

## 作用和调用方式

`append` 把一个元素加到顺序表末尾，原有元素的顺序保持不变。

```cpp
DynamicList<std::string> list;
list.append("202530551453");
```

这里的 `list` 是顺序表对象，`.` 表示调用这个对象的方法，括号内是传给方法的实际参数。学号使用字符串，不做数值运算。

## 代码与执行过程

```cpp
void append(const T& value) {
    insert(size_, value);
}
```

`size_` 是当前有效元素个数，也恰好是表尾下一个空位的下标。例如当前有 4 项，有效下标是 0、1、2、3，追加就是在下标 4 插入。因此 `append` 可以复用 `insert`，不必再写一次扩容逻辑。

`T` 是元素类型，`const T& value` 表示通过只读引用接收元素，避免调用时额外复制一份参数。方法并没有保存这个引用，而是在 `insert` 内把元素复制进表。

当 `size_ < capacity_` 时，不需要移动已有元素，直接写入新位置；当两者相等时，`insert` 先调用 `resize` 扩容。例如长度和容量都是 4，追加后长度变成 5，容量变成 8。

## 时间复杂度

不扩容时 O(1)，扩容时 O(n)。容量按 4、8、16 等倍数增长，连续追加 n 次时，扩容搬移的总量与 n 同阶，所以每次追加的摊还复杂度为 O(1)。摊还 O(1) 不意味着每一次追加都只需要常数时间。

可以在顺序表窗口点一次“追加”，对照长度和容量，再沿着按钮回调 → `append` → `insert` → `resize` 阅读代码。

# 顺序表成员变量
```cpp
std::unique_ptr<T[]> data_;
std::size_t size_ = 0, capacity_ = 4;
```

`data_` 管理连续数组，`size_` 表示有效元素数，`capacity_` 表示已分配的槽位数。始终应满足 `0 <= size_ <= capacity_`；有效元素在 `[0, size_)` 内。

例如长度为 2、容量为 4 时，可以理解为 `[A, B, 空闲, 空闲]`。空闲槽位已经属于数组，但不属于顺序表的有效内容，遍历和查找不能把它们算进去。

成员变量末尾的下划线是命名约定，用来与局部变量区分，不是特殊语法。

# insert方法
对应源码：`dynamic_list.h` 中的 `insert(std::size_t index, const T& value)`。

## 边界

插入允许 `0 <= index <= size_`。`index == 0` 是头插，`index == size_` 是尾插。这里允许等于长度，与删除、取值不同。

## 执行步骤

1. 检查下标是否合法
2. 把输入元素复制到局部变量 `copy`
3. 若数组已满，将容量翻倍
4. 从末尾向插入位置逐项右移
5. 写入新元素，长度加一

核心循环如下：

```cpp
for (std::size_t i = size_; i > index; --i) {
    data_[i] = data_[i - 1];
}
data_[index] = copy;
++size_;
```

例如 `[A, B, C]` 在下标 1 插入 X，先把 C 搬到下标 3，再把 B 搬到下标 2，最后把 X 写入下标 1，结果是 `[A, X, B, C]`。如果从前向后搬，B 会覆盖 C，导致原数据丢失。

`T copy = value` 还处理了 `list.append(list.find(0))` 这种参数引用表内元素的情况。扩容会释放旧数组，先保存一份值，后续就不会读取失效的引用。

在正常自动扩缩容策略下，中间插入的最坏时间为 O(n)，因为最多要移动 n 项。

# resize方法
`resize` 调整的是容量，不是有效元素数量。本实验没有采用 `std::vector::resize` 的语义。

```cpp
auto next = std::make_unique<T[]>(capacity);
for (std::size_t i = 0; i < size_; ++i) {
    next[i] = data_[i];
}
data_.swap(next);
capacity_ = capacity;
```

先申请新数组，复制有效元素，最后交换数组所有权。交换后 `data_` 管理新数组，`next` 管理旧数组；方法返回时，`next` 自动销毁旧数组。

容量不能小于当前长度，否则会丢元素，也不能小于最小容量 4。分配或复制新数组失败时，尚未执行交换，原数组仍然存在。

复制有效元素需要 O(n)，本实现还会构造新数组的全部槽位，并销毁旧数组的全部槽位，所以完整成本还与新旧容量有关。手工把容量设置得非常大时，不能忽略这些成本。顺序表使用的空间是 O(C)，其中 C 表示容量。

# remove方法
删除要求 `0 <= index < size_`。空表不能删除，`index == size_` 也不能删除，因为这一位置没有有效元素。

```cpp
for (std::size_t i = index; i + 1 < size_; ++i) {
    data_[i] = data_[i + 1];
}
data_[--size_] = T{};
```

删除 `[A, B, C, D]` 的下标 1 时，先用 C 覆盖 B，再用 D 覆盖 C，得到有效内容 `[A, C, D]`。左移应从前往后执行。

`--size_` 先把长度减一，再作为数组下标。`T{}` 是默认构造的值，用它重置原末尾槽位，可及时释放该槽位里旧字符串持有的资源。

## 缩容条件

```cpp
if (capacity_ > 4 && size_ <= capacity_ / 4) {
    resize(std::max<std::size_t>(4, capacity_ / 2));
}
```

当使用率降到四分之一或以下时，容量减半，但不低于 4。容量为 8、长度降到 2 时，会缩为 4。采用四分之一阈值，可以避免在半满附近反复插入和删除时不停地扩容、缩容。

自动容量策略下，删除最坏需要 O(n) 时间。若此前手工预留了远大于长度的容量，释放旧数组的成本也要计入。

# find方法和at方法
任务书的命名与标准库容易混淆，本项目按任务书实现：

| 方法 | 输入 | 返回值 | 复杂度 |
|---|---|---|---|
| `find(index)` | 下标 | 对应位置的元素引用 | O(1) |
| `at(value)` | 元素值 | 第一次出现的下标，未找到为 -1 | O(n) |

`find` 直接使用数组下标访问。它有可修改和只读两个重载，`const` 版本可供只读对象调用。

`at` 从头向后比较值。示例学号表允许按任意位置插入，未保证排序，因此不能直接使用二分查找。

不要长期保存 `find` 返回的引用：扩容会更换数组，删除也会移动元素，旧引用可能失效或指向不同的逻辑记录。

# init方法和状态查询
```cpp
void init() {
    DynamicList empty;
    swap(empty);
}
```

创建最小容量的空表，与当前表交换；临时表销毁时释放旧数组。清空后长度为 0、容量为 4、`isEmpty()` 返回 `true`。

`length()`、`capacity()`、`isEmpty()` 只读取成员变量，为 O(1)。清空旧数组需要销毁其槽位中的对象，不能把本实现的整个 `init` 一概当作 O(1)。

# print方法
本项目让调用者决定元素如何输出：

```cpp
template<class Visitor>
void print(Visitor visitor) const {
    for (std::size_t i = 0; i < size_; ++i) {
        visitor(i, data_[i]);
    }
}
```

`visitor` 是可以调用的对象，每访问一个元素，就把下标和元素交给它。Qt 窗口传入 lambda，把学号加入 `QStringList`，最后显示到操作记录区。这样算法类不需要依赖控制台，也不需要包含 Qt 的头文件。

# template与类型参数T
`template<class T>` 表示把元素类型暂时写成 T，在使用时再确定具体类型。同一份 `DynamicList` 代码既能存 `std::string`，也能存 `Student` 或 `Grade`。

本实现要求 T 可默认构造、可复制赋值；只有调用 `at` 时，元素还必须支持 `==` 比较。模板并不保证任意类型都能使用所有方法。

# unique_ptr与深拷贝

`std::unique_ptr<T[]>` 独占数组，并在自身销毁时自动释放数组。它不能直接复制，因为两个指针不能同时独占同一个数组。

顺序表的 `resize` 先创建新数组、复制有效元素，再通过 `data_.swap(next)` 交换所有权。方法结束时，临时指针 `next` 释放旧数组。

顺序表自己实现了深拷贝：新建数组，再逐项复制。不能把数组地址简单复制给另一个表，否则两个表会共享数据或发生重复释放。成绩导入中使用临时 `GradeBook`，也依赖其成员顺序表能正确复制。

`std::move` 表达“允许移动”，实际是否转移资源由目标类型的移动操作决定，不能把它理解成复制数组或直接移动内存中的元素。

# student方法与字典索引
对应源码：`exp1-2/grade_book.h`。

学生记录存放在 `DynamicList<Student>`，成绩记录存放在 `DynamicList<Grade>`。`std::map<std::string, std::size_t>` 只记录“学号 → 学生表下标”，不替代顺序表存储。

`student(id)` 先用 `map::find` 查字典，再用得到的下标访问学生表；找不到时返回 `nullptr`。这里的查找约为 O(log S)，S 是学生数量。

学生表只追加，不提供删除学生功能，所以已有下标保持稳定。如果将来增加删除学生功能，就必须同步维护受影响的字典下标。

# addStudent方法
依次检查学号、姓名非空以及学号唯一；验证通过后把学生追加到顺序表，再把新下标记入字典。

学号相同而姓名不同仍然属于重复学生，不能新增第二条。姓名相同、学号不同则允许保存。

# addGrade方法
先确认学生存在，再检查课程非空、分数为有限数且位于 0–100，最后扫描成绩表检查“学号 + 课程名”是否重复。

同一个学生可以有多门课程，每门课程只能有一条成绩。本项目修改成绩采用“删除旧成绩，再添加新成绩”的方式，任务书并未要求单独的修改接口。

`std::isfinite` 可以排除 NaN、正无穷和负无穷。尤其 NaN 与普通数的大小比较都不能按正常数值理解，不能只写 `score < 0 || score > 100`。

# removeGrade方法
以学号和课程名共同定位一条成绩，调用顺序表的 `remove` 删除，找到后立即返回。删除成绩不删除学生基本信息。不存在的成绩会抛出异常，由窗口显示错误。

# query方法与降序排序
学生筛选条件为空时显示所有学生；不为空时，匹配完整学号或姓名包含关系。课程条件为空时显示所有课程，不为空时匹配完整课程名。

每命中一条成绩，就将其插入到结果表中正确的位置：

```cpp
std::size_t j = 0;
while (j < result.length() && result.find(j).score >= g.score) {
    ++j;
}
result.insert(j, g);
```

越过所有分数不低于新成绩的记录，再插入新记录，结果就保持降序。使用 `>=` 还能让同分记录维持原先后关系。

若总成绩数为 G、学生数为 S、命中结果数为 K，最坏成本约为 O(G log S + K²)，结果表使用 O(K) 空间。原始成绩表不会因查询而被重新排序。

# QApplication与main函数
对应各题 `main.cpp`：

```cpp
QApplication app(argc, argv);
styleApp(app);
ListWindow window;
window.show();
return app.exec();
```

`QApplication` 管理图形应用，`show()` 显示窗口，`app.exec()` 进入事件循环，等待鼠标、键盘和窗口事件。没有事件循环，程序可能创建完窗口就结束。

`argc` 和 `argv` 是命令行参数入口，但普通用户仍然可以双击 exe 使用窗口，不需要输入命令。本项目自动截图验证才使用额外参数。

# QWidget与布局
`QWidget` 是窗口和很多控件的基础类，`QPushButton` 是按钮，`QLineEdit` 是输入框，`QSpinBox` 是整数输入控件，`QTableWidget` 是表格，`QPlainTextEdit` 是操作记录区。

`QVBoxLayout` 纵向排列控件，`QHBoxLayout` 横向排列控件，`QFormLayout` 按标签和输入框排列。布局比给每个按钮写固定坐标更容易适应不同窗口尺寸。

Qt 的父子对象机制负责销毁子控件。本项目 `new QVBoxLayout(this)` 将布局交给窗口，控件加入窗口布局后也由窗口层级管理；顺序表数组则由 `unique_ptr` 管理。

# connect与lambda表达式
对应 `ui_common.h` 中的 `action`：

```cpp
QObject::connect(button, &QPushButton::clicked, owner, [owner, fn] {
    fn();
});
```

`button` 是发出信号的对象，`clicked` 是点击信号，`owner` 是连接上下文，lambda 是点击后执行的函数。上下文对象销毁后，Qt 会使这个连接失效。可对照 [Qt 信号与槽文档](https://doc.qt.io/qt-6/signalsandslots.html) 和 [QObject 连接接口](https://doc.qt.io/qt-6/qobject.html#connect) 阅读。

方括号是 lambda 捕获列表。`[this]` 捕获当前对象指针，`[owner, fn]` 按值保存指定变量，`[&]` 按引用捕获，`[=]` 默认按值捕获需要使用的局部变量。在 C++17 中，`[=]` 使用成员时会捕获 `this` 指针，并不是复制整个窗口对象。

异步执行的回调不能引用已经销毁的局部变量。本项目按钮回调所需的控件是窗口成员，公共函数的连接也绑定了窗口生命周期。

# 一次点击如何调用append
调用顺序是：按钮发出 `clicked` → `action` 执行传入的回调 → `read` 读取并检查输入 → `list.append` 更新数据 → `refresh` 更新表格与状态。

```cpp
action(row, "追加 append", "append", this, [=] {
    list.append(read());
    refresh("追加成功");
});
```

`"append"` 这个对象名供自动化检查定位按钮，不负责执行算法。真正调用算法的是 `list.append(read())`。

界面刷新不等于算法执行：直接修改表格单元格不能替代修改顺序表。本项目表格只读，保证所有修改经过算法接口。

# 异常处理
输入越界或业务规则不满足时，算法通过 `throw std::out_of_range(...)` 或 `throw std::invalid_argument(...)` 报告失败。

公共 `action` 用 `try/catch` 捕获 `std::exception`，把错误写入操作记录区，让事件循环继续处理后续操作。捕获异常不代表操作成功，回调中发生异常后不会继续执行正常的成功刷新。

# QString与std string
界面使用 `QString`，算法与业务类使用 `std::string`。`toStdString()` 把界面文本转换成 UTF-8 字符串，`QString::fromStdString()` 转回界面文本；错误信息使用 `QString::fromUtf8(e.what())`。

`trimmed()` 去除输入首尾空白，`QString::number()` 把数字转换为文本，`arg()` 把数值填入 `%1`、`%2` 等占位符。

姓名的“包含查询”使用 UTF-8 字符串子串匹配。本实验不涉及拼音检索、大小写归一或复杂自然语言匹配。

# loadFile方法与saveFile方法
成绩文件包含 `students` 和 `grades` 两个 JSON 数组。导入依次检查文件读取、JSON 语法、数组是否存在、字段类型、学生是否存在和课程是否重复。

读取时先创建临时 `GradeBook next`，全部验证成功后才执行 `book = next`。如果文件中途有坏记录，当前成绩表仍然保留。

保存时先生成 JSON，使用 `QSaveFile` 写入临时文件，最后 `commit()` 提交。写入长度不足或提交失败都应报告错误。程序不会在退出时自动保存，所以保留修改需要点击保存按钮。

顺序表的文本导入采用同样的“先准备、后替换”思路：每个非空白学号作为一个元素，先载入临时顺序表，再与当前表交换。

# CMake与图形界面运行

本实验使用 Qt 6.5 以上、C++17 和 CMake 3.21 以上。可以在 Qt Creator 中打开本文件夹的 `CMakeLists.txt`，选择与已安装 Qt 库匹配的构建套件，再点击构建和运行。也可以单独打开某个 `exp` 文件夹中的 `CMakeLists.txt`。构建成功后，程序通过窗口和按钮操作。

Qt 工程需要编译器知道 Qt 的头文件位置，还需要链接 Qt 的库。普通的“编译当前 cpp 文件”没有这些信息，可能报 `QtWidgets: No such file or directory`。

`find_package(Qt6 ... Widgets)` 找到 Qt，`target_link_libraries(... Qt6::Widgets)` 把所需头文件路径和链接依赖加到目标上。

如果使用 VS Code，应通过 CMake Tools 配置整个工程，选择与 Qt 匹配的编译器，并让 CMake 找到对应的 Qt 安装目录。配置时找不到 Qt，可以设置 `CMAKE_PREFIX_PATH` 为 Qt 套件目录，例如本机的 `C:/Qt/6.11.1/mingw_64`。换电脑时应使用实际安装路径。

目录调整后，原有运行配置中的旧路径可能失效，需要指向当前工程构建出的程序。修改源码后先重新构建，再运行。只启动旧的可执行文件，会继续看到旧界面；逐行断点调试还需要选择包含调试信息的构建。

# 验证代码是否正确

可以用 `std::vector` 作为测试参照：对自写顺序表与标准容器执行同样的插入、删除和查找，再比较长度、内容与查找结果。标准容器用于核对结果，不替代实验的顺序表实现。

手工检查顺序表时，至少覆盖：

1. 清空后长度为 0、容量为 4，删除和取值越界会显示错误
2. 在头部、中间、尾部插入，验证右移后的元素顺序
3. 长度为 4 时追加第 5 项，验证容量从 4 变为 8
4. 容量为 8、长度降到 2，验证缩容到 4
5. 查找重复值返回第一次出现的位置，未找到返回 -1
6. 从文件导入非法内容时，检查原数据是否保留

成绩管理至少覆盖重复学号、重复课程、同名不同学号、分数边界 0 与 100、按学号或姓名查询、按课程筛选和同分排序。保存后重新导入，比较学生与成绩是否一致；导入坏文件时应显示错误，并保留原数据。
