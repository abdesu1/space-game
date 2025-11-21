#include <cstdint>
#include <iostream>
#include <SFML/Graphics.hpp>

const int MAX_ENEMIES = 14;
const int MAX_BULLETS = 20;
const int MAX_SPARKLES = 30;

int score_ = 0;
int retries = 3;
int level = 1;

bool StartScreen = true;
bool InstructionsScreen = false;
bool pauseScreenB = false;


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


struct Bullet {
    sf::RectangleShape rect;
    sf::Vector2f Position;
    float speedY = 500.f;
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


int enemiesAlive = 0;

sf::Vector2f playerShipPosition(350, 700);

sf::Clock respawnTimer;
sf::Clock shootTimer;
sf::Clock sparkleTimer;
sf::Clock selectorTimer;
sf::Clock pauseTimer;
sf::Clock healthTimer;

const float shootDelaySeconds = 0.3f;
const float spawnDelaySeconds = 0.75f;
const float sSpawnDelaySeconds = 0.5f;
const float selectionDelaySeconds = 0.2f;
const float pauseDelaySeconds = 0.5f;
const float healthDelaySeconds = 0.5f;

const float ENEMY_WIDTH = 45.0f;
const float ENEMY_HEIGHT = 30.0;
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

        sparklesArray[i].rect.setSize(sf::Vector2f(10.f,10.f));
        sparklesArray[i].rect.setFillColor(sf::Color::Color(50, 50, 50));

        sparklesArray[i].rect.setOrigin(sf::Vector2f(sparklesArray[i].rect.getSize().x / 2, sparklesArray[i].rect.getSize().y / 2));

    }
}

void initializeEnemies() {

    for (int i = 0; i < MAX_ENEMIES; i++) {
        float randR = rand() % (30 - 10) + 1;
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
        enemiesArray[i].rect.setOrigin(sf::Vector2f((enemiesArray[i].rect.getGlobalBounds().position.x + 7)/2, (enemiesArray[i].rect.getGlobalBounds().position.y + 7)/2));
        enemiesArray[i].isAlive = false;

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
            bulletArray[i].Position.x = playerShipPosition.x + 18;
            bulletArray[i].Position.y = playerShipPosition.y;
            bulletArray[i].rect.setPosition(bulletArray[i].Position);
        }
        if (bulletArray[i].shooting) {
            bulletArray[i].Position.y -= bulletArray[i].speedY * deltaTime;
            bulletArray[i].rect.setPosition(bulletArray[i].Position);
        }if (bulletArray[i].Position.y < 50 && bulletArray[i].shooting) {
            bulletArray[i].shooting = false;
            bulletArray[i].Position.x = playerShipPosition.x + 18;
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

        if (enemiesArray[i].Position.y > 650) {
            enemiesArray[i].isAlive = false;
            if (healthTimer.getElapsedTime().asSeconds() >= healthDelaySeconds) {
                if (retries > 0) {
                    retries--;
                    healthTimer.restart();
                }
            }
            
        }
        

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

void pauseScreen(sf::RenderWindow& win, float deltaTime) {
    updateSparkles(deltaTime);
    sf::Font font("font.ttf");
    sf::Text label1(font);
    sf::Text label3(font);
    sf::Text label2(font);

    label1.setPosition(sf::Vector2f(320, 350));
    label1.setFillColor(sf::Color::White);
    label1.setCharacterSize(25);
    label1.setString("PAUSED");

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q)) {
        StartScreen = true;
        InstructionsScreen= false;
        pauseScreenB = false;
        
        
    }

    label3.setPosition(sf::Vector2f(20, 750));
    label3.setFillColor(sf::Color::White);
    label3.setCharacterSize(15);
    label3.setString("PRESS [Q] TO EXIT TO MAIN MENU");

    label2.setPosition(sf::Vector2f(20, 700));
    label2.setFillColor(sf::Color::White);
    label2.setCharacterSize(15);
    label2.setString("PRESS [ESC] TO EXIT TO RETURN TO THE GAME");

    win.draw(label1);
    win.draw(label2);
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
    label2.setString("USE YOUR [A] AND [D] KEYS TO MOVE, USE [SPACE] TO \n\nSHOOT THE ENEMIES AND ASTEROIDS THAT COME YOUR WAY \n\nYOUR OBJECTIVE IS TO KILL ENEMIES/ASTEROIDS AND TO \n\nINCREASE YOUR SCORE AND LEVEL UP THERE ARE \n\n10 LEVELS");
    
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


    label1.setPosition(sf::Vector2f(50, 350));
    label1.setFillColor(sf::Color::White);
    label1.setCharacterSize(40);
    label1.setString("SPACE INVADERS");

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
    //label6.setOutlineColor(sf::Color::Color(50, 50, 50));
    label6.setOutlineThickness(5);
    label6.setCharacterSize(15);
    label6.setString("USE YOUR ARROW KEYS TO NAVIGATE");

    label7.setPosition(sf::Vector2f(50, 720));
    label7.setFillColor(sf::Color::White);
    //label7.setOutlineColor(sf::Color::Color(50, 50, 50));
    label7.setOutlineThickness(5);
    label7.setCharacterSize(15);
    label7.setString("PRESS [ENTER] TO SELECT");



    if (selector == 0) {
        label2.setOutlineColor(sf::Color::Color(50, 50, 50));
        label3.setOutlineColor(sf::Color::Transparent);
        label4.setOutlineColor(sf::Color::Transparent);
        label5.setOutlineColor(sf::Color::Transparent);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter)) {
            StartScreen = false;
            InstructionsScreen = false;
        }

    }
    else if (selector == 1) {
        label3.setOutlineColor(sf::Color::Color(50, 50, 50));
        label2.setOutlineColor(sf::Color::Transparent);
        label4.setOutlineColor(sf::Color::Transparent);
        label5.setOutlineColor(sf::Color::Transparent);
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
}




