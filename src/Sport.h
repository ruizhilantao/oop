#ifndef SPORT_H
#define SPORT_H

#include "Activity.h"

#include <string>

// 体育类活动：公共信息继承自 Activity，特有信息是体育项目名称。
class Sport : public Activity {
public:
    Sport(const std::string& id, const std::string& name, const std::string& time,
          const std::string& place, int capacity, const std::string& sportName);

    const std::string& getSportName() const;

private:
    std::string sportName_;
};

#endif
