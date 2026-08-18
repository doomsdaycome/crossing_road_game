#include "gameplay/collision_system.hpp"
#include "entities/player.hpp"
#include "gameplay/lane_manager.hpp"
#include "entities/monster.hpp"
#include "entities/carpet.hpp"
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

// Sửa tham số thành Player& để có thể gọi setRiding()
bool CollisionSystem::checkPlayerVsMonsters(Player& player, const LaneManager& laneManager) {
    sf::FloatRect rawPlayerBox = player.getGlobalBounds();
    
    // Bóp hitbox Player còn 60% bề ngang và 80% chiều cao
    sf::FloatRect playerBox = scaleRect(rawPlayerBox, 0.6f, 0.8f);

    // Reset trạng thái riding mỗi frame, nếu tí nữa phát hiện đang chạm thảm thì mới set true
    player.setRiding(false, 0.f);

    for (const auto& lane : laneManager.getLanes()) {
        if (lane->getType() == LaneType::REST) continue;

        // Xử lý Làn vực thẳm (CHASM) - Inverted Collision
        if (lane->getType() == LaneType::CHASM) {
            // Kiểm tra xem người chơi có nằm trong làn này không
            // Người chơi nằm trên Làn nếu tọa độ Y của người chơi nằm trong phạm vi của Làn
            float laneTop = lane->getYPosition();
            float laneBottom = laneTop + Config::LANE_HEIGHT;
            float playerCenterY = rawPlayerBox.position.y + rawPlayerBox.size.y / 2.f;

            if (playerCenterY >= laneTop && playerCenterY <= laneBottom) {
                bool isTouchingCarpet = false;
                for (const auto& carpet : lane->getCarpets()) {
                    sf::FloatRect rawCarpetBox = carpet.getGlobalBounds();
                    sf::FloatRect carpetBox = scaleRect(rawCarpetBox, 0.9f, 0.9f); // Bóp hitbox thảm một chút để dễ đứng
                    
                    if (playerBox.findIntersection(carpetBox).has_value()) {
                        isTouchingCarpet = true;
                        player.setRiding(true, carpet.getSpeed());
                        break;
                    }
                }

                // INVERTED COLLISION: Nếu ở trên làn vực mà KHÔNG chạm thảm nào -> CHẾT
                if (!isTouchingCarpet) {
                    return true;
                }
            }
            continue; // Bỏ qua việc check quái thường cho làn này
        }

        // Xử lý Làn thường (ROAD, GRASS)
        for (const auto& monster : lane->getMonsters()) {
            sf::FloatRect rawMonsterBox = monster.getGlobalBounds();
            
            // Bóp hitbox Quái vật còn 80% x 80%
            sf::FloatRect monsterBox = scaleRect(rawMonsterBox, 0.8f, 0.8f);

            // SFML 3.x: Dùng findIntersection thay cho intersects
            if (playerBox.findIntersection(monsterBox).has_value()) {
                return true; 
            }
        }
    }
    return false; 
}

int CollisionSystem::checkAndCollectCoins(const Player& player, LaneManager& laneManager) {
    // Lấy tọa độ thật của nhân vật
    float pX = static_cast<float>(player.getGridX() * Config::TILE_SIZE);
    float pY = static_cast<float>(player.getGridY() * Config::TILE_SIZE);

    for (const auto& lanePtr : laneManager.getLanes()) {
        // Tìm đúng cái Làn có tọa độ Y khớp với Y của nhân vật (sai số nhỏ hơn 5px)
        if (std::abs(lanePtr->getYPosition() - pY) < 5.0f) {
            // Nhờ Làn đó xử lý việc nhặt vàng và trả về kết quả
            return lanePtr->collectItemAt(pX);
        }
    }
    return 0;
}