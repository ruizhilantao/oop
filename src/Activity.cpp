#include "Activity.h"

#include <cstddef>
#include <sstream>

std::string describeCheckResult(const RegistrationCheck& check) {
    std::ostringstream out;
    out << (check.allowed ? "允许报名" : "拒绝报名");
    out << " | 规则: " << check.rule;
    out << " | 资格: " << check.qualification;
    out << " | 费用: " << check.fee << " 元";
    if (!check.allowed && !check.reason.empty()) {
        out << " | 原因: " << check.reason;
    }
    return out.str();
}

Activity::Activity(const std::string& id, const std::string& name, const std::string& time,
                   const std::string& place, int capacity)
    : id_(id), name_(name), time_(time), place_(place), capacity_(capacity) {
}

Activity::~Activity() {
}

// 基类只负责所有活动都有的规则：剩余容量。
// 派生类在自己的 checkRegistration() 里先调用它，再追加本类型的规则，
// 因此“容量检查”只有一份实现，不会在各类型里重复。
RegistrationCheck Activity::checkRegistration(const User&) const {
    RegistrationCheck check;
    check.rule = ruleText();  // 虚函数：调用当前活动类型自己的规则文本
    check.qualification = "无需资格检查";

    if (isFull()) {
        check.allowed = false;
        check.reason = "名额已满：活动 " + id_ + " 已报名 " + std::to_string(getRegisteredCount()) +
                       "/" + std::to_string(capacity_) + " 人";
        return check;
    }

    check.allowed = true;
    check.fee = 0;
    return check;
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

bool Activity::addRegistrant(const std::string& userId) {
    if (userId.empty() || hasRegistrant(userId)) {
        return false;  // 重复报名不能写入第二条记录
    }
    registrants_.push_back(userId);
    return true;
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
        << " | 类型: " << typeName()
        << " | 时间: " << time_
        << " | 地点: " << place_
        << " | 容量: " << capacity_
        << " | 已报名: " << getRegisteredCount();
    return out.str();
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
