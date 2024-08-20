#pragma once
#include <string>
#include <random>

const int GAME_WINDOW_HEIGHT = 800;
const int GAME_WINDOW_WIDTH = 1200;
const int PROJECTILE_VELOCITY = 150;
const float DROP_EXPIRATION_SECONDS = 7;
const std::string ASSETS_FOLDER = "resources/assets";
const std::string GAME_FONT = "resources/fonts/AGoblinAppears-o2aV.ttf";
const std::string MENU_IMAGE = "resources/assets/menubg.png";
const std::string BACKGROUND_GAME = "resources/assets/grassbg3.png";
const std::string BASE_IMAGE = "resources/assets/sprites/tower01.png";
const std::string HERO_WALK_IMAGE = "resources/assets/sprites/hero/Wizard-Walk.png";
const std::string HERO_IDLE_IMAGE = "resources/assets/sprites/hero/Wizard-Idle.png";
const std::string HERO_ATTACK_IMAGE = "resources/assets/sprites/hero/Wizard-Attack02.png";
const std::string POTION_IMAGE = "resources/assets/items/red-potion.png";
const std::string BLUE_POTION_IMAGE = "resources/assets/items/blue-potion.png";
const std::string ENEMY_IMAGE = "resources/assets/sprites/enemy/Skeleton.png";
const std::string ENEMY_WALK_IMAGE = "resources/assets/sprites/enemy/Skeleton-Walk.png";
const std::string ENEMY_ATTACK_IMAGE = "resources/assets/sprites/enemy/Skeleton-Attack.png";
const std::string QUIVER_IMAGE = "resources/assets/items/quiver.png";
const std::string MENU_MUSIC = "resources/assets/music/menu-soundtrack.ogg";
const std::string GAMEOVER_MUSIC = "resources/assets/music/gameover-soundtrack.wav";
const std::string BATTLE_MUSIC = "resources/assets/music/battle-soundtrack.ogg";
const std::string SPELL_MUSIC = "resources/assets/music/spell.wav";
const std::string ARROW_MUSIC = "resources/assets/music/arrowhit.wav";

inline int getRandomNumber(int min, int max)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(min, max);

    return dist(gen);
}
