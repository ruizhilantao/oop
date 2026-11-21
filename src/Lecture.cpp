#include "Lecture.h"

Lecture::Lecture(const std::string& id, const std::string& name, const std::string& time,
                 const std::string& place, int capacity, const std::string& speaker)
    : Activity(id, name, time, place, capacity), speaker_(speaker) {
}

std::string Lecture::typeName() const {
    return "讲座";
}

std::string Lecture::specificInfo() const {
    return "主讲人: " + speaker_;
}

std::string Lecture::ruleText() const {
    return "讲座：只检查剩余容量，免费参加";
}

const std::string& Lecture::getSpeaker() const {
    return speaker_;
}
