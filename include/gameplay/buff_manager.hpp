#pragma once

class BuffManager {
 public:
  BuffManager();

  void update(float dt);

  // Buff properties
  bool isMagnetActive() const;
  bool isInvisible() const;

  // Actions
  bool consumeShield();
  bool consumeHolyNova();

  // Using active items
  void activateMagnet(float duration);
  void activateInvisibility(float duration);
  bool consumeMagnetItem(float duration);
  bool consumeInvisibilityItem(float duration);

  // Sync with ScoreManager
  void setCounts(int magnet, int shield, int invis, int nova);
  int getMagnetCount() const;
  int getShieldCount() const;
  int getInvisCount() const;
  int getNovaCount() const;

  // For testing/cheating or shop integration
  void addShields(int count);
  void addHolyNovas(int count);
  void addMagnets(int count);
  void addInvisibilityCloaks(int count);

 private:
  // Quantities (Defaulting to 0)
  int shieldCount_ = 0;
  int magnetCount_ = 0;
  int invisibilityCount_ = 0;
  int holyNovaCount_ = 0;

  // Active State Timers
  bool magnetActive_ = false;
  float magnetTimer_ = 0.f;

  bool invisibilityActive_ = false;
  float invisibilityTimer_ = 0.f;
};
