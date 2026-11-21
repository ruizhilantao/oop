#include "ActivityManager.h"

#include <cstddef>

ActivityManager::ActivityManager() {
}

ActivityManager::~ActivityManager() {
    // 管理器是这些活动对象的唯一所有者，析构时统一释放。
    // 因为基类析构函数是虚函数，派生部分的成员也会被正确释放。
    for (std::size_t i = 0; i < activities_.size(); ++i) {
        delete activities_[i];
    }
    activities_.clear();
}

// ==================== 用户 ====================

const std::vector<User>& ActivityManager::getUsers() const {
    return users_;
}

User* ActivityManager::findUser(const std::string& id) {
    for (std::size_t i = 0; i < users_.size(); ++i) {
        if (users_[i].getId() == id) {
            return &users_[i];
        }
    }
    return nullptr;
}

const User* ActivityManager::findUser(const std::string& id) const {
    for (std::size_t i = 0; i < users_.size(); ++i) {
        if (users_[i].getId() == id) {
            return &users_[i];
        }
    }
    return nullptr;
}

bool ActivityManager::addUser(const std::string& id, const std::string& name,
                              const std::string& contact, int level, std::string& message) {
    if (id.empty()) {
        message = "用户编号不能为空。";
        return false;
    }
    if (findUser(id) != nullptr) {
        message = "用户编号已存在：" + id + "（编号必须唯一）。";
        return false;
    }
    if (name.empty()) {
        message = "用户姓名不能为空。";
        return false;
    }
    if (contact.empty()) {
        message = "联系方式不能为空。";
        return false;
    }
    if (level < User::kMinLevel || level > User::kMaxLevel) {
        message = "参赛等级必须在 1 ~ 10 之间。";
        return false;
    }
    users_.push_back(User(id, name, contact, level));
    message = "用户创建成功：" + users_.back().describe();
    return true;
}

bool ActivityManager::modifyUser(const std::string& id, const std::string& name,
                                 const std::string& contact, int level, std::string& message) {
    User* user = findUser(id);
    if (user == nullptr) {
        message = "用户不存在：" + id + "（未做任何修改）。";
        return false;
    }
    if (name.empty()) {
        message = "姓名不能为空（未做任何修改）。";
        return false;
    }
    if (contact.empty()) {
        message = "联系方式不能为空（未做任何修改）。";
        return false;
    }
    if (level < User::kMinLevel || level > User::kMaxLevel) {
        message = "参赛等级必须在 1 ~ 10 之间（未做任何修改）。";
        return false;
    }
    // 编号保持不变：这里只修改允许变更的字段。
    user->setName(name);
    user->setContact(contact);
    user->setLevel(level);
    message = "修改成功：" + user->describe();
    return true;
}

bool ActivityManager::removeUser(const std::string& id, std::string& message) {
    std::size_t index = users_.size();
    for (std::size_t i = 0; i < users_.size(); ++i) {
        if (users_[i].getId() == id) {
            index = i;
            break;
        }
    }
    if (index == users_.size()) {
        message = "用户不存在：" + id;
        return false;
    }

    // 数据一致性：存在报名关系时拒绝删除，避免活动里残留指向不存在用户的报名记录。
    for (std::size_t i = 0; i < activities_.size(); ++i) {
        if (activities_[i]->hasRegistrant(id)) {
            message = "用户 " + id + " 已报名活动 " + activities_[i]->getId() + "，请先取消报名。";
            return false;
        }
    }

    users_.erase(users_.begin() + static_cast<std::ptrdiff_t>(index));
    message = "用户 " + id + " 已删除。";
    return true;
}

// ==================== 活动 ====================

const std::vector<Activity*>& ActivityManager::getActivities() const {
    return activities_;
}

Activity* ActivityManager::findActivity(const std::string& id) {
    for (std::size_t i = 0; i < activities_.size(); ++i) {
        if (activities_[i]->getId() == id) {
            return activities_[i];
        }
    }
    return nullptr;
}

const Activity* ActivityManager::findActivity(const std::string& id) const {
    for (std::size_t i = 0; i < activities_.size(); ++i) {
        if (activities_[i]->getId() == id) {
            return activities_[i];
        }
    }
    return nullptr;
}

bool ActivityManager::addActivity(Activity* activity, std::string& message) {
    if (activity == nullptr) {
        message = "活动对象为空。";
        return false;
    }
    if (activity->getId().empty()) {
        message = "活动编号不能为空。";
        return false;
    }
    if (findActivity(activity->getId()) != nullptr) {
        message = "活动编号已存在：" + activity->getId() + "（编号必须唯一）。";
        return false;
    }
    if (activity->getName().empty()) {
        message = "活动名称不能为空。";
        return false;
    }
    if (activity->getCapacity() <= 0) {
        message = "活动容量必须是正整数。";
        return false;
    }
    activities_.push_back(activity);
    message = "活动创建成功：" + activity->describeCommon();
    return true;
}

bool ActivityManager::modifyActivity(const std::string& id, const std::string& name,
                                     const std::string& time, const std::string& place, int capacity,
                                     std::string& message) {
    Activity* activity = findActivity(id);
    if (activity == nullptr) {
        message = "活动不存在：" + id + "（未做任何修改）。";
        return false;
    }
    // 先给出具体原因，再由 Activity::updateBasicInfo 兜底校验，保证规则不会被绕过。
    if (name.empty() || time.empty() || place.empty()) {
        message = "名称、举办时间和地点都不能为空（未做任何修改）。";
        return false;
    }
    if (capacity <= 0) {
        message = "容量必须是正整数（未做任何修改）。";
        return false;
    }
    if (capacity < activity->getRegisteredCount()) {
        message = "容量不能小于已报名人数 " + std::to_string(activity->getRegisteredCount()) +
                  "（未做任何修改）。";
        return false;
    }
    if (!activity->updateBasicInfo(name, time, place, capacity)) {
        message = "输入信息不合法（未做任何修改）。";
        return false;
    }
    message = "修改成功：" + activity->describeCommon();
    return true;
}

