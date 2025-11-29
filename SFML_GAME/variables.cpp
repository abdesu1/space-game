#include <iostream>
#include <SFML/graphics.hpp>
#pragma once


extern const int MAX_ENEMIES = 18;
extern const int MAX_BULLETS = 20;
extern const int MAX_SPARKLES = 30;
extern const int MAX_SHOOTERS = 5;
extern const int MAX_SHOOTER_BULLETS = MAX_SHOOTERS;




int enemiesAlive = 0;

extern sf::Vector2f playerShipPosition(350, 700);



extern sf::Clock respawnTimer;
extern sf::Clock shootTimer;
extern sf::Clock sparkleTimer;
extern sf::Clock selectorTimer;
extern sf::Clock pauseTimer;
extern sf::Clock healthTimer;
extern sf::Clock levelUpTimer;
extern sf::Clock shooterRespawnTimer;
extern sf::Clock shooterBulletTimer;
extern sf::Clock retriesTimer;
extern sf::Clock powerUpTimer;

extern const float shootDelaySeconds = 0.5f;
extern const float spawnDelaySeconds = 0.3f;
extern const float sSpawnDelaySeconds = 0.5f;
extern const float selectionDelaySeconds = 0.2f;
extern const float pauseDelaySeconds = 0.5f;
extern const float healthDelaySeconds = 0.5f;
extern const float levelUpTimerDelaySeconds = 3.0f;
extern const float shooterRespawnDelaySeconds = 1.2f;
extern const float shooterBulletDelaySeconds = 0.9f;
extern const float retriesDelaySeconds = 0.2f;
extern const float powerUpDelaySeconds = 20.0f;

extern const float ENEMY_WIDTH = 45.0f;
extern const float ENEMY_HEIGHT = 30.0;
extern const float X_SPACE = 60.0f;
extern const float Y_SPACE = 60.0f;