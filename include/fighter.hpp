#pragma once

#include <string>
#include <atomic>
#include <mutex>
#include <future>
#include <vector>

class Fighter {
public:
    Fighter(const std::string& name, const int health, const int attack, const int defense, const std::string& teamName);
    Fighter(const Fighter& other);

    void planAction(std::promise<std::string>& actionPromise,
        const std::vector<Fighter*>& enemies);

    void applyAction(const std::string& action);

    void takeDamage(int amount);

    bool isAlive() const;
    const std::string& getName() const;
    const std::string& getTeamName() const;
    const void printStats() const;

private:
    std::string name_;
    int health_;
    int attack_;
    int defense_;
    std::mutex mutex_;
    std::atomic<bool> alive_;
    std::string teamName_;
};
