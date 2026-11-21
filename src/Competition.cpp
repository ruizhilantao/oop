#include "Competition.h"

Competition::Competition(const std::string& id, const std::string& name, const std::string& time,
                         const std::string& place, int capacity, const std::string& requirement,
                         int requiredLevel, int fee)
    : Activity(id, name, time, place, capacity),
      requirement_(requirement),
      requiredLevel_(requiredLevel),
      fee_(fee) {
}

std::string Competition::typeName() const {
    return "竞赛";
}

std::string Competition::specificInfo() const {
    return "参赛要求: " + requirement_ + " | 最低参赛等级: " + std::to_string(requiredLevel_) +
           " | 报名费: " + std::to_string(fee_) + " 元";
}

std::string Competition::ruleText() const {
    return "竞赛：参赛等级不低于 " + std::to_string(requiredLevel_) + " 级，报名费 " +
           std::to_string(fee_) + " 元";
}

// 先调用基类完成所有活动共有的容量检查，再追加竞赛特有的资格与费用规则。
RegistrationCheck Competition::checkRegistration(const User& user) const {
    RegistrationCheck check = Activity::checkRegistration(user);

    check.rule = ruleText();
    if (!check.allowed) {
        return check;  // 已经满员：直接返回基类的拒绝结果
    }

    if (user.getLevel() < requiredLevel_) {
        check.allowed = false;
        check.qualification = "资格不通过";
        check.reason = "参赛等级不足：活动要求 " + std::to_string(requiredLevel_) + " 级，用户 " +
                       user.getId() + " 为 " + std::to_string(user.getLevel()) + " 级";
        return check;
    }

    check.qualification = "资格通过：用户等级 " + std::to_string(user.getLevel()) + " ≥ 要求 " +
                          std::to_string(requiredLevel_);
    check.fee = fee_;
    return check;
}

const std::string& Competition::getRequirement() const {
    return requirement_;
}

int Competition::getRequiredLevel() const {
    return requiredLevel_;
}

int Competition::getFee() const {
    return fee_;
}
