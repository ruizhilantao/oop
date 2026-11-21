#ifndef LECTURE_H
#define LECTURE_H

#include "Activity.h"

#include <string>

// 讲座：公共信息继承自 Activity，特有信息是主讲人。
// 报名规则最简单：只要求剩余容量足够，免费参加。
// 它没有重写 checkRegistration()，直接复用基类的“容量检查”实现，
// 只用 ruleText() 给出本类型的规则说明。
class Lecture : public Activity {
public:
    Lecture(const std::string& id, const std::string& name, const std::string& time,
            const std::string& place, int capacity, const std::string& speaker);

    std::string typeName() const override;
    std::string specificInfo() const override;
    std::string ruleText() const override;

    const std::string& getSpeaker() const;

private:
    std::string speaker_;
};

#endif
