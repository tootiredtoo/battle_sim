#include "battle_manager.hpp"

int main() {
    auto& manager = BattleManager::getInstance();
    manager.setup();
    manager.run();
    return 0;
}
