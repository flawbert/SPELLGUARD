#include "interfaces/Game.hpp"
#include "interfaces/Enemy.hpp"
#include "../common.h"
#include <iostream>

using namespace sf;
using namespace std;

void Game::run()
{
    Clock clock;

    while (gameWindow->isOpen())
    {
        float deltaTime = clock.restart().asSeconds();
        // TODO: use this field instead deltaTIme as parameter
        setDeltaTime(deltaTime);
        render();
        handleEvents();
        update(deltaTime);
    }
}

void Game::renderStatus()
{
    Font font = Font();
    int baseLife = 30;
    if (!font.loadFromFile("resources/fonts/ProggyClean.ttf"))
    {
        std::cout << "Couldn't load font. Exiting.";
        return;
    }
    Text heroLifeText, ammoText, baseLifeText;
    heroLifeText.setFont(font);
    heroLifeText.setString("LIFE: " + to_string(hero->getLife()));
    heroLifeText.setCharacterSize(24);
    heroLifeText.setFillColor(sf::Color::Black);
    heroLifeText.setPosition(GAMEWINDOWWIDTH - 200, 25);

    ammoText.setFont(font);
    ammoText.setString("AMMO: " + to_string(hero->getRangedWeapon()->getAmmo()));
    ammoText.setCharacterSize(24);
    ammoText.setFillColor(sf::Color::Black);
    ammoText.setPosition(GAMEWINDOWWIDTH - 200, 50);

    baseLifeText.setFont(font);
    baseLifeText.setString("BASE LIFE: " + to_string(base->getLife()));
    baseLifeText.setCharacterSize(24);
    baseLifeText.setFillColor(sf::Color::Black);
    baseLifeText.setPosition(GAMEWINDOWWIDTH - 200, 100);

    gameWindow->draw(heroLifeText);
    gameWindow->draw(ammoText);
    gameWindow->draw(baseLifeText);
}

void Game::render()
{
    gameWindow->clear(Color::White);
    gameWindow->draw(hero->getShape());
    gameWindow->draw(base->getShape());

    for (const auto &enemy : *enemies)
    {
        gameWindow->draw(enemy->getShape());
        auto enemyProjectiles = *enemy->getRangedWeapon()->getLaunchedProjectiles();
        if (!enemyProjectiles.empty())
        {
            for (const auto &projectile : enemyProjectiles) // Using value instead pointer to do this foreach
            {
                gameWindow->draw(projectile->getShape());
            }
        }
    }

    auto heroProjectiles = *hero->getRangedWeapon()->getLaunchedProjectiles();
    if (!heroProjectiles.empty())
    {
        for (const auto &projectile : heroProjectiles)
        {
            gameWindow->draw(projectile->getShape());
        }
    }

    this->renderStatus(); // Should be the last to render to be above other objects
    gameWindow->display();
}

void Game::handleEvents()
{
    Event event;
    while (gameWindow->pollEvent(event))
    {
        if (event.type == Event::Closed)
        {
            gameWindow->close();
        }
        else if (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape)
        {
            gameWindow->close();
        }
        else if (event.type == Event::KeyPressed && event.key.code == Keyboard::Q)
        {
            sf::Vector2f mousePosition = this->getMousePosition();
            hero->doAttack(mousePosition);
        }
    }

    if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {
        hero->setTargetPosition(getMousePosition());
    }
}

void Game::update(float deltaTime)
{
    // Updates everything related to hero
    hero->move();
    auto heroProjectiles = hero->getRangedWeapon()->getLaunchedProjectiles();
    if (!heroProjectiles->empty())
    {
        for (auto &projectile : *heroProjectiles)
            projectile->update(deltaTime);

        this->calculateCollisionsWithProjectiles(heroProjectiles, enemies);
    }

    // Updates everything related to enemies
    for (const auto &enemy : *enemies)
    {
        enemy->move(deltaTime);
        int randNum = rand();
        if (randNum % 2 == 0)
        {
            auto basePosition = sf::Vector2f(base->getShape().getPosition());
            enemy->doAttack(basePosition);
        }
        auto enemyProjectiles = enemy->getRangedWeapon()->getLaunchedProjectiles();
        for (auto &projectile : *enemyProjectiles)
        {
            projectile->update(deltaTime);
            if (base->isCollidingWith(projectile->getBounds()))
            {
                base->takeDamage(projectile->getDamage());
                enemyProjectiles->erase(std::remove(enemyProjectiles->begin(), enemyProjectiles->end(), projectile), enemyProjectiles->end());
            }
        }
    }

    // Resolve collisions between hero with enemies and enemies with other enemies
    for (const auto &enemy : *enemies)
    {
        if (hero->isCollidingWith(enemy))
        {
            hero->resolveCollision(enemy);
        }

        if (base->isCollidingWith(enemy->getShape().getGlobalBounds()))
        {
            base->takeDamage(50);
            enemies->erase(std::remove(enemies->begin(), enemies->end(), enemy), enemies->end());
        }

        for (const auto &otherEnemy : *enemies)
        {
            if (&enemy != &otherEnemy && enemy->isCollidingWith(otherEnemy))
            {
                enemy->resolveCollision(otherEnemy);
            }
        }
    }

    this->spawnTimer += deltaTime;
    if (this->spawnTimer >= this->spawnInterval)
    {
        this->enemies->push_back(this->spawnEnemy());
        this->spawnTimer = 0.f;
    }
}

std::shared_ptr<Enemy> Game::spawnEnemy()
{
    float spawnX;
    float spawnY;

    int side = rand() % 4;

    switch (side)
    {
    case 0: // Top
        spawnX = rand() % GAMEWINDOWWIDTH;
        spawnY = -20;
        break;
    case 1: // Right
        spawnX = GAMEWINDOWWIDTH + 20;
        spawnY = rand() % GAMEWINDOWHEIGHT;
        break;
    case 2: // Bottom
        spawnX = rand() % GAMEWINDOWWIDTH;
        spawnY = GAMEWINDOWHEIGHT + 20.f;
        break;
    case 3: // Left
        spawnX = -20.f;
        spawnY = rand() % GAMEWINDOWHEIGHT;
        break;
    }

    return std::make_shared<Enemy>(40, 40, 50, 80, spawnX, spawnY, this->centerX, this->centerY);
}

void Game::close()
{
    gameWindow->close();
}