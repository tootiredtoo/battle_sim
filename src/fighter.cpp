#include "fighter.hpp"
#include <chrono>
#include <thread>
#include <iostream>

Fighter::Fighter(std::string name, int health, int attack, int defense)
    : name_(std::move(name)), health_(health), attack_(attack), defense_(defense), alive_(true) {}

void Fighter::planAction(std::promise<std::string>& resultPromise) {
    std::this_thread::sleep_for(std::chrono::milliseconds(50 + rand() % 100));
    resultPromise.set_value("Attack"); // Placeholder logic
}

void Fighter::applyAction(const std::string& action) {
    std::lock_guard<std::mutex> lock(mutex_);
    if (action == "Attack") {
        std::cout << name_ << " attacks!\n";
    }
}

bool Fighter::isAlive() const {
    return alive_;
}

const std::string& Fighter::getName() const {
    return name_;
}

Fighter::Fighter(const Fighter& other)
    : name_(other.name_), health_(other.health_), attack_(other.attack_) {
    std::cout << "Fighter copied: " << name_ << "\n";
}
