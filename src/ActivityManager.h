#ifndef ACTIVITY_MANAGER_H
#define ACTIVITY_MANAGER_H

#include "Activity.h"
#include "User.h"

#include <string>
#include <vector>

// 系统的数据仓库与业务入口。
// 职责划分：
//   1. 持有全部 User 对象和全部 Activity 派生对象（组合关系），负责它们的创建、查找与释放；
//   2. 完成“用户 + 活动”的协作：报名、取消报名、按用户查活动、按活动查用户；
//   3. 保证编号唯一、容量合法、报名关系与人数一致等业务规则。
// 说明：Activity 对象由本类 new 出来并负责 delete，因此禁止拷贝，避免两个管理器释放同一批对象。
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

    // 创建用户：编号不能为空、不能重复，姓名和联系方式不能为空。
    // 成功时 message 为结果说明，失败时 message 为失败原因。
    bool addUser(const std::string& id, const std::string& name, const std::string& contact,
                 std::string& message);

    // 修改用户可变更的信息；编号不变；找不到用户时不修改任何数据。
    bool modifyUser(const std::string& id, const std::string& name, const std::string& contact,
                    std::string& message);

    // 删除用户：仍有报名关系时拒绝删除并说明原因，保证报名关系不被破坏。
    bool removeUser(const std::string& id, std::string& message);

    // ---------------- 活动 ----------------
    const std::vector<Activity*>& getActivities() const;

    Activity* findActivity(const std::string& id);
    const Activity* findActivity(const std::string& id) const;

    // 创建活动：接管传入的对象；返回 false 时调用者仍需负责释放该对象。
    bool addActivity(Activity* activity, std::string& message);

    // 修改活动可变更的信息（编号不变）：名称、举办时间、地点、容量。
    // 容量被改小到小于已报名人数时拒绝修改，保证人数与容量始终自洽。
    bool modifyActivity(const std::string& id, const std::string& name, const std::string& time,
                        const std::string& place, int capacity, std::string& message);

    // 删除活动：仍有报名记录时拒绝删除并说明原因。
    bool removeActivity(const std::string& id, std::string& message);

    // 按类型浏览：只返回指定类型的活动，供“分类显示”菜单使用。
    std::vector<const Activity*> findActivitiesByType(ActivityType type) const;
    int countActivitiesByType(ActivityType type) const;

    // ---------------- 报名 ----------------
    // 报名：先确认用户和活动都存在，再交给 Activity 判断容量和重复报名。
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
