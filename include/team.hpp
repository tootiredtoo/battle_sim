#pragma once
#include <string>
#include <vector>
#include "fighter.hpp"

class Team {
public:
    Team(std::string name);

    void addFighter(const Fighter& fighter);
    std::vector<Fighter>& getFighters();
    const std::string& getName() const;
    bool hasAliveFighters() const;

private:
    std::string name_;
    std::vector<Fighter> fighters_;
};
