#include "gameplay/collision_system.hpp"
#include "entities/player.hpp"
#include "gameplay/lane_manager.hpp"
#include "entities/lane.hpp"
#include "entities/monster.hpp"
#include <SFML/Graphics.hpp>

sf::FloatRect scaleRect(const sf::FloatRect& rect, float scaleX, float scaleY) {
    sf::FloatRect scaled;
    
    // SFML 3.x dùng .size thay vì .width và .height
    scaled.size.x = rect.size.x * scaleX;
    scaled.size.y = rect.size.y * scaleY;
    
    // SFML 3.x dùng .position thay vì .left và .top
    // Vẫn giữ nguyên logic tính toán để hitbox nằm ở tâm bức ảnh
    scaled.position.x = rect.position.x + (rect.size.x - scaled.size.x) / 2.f;
    scaled.position.y = rect.position.y + (rect.size.y - scaled.size.y) / 2.f;
    
    return scaled;
}

bool CollisionSystem::checkPlayerVsMonsters(const Player& player, const LaneManager& laneManager) {
    sf::FloatRect rawPlayerBox = player.getGlobalBounds();
    
    // Bóp hitbox Player còn 60% bề ngang và 80% chiều cao
    sf::FloatRect playerBox = scaleRect(rawPlayerBox, 0.6f, 0.8f);

    for (const auto& lane : laneManager.getLanes()) {
        if (lane->getType() == LaneType::REST) continue;

        for (const auto& monster : lane->getMonsters()) {
            sf::FloatRect rawMonsterBox = monster.getGlobalBounds();
            
            // Bóp hitbox Quái vật còn 80% x 80%
            sf::FloatRect monsterBox = scaleRect(rawMonsterBox, 0.8f, 0.8f);

            // SFML 3.x: Dùng findIntersection thay cho intersects
            // Nó trả về std::optional, nên ta dùng .has_value() để check xem có va chạm không
            if (playerBox.findIntersection(monsterBox).has_value()) {
                return true; 
            }
        }
    }
    return false; 
}

