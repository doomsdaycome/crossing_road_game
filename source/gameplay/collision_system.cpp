#include "gameplay/collision_system.hpp"
#include "entities/player.hpp"
#include "gameplay/lane_manager.hpp"
#include "entities/lane.hpp"
#include "entities/monster.hpp"
#include <SFML/Graphics.hpp>

bool CollisionSystem::checkPlayerVsMonsters(const Player& player, const LaneManager& laneManager) {
    if (!player.isAlive()) return false;

    sf::FloatRect playerBounds = player.getGlobalBounds();

    for (const auto& lanePtr : laneManager.getLanes()) {
        for (const auto& monster : lanePtr->getMonsters()) {
            if (playerBounds.findIntersection(monster.getGlobalBounds()).has_value()) {
                return true;
            }
        }
    }
    return false;
}

