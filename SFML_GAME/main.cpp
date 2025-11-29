#include <cstdint>
#include <iostream>
#include <fstream>
#include <string>
#include <SFML/Graphics.hpp>

//ARRAY LENGTHS
const int MAX_ENEMIES = 18;
const int MAX_BULLETS = 20;
const int MAX_SPARKLES = 30;
const int MAX_SHOOTERS = 5;
const int MAX_SHOOTER_BULLETS = 5;

int score_ = 0;
int retries = 3;
int level = 1;
int prevScore = 0;

// CHANGE CURRENT SCREEN
bool StartScreen = true;
bool InstructionsScreen = false;
bool pauseScreenB = false;
bool levelUp = false;
bool gameOver = false;
bool sheilded = false;


// MENU SELECTION
int selector = 0;

struct Enemy {
    sf::Vector2f Position; 
    sf::Texture texture;
    sf::Sprite rect;
    float speedY = 50.0f;
    sf::Angle speedR = sf::degrees(1.f);
    bool isAlive = true;  

    Enemy() : texture("rock.png"), rect(texture) {
        
    }
};


struct Shooter {
    sf::Vector2f Position;
    sf::Texture texture;
    sf::Sprite rect;
    float speedY = 50.0f;
    bool isAlive = true;

    Shooter() : texture("shooter.png"), rect(texture) {

    }
};


struct PowerUp {
    sf::Vector2f Position;
    sf::Texture texture;
    sf::Sprite rect;
    float speedY = 100.0f;
    bool isVisible = true;

    PowerUp() : texture("p.png"), rect(texture) {
        rect.setScale(sf::Vector2f(2, 2));
    }
};

struct Sheild {
    sf::Vector2f Position;
    sf::Texture texture;
    sf::Sprite rect;
    float speedY = 100.0f;
    bool isVisible = true;

    Sheild() : texture("sheild.png"), rect(texture) {
        rect.setScale(sf::Vector2f(2.5, 2.5));
    }
};


struct Bullet {
    sf::RectangleShape rect;
    sf::Vector2f Position;
    float speedY = 500.f;
    bool shooting = false;
};

struct ShooterBullet {
    sf::RectangleShape rect;
    sf::Vector2f Position;
    float speedY = 300.f;
    bool shooting = false;
};

struct Sparkle {
    sf::RectangleShape rect;
    sf::Vector2f Position;
    float speedY = 300.f;
    sf::Angle speedR = sf::degrees(1.f);
    bool visible = false;
};

Enemy enemiesArray[MAX_ENEMIES];
Bullet bulletArray[MAX_BULLETS];
Sparkle sparklesArray[MAX_SPARKLES];
Shooter shootersArray[MAX_SHOOTERS];
ShooterBullet shooterBulletArray[MAX_SHOOTER_BULLETS];
PowerUp powerUp;
Sheild sheild;

int enemiesAlive = 0;

sf::Vector2f playerShipPosition(350, 700);

sf::Clock respawnTimer;
sf::Clock shootTimer;
sf::Clock sparkleTimer;
sf::Clock selectorTimer;
sf::Clock pauseTimer;
sf::Clock healthTimer;
sf::Clock levelUpTimer;
sf::Clock shooterRespawnTimer;
sf::Clock shooterBulletTimer;
sf::Clock retriesTimer;
sf::Clock powerUpTimer;
sf::Clock nsgTimer;

const float shootDelaySeconds = 0.5f;
const float spawnDelaySeconds = 0.3f;
const float sSpawnDelaySeconds = 0.5f;
const float selectionDelaySeconds = 0.2f;
const float pauseDelaySeconds = 0.5f;
const float healthDelaySeconds = 0.5f;
const float levelUpTimerDelaySeconds = 3.0f;
const float shooterRespawnDelaySeconds = 1.2f;
const float shooterBulletDelaySeconds = 0.01f;
const float retriesDelaySeconds = 0.2f;
const float powerUpDelaySeconds = 20.0f;

const float X_SPACE = 60.0f;
const float Y_SPACE = 60.0f;



void initializeBullets() {
    for (int i = 0; i < MAX_BULLETS; i++) {
        bulletArray[i].rect.setSize(sf::Vector2f(10, 10));
        bulletArray[i].Position = playerShipPosition;
        bulletArray[i].shooting = false;
        bulletArray[i].rect.setPosition(bulletArray[i].Position);
    }
}

void initializeSparkles() {
    for (int i = 0; i < MAX_SPARKLES; i++) {
        float randX = rand() % (800 - 0 + 1) + 0;
        float randY = rand() % (100 - 50 + 1) + 50;

        for (int j = 0; j < MAX_ENEMIES; j++) {
            if (randX + 20 < X_SPACE || randY + 20 < Y_SPACE) {
                randX = rand() % (800 - 0 + 1) + 0;
                randY = rand() % (100 - 50 + 1) + 50;
            }
        }

       
        sparklesArray[i].Position.x = randX;
        sparklesArray[i].Position.y = randY;

        sparklesArray[i].rect.setPosition(sparklesArray[i].Position);
        sparklesArray[i].visible = false;

        sparklesArray[i].rect.setSize(sf::Vector2f(10,10));
        sparklesArray[i].rect.setFillColor(sf::Color::Color(50, 50, 50));

        sparklesArray[i].rect.setOrigin(sf::Vector2f(sparklesArray[i].rect.getSize().x / 2, sparklesArray[i].rect.getSize().y / 2));

    }
}

