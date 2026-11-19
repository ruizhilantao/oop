#include "Sport.h"

Sport::Sport(const std::string& id, const std::string& name, const std::string& time,
             const std::string& place, int capacity, const std::string& sportName)
    : Activity(id, name, time, place, capacity, ActivityType::Sport), sportName_(sportName) {
}

const std::string& Sport::getSportName() const {
    return sportName_;
}
