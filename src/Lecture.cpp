#include "Lecture.h"

Lecture::Lecture(const std::string& id, const std::string& name, const std::string& time,
                 const std::string& place, int capacity, const std::string& speaker)
    : Activity(id, name, time, place, capacity, ActivityType::Lecture), speaker_(speaker) {
}

const std::string& Lecture::getSpeaker() const {
    return speaker_;
}