void initializeShooterBullets() {
    for (int i = 0; i < MAX_SHOOTER_BULLETS; i++) {
        shooterBulletArray[i].rect.setFillColor(sf::Color::Red);
        shooterBulletArray[i].rect.setSize(sf::Vector2f(10, 10));
        shooterBulletArray[i].rect.setOrigin(sf::Vector2f((shooterBulletArray[i].rect.getSize().x) / 2, (shooterBulletArray[i].rect.getSize().y) / 2));
        shooterBulletArray[i].Position = shootersArray[i].rect.getPosition();
        shooterBulletArray[i].rect.setPosition(shooterBulletArray[i].Position);
        shooterBulletArray[i].Position.x -= 5;
        shooterBulletArray[i].shooting = false;
        
    }
}

void initializeShooters() {

    for (int i = 0; i < MAX_SHOOTERS; i++) {
        float randX = rand() % (700 - 100 + 1) + 100;
        float randY = rand() % (200 - 100 + 1) + 100;

        for (int j = 0; j < MAX_SHOOTERS; j++) {
            if (randX + 20 < X_SPACE || randY + 20 < Y_SPACE) {
                randX = rand() % (700 - 100 + 1) + 100;
                randY = rand() % (200 - 100 + 1) + 100;
            }
        }


        shootersArray[i].rect.setScale(sf::Vector2f(0.8, 0.8));
        shootersArray[i].Position = sf::Vector2f(randX, randY);
        shootersArray[i].rect.setPosition(shootersArray[i].Position);
        shootersArray[i].rect.setOrigin(sf::Vector2f((shootersArray[i].texture.getSize().x) / 2, (shootersArray[i].texture.getSize().y) / 2));
        shootersArray[i].rect.setRotation(sf::degrees(180));
        shootersArray[i].isAlive = false;
        shootersArray[i].rect.scale(sf::Vector2f(3,3));

    }


}

void initializePowerUp() {
    powerUp.isVisible = false;
    float randX = rand() % (700 - 100 + 1) + 100;
    float randY = rand() % (200 - 100 + 1) + 100;
    powerUp.Position = sf::Vector2f(randX, randY);
    powerUp.rect.setPosition(powerUp.Position);
}

void initializeEnemies() {

    for (int i = 0; i < MAX_ENEMIES; i++) {
        //SETS ORIGIN TO THE CENTER OF THE IMAGE
        enemiesArray[i].rect.setOrigin(sf::Vector2f((enemiesArray[i].texture.getSize().x) / 2, (enemiesArray[i].texture.getSize().y) / 2));

        float randR = rand() % (60 - 30) + 1;
        randR /= 100;
        float randX = rand() % (700 - 100 + 1) + 100;
        float randY = rand() % (200 - 100 + 1) + 100;

        for (int j = 0; j < MAX_ENEMIES; j++) {
            if (randX + 20 < X_SPACE || randY + 20 < Y_SPACE) {
                randX = rand() % (700 - 100 + 1) + 100;
                randY = rand() % (200 - 100 + 1) + 100;
            }
        }
        
        
        enemiesArray[i].speedR = sf::degrees(randR);
        /*enemiesArray[i].rect.setSize(sf::Vector2f(20.f, 20.f));
        enemiesArray[i].rect.setFillColor(sf::Color::Green);*/

        
        enemiesArray[i].rect.setScale(sf::Vector2f(2.5, 2.5));
        enemiesArray[i].rect.setPosition(enemiesArray[i].Position);
        enemiesArray[i].isAlive = false;

    }

    
}

void updateShootersBullets(float deltaTime, sf::Sprite player) {
    for (int i = 0; i < MAX_SHOOTER_BULLETS; i++) {
        if (level == 4) {
            shooterBulletArray[i].speedY = 450;
            //if (shooterBulletTimer.getElapsedTime().asSeconds() >= 0.6f) {
                if (!shooterBulletArray[i].shooting && shootersArray[i].isAlive) {
                    shooterBulletArray[i].Position = (sf::Vector2f(shootersArray[i].Position.x, shootersArray[i].Position.y));
                    shooterBulletArray[i].shooting = true;
                    //shooterBulletTimer.restart();
                }
            //}
        }
        else if (level == 5) {
            shooterBulletArray[i].speedY = 550;
            //if (shooterBulletTimer.getElapsedTime().asSeconds() >= 0.6f) {
                if (!shooterBulletArray[i].shooting && shootersArray[i].isAlive) {
                    shooterBulletArray[i].Position = shootersArray[i].rect.getPosition();
                    shooterBulletArray[i].shooting = true;
                    
                }
            //}
        }
        else {
            //if (shooterBulletTimer.getElapsedTime().asSeconds() >= shooterBulletDelaySeconds) {
                if (!shooterBulletArray[i].shooting && shootersArray[i].isAlive) {
                    shooterBulletArray[i].Position = shootersArray[i].rect.getPosition();
                    std::cout << shootersArray[i].rect.getPosition().x;
                    shooterBulletArray[i].shooting = true;
                    //shooterBulletTimer.restart();
                }
            //}
        }



        if (shooterBulletArray[i].Position.y > 750) {
            shooterBulletArray[i].Position = shootersArray[i].rect.getPosition();
            shooterBulletArray[i].shooting = false;
        }

        if (shooterBulletArray[i].shooting && shootersArray[i].isAlive) {
            //std::cout << i;
            shooterBulletArray[i].Position.y += shooterBulletArray[i].speedY * deltaTime;
            shooterBulletArray[i].rect.setPosition(shooterBulletArray[i].Position);
            if (shooterBulletArray[i].Position.x + 10 >= playerShipPosition.x && shooterBulletArray[i].Position.x - 45 <= playerShipPosition.x) {
                if (shooterBulletArray[i].Position.y + 15 >= playerShipPosition.y && shooterBulletArray[i].Position.y - 15 <= playerShipPosition.y) {
                    if (retriesTimer.getElapsedTime().asSeconds() >= retriesDelaySeconds) {
                        if (sheilded) {
                            sheilded = false;
                        }
                        else {
                            retries--;
                            retriesTimer.restart();
                        }
                    }

                    shooterBulletArray[i].shooting = false;
                }
            }
            
        }

        //std::cout << "HELLO4";

        shooterBulletArray[i].rect.setPosition(shooterBulletArray[i].Position);
        
    }
}

