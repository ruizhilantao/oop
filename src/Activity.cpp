#include "Activity.h"

#include "Competition.h"
#include "Lecture.h"
#include "Sport.h"

#include <cstddef>
#include <sstream>

std::string activityTypeName(ActivityType type) {
    switch (type) {
        case ActivityType::Lecture:
            return "讲座";
        case ActivityType::Competition:
            return "竞赛";
        case ActivityType::Sport:
            return "体育";
    }
    return "未知";
}

Activity::Activity(const std::string& id, const std::string& name, const std::string& time,
                   const std::string& place, int capacity, ActivityType type)
    : id_(id), name_(name), time_(time), place_(place), capacity_(capacity), type_(type) {
}

Activity::~Activity() {
}

const std::string& Activity::getId() const {
    return id_;
}

const std::string& Activity::getName() const {
    return name_;
}

const std::string& Activity::getTime() const {
    return time_;
}

const std::string& Activity::getPlace() const {
    return place_;
}

int Activity::getCapacity() const {
    return capacity_;
}

ActivityType Activity::getType() const {
    return type_;
}

int Activity::getRegisteredCount() const {
    return static_cast<int>(registrants_.size());
}

const std::vector<std::string>& Activity::getRegistrants() const {
    return registrants_;
}

bool Activity::isFull() const {
    return getRegisteredCount() >= capacity_;
}

bool Activity::hasRegistrant(const std::string& userId) const {
    for (std::size_t i = 0; i < registrants_.size(); ++i) {
        if (registrants_[i] == userId) {
            return true;
        }
    }
    return false;
}

bool Activity::updateBasicInfo(const std::string& name, const std::string& time,
                               const std::string& place, int capacity) {
    if (name.empty() || time.empty() || place.empty()) {
        return false;
    }
    if (capacity <= 0 || capacity < getRegisteredCount()) {
        return false;  // 容量不能小于已报名人数，否则人数与容量会互相矛盾
    }
    name_ = name;
    time_ = time;
    place_ = place;
    capacity_ = capacity;
    return true;
}

RegisterResult Activity::registerUser(const std::string& userId) {
    if (hasRegistrant(userId)) {
        return RegisterResult::AlreadyRegistered;  // 同一用户不能重复报名同一活动
    }
    if (isFull()) {
        return RegisterResult::FullActivity;  // 满员后不再接受报名
    }
    registrants_.push_back(userId);
    return RegisterResult::Success;
}

bool Activity::unregisterUser(const std::string& userId) {
    for (std::size_t i = 0; i < registrants_.size(); ++i) {
        if (registrants_[i] == userId) {
            registrants_.erase(registrants_.begin() + static_cast<std::ptrdiff_t>(i));
            return true;
        }
    }
    return false;  // 没有报名记录时集合保持不变
}

std::string Activity::describeCommon() const {
    std::ostringstream out;
    out << "[" << id_ << "] " << name_
        << " | 类型: " << activityTypeName(type_)
        << " | 时间: " << time_
        << " | 地点: " << place_
        << " | 容量: " << capacity_
        << " | 已报名: " << getRegisteredCount();
    return out.str();
}

std::string activitySpecificInfo(const Activity& activity) {
    // type_ 在派生类构造时就被固定，因此按类型向下转型是安全的。
    // 显示特有信息是第四次上机的需求；第五次上机会把这里的分支改成虚函数，
    // 让基类指针自己完成“显示本类型信息”。
    switch (activity.getType()) {
        case ActivityType::Lecture:
            return "主讲人: " + static_cast<const Lecture&>(activity).getSpeaker();
        case ActivityType::Competition:
            return "参赛要求: " + static_cast<const Competition&>(activity).getRequirement();
        case ActivityType::Sport:
            return "体育项目: " + static_cast<const Sport&>(activity).getSportName();
    }
    return "";
}
