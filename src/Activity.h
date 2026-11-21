#ifndef ACTIVITY_H
#define ACTIVITY_H

#include "User.h"

#include <string>
#include <vector>

// 一次报名检查的完整结果。
// 各派生类按自己的规则填好它，界面层直接显示即可，不需要再判断活动类型。
struct RegistrationCheck {
    bool allowed = false;       // 是否允许报名
    int fee = 0;                // 本次报名费用（元）
    std::string rule;           // 本次采用的规则说明
    std::string qualification;  // 资格检查结论
    std::string reason;         // 拒绝原因（allowed == false 时有效）
};

// 把检查结果拼成一行可显示的文本（允许/拒绝、规则、资格、费用、原因）。
std::string describeCheckResult(const RegistrationCheck& check);

// 活动基类：保存公共字段与报名记录，并把“各类型不同的部分”交给虚函数实现。
//
// 第 5 次上机的改造要点：
//   1. 原来按类型 switch 的 activitySpecificInfo() 被删除，改成纯虚函数 specificInfo()；
//   2. 新增 ruleText() 与 checkRegistration()，由派生类实现各自的报名规则；
//   3. 所有对活动行为的调用（显示特有信息、显示规则、检查报名）都通过基类指针/引用完成，
//      代码里不再出现“先判断类型再分支”的集中式类型判断。
class Activity {
public:
    Activity(const std::string& id, const std::string& name, const std::string& time,
             const std::string& place, int capacity);
    virtual ~Activity();

    // ---------- 由派生类实现的多态接口 ----------
    virtual std::string typeName() const = 0;      // 类型名，如“讲座”
    virtual std::string specificInfo() const = 0;  // 本类型特有信息
    virtual std::string ruleText() const = 0;      // 本类型的报名规则说明

    // 按本类型规则检查一次报名。
    // 只读：不修改报名记录，相同输入条件下多次调用结果完全一致。
    virtual RegistrationCheck checkRegistration(const User& user) const;

    // ---------- 公共字段访问 ----------
    const std::string& getId() const;
    const std::string& getName() const;
    const std::string& getTime() const;
    const std::string& getPlace() const;
    int getCapacity() const;

    // 已报名人数直接由报名记录算出，没有单独计数器，因此不会与记录不一致。
    int getRegisteredCount() const;
    const std::vector<std::string>& getRegistrants() const;

    bool isFull() const;
    bool hasRegistrant(const std::string& userId) const;

    // 检查通过后由 ActivityManager 调用，写入报名记录。
    bool addRegistrant(const std::string& userId);
    // 取消报名：没有记录时返回 false，集合保持不变。
    bool unregisterUser(const std::string& userId);

    // 公共信息的单行显示（编号、名称、类型、时间、地点、容量、已报名人数）。
    std::string describeCommon() const;

    // 修改允许变更的公共信息（编号不可修改）。
    // 名称/时间/地点不能为空；容量必须为正且不小于已报名人数，否则整体不修改并返回 false。
    bool updateBasicInfo(const std::string& name, const std::string& time,
                         const std::string& place, int capacity);

protected:
    // 派生类要读取公共信息拼出完整信息，因此公共字段放在 protected。
    std::string id_;
    std::string name_;
    std::string time_;
    std::string place_;
    int capacity_;

private:
    // 只存报名用户的编号：避免用户对象销毁后出现野指针，也保证报名关系只有一份来源。
    std::vector<std::string> registrants_;
};

#endif