bool ActivityManager::removeActivity(const std::string& id, std::string& message) {
    for (std::size_t i = 0; i < activities_.size(); ++i) {
        if (activities_[i]->getId() != id) {
            continue;
        }
        // 数据一致性：仍有报名记录时拒绝删除，报名人数不会因此与记录不一致。
        if (!activities_[i]->getRegistrants().empty()) {
            message = "活动 " + id + " 已有 " +
                      std::to_string(activities_[i]->getRegisteredCount()) + " 人报名，请先取消报名。";
            return false;
        }
        delete activities_[i];
        activities_.erase(activities_.begin() + static_cast<std::ptrdiff_t>(i));
        message = "活动 " + id + " 已删除。";
        return true;
    }
    message = "活动不存在：" + id;
    return false;
}

std::vector<std::string> ActivityManager::activityTypeNames() const {
    // 类型名全部来自各活动的虚函数 typeName()，这里只做去重，不知道一共有几种类型。
    std::vector<std::string> names;
    for (std::size_t i = 0; i < activities_.size(); ++i) {
        const std::string name = activities_[i]->typeName();
        bool exists = false;
        for (std::size_t j = 0; j < names.size(); ++j) {
            if (names[j] == name) {
                exists = true;
                break;
            }
        }
        if (!exists) {
            names.push_back(name);
        }
    }
    return names;
}

std::vector<const Activity*> ActivityManager::findActivitiesByTypeName(
    const std::string& typeName) const {
    std::vector<const Activity*> matched;
    for (std::size_t i = 0; i < activities_.size(); ++i) {
        if (activities_[i]->typeName() == typeName) {
            matched.push_back(activities_[i]);
        }
    }
    return matched;
}

int ActivityManager::countActivitiesByTypeName(const std::string& typeName) const {
    int count = 0;
    for (std::size_t i = 0; i < activities_.size(); ++i) {
        if (activities_[i]->typeName() == typeName) {
            ++count;
        }
    }
    return count;
}

// ==================== 报名 ====================

bool ActivityManager::previewRegistration(const std::string& userId,
                                          const std::string& activityId,
                                          std::string& message) const {
    const User* user = findUser(userId);
    if (user == nullptr) {
        message = "用户不存在：" + userId;
        return false;
    }
    const Activity* activity = findActivity(activityId);
    if (activity == nullptr) {
        message = "活动不存在：" + activityId;
        return false;
    }
    if (activity->hasRegistrant(userId)) {
        message = "该用户已经报名此活动，无需再检查。";
        return false;
    }
    // 虚函数调用：具体用哪套规则由活动对象的实际类型决定，这里不做任何类型判断。
    const RegistrationCheck check = activity->checkRegistration(*user);
    message = describeCheckResult(check);
    return check.allowed;
}

bool ActivityManager::registerForActivity(const std::string& userId,
                                          const std::string& activityId, std::string& message) {
    const User* user = findUser(userId);
    if (user == nullptr) {
        message = "报名失败：用户不存在：" + userId;
        return false;
    }
    Activity* activity = findActivity(activityId);
    if (activity == nullptr) {
        message = "报名失败：活动不存在：" + activityId;
        return false;
    }
    if (activity->hasRegistrant(userId)) {
        message = "报名失败：该用户已经报名活动 " + activityId + "，不能重复报名。";
        return false;
    }

    // 多态：同一行代码对讲座/竞赛/体育会执行不同的规则检查。
    const RegistrationCheck check = activity->checkRegistration(*user);
    if (!check.allowed) {
        // 拒绝时只返回结果，不修改报名记录和人数。
        message = "报名失败：" + describeCheckResult(check);
        return false;
    }

    activity->addRegistrant(userId);
    message = "报名成功：" + describeCheckResult(check) + " | 活动 " + activityId + " 当前已报名 " +
              std::to_string(activity->getRegisteredCount()) + "/" +
              std::to_string(activity->getCapacity()) + " 人。";
    return true;
}

bool ActivityManager::cancelRegistration(const std::string& userId, const std::string& activityId,
                                         std::string& message) {
    if (findUser(userId) == nullptr) {
        message = "取消失败：用户不存在：" + userId;
        return false;
    }
    Activity* activity = findActivity(activityId);
    if (activity == nullptr) {
        message = "取消失败：活动不存在：" + activityId;
        return false;
    }
    if (!activity->unregisterUser(userId)) {
        message = "取消失败：用户 " + userId + " 没有报名活动 " + activityId + "。";
        return false;
    }
    message = "取消成功：活动 " + activityId + " 当前已报名 " +
              std::to_string(activity->getRegisteredCount()) + "/" +
              std::to_string(activity->getCapacity()) + " 人。";
    return true;
}

std::vector<const Activity*> ActivityManager::findActivitiesOfUser(const std::string& userId) const {
    std::vector<const Activity*> matched;
    for (std::size_t i = 0; i < activities_.size(); ++i) {
        if (activities_[i]->hasRegistrant(userId)) {
            matched.push_back(activities_[i]);
        }
    }
    return matched;
}
