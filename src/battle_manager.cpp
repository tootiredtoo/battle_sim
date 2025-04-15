#include "battle_manager.hpp"

#include <iostream>
#include <thread>
#include <future>

void BattleManager::setup() {
    Team teamA("Alpha");
    Team teamB("Bravo");

    teamA.addFighter(Fighter("A1", 100, 15, 5, "Alpha"));
    teamA.addFighter(Fighter("A2", 90, 18, 3, "Alpha"));
    teamB.addFighter(Fighter("B1", 95, 14, 4, "Bravo"));
    teamB.addFighter(Fighter("B2", 85, 20, 2, "Bravo"));

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

        std::vector<Fighter*> enemies;
        for (auto* candidate : allFighters_) {
            if (candidate->isAlive() && candidate->getTeamName() != fighter->getTeamName()) {
                enemies.push_back(candidate);
            }
        }

        std::thread([fighter,
                     promise = std::move(promise),
                     enemies]() mutable {
            fighter->planAction(promise, enemies);
        }).detach();

        tasks.emplace_back(fighter, std::move(fut));
    }

    for (auto& [fighter, fut] : tasks) {
        try {
            std::string action = fut.get();
            fighter->applyAction(action); // TODO: add logic
        } catch (const std::exception& ex) {
            std::cerr << "Error getting action for " << fighter->getName()
                      << ": " << ex.what() << '\n';
        }
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

BattleManager& BattleManager::getInstance() {
    static BattleManager instance;
    return instance;
}

const std::vector<Fighter*>& BattleManager::getAllFighters() const {
    return allFighters_;
}