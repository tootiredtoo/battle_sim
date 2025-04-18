#include "fighter.hpp"
#include "battle_manager.hpp"
#include <chrono>
#include <thread>
#include <iostream>
#include <sstream>

Fighter::Fighter(const std::string& name, const int health, const int attack, const int defense, const std::string& teamName)
    : name_(std::move(name)), health_(health), attack_(attack), defense_(defense), teamName_(std::move(teamName)), alive_(true) {
        std::cout << "Fighter created\n";
        printStats();
    }

Fighter::Fighter(const Fighter& other)
    : name_(other.name_), health_(other.health_), attack_(other.attack_), defense_(other.defense_), teamName_(other.teamName_) {
    std::cout << "Fighter copied: " << name_ << "\n";
}

void Fighter::planAction(std::promise<std::string>& actionPromise,
                        const std::vector<Fighter*>& enemies) {
    std::this_thread::sleep_for(std::chrono::milliseconds(50 + rand() % 100)); // Simulate thinking

    std::vector<Fighter*> aliveEnemies;
    for (auto* enemy : enemies) {
        if (enemy->isAlive()) {
            aliveEnemies.push_back(enemy);
        }
    }

    if (aliveEnemies.empty()) {
        actionPromise.set_value("wait");
        return;
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(0, static_cast<int>(aliveEnemies.size()) - 1);
    int targetIndex = distrib(gen);
    
    Fighter* target = aliveEnemies[targetIndex];

    std::ostringstream oss;
    oss << "attack:" << target->getTeamName() << ":" << target->getName();
    actionPromise.set_value(oss.str());
}


void Fighter::applyAction(const std::string& action) {
    std::istringstream iss(action);
    std::string verb, targetTeam, targetName;

    std::cout << name_ << " has planned action: " << action << "\n";

    if (!std::getline(iss, verb, ':') ||
        !std::getline(iss, targetTeam, ':') ||
        !std::getline(iss, targetName)) {
        std::cerr << name_ << ": Invalid action format: " << action << "\n";
        return;
    }

    if (verb == "attack") {
        Fighter* target = nullptr;
        for (auto* f : BattleManager::getInstance().getAllFighters()) {
            if (f->getTeamName() == targetTeam && f->getName() == targetName && f->isAlive()) {
                target = f;
                break;
            }
        }

        if (target) {
            std::cout << name_ << " attacks " << targetName << "!\n";
            target->takeDamage(attack_);
        } else {
            std::cout << name_ << " tried to attack " << targetName << ", but target is gone.\n";
        }
    }
}

bool Fighter::isAlive() const {
    return alive_;
}

const std::string& Fighter::getName() const {
    return name_;
}

const std::string& Fighter::getTeamName() const {
    return teamName_;
}

void Fighter::takeDamage(int amount) {
    health_ -= amount;
    if (health_ <= 0) {
        health_ = 0;
        alive_ = false;
    }
    std::cout << name_ << " takes " << amount << " damage. Remaining HP: " << health_ << "\n";
}

const void Fighter::printStats() const {
    std::cout << "Fighter: " << name_ << "\n"
              << "Team: " << teamName_ << "\n"
              << "Health: " << health_ << "\n"
              << "Attack: " << attack_ << "\n"
              << "Defense: " << defense_ << "\n";
}