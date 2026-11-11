/*
* SID: 202530551453
* Author: 徐克楠
* Date: 2026.11.11
*/
#pragma once
#include "dynamic_list.h"
#include <map>
#include <string>
#include <cmath>

// 学号使用字符串保存，以保留可能存在的前导零
struct Student { std::string id, name; };
// 成绩通过学号关联学生，不重复保存学生姓名
struct Grade { std::string id, course; double score=0; };
class GradeBook {
    // 两张顺序表分别保存学生和成绩，实际存储由自写 DynamicList 完成
    DynamicList<Student> students_;
    DynamicList<Grade> grades_;
    // 字典提供学号到学生下标的接口，实际记录仍存放在自写顺序表中
    std::map<std::string,std::size_t> index_;
public:
    // 只读接口用于窗口显示，避免外部直接修改记录而破坏字典索引
    const DynamicList<Student>& students() const { return students_; }
    const DynamicList<Grade>& grades() const { return grades_; }
    // student 方法：通过学号字典定位学生，找不到时返回 nullptr
    // map::find 为 O(log S)，随后按顺序表下标取值为 O(1)
    const Student* student(const std::string& id) const {
        auto it=index_.find(id); return it==index_.end()?nullptr:&students_.find(it->second);
    }
    // addStudent 方法：验证非空和学号唯一性，再追加记录并建立字典索引
    // 学生表只追加不删除，因此旧记录的下标不会因本操作发生变化
    void addStudent(const std::string& id,const std::string& name) {
        if (id.empty()||name.empty()) throw std::invalid_argument("学号和姓名不能为空");
        if (student(id)) throw std::invalid_argument("学号重复");
        students_.append({id,name}); index_[id]=students_.length()-1;
    }
    // addGrade 方法：先验证学生存在，再检查分数范围和学生课程组合唯一性
    // 同一学生允许有多门课程，但同一门课程只能保存一条成绩
    // 扫描 G 条已有成绩进行防重，时间复杂度为 O(G + log S)
    void addGrade(const std::string& id,const std::string& course,double score) {
        if (!student(id)) throw std::invalid_argument("请先添加该学生");
        // isfinite 同时排除 NaN 与无穷大，避免它们绕过范围比较
        if (course.empty()||!std::isfinite(score)||score<0||score>100)
            throw std::invalid_argument("课程不能为空，分数必须为 0 到 100");
        for (std::size_t i=0;i<grades_.length();++i)
            if (grades_.find(i).id==id && grades_.find(i).course==course)
                throw std::invalid_argument("该学生已有此课程成绩，请先删除再添加");
        grades_.append({id,course,score});
    }
    // removeGrade 方法：根据学号和课程名确定一条成绩，找到后删除并立即返回
    // 删除的是成绩记录，学生基本信息仍保留在学生表中
    void removeGrade(const std::string& id,const std::string& course) {
        for (std::size_t i=0;i<grades_.length();++i)
            if (grades_.find(i).id==id && grades_.find(i).course==course) { grades_.remove(i); return; }
        throw std::invalid_argument("没有找到此课程成绩");
    }
    // query 方法：学生条件为空时不过滤，否则匹配完整学号或姓名包含关系
    // 课程条件为空时查询全部课程，否则要求课程名完全相同
    // 返回独立的结果表，查询和排序不会改变原始成绩表
    DynamicList<Grade> query(const std::string& studentText,const std::string& course) const {
        DynamicList<Grade> result;
        for (std::size_t i=0;i<grades_.length();++i) {
            // 添加与导入时已检查学生存在，所以这里的学号关联应始终有效
            const auto& g=grades_.find(i); const auto* s=student(g.id);
            if ((!studentText.empty() && g.id!=studentText && s->name.find(studentText)==std::string::npos)
                || (!course.empty() && g.course!=course)) continue;
            // 插入排序：将查询结果按成绩降序排好
            // 越过所有分数不低于新记录的项，再插入新记录
            // 使用 >= 可让同分记录保持原来的先后顺序
            std::size_t j=0; while (j<result.length() && result.find(j).score>=g.score) ++j;
            result.insert(j,g);
        }
        return result;
    }
};