void updatePowerUp(float deltaTime) {
    if (!powerUp.isVisible) {
        float randX = rand() % (700 - 100 + 1) + 100;
        float randY = rand() % (200 - 100 + 1) + 100;

        powerUp.Position = sf::Vector2f(randX, randY);
        powerUp.rect.setPosition(powerUp.Position);

        if (powerUpTimer.getElapsedTime().asSeconds() >= powerUpDelaySeconds) {
            powerUp.isVisible = true;
            powerUpTimer.restart();
        }
    }
    if (powerUp.isVisible) {
        powerUp.Position.y += powerUp.speedY * deltaTime;
        powerUp.rect.setPosition(powerUp.Position);

        if (powerUp.Position.x + 30 >= playerShipPosition.x && powerUp.Position.x - 30 <= playerShipPosition.x && powerUp.Position.y + 30 >= playerShipPosition.y && powerUp.Position.y - 30 <= playerShipPosition.y) {
            powerUp.isVisible = false;
            sheilded = true;
        }
        if (powerUp.Position.y >= 750) {
            powerUp.isVisible = false;
            powerUpTimer.restart();
        }
    }
    
}

void updateShooters(float deltaTime, sf::RenderWindow& win) {
    for (int i = 0; i < MAX_SHOOTERS; i++) {
        
        if (shootersArray[i].isAlive == false) {


            if (shooterRespawnTimer.getElapsedTime().asSeconds() >= shooterRespawnDelaySeconds)
            {

                float randX = rand() % (700 - 100 + 1) + 100;
                float randY = rand() % (200 - 100 + 1) + 100;

                for (int j = 0; j < MAX_ENEMIES; j++) {
                    if (randX + 20 < X_SPACE || randY + 20 < Y_SPACE) {
                        randX = rand() % (700 - 100 + 1) + 100;
                        randY = rand() % (200 - 100 + 1) + 100;
                    }
                }

                shootersArray[i].Position.x = randX;
                shootersArray[i].Position.y = randY;

                shootersArray[i].isAlive = true;

                shooterRespawnTimer.restart();
            }


        }

        shootersArray[i].Position.y += shootersArray[i].speedY * deltaTime;
        shootersArray[i].rect.setPosition(shootersArray[i].Position);
        

        if (shootersArray[i].Position.y > 750){
            shootersArray[i].isAlive = false;


        }

        if (shootersArray[i].isAlive) {
            
            shootersArray[i].rect.setPosition(shootersArray[i].Position);
            if (shootersArray[i].Position.x + 10 >= playerShipPosition.x && shootersArray[i].Position.x - 45 <= playerShipPosition.x) {
                if (shootersArray[i].Position.y + 15 >= playerShipPosition.y && shootersArray[i].Position.y - 15 <= playerShipPosition.y) {
                    if (retriesTimer.getElapsedTime().asSeconds() >= retriesDelaySeconds) {
                        if (sheilded) {
                            sheilded = false;
                        }
                        else {
                            retries--;
                            retriesTimer.restart();
                        }
                    }

                    shootersArray[i].isAlive = false;
                }

            }
        }

        for (int i = 0; i < MAX_SHOOTERS; i++) {
            
                if (shootersArray[i].Position.x - 32 <= playerShipPosition.x && shootersArray[i].Position.x + 28 >= playerShipPosition.x) {
                    if (shootersArray[i].Position.y - 32 <= playerShipPosition.y && shootersArray[i].Position.y + 28 >= playerShipPosition.y) {
                        if (shootersArray[i].isAlive) {
                            shootersArray[i].isAlive = false;
                            if (retriesTimer.getElapsedTime().asSeconds() >= retriesDelaySeconds) {
                                retries--;
                                retriesTimer.restart();
                            }
                            
                        }
                    }
                
            }
        }


    }
}

