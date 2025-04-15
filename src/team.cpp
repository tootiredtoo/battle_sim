#include "team.hpp"

Team::Team(std::string name) : name_(std::move(name)) {}

void Team::addFighter(const Fighter& fighter) {
    fighters_.push_back(fighter);
}

std::vector<Fighter>& Team::getFighters() {
    return fighters_;
}

bool Team::hasAliveFighters() const {
    for (const auto& f : fighters_) {
        if (f.isAlive()) return true;
    }
    return false;
}
