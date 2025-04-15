#include "battle_manager.hpp"

#include <iostream>
#include <thread>
#include <future>

void BattleManager::setup() {
    Team teamA("Alpha");
    Team teamB("Bravo");

    teamA.addFighter(Fighter("A1", 100, 15, 5));
    teamA.addFighter(Fighter("A2", 90, 18, 3));
    teamB.addFighter(Fighter("B1", 95, 14, 4));
    teamB.addFighter(Fighter("B2", 85, 20, 2));

    teams_.push_back(teamA);
    teams_.push_back(teamB);

    for (auto& team : teams_) {
        for (auto& fighter : team.getFighters()) {
            allFighters_.push_back(&fighter);
        }
    }
}

void BattleManager::run() {
    while (!gameOver_) {
        processTurn();
        checkVictory();
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
    }
}

void BattleManager::processTurn() {
    std::cout << "== New Turn ==\n";

    std::vector<std::pair<Fighter*, std::future<std::string>>> tasks;

    for (auto* fighter : allFighters_) {
        if (!fighter->isAlive()) continue;

        std::promise<std::string> promise;
        auto fut = promise.get_future();

        std::thread([fighter, promise = std::move(promise)]() mutable {
            fighter->planAction(promise);
        }).detach();

        tasks.emplace_back(fighter, std::move(fut));
    }

    for (auto& [fighter, fut] : tasks) {
        std::string action = fut.get();
        fighter->applyAction(action);
    }
}

void BattleManager::checkVictory() {
    int aliveTeams = 0;
    for (const auto& team : teams_) {
        if (team.hasAliveFighters()) ++aliveTeams;
    }
    if (aliveTeams <= 1) {
        gameOver_ = true;
        std::cout << "Game Over!\n";
    }
}