void updateSparkles(float deltaTime) {


    for (int i = 0; i < MAX_SPARKLES; i++) {


        if (sparklesArray[i].visible == false) {


            if (sparkleTimer.getElapsedTime().asSeconds() >= sSpawnDelaySeconds)
            {

                float randX = rand() % (800 - 0 + 1) + 0;
                float randY = rand() % (100 - 50 + 1) + 50;

                for (int j = 0; j < MAX_ENEMIES; j++) {
                    if (randX + 20 < X_SPACE || randY + 20 < Y_SPACE) {
                        randX = rand() % (800 - 0 + 1) + 0;
                        randY = rand() % (100 - 50 + 1) + 50;
                    }
                }

                sparklesArray[i].Position.x = randX;
                sparklesArray[i].Position.y = randY;

                sparklesArray[i].visible = true;

                sparkleTimer.restart();
            }


        }

        sparklesArray[i].Position.y += sparklesArray[i].speedY * deltaTime;
        sparklesArray[i].rect.setPosition(sparklesArray[i].Position);
        sparklesArray[i].rect.rotate(sparklesArray[i].speedR);

        if (sparklesArray[i].Position.y > 650) {
            sparklesArray[i].visible = false;
            
        }


    }


}

void updateBullets(float deltaTime) {
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (bulletArray[i].shooting == false) {
            bulletArray[i].Position.x = playerShipPosition.x - 4;
            bulletArray[i].Position.y = playerShipPosition.y;
            bulletArray[i].rect.setPosition(bulletArray[i].Position);
        }
        if (bulletArray[i].shooting) {
            bulletArray[i].Position.y -= bulletArray[i].speedY * deltaTime;
            bulletArray[i].rect.setPosition(bulletArray[i].Position);
        }if (bulletArray[i].Position.y < 50 && bulletArray[i].shooting) {
            bulletArray[i].shooting = false;
            bulletArray[i].Position.x = playerShipPosition.x - 4;
            bulletArray[i].Position.y = playerShipPosition.y;
            bulletArray[i].rect.setPosition(bulletArray[i].Position);

        }
    }

    for (int i = 0; i < MAX_BULLETS; i++) {
        for (int j = 0; j < MAX_ENEMIES; j++){
            if (bulletArray[i].Position.x - 32 <= enemiesArray[j].Position.x && bulletArray[i].Position.x + 28 >= enemiesArray[j].Position.x) {
                if (bulletArray[i].Position.y - 28 <= enemiesArray[j].Position.y && bulletArray[i].Position.y + 28 >= enemiesArray[j].Position.y) {
                    if (enemiesArray[j].isAlive && bulletArray[i].shooting) {
                        enemiesArray[j].isAlive = false;
                        bulletArray[i].shooting = false;
                        score_ += 10;
                    }
                }
            }
        }
    }

    for (int i = 0; i < MAX_BULLETS; i++) {
        for (int j = 0; j < MAX_ENEMIES; j++) {
            if (bulletArray[i].Position.x - 40 <= shootersArray[j].Position.x && bulletArray[i].Position.x + 40 >= shootersArray[j].Position.x) {
                if (bulletArray[i].Position.y - 40 <= shootersArray[j].Position.y && bulletArray[i].Position.y + 40 >= shootersArray[j].Position.y) {
                    if (enemiesArray[j].isAlive && bulletArray[i].shooting) {
                        shootersArray[j].isAlive = false;
                        bulletArray[i].shooting = false;
                        score_ += 30;
                    }
                }
            }
        }
    }

    
    if (shootTimer.getElapsedTime().asSeconds() >= shootDelaySeconds) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) {
            for (int i = 0; i < MAX_BULLETS; i++) {
                if (!bulletArray[i].shooting) {
                    bulletArray[i].shooting = true;
                    break;
                }
            }
            shootTimer.restart();
        }
        
    }
    
}


void updateEnemies(float deltaTime) {
    
    
    for (int i = 0; i < MAX_ENEMIES; i++) {
 
        if (level >= 2 && level < 5) {
            enemiesArray[i].speedY = 80.0f;
        }
        else if (level == 5) {
            enemiesArray[i].speedY = 120.0f;
        }

        if (enemiesArray[i].isAlive == false) {
            

            if (respawnTimer.getElapsedTime().asSeconds() >= spawnDelaySeconds)
            {

                float randX = rand() % (700 - 100 + 1) + 100;
                float randY = rand() % (200 - 100 + 1) + 100;

                for (int j = 0; j < MAX_ENEMIES; j++) {
                    if (randX + 20 < X_SPACE || randY + 20 < Y_SPACE) {
                        randX = rand() % (700 - 100 + 1) + 100;
                        randY = rand() % (200 - 100 + 1) + 100;
                    }
                }

                enemiesArray[i].Position.x = randX;
                enemiesArray[i].Position.y = randY;

                enemiesArray[i].isAlive = true;

                respawnTimer.restart();
            }


        }

        enemiesArray[i].Position.y += enemiesArray[i].speedY * deltaTime;
        enemiesArray[i].rect.setPosition(enemiesArray[i].Position);
        enemiesArray[i].rect.rotate(enemiesArray[i].speedR);

        if (enemiesArray[i].Position.y > 750) {
            enemiesArray[i].isAlive = false;
            
            


        }


        for (int i = 0; i < MAX_SHOOTERS; i++) {

            if (enemiesArray[i].Position.x - 32 <= playerShipPosition.x && enemiesArray[i].Position.x + 32 >= playerShipPosition.x) {
                if (enemiesArray[i].Position.y - 32 <= playerShipPosition.y && enemiesArray[i].Position.y + 32 >= playerShipPosition.y) {
                    if (enemiesArray[i].isAlive) {
                        enemiesArray[i].isAlive = false;
                        if (retriesTimer.getElapsedTime().asSeconds() >= retriesDelaySeconds) {
                            if (sheilded) {
                                sheilded = false;
                          
                            }
                            else {
                                retries--;
                                retriesTimer.restart();
                            }
                        }

                    }
                }

            }
        }
        

    }

    
}

