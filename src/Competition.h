#ifndef COMPETITION_H
#define COMPETITION_H

#include "Activity.h"

#include <string>

// 竞赛：公共信息继承自 Activity，特有信息是参赛要求、最低参赛等级与报名费用。
// 报名规则最严格：先过基类的容量检查，再检查参赛资格（用户等级），最后计算费用。
class Competition : public Activity {
public:
    Competition(const std::string& id, const std::string& name, const std::string& time,
                const std::string& place, int capacity, const std::string& requirement,
                int requiredLevel, int fee);

    std::string typeName() const override;
    std::string specificInfo() const override;
    std::string ruleText() const override;
    // 重写：容量检查 + 资格检查 + 费用计算
    RegistrationCheck checkRegistration(const User& user) const override;

    const std::string& getRequirement() const;
    int getRequiredLevel() const;
    int getFee() const;

private:
    std::string requirement_;
    int requiredLevel_;
    int fee_;
};

#endif
