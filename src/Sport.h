#ifndef SPORT_H
#define SPORT_H

#include "Activity.h"

#include <string>

// 体育类活动：公共信息继承自 Activity，特有信息是体育项目与固定费用。
// 报名规则：人数上限（基类的容量检查）+ 固定费用。
class Sport : public Activity {
public:
    Sport(const std::string& id, const std::string& name, const std::string& time,
          const std::string& place, int capacity, const std::string& sportName, int fee);

    std::string typeName() const override;
    std::string specificInfo() const override;
    std::string ruleText() const override;
    // 重写：容量检查 + 固定费用
    RegistrationCheck checkRegistration(const User& user) const override;

    const std::string& getSportName() const;
    int getFee() const;

private:
    std::string sportName_;
    int fee_;
};

#endif