int main() {
    
    srand(time(NULL));
    sf::RenderWindow window(sf::VideoMode({ 800, 800 }), "Space Invaders");
    sf::RectangleShape playerShip({ 100, 20 });
    sf::Font font("font.ttf");
    sf::Text score(font);
    sf::Text health(font);
    sf::Text escT(font);

  

    sf::Texture playerTexture;
    playerTexture.loadFromFile("shipa.png");
    sf::Sprite player(playerTexture);
    player.scale(sf::Vector2f(2.7,2.7));
    
    
    sf::Clock deltaClock;
    float xVelocity = 600;

    window.setFramerateLimit(60);

    initializeEnemies();
    initializeBullets();
    initializeSparkles();

    
    
    score.setPosition(sf::Vector2f(10, 10));
    score.setFillColor(sf::Color::White);
    score.setCharacterSize(20);
    

    health.setPosition(sf::Vector2f(590, 10));
    health.setFillColor(sf::Color::White);
    health.setCharacterSize(20);


    escT.setPosition(sf::Vector2f(10, 780));
    escT.setFillColor(sf::Color::White);
    escT.setCharacterSize(12);
    escT.setString("PRESS [ESC] TO PAUSE");

    playerShip.setFillColor(sf::Color::White);

    
	while (window.isOpen()) {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }
        sf::Clock fireClock;
        const float ENEMY_FIRE_RATE = 1.0f;

        sf::Time deltaTime = deltaClock.restart();
        float deltaAsTimeSeconds = deltaTime.asSeconds();

        

        score.setString("SCORE: " + std::to_string(score_));
        health.setString("HEALTH: " + std::to_string(retries));

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

        if (StartScreen && !InstructionsScreen && !pauseScreenB) {
            window.clear();
            startScreen(window, deltaAsTimeSeconds);
            window.display();
        }
        else if (InstructionsScreen && !StartScreen && !pauseScreenB) {
            window.clear();
            instrucionsScreen(window, deltaAsTimeSeconds);
            window.display();
        }
        else if (pauseScreenB && !InstructionsScreen && !StartScreen) {
            window.clear();
            pauseScreen(window, deltaAsTimeSeconds);
            window.display();
        }
        else {
            updateEnemies(deltaAsTimeSeconds);
            updateBullets(deltaAsTimeSeconds);
            updateSparkles(deltaAsTimeSeconds);
            player.setPosition(playerShipPosition);
         
            window.clear();
            window.draw(player);
            window.draw(health);
            window.draw(score);
            window.draw(escT);
            
            drawSparkles(window);
            drawEnemies(window);
            drawBullets(window);
            
            window.display();
        }
	}
}