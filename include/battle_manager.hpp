#pragma once

#include "fighter.hpp"
#include "team.hpp"

#include <vector>
#include <memory>

class BattleManager {
public:
    void setup();   // Create teams, fighters
    void run();     // Turn-based game loop
private:
    std::vector<Team> teams_;
    std::vector<Fighter*> allFighters_; // Flat list for easier action processing
    bool gameOver_ = false;

    void processTurn();
    void checkVictory();
};
