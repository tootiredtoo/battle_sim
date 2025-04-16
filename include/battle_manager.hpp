#pragma once

#include "fighter.hpp"
#include "team.hpp"

#include <vector>
#include <memory>

class BattleManager {
public:
    void setup();
    void run();
    void processTurn();
    void checkVictory();

    static BattleManager& getInstance();
    const std::vector<Fighter*>& getAllFighters() const;

private:
    std::vector<Team> teams_;
    std::vector<Fighter*> allFighters_;
    bool gameOver_ = false;

    BattleManager() = default;
    BattleManager(const BattleManager&) = delete;
    BattleManager& operator=(const BattleManager&) = delete;
};
