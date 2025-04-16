#include "battle_manager.hpp"

#include <iostream>
#include <thread>
#include <future>

void BattleManager::setup() {
    Team teamA("Alpha");
    Team teamB("Bravo");
    Team teamC("Charlie");

    teamA.addFighter(Fighter("A1", 100, 15, 5, teamA.getName()));
    teamA.addFighter(Fighter("A2", 90, 18, 3, teamA.getName()));
    teamB.addFighter(Fighter("B1", 95, 14, 4, teamB.getName()));
    teamB.addFighter(Fighter("B2", 85, 20, 2, teamB.getName()));
    teamC.addFighter(Fighter("C1", 90, 18, 3, teamC.getName()));
    teamC.addFighter(Fighter("C2", 95, 16, 4, teamC.getName()));

    teams_.push_back(teamA);
    teams_.push_back(teamB);
    teams_.push_back(teamC);

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
            fighter->applyAction(action);
        } catch (const std::exception& ex) {
            std::cerr << "Error getting action for " << fighter->getName()
                      << ": " << ex.what() << '\n';
        }
    }
}

void BattleManager::checkVictory() {
    int aliveTeams = 0;
    std::string winningTeam;

    for (const auto& team : teams_) {
        if (team.hasAliveFighters()) {
            ++aliveTeams;
            winningTeam = team.getName();  // Store the last alive team
        }
    }

    if (aliveTeams == 0) {
        std::cout << "\n==============================\n All teams are defeated!\n==============================\n";
        gameOver_ = true;
    } else if (aliveTeams <= 1) {
        std::cout << "\n==============================\n Team " << winningTeam << " wins the battle!\n==============================\n";
        gameOver_ = true;
    }
}

BattleManager& BattleManager::getInstance() {
    static BattleManager instance;
    return instance;
}

const std::vector<Fighter*>& BattleManager::getAllFighters() const {
    return allFighters_;
}