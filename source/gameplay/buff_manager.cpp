#include "gameplay/buff_manager.hpp"

BuffManager::BuffManager() {
}

void BuffManager::update(float dt) {
    if (magnetActive_) {
        magnetTimer_ -= dt;
        if (magnetTimer_ <= 0.f) {
            magnetActive_ = false;
            magnetTimer_ = 0.f;
        }
    }

    if (invisibilityActive_) {
        invisibilityTimer_ -= dt;
        if (invisibilityTimer_ <= 0.f) {
            invisibilityActive_ = false;
            invisibilityTimer_ = 0.f;
        }
    }
}

bool BuffManager::isMagnetActive() const {
    return magnetActive_;
}

bool BuffManager::isInvisible() const {
    return invisibilityActive_;
}

bool BuffManager::consumeShield() {
    if (shieldCount_ > 0) {
        shieldCount_--;
        return true;
    }
    return false;
}

bool BuffManager::consumeHolyNova() {
    if (holyNovaCount_ > 0) {
        holyNovaCount_--;
        return true;
    }
    return false;
}

void BuffManager::activateMagnet(float duration) {
    magnetActive_ = true;
    magnetTimer_ = duration;
}

void BuffManager::activateInvisibility(float duration) {
    invisibilityActive_ = true;
    invisibilityTimer_ = duration;
}

bool BuffManager::consumeMagnetItem(float duration) {
    if (magnetCount_ > 0) {
        magnetCount_--;
        activateMagnet(duration);
        return true;
    }
    return false;
}

bool BuffManager::consumeInvisibilityItem(float duration) {
    if (invisibilityCount_ > 0) {
        invisibilityCount_--;
        activateInvisibility(duration);
        return true;
    }
    return false;
}

void BuffManager::setCounts(int magnet, int shield, int invis, int nova) {
    magnetCount_ = magnet;
    shieldCount_ = shield;
    invisibilityCount_ = invis;
    holyNovaCount_ = nova;
}

int BuffManager::getMagnetCount() const { return magnetCount_; }
int BuffManager::getShieldCount() const { return shieldCount_; }
int BuffManager::getInvisCount() const { return invisibilityCount_; }
int BuffManager::getNovaCount() const { return holyNovaCount_; }

void BuffManager::addShields(int count) { shieldCount_ += count; }
void BuffManager::addHolyNovas(int count) { holyNovaCount_ += count; }
void BuffManager::addMagnets(int count) { magnetCount_ += count; }
void BuffManager::addInvisibilityCloaks(int count) { invisibilityCount_ += count; }