void levelUpScreen(sf::RenderWindow& win) {
    sf::Font font("font.ttf");
    sf::Text levelUpText(font);

    levelUpText.setPosition(sf::Vector2f(300, 350));
    levelUpText.setFillColor(sf::Color::White);
    levelUpText.setCharacterSize(25);
    levelUpText.setString("LEVEL " + std::to_string(level) + "!");

    win.draw(levelUpText);

}

void updateLevels(sf::RenderWindow& win) {
    if (score_ >= (level * 500) + prevScore) {
        if (level < 5) {
            level++;
            prevScore = score_;
            
            levelUp = true;
            initializeEnemies();
            levelUpTimer.restart();

        }

    }

    if (levelUpTimer.getElapsedTime().asSeconds() >= levelUpTimerDelaySeconds) {
        levelUp = false;
    }
}

void drawEnemies(sf::RenderWindow& win) {
    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (enemiesArray[i].isAlive) {
            win.draw(enemiesArray[i].rect);
        }
    }
}

void drawBullets(sf::RenderWindow& win) {
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (bulletArray[i].shooting) {
            win.draw(bulletArray[i].rect);
        }
    }
}

void drawSparkles(sf::RenderWindow& win) {
    for (int i = 0; i < MAX_SPARKLES; i++) {
        if (sparklesArray[i].visible) {
            win.draw(sparklesArray[i].rect);
        }
    }
}

void drawShooters(sf::RenderWindow& win) {
    for (int i = 0; i < MAX_SHOOTERS; i++) {
        if (shootersArray[i].isAlive) {
            win.draw(shootersArray[i].rect);
        }
    }
}

void drawShooterBullets(sf::RenderWindow& win) {
    for (int i = 0; i < MAX_SHOOTER_BULLETS; i++) {
        if (shooterBulletArray[i].shooting && shootersArray[i].isAlive) {
            win.draw(shooterBulletArray[i].rect);
        }
    }
}

void drawPowerUp(sf::RenderWindow& win) {
    if (powerUp.isVisible) {
        win.draw(powerUp.rect);
    }
    if (sheilded) {
        win.draw(sheild.rect);
    }
}

void pauseScreen(sf::RenderWindow& win, float deltaTime) {
    updateSparkles(deltaTime);
    sf::Font font("font.ttf");
    sf::Text label1(font);
    sf::Text label3(font);
    sf::Text label2(font);
    sf::Text label4(font);

    label1.setPosition(sf::Vector2f(340, 350));
    label1.setFillColor(sf::Color::White);
    label1.setCharacterSize(25);
    label1.setString("PAUSED");

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q)) {
        StartScreen = true;
        InstructionsScreen= false;
        pauseScreenB = false;
        
        
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
        std::ofstream fout;
        fout.open("save.txt");
        fout << score_ << '\n';
        fout << level << '\n';
        fout << retries;
        fout.close();

        StartScreen = true;
        InstructionsScreen = false;
        pauseScreenB = false;


    }

    label4.setPosition(sf::Vector2f(20, 740));
    label4.setFillColor(sf::Color::White);
    label4.setCharacterSize(15);
    label4.setString("PRESS [Q] TO EXIT TO MAIN MENU");

    label2.setPosition(sf::Vector2f(20, 700));
    label2.setFillColor(sf::Color::White);
    label2.setCharacterSize(15);
    label2.setString("PRESS [ESC] TO RETURN TO THE GAME");

    label3.setPosition(sf::Vector2f(20, 720));
    label3.setFillColor(sf::Color::White);
    label3.setCharacterSize(15);
    label3.setString("PRESS [S] TO SAVE AND EXIT");

    win.draw(label1);
    win.draw(label2);
    win.draw(label3);
    win.draw(label4);
}

void gameOverScreen(sf::RenderWindow& win, float deltaTime) {
    updateSparkles(deltaTime);
    sf::Font font("font.ttf");
    sf::Text label1(font);
    sf::Text label3(font);

    label1.setPosition(sf::Vector2f(280, 350));
    label1.setFillColor(sf::Color::White);
    label1.setCharacterSize(25);
    label1.setString("GAME OVER");

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q)) {
        std::ifstream fin;

        fin.open("highscore.txt");
        if (!fin) {
            std::ofstream fout;
            fout.open("highscore.txt");
            fout << score_;
            fout.close();
        }
        else {
            std::cout << std::endl << score_;
            std::string line;
            int hs;
            while (std::getline(fin, line)) {
                hs = std::stoi(line);
            }

            if (hs < score_) {
                fin.close();
                std::ofstream fout;
                fout.open("highscore.txt");
                fout << score_;
                fout.close();
            }
            
            
        }

        fin.close();
        StartScreen = true;
        gameOver = false;


    }

    label3.setPosition(sf::Vector2f(20, 750));
    label3.setFillColor(sf::Color::White);
    label3.setCharacterSize(15);
    label3.setString("PRESS [Q] TO EXIT TO MAIN MENU");


    win.draw(label1);
    win.draw(label3);
}

