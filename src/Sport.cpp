#include "Sport.h"

Sport::Sport(const std::string& id, const std::string& name, const std::string& time,
             const std::string& place, int capacity, const std::string& sportName, int fee)
    : Activity(id, name, time, place, capacity), sportName_(sportName), fee_(fee) {
}

std::string Sport::typeName() const {
    return "体育";
}

std::string Sport::specificInfo() const {
    return "体育项目: " + sportName_ + " | 固定费用: " + std::to_string(fee_) + " 元";
}

std::string Sport::ruleText() const {
    return "体育：人数上限 " + std::to_string(getCapacity()) + " 人，固定费用 " +
           std::to_string(fee_) + " 元";
}

// 先调用基类完成容量（人数上限）检查，再追加固定费用。
RegistrationCheck Sport::checkRegistration(const User& user) const {
    RegistrationCheck check = Activity::checkRegistration(user);

    check.rule = ruleText();
    if (!check.allowed) {
        return check;
    }
    check.fee = fee_;
    return check;
}

const std::string& Sport::getSportName() const {
    return sportName_;
}

int Sport::getFee() const {
    return fee_;
}
