#pragma once
#include <string>
#include <atomic>
#include <mutex>
#include <future>

class Fighter {
public:
    Fighter(std::string name, int health, int attack, int defense);
    Fighter(const Fighter& other); // Copy constructor

    void planAction(std::promise<std::string>& resultPromise);
    void applyAction(const std::string& action);

    bool isAlive() const;
    const std::string& getName() const;

private:
    std::string name_;
    int health_;
    int attack_;
    int defense_;
    std::mutex mutex_;
    std::atomic<bool> alive_;
};