void instrucionsScreen(sf::RenderWindow& win, float deltaTime) {
    
    updateSparkles(deltaTime);
    sf::Font font("font.ttf");
    sf::Text label1(font);
    sf::Text label2(font);
    sf::Text label3(font);

    label1.setPosition(sf::Vector2f(20, 50));
    label1.setFillColor(sf::Color::White);
    label1.setCharacterSize(25);
    label1.setString("HOW TO PLAY");

    label2.setPosition(sf::Vector2f(20, 100));
    label2.setFillColor(sf::Color::White);
    label2.setOutlineThickness(5);
    label2.setCharacterSize(15);
    label2.setString("USE YOUR [A] AND [D] KEYS TO MOVE, USE [SPACE] TO \n\nSHOOT THE ENEMIES AND ASTEROIDS THAT COME YOUR WAY \n\nYOUR OBJECTIVE IS TO KILL ENEMIES/ASTEROIDS AND TO \n\nINCREASE YOUR SCORE AND LEVEL UP THERE ARE 5 LEVELS\n\nSOME ASTEROIDS/ENEMIES DAMAGE YOU IF YOU COLLIDE \n\nWITH THEM AND SOME WONT. BULLETS WILL DAMAGE YOU\n\nCOLLECT POWERUPS TO GET A SHEILD THAT WILL\n\nPROTECT YOU FROM BULLETS.");
    
    label3.setPosition(sf::Vector2f(20, 750));
    label3.setFillColor(sf::Color::White);
    label3.setCharacterSize(15);
    label3.setString("PRESS [ESC] TO EXIT TO MAIN MENU");

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)) {
        InstructionsScreen = false;
        StartScreen = true;
    }

    win.draw(label1);
    win.draw(label2);
    win.draw(label3);
    drawSparkles(win);
}
void startScreen(sf::RenderWindow& win, float deltaTime) {

    updateSparkles(deltaTime);
    if (selectorTimer.getElapsedTime().asSeconds() >= selectionDelaySeconds) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) {
            if (selector > 0) {
                selector--;
            }
            selectorTimer.restart();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) {
            if (selector < 3) {
                selector++;
            }
            selectorTimer.restart();
        }
    }



    sf::Font font("font.ttf");
    sf::Text label1(font);
    sf::Text label2(font);
    sf::Text label3(font);
    sf::Text label4(font);
    sf::Text label5(font);
    sf::Text label6(font);
    sf::Text label7(font);
    sf::Text label8(font);


    label1.setPosition(sf::Vector2f(50, 350));
    label1.setFillColor(sf::Color::White);
    label1.setCharacterSize(40);
    label1.setString("SPACE WARS");

    label2.setPosition(sf::Vector2f(50, 430));
    label2.setFillColor(sf::Color::White);
    //label2.setOutlineColor(sf::Color::Color(50, 50, 50));
    label2.setOutlineThickness(5);
    label2.setCharacterSize(20);
    label2.setString("START NEW GAME");

    label3.setPosition(sf::Vector2f(50, 470));
    label3.setFillColor(sf::Color::White);
    //label3.setOutlineColor(sf::Color::Color(50, 50, 50));
    label3.setOutlineThickness(5);
    label3.setCharacterSize(20);
    label3.setString("LOAD SAVED GAME");

    label4.setPosition(sf::Vector2f(50, 510));
    label4.setFillColor(sf::Color::White);
    //label4.setOutlineColor(sf::Color::Color(50, 50, 50));
    label4.setOutlineThickness(5);
    label4.setCharacterSize(20);
    label4.setString("VIEW INSTRUCTIONS");

    label5.setPosition(sf::Vector2f(50, 550));
    label5.setFillColor(sf::Color::White);
    //label5.setOutlineColor(sf::Color::Color(50, 50, 50));
    label5.setOutlineThickness(5);
    label5.setCharacterSize(20);
    label5.setString("EXIT");

    label6.setPosition(sf::Vector2f(50, 700));
    label6.setFillColor(sf::Color::White);
    label6.setOutlineThickness(5);
    label6.setCharacterSize(15);
    label6.setString("USE YOUR ARROW KEYS TO NAVIGATE");

    label7.setPosition(sf::Vector2f(50, 720));
    label7.setFillColor(sf::Color::White);
    label7.setOutlineThickness(5);
    label7.setCharacterSize(15);
    label7.setString("PRESS [ENTER] TO SELECT");

    int highscore;

    std::ifstream fin;
    fin.open("highscore.txt");
    if (!fin) {
        highscore = 0;
    }
    else {
        std::string line;
        while (std::getline(fin, line)) {
            highscore = std::stoi(line);
        }
    }

    fin.close();
    label8.setPosition(sf::Vector2f(50, 760));
    label8.setFillColor(sf::Color::White);
    label8.setOutlineThickness(5);
    label8.setCharacterSize(15);
    label8.setString("HIGHSCORE: " + std::to_string(highscore));



    if (selector == 0) {
        label2.setOutlineColor(sf::Color::Color(50, 50, 50));
        label3.setOutlineColor(sf::Color::Transparent);
        label4.setOutlineColor(sf::Color::Transparent);
        label5.setOutlineColor(sf::Color::Transparent);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter)) {
            StartScreen = false;
            InstructionsScreen = false;
            gameOver = false;
            initializeEnemies();
            initializeBullets();
            initializeShooters();
            initializePowerUp();
            initializeShooterBullets();
            initializePowerUp();
            score_ = 0;
            level = 1;
            retries = 3;
        }

    }
    else if (selector == 1) {
        label3.setOutlineColor(sf::Color::Color(50, 50, 50));
        label2.setOutlineColor(sf::Color::Transparent);
        label4.setOutlineColor(sf::Color::Transparent);
        label5.setOutlineColor(sf::Color::Transparent);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter)) {
            initializeEnemies();
            initializeBullets();
            initializeShooters();
            initializePowerUp();
            initializeShooterBullets();
            initializePowerUp();

            std::ifstream fin;
            std::string line;

            fin.open("save.txt");
            if (!fin) {
                sf::Text nsg(font);
                nsg.setFillColor(sf::Color::Red);
                nsg.setOutlineColor(sf::Color(50, 50, 50));
                nsg.setPosition(sf::Vector2f(50, 50));
                nsg.setCharacterSize(30);
                nsg.Underlined;
                nsg.setString("NO SAVE DATA");

                

                win.draw(nsg);
            }
            else {
                int counter = 0;
                while (std::getline(fin, line)) {
                    if (counter == 0) {
                        score_ = std::stoi(line);
                    }
                    else if (counter == 1) {
                        level = std::stoi(line);
                    }
                    else if (counter == 2) {
                        retries = std::stoi(line);
                    }

                    counter++;
                }

                StartScreen = false;
                InstructionsScreen = false;
            

                fin.close();
            }

            
        }
    }
    else if (selector == 2) {
        label4.setOutlineColor(sf::Color::Color(50, 50, 50));
        label3.setOutlineColor(sf::Color::Transparent);
        label2.setOutlineColor(sf::Color::Transparent);
        label5.setOutlineColor(sf::Color::Transparent);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter)) {
            StartScreen = false;
            InstructionsScreen = true;
        }
    }
    else if (selector == 3) {
        label5.setOutlineColor(sf::Color::Color(50, 50, 50));
        label3.setOutlineColor(sf::Color::Transparent);
        label4.setOutlineColor(sf::Color::Transparent);
        label2.setOutlineColor(sf::Color::Transparent);

        


        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter)) {
            win.close();
        }
    }




    drawSparkles(win);
    win.draw(label1);
    win.draw(label2);
    win.draw(label3);
    win.draw(label4);
    win.draw(label5);
    win.draw(label6);
    win.draw(label7);
    win.draw(label8);
}




