#include "Competition.h"

Competition::Competition(const std::string& id, const std::string& name, const std::string& time,
                         const std::string& place, int capacity, const std::string& requirement)
    : Activity(id, name, time, place, capacity, ActivityType::Competition),
      requirement_(requirement) {
}

const std::string& Competition::getRequirement() const {
    return requirement_;
}
