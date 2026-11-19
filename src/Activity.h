#ifndef ACTIVITY_H
#define ACTIVITY_H

#include <string>
#include <vector>

// 活动类型。第四次上机新增：活动不再是单一形态，而是由基类 + 派生类构成。
enum class ActivityType { Lecture, Competition, Sport };

// 类型的中文名称，用于菜单、列表和统计显示。
std::string activityTypeName(ActivityType type);

// 报名结果。把“重复报名 / 已满员 / 未报名”的判断集中到 Activity 内部，
// 调用处不必重复判断，也不会出现两处规则不一致的问题。
enum class RegisterResult { Success, AlreadyRegistered, FullActivity };

// 活动基类：抽取所有活动共有的字段（编号、名称、时间、地点、容量）和共有行为
// （容量判断、报名人数统计、报名 / 取消报名）。
// 派生类只需要补充本类型的特有信息，公共逻辑全部复用基类实现。
class Activity {
public:
    Activity(const std::string& id, const std::string& name, const std::string& time,
             const std::string& place, int capacity, ActivityType type);
    // 派生对象由基类指针统一释放，所以析构函数必须是虚函数（避免派生部分不析构）。
    virtual ~Activity();

    const std::string& getId() const;
    const std::string& getName() const;
    const std::string& getTime() const;
    const std::string& getPlace() const;
    int getCapacity() const;
    ActivityType getType() const;

    // 已报名人数直接由报名记录算出，没有单独的人数计数器，因此不会出现“人数与记录不一致”。
    int getRegisteredCount() const;
    const std::vector<std::string>& getRegistrants() const;

    bool isFull() const;
    bool hasRegistrant(const std::string& userId) const;

    // 修改允许变更的公共信息（编号不可修改）。
    // 名称/时间/地点不能为空；容量必须为正且不小于已报名人数，否则整体不修改并返回 false。
    // 该检查写在类内部，保证无论谁调用都不会把容量改到比已报名人数还小。
    bool updateBasicInfo(const std::string& name, const std::string& time,
                         const std::string& place, int capacity);

    // 报名 / 取消报名：容量、重复报名等规则都在基类实现，所有活动类型共用。
    RegisterResult registerUser(const std::string& userId);
    bool unregisterUser(const std::string& userId);

    // 公共信息的单行显示（编号、名称、类型、时间、地点、容量、已报名人数）。
    std::string describeCommon() const;

protected:
    // 派生类要读取公共信息拼出“完整信息”，因此把公共字段放在 protected。
    std::string id_;
    std::string name_;
    std::string time_;
    std::string place_;
    int capacity_;
    ActivityType type_;

private:
    // 只存报名用户的编号，不存 User 指针：
    // 一是避免用户对象销毁后出现野指针，二是保证报名关系只有这一份数据来源。
    std::vector<std::string> registrants_;
};

// 显示本类型特有信息的文本（讲座主讲人 / 竞赛参赛要求 / 体育项目）。
std::string activitySpecificInfo(const Activity& activity);

#endif
