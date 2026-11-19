#ifndef COMPETITION_H
#define COMPETITION_H

#include "Activity.h"

#include <string>

// 竞赛：公共信息继承自 Activity，特有信息是参赛要求。
class Competition : public Activity {
public:
    Competition(const std::string& id, const std::string& name, const std::string& time,
                const std::string& place, int capacity, const std::string& requirement);

    const std::string& getRequirement() const;

private:
    std::string requirement_;
};

#endif