int main() {
    
    srand(time(NULL));
    sf::RenderWindow window(sf::VideoMode({ 800, 800 }), "Space Wars");

    sf::Font font("font.ttf");
    sf::Text score(font);
    sf::Text health(font);
    sf::Text escT(font);
    

    sf::Texture playerTexture;
    playerTexture.loadFromFile("shipa.png");
    sf::Sprite player(playerTexture);
    player.scale(sf::Vector2f(2.7,2.7));

    sf::Texture bgTexture;
    bgTexture.loadFromFile("bg.png");
    sf::Sprite bgSprite(bgTexture);

    sf::Texture planetTexture;
    planetTexture.loadFromFile("planet.png");
    sf::Sprite planetSprite(planetTexture);
    
    sf::Texture planetTexture2;
    planetTexture2.loadFromFile("sat.png");
    sf::Sprite planetSprite2(planetTexture2);

    planetSprite.setPosition(sf::Vector2f(750, 350));
    planetSprite.setRotation(sf::degrees(45));
    planetSprite.setScale(sf::Vector2f(3, 3));
    
    planetSprite2.setPosition(sf::Vector2f(800, 0));
    planetSprite2.setScale(sf::Vector2f(3, 3));
    planetSprite2.setOrigin(sf::Vector2f(planetTexture2.getSize().x / 2, planetTexture2.getSize().y / 2));
    
    
    sheild.Position = playerShipPosition;
    //sheild.rect.setOrigin(playerShipPosition);
    sheild.rect.setPosition(sf::Vector2f(sheild.Position.x - 30, sheild.Position.y + 10));
    sheild.rect.setScale(sf::Vector2f(4.2, 4.2));
    

    sf::Clock deltaClock;
    float xVelocity = 600;

    window.setFramerateLimit(60);

    initializeEnemies();
    initializeBullets();
    initializeSparkles();
    initializeShooters();
    initializeShooterBullets();
    initializePowerUp();
    
    
    score.setPosition(sf::Vector2f(10, 10));
    score.setFillColor(sf::Color::White);
    score.setCharacterSize(20);
    

    health.setPosition(sf::Vector2f(550, 10));
    health.setFillColor(sf::Color::White);
    health.setCharacterSize(20);


    escT.setPosition(sf::Vector2f(10, 780));
    escT.setFillColor(sf::Color::White);
    escT.setCharacterSize(12);
    escT.setString("PRESS [ESC] TO PAUSE");


    sf::Text levelUpText(font);

    levelUpText.setPosition(sf::Vector2f(650, 780));
    levelUpText.setFillColor(sf::Color::White);
    levelUpText.setCharacterSize(15);
    

    player.setOrigin(sf::Vector2f(playerTexture.getSize().x / 2, playerTexture.getSize().y / 2));
    
    sf::Vector2f planetPos = {750,350};
    sf::Vector2f sattelitePos = { 600, -100 };
	while (window.isOpen()) {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        if (retries <= 0) {
            std::cout << "HEY";
            gameOver = true;
        }

        sf::Clock fireClock;
        const float ENEMY_FIRE_RATE = 1.0f;

        sf::Time deltaTime = deltaClock.restart();
        float deltaAsTimeSeconds = deltaTime.asSeconds();

        levelUpText.setString("LEVEL " + std::to_string(level));

        score.setString("SCORE: " + std::to_string(score_));

        switch (retries) {
        case 3:
            health.setString("HEALTH: ***");
            health.setFillColor(sf::Color::Green);
            break;
        case 2:
            health.setString("HEALTH: **");
            health.setFillColor(sf::Color::Yellow);
            break;
        case 1:
            health.setString("HEALTH: *");
            health.setFillColor(sf::Color::Red);
            break;
        case 0:
            health.setString("HEALTH: NONE");
            health.setFillColor(sf::Color::White);
            break;
        }
        

        
        

        if (playerShipPosition.x > 650) {
            playerShipPosition.x = 650;
        }

        if (playerShipPosition.x < 50) {
            playerShipPosition.x = 50;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
            playerShipPosition.x -= xVelocity * deltaAsTimeSeconds;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
            playerShipPosition.x += xVelocity * deltaAsTimeSeconds;
        }

        if (pauseTimer.getElapsedTime().asSeconds() >= pauseDelaySeconds) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)) {
                if (StartScreen == false && InstructionsScreen == false && pauseScreenB == true) {
                    StartScreen = false;
                    InstructionsScreen = false;
                    pauseScreenB = false;
                }
                else if (StartScreen == false && InstructionsScreen == false && pauseScreenB == false) {
                    StartScreen = false;
                    InstructionsScreen = false;
                    pauseScreenB = true;
                }

                pauseTimer.restart();
            }
        }

        updateLevels(window);

        planetPos.y += 20 * deltaAsTimeSeconds;
        planetSprite.setPosition(sf::Vector2(planetPos.x, planetPos.y));

        if (planetPos.y > 900) {
            planetPos.y = -300;
        }

        sattelitePos.y += 20 * deltaAsTimeSeconds;
        sattelitePos.x -= 20 * deltaAsTimeSeconds;
        planetSprite2.rotate(sf::degrees(0.5));
        planetSprite2.setPosition(sf::Vector2(sattelitePos.x, sattelitePos.y));

        if (sattelitePos.y > 900 && sattelitePos.x < 0) {
            sattelitePos.y = -100;
            sattelitePos.x = 600;
        }



        if (StartScreen && !InstructionsScreen && !pauseScreenB) {
            window.clear();
            window.draw(bgSprite);
            window.draw(planetSprite);
            window.draw(planetSprite2);
            startScreen(window, deltaAsTimeSeconds);
            window.display();
        }
        else if (InstructionsScreen && !StartScreen && !pauseScreenB) {
            window.clear();
            window.draw(bgSprite);
            window.draw(planetSprite);
            instrucionsScreen(window, deltaAsTimeSeconds);
            window.display();
        }
        else if (pauseScreenB && !InstructionsScreen && !StartScreen) {
            window.clear();
            pauseScreen(window, deltaAsTimeSeconds);
            window.display();
        }
        else if (gameOver) {
            initializeEnemies();
            initializeShooters();

            window.clear();
            gameOverScreen(window, deltaAsTimeSeconds);
            window.display();
        }
        else if (levelUp) {
            window.clear();
            levelUpScreen(window);
            window.display();

        }
        else if(!pauseScreenB && !InstructionsScreen && !StartScreen && !gameOver){
            if (level <= 2) {
                //updatePowerUp(deltaAsTimeSeconds);
                updateEnemies(deltaAsTimeSeconds);
                updateBullets(deltaAsTimeSeconds);
                updateSparkles(deltaAsTimeSeconds);
                updateLevels(window);
                //updateShooters(deltaAsTimeSeconds);
                //updateShootersBullets(deltaAsTimeSeconds, player);

                player.setPosition(playerShipPosition);

                window.clear();

                if (sheilded) {
                    sheild.Position = (playerShipPosition);
                    sheild.rect.setPosition(sf::Vector2f(sheild.Position.x - 30, sheild.Position.y - 25));
                    window.draw(sheild.rect);
                }
               
                window.draw(bgSprite);
                window.draw(planetSprite);
                window.draw(planetSprite2);
                window.draw(player);
                window.draw(health);
                window.draw(score);
                window.draw(escT);
                window.draw(levelUpText);

                //drawShooterBullets(window);
                //drawShooters(window);
                drawSparkles(window);
                drawEnemies(window);
                drawBullets(window);
                //drawPowerUp(window);
            }if (level >= 3) {

                updateEnemies(deltaAsTimeSeconds);
                updateBullets(deltaAsTimeSeconds);
                updateSparkles(deltaAsTimeSeconds);
                updateShooters(deltaAsTimeSeconds, window);
                updateShootersBullets(deltaAsTimeSeconds, player);
                updatePowerUp(deltaAsTimeSeconds);
                updateLevels(window);

                player.setPosition(playerShipPosition);

                window.clear();

                if (sheilded) {
                    sheild.Position = (playerShipPosition);
                    sheild.rect.setPosition(sf::Vector2f(sheild.Position.x - 32, sheild.Position.y - 30));
                    window.draw(sheild.rect);
                }

                window.draw(bgSprite);
                window.draw(planetSprite);
                window.draw(planetSprite2);
                window.draw(player);
                window.draw(health);
                window.draw(score);
                window.draw(escT);
                window.draw(levelUpText);

                drawShooterBullets(window);
                drawShooters(window);
                drawSparkles(window);
                drawEnemies(window);
                drawBullets(window);
                drawPowerUp(window);
            }
           
            
            window.display();
        }
	}
}