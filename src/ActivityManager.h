#ifndef ACTIVITY_MANAGER_H
#define ACTIVITY_MANAGER_H

#include "Activity.h"
#include "User.h"

#include <string>
#include <vector>

// 系统的数据仓库与业务入口。
// 职责划分：
//   1. 持有全部 User 对象和全部 Activity 派生对象（组合关系），负责创建、查找与释放；
//   2. 完成“用户 + 活动”的协作：报名、取消报名、按用户查活动、按活动查用户；
//   3. 保证编号唯一、容量合法、报名关系与人数一致等业务规则。
//
// 第 5 次上机后，报名是否允许**不再由本类判断**：本类只负责找到对象并把用户交给
// activity->checkRegistration()（虚函数），具体规则由各派生类自己实现。
class ActivityManager {
public:
    ActivityManager();
    ~ActivityManager();

    ActivityManager(const ActivityManager&) = delete;
    ActivityManager& operator=(const ActivityManager&) = delete;

    // ---------------- 用户 ----------------
    const std::vector<User>& getUsers() const;

    User* findUser(const std::string& id);
    const User* findUser(const std::string& id) const;

    // 创建用户：编号不能为空、不能重复，姓名和联系方式不能为空，等级必须在 1~10。
    bool addUser(const std::string& id, const std::string& name, const std::string& contact,
                 int level, std::string& message);

    // 修改用户可变更的信息；编号不变；找不到用户时不修改任何数据。
    bool modifyUser(const std::string& id, const std::string& name, const std::string& contact,
                    int level, std::string& message);

    // 删除用户：仍有报名关系时拒绝删除并说明原因。
    bool removeUser(const std::string& id, std::string& message);

    // ---------------- 活动 ----------------
    const std::vector<Activity*>& getActivities() const;

    Activity* findActivity(const std::string& id);
    const Activity* findActivity(const std::string& id) const;

    // 创建活动：接管传入的对象；返回 false 时调用者仍需负责释放该对象。
    bool addActivity(Activity* activity, std::string& message);

    // 修改活动可变更的公共信息（编号不变）：名称、举办时间、地点、容量。
    bool modifyActivity(const std::string& id, const std::string& name, const std::string& time,
                        const std::string& place, int capacity, std::string& message);

    // 删除活动：仍有报名记录时拒绝删除并说明原因。
    bool removeActivity(const std::string& id, std::string& message);

    // 当前出现过的活动类型名（去重），供“分类浏览/统计”菜单动态列出。
    std::vector<std::string> activityTypeNames() const;
    // 按类型名筛选：类型名来自虚函数 typeName()，这里只做字符串比较，没有类型分支。
    std::vector<const Activity*> findActivitiesByTypeName(const std::string& typeName) const;
    int countActivitiesByTypeName(const std::string& typeName) const;

    // ---------------- 报名 ----------------
    // 试算：只调用 checkRegistration() 做规则检查，不写入任何记录。
    // 相同输入重复调用结果完全一致。
    bool previewRegistration(const std::string& userId, const std::string& activityId,
                             std::string& message) const;

    // 正式报名：通过基类指针调用该活动类型自己的检查规则，通过后才写入报名记录。
    bool registerForActivity(const std::string& userId, const std::string& activityId,
                             std::string& message);

    // 取消报名：没有报名记录时失败，报名集合保持不变。
    bool cancelRegistration(const std::string& userId, const std::string& activityId,
                            std::string& message);

    // 查询某个用户已经报名的全部活动（按活动记录反查，保证与人数一致）。
    std::vector<const Activity*> findActivitiesOfUser(const std::string& userId) const;

private:
    std::vector<User> users_;
    std::vector<Activity*> activities_;  // 基类指针存放不同派生类型的活动
};

#endif
