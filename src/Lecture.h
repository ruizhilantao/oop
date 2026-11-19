#ifndef LECTURE_H
#define LECTURE_H

#include "Activity.h"

#include <string>

// 讲座：公共信息全部继承自 Activity，特有信息是主讲人。
class Lecture : public Activity {
public:
    Lecture(const std::string& id, const std::string& name, const std::string& time,
            const std::string& place, int capacity, const std::string& speaker);

    const std::string& getSpeaker() const;

private:
    std::string speaker_;
};

#endif
