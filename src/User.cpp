#include "User.h"

User::User() : level_(kMinLevel) {
}

User::User(const std::string& id, const std::string& name, const std::string& contact, int level)
    : id_(id), name_(name), contact_(contact), level_(level) {
}

const std::string& User::getId() const {
    return id_;
}

const std::string& User::getName() const {
    return name_;
}

const std::string& User::getContact() const {
    return contact_;
}

int User::getLevel() const {
    return level_;
}

bool User::setName(const std::string& name) {
    if (name.empty()) {
        return false;  // 空姓名会破坏数据完整性，直接拒绝
    }
    name_ = name;
    return true;
}

bool User::setContact(const std::string& contact) {
    if (contact.empty()) {
        return false;
    }
    contact_ = contact;
    return true;
}

bool User::setLevel(int level) {
    if (level < kMinLevel || level > kMaxLevel) {
        return false;  // 等级越界：拒绝，保持原值
    }
    level_ = level;
    return true;
}

std::string User::describe() const {
    return "[" + id_ + "] " + name_ + " | 联系方式: " + contact_ + " | 等级: " +
           std::to_string(level_);
}
