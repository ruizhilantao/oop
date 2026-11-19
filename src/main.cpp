#include "ActivityManager.h"
#include "Competition.h"
#include "Input.h"
#include "Lecture.h"
#include "Sport.h"

#include <cstddef>
#include <iostream>
#include <string>
#include <vector>

// 主程序：只负责菜单、输入提示和结果显示，业务规则都交给 ActivityManager 与 Activity。
namespace {

// ==================== 菜单显示 ====================

void printMainMenu() {
    std::cout << "\n========== 校园活动组织与协同平台 ==========\n"
                 " 1. 用户管理        2. 活动管理          3. 浏览活动\n"
                 " 4. 报名            5. 取消报名          6. 查询 / 统计\n"
                 " 0. 退出\n"
              << std::endl;
}

void printUserMenu() {
    std::cout << "\n--- 用户管理 ---\n"
                 " 1. 新建用户        2. 修改用户信息      3. 显示全部用户\n"
                 " 4. 删除用户        0. 返回上一级\n"
              << std::endl;
}

void printActivityMenu() {
    std::cout << "\n--- 发布 / 修改 / 删除活动 ---\n"
                 " 1. 发布新活动      2. 修改活动信息      3. 删除活动\n"
                 " 0. 返回上一级\n"
              << std::endl;
}

void printActivityTypeMenu() {
    std::cout << "可选活动类型: 1. 讲座   2. 竞赛   3. 体育" << std::endl;
}

void printBrowseMenu() {
    std::cout << "\n--- 浏览活动 ---\n"
                 " 1. 显示全部活动    2. 只查看某一类活动  0. 返回上一级\n"
              << std::endl;
}

void printQueryMenu() {
    std::cout << "\n--- 查询 / 统计 ---\n"
                 " 1. 查询某活动的报名用户\n"
                 " 2. 查询某用户已报名的活动\n"
                 " 3. 按类型统计活动数量\n"
                 " 0. 返回上一级\n"
              << std::endl;
}

// ==================== 输入辅助 ====================

// 读取一个必填字段：空输入时说明原因并取消本次操作，绝不会带着空值创建数据。
bool readRequired(const std::string& prompt, const std::string& fieldName, std::string& out) {
    if (!input::readLine(prompt, out)) {
        return false;  // 输入结束，交给上层结束循环
    }
    if (input::isBlank(out)) {
        std::cout << "输入无效：" << fieldName << "不能为空，本次操作已取消。" << std::endl;
        return false;
    }
    return true;
}

// ==================== 显示辅助 ====================

// 单个活动的完整信息 = 基类公共信息 + 本类型特有信息。
void printActivityDetail(const Activity& activity) {
    std::cout << "  " << activity.describeCommon() << " | " << activitySpecificInfo(activity)
              << std::endl;
}

void showUserList(const ActivityManager& manager) {
    const std::vector<User>& users = manager.getUsers();
    if (users.empty()) {
        std::cout << "当前没有用户记录。" << std::endl;
        return;
    }
    std::cout << "---- 用户列表（共 " << users.size() << " 人）----" << std::endl;
    for (std::size_t i = 0; i < users.size(); ++i) {
        std::cout << "  " << users[i].describe() << std::endl;
    }
}

void showAllActivities(const ActivityManager& manager) {
    const std::vector<Activity*>& activities = manager.getActivities();
    if (activities.empty()) {
        std::cout << "当前没有活动记录。" << std::endl;
        return;
    }
    std::cout << "---- 活动列表（共 " << activities.size() << " 个）----" << std::endl;
    for (std::size_t i = 0; i < activities.size(); ++i) {
        printActivityDetail(*activities[i]);
    }
}

// 选择活动类型；0 或非法编号都会返回 false（非法编号额外给出提示）。
bool readTypeChoice(ActivityType& type) {
    printActivityTypeMenu();
    std::string choice;
    if (!input::readLine("请选择活动类型编号: ", choice)) {
        return false;
    }
    if (choice == "1") {
        type = ActivityType::Lecture;
        return true;
    }
    if (choice == "2") {
        type = ActivityType::Competition;
        return true;
    }
    if (choice == "3") {
        type = ActivityType::Sport;
        return true;
    }
    if (!choice.empty() && choice != "0") {
        std::cout << "无效的活动类型编号：" << choice << "，本次操作已取消。" << std::endl;
    }
    return false;
}

void showActivitiesByType(const ActivityManager& manager) {
    ActivityType type = ActivityType::Lecture;
    if (!readTypeChoice(type)) {
        return;
    }
    std::vector<const Activity*> matched = manager.findActivitiesByType(type);
    if (matched.empty()) {
        std::cout << "当前没有" << activityTypeName(type) << "类活动。" << std::endl;
        return;
    }
    std::cout << "---- " << activityTypeName(type) << "类活动（共 " << matched.size() << " 个）----"
              << std::endl;
    for (std::size_t i = 0; i < matched.size(); ++i) {
        printActivityDetail(*matched[i]);
    }
}

// ==================== 用户相关操作 ====================

void createUser(ActivityManager& manager) {
    std::string id;
    std::string name;
    std::string contact;
    std::string message;
    if (!readRequired("用户编号: ", "用户编号", id)) {
        return;
    }
    if (!readRequired("用户姓名: ", "用户姓名", name)) {
        return;
    }
    if (!readRequired("联系方式: ", "联系方式", contact)) {
        return;
    }
    if (!manager.addUser(id, name, contact, message)) {
        std::cout << "创建失败：" << message << std::endl;
        return;
    }
    std::cout << message << std::endl;
}

void modifyUser(ActivityManager& manager) {
    std::string id;
    if (!readRequired("要修改的用户编号: ", "用户编号", id)) {
        return;
    }
    const User* existing = manager.findUser(id);
    if (existing == nullptr) {
        std::cout << "修改失败：用户不存在：" << id << "（未做任何修改）。" << std::endl;
        return;
    }
    const std::string oldName = existing->getName();
    const std::string oldContact = existing->getContact();
    std::cout << "当前信息: " << existing->describe() << std::endl;

    std::string name;
    std::string contact;
    if (!input::readLine("新的姓名（直接回车表示不修改）: ", name)) {
        return;
    }
    if (!input::readLine("新的联系方式（直接回车表示不修改）: ", contact)) {
        return;
    }
    if (name.empty()) {
        name = oldName;
    }
    if (contact.empty()) {
        contact = oldContact;
    }

    std::string message;
    if (!manager.modifyUser(id, name, contact, message)) {
        std::cout << "修改失败：" << message << std::endl;
        return;
    }
    std::cout << message << std::endl;
}

void deleteUser(ActivityManager& manager) {
    std::string id;
    if (!readRequired("要删除的用户编号: ", "用户编号", id)) {
        return;
    }
    std::string message;
    if (!manager.removeUser(id, message)) {
        std::cout << "删除失败：" << message << std::endl;
        return;
    }
    std::cout << message << std::endl;
}

void handleUserMenu(ActivityManager& manager) {
    printUserMenu();
    std::string choice;
    while (input::readLine("请选择操作编号: ", choice)) {
        if (choice == "0") {
            return;
        }
        if (choice == "1") {
            createUser(manager);
        } else if (choice == "2") {
            modifyUser(manager);
        } else if (choice == "3") {
            showUserList(manager);
        } else if (choice == "4") {
            deleteUser(manager);
        } else {
            std::cout << "无效的菜单编号：" << choice << "，请重新输入。" << std::endl;
        }
        printUserMenu();
    }
}

// ==================== 活动相关操作 ====================

void createActivity(ActivityManager& manager) {
    ActivityType type = ActivityType::Lecture;
    if (!readTypeChoice(type)) {
        return;
    }

    std::string id;
    std::string name;
    std::string time;
    std::string place;
    if (!readRequired("活动编号: ", "活动编号", id)) {
        return;
    }
    if (!readRequired("活动名称: ", "活动名称", name)) {
        return;
    }
    if (!readRequired("举办时间: ", "举办时间", time)) {
        return;
    }
    if (!readRequired("举办地点: ", "举办地点", place)) {
        return;
    }

    int capacity = 0;
    if (!input::readPositiveInt("活动容量: ", capacity)) {
        return;
    }

    // 按类型补充特有信息，再创建对应的派生类对象。
    // 三种派生对象都用 Activity* 接管，后面的报名、查询、删除逻辑完全共用。
    std::string extra;
    Activity* activity = nullptr;
    if (type == ActivityType::Lecture) {
        if (!readRequired("主讲人: ", "主讲人", extra)) {
            return;
        }
        activity = new Lecture(id, name, time, place, capacity, extra);
    } else if (type == ActivityType::Competition) {
        if (!readRequired("参赛要求: ", "参赛要求", extra)) {
            return;
        }
        activity = new Competition(id, name, time, place, capacity, extra);
    } else {
        if (!readRequired("体育项目: ", "体育项目", extra)) {
            return;
        }
        activity = new Sport(id, name, time, place, capacity, extra);
    }

    std::string message;
    if (!manager.addActivity(activity, message)) {
        delete activity;  // 管理器没有接管对象，由这里释放，避免内存泄漏
        std::cout << "创建失败：" << message << std::endl;
        return;
    }
    std::cout << "创建成功。" << std::endl;
    const Activity* saved = manager.findActivity(id);
    if (saved != nullptr) {
        printActivityDetail(*saved);
    }
}

void deleteActivity(ActivityManager& manager) {
    std::string id;
    if (!readRequired("要删除的活动编号: ", "活动编号", id)) {
        return;
    }
    std::string message;
    if (!manager.removeActivity(id, message)) {
        std::cout << "删除失败：" << message << std::endl;
        return;
    }
    std::cout << message << std::endl;
}

// 修改活动的公共信息：编号不变，直接回车表示保留原值。
void modifyActivity(ActivityManager& manager) {
    std::string id;
    if (!readRequired("要修改的活动编号: ", "活动编号", id)) {
        return;
    }
    const Activity* existing = manager.findActivity(id);
    if (existing == nullptr) {
        std::cout << "修改失败：活动不存在：" << id << "（未做任何修改）。" << std::endl;
        return;
    }
    std::cout << "当前信息: " << existing->describeCommon() << " | " << activitySpecificInfo(*existing)
              << std::endl;

    const std::string oldName = existing->getName();
    const std::string oldTime = existing->getTime();
    const std::string oldPlace = existing->getPlace();
    const int oldCapacity = existing->getCapacity();

    std::string name;
    std::string time;
    std::string place;
    std::string capacityText;
    if (!input::readLine("新的名称（直接回车表示不修改）: ", name)) {
        return;
    }
    if (!input::readLine("新的举办时间（直接回车表示不修改）: ", time)) {
        return;
    }
    if (!input::readLine("新的举办地点（直接回车表示不修改）: ", place)) {
        return;
    }
    if (!input::readLine("新的容量（直接回车表示不修改）: ", capacityText)) {
        return;
    }

    int capacity = oldCapacity;
    if (!capacityText.empty() && !input::parseInt(capacityText, capacity)) {
        std::cout << "修改失败：容量格式错误（未做任何修改）。" << std::endl;
        return;
    }
    if (name.empty()) {
        name = oldName;
    }
    if (time.empty()) {
        time = oldTime;
    }
    if (place.empty()) {
        place = oldPlace;
    }

    std::string message;
    if (!manager.modifyActivity(id, name, time, place, capacity, message)) {
        std::cout << "修改失败：" << message << std::endl;
        return;
    }
    std::cout << message << std::endl;
}

void handleActivityMenu(ActivityManager& manager) {
    printActivityMenu();
    std::string choice;
    while (input::readLine("请选择操作编号: ", choice)) {
        if (choice == "0") {
            return;
        }
        if (choice == "1") {
            createActivity(manager);
        } else if (choice == "2") {
            modifyActivity(manager);
        } else if (choice == "3") {
            deleteActivity(manager);
        } else {
            std::cout << "无效的菜单编号：" << choice << "，请重新输入。" << std::endl;
        }
        printActivityMenu();
    }
}

void handleBrowseMenu(ActivityManager& manager) {
    printBrowseMenu();
    std::string choice;
    while (input::readLine("请选择操作编号: ", choice)) {
        if (choice == "0") {
            return;
        }
        if (choice == "1") {
            showAllActivities(manager);
        } else if (choice == "2") {
            showActivitiesByType(manager);
        } else {
            std::cout << "无效的菜单编号：" << choice << "，请重新输入。" << std::endl;
        }
        printBrowseMenu();
    }
}

// ==================== 报名 / 取消报名 ====================

void doRegister(ActivityManager& manager) {
    std::string userId;
    std::string activityId;
    if (!readRequired("报名用户编号: ", "用户编号", userId)) {
        return;
    }
    if (!readRequired("报名活动编号: ", "活动编号", activityId)) {
        return;
    }
    std::string message;
    manager.registerForActivity(userId, activityId, message);
    std::cout << message << std::endl;
}

void doCancel(ActivityManager& manager) {
    std::string userId;
    std::string activityId;
    if (!readRequired("取消报名的用户编号: ", "用户编号", userId)) {
        return;
    }
    if (!readRequired("取消报名的活动编号: ", "活动编号", activityId)) {
        return;
    }
    std::string message;
    manager.cancelRegistration(userId, activityId, message);
    std::cout << message << std::endl;
}

// ==================== 查询 / 统计 ====================

void showRegistrantsOfActivity(const ActivityManager& manager) {
    std::string activityId;
    if (!readRequired("活动编号: ", "活动编号", activityId)) {
        return;
    }
    const Activity* activity = manager.findActivity(activityId);
    if (activity == nullptr) {
        std::cout << "活动不存在：" << activityId << std::endl;
        return;
    }
    const std::vector<std::string>& registrants = activity->getRegistrants();
    std::cout << "活动 " << activity->getId() << "（" << activity->getName() << "）的报名用户共 "
              << registrants.size() << " 人: ";
    if (registrants.empty()) {
        std::cout << "（空）" << std::endl;
        return;
    }
    for (std::size_t i = 0; i < registrants.size(); ++i) {
        if (i > 0) {
            std::cout << ", ";
        }
        std::cout << registrants[i];
        const User* user = manager.findUser(registrants[i]);
        if (user != nullptr) {
            std::cout << "(" << user->getName() << ")";
        }
    }
    std::cout << std::endl;
}

void showActivitiesOfUser(const ActivityManager& manager) {
    std::string userId;
    if (!readRequired("用户编号: ", "用户编号", userId)) {
        return;
    }
    const User* user = manager.findUser(userId);
    if (user == nullptr) {
        std::cout << "用户不存在：" << userId << std::endl;
        return;
    }
    std::vector<const Activity*> matched = manager.findActivitiesOfUser(userId);
    if (matched.empty()) {
        std::cout << "用户 " << userId << " 目前没有报名任何活动（空）。" << std::endl;
        return;
    }
    std::cout << "用户 " << userId << "（" << user->getName() << "）已报名 " << matched.size()
              << " 个活动:" << std::endl;
    for (std::size_t i = 0; i < matched.size(); ++i) {
        printActivityDetail(*matched[i]);
    }
}

void showStatistics(const ActivityManager& manager) {
    const ActivityType types[] = {ActivityType::Lecture, ActivityType::Competition,
                                  ActivityType::Sport};
    const int typeCount = static_cast<int>(sizeof(types) / sizeof(types[0]));

    std::cout << "---- 活动统计 ----" << std::endl;
    int total = 0;
    for (int i = 0; i < typeCount; ++i) {
        const int count = manager.countActivitiesByType(types[i]);
        total += count;
        std::cout << "  " << activityTypeName(types[i]) << "类活动: " << count << " 个" << std::endl;
    }
    std::cout << "  活动总数: " << total << " 个；用户总数: " << manager.getUsers().size() << " 人"
              << std::endl;

    // 同时显示“已报名人数”和“报名记录条数”，两者由同一份数据算出，因此必然一致。
    const std::vector<Activity*>& activities = manager.getActivities();
    if (!activities.empty()) {
        std::cout << "  各活动报名情况:" << std::endl;
        for (std::size_t i = 0; i < activities.size(); ++i) {
            std::cout << "    [" << activities[i]->getId() << "] " << activities[i]->getName()
                      << " 已报名 " << activities[i]->getRegisteredCount() << "/"
                      << activities[i]->getCapacity() << "，报名记录 "
                      << activities[i]->getRegistrants().size() << " 条" << std::endl;
        }
    }
}

void handleQueryMenu(ActivityManager& manager) {
    printQueryMenu();
    std::string choice;
    while (input::readLine("请选择操作编号: ", choice)) {
        if (choice == "0") {
            return;
        }
        if (choice == "1") {
            showRegistrantsOfActivity(manager);
        } else if (choice == "2") {
            showActivitiesOfUser(manager);
        } else if (choice == "3") {
            showStatistics(manager);
        } else {
            std::cout << "无效的菜单编号：" << choice << "，请重新输入。" << std::endl;
        }
        printQueryMenu();
    }
}

}  // namespace

int main() {
    ActivityManager manager;
    std::cout << "欢迎使用校园活动组织与协同平台（第 4 次上机版本：多类型活动）" << std::endl;

    printMainMenu();
    std::string choice;
    while (input::readLine("请选择操作编号: ", choice)) {
        if (choice == "0") {
            break;
        }
        if (choice == "1") {
            handleUserMenu(manager);
        } else if (choice == "2") {
            handleActivityMenu(manager);
        } else if (choice == "3") {
            handleBrowseMenu(manager);
        } else if (choice == "4") {
            doRegister(manager);
        } else if (choice == "5") {
            doCancel(manager);
        } else if (choice == "6") {
            handleQueryMenu(manager);
        } else {
            std::cout << "无效的菜单编号：" << choice << "，请重新输入。" << std::endl;
        }
        printMainMenu();
    }

    std::cout << "程序结束。" << std::endl;
    return 0;
}
