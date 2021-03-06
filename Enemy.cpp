#include <algorithm>
#include <string>
#include <math.h>

#include "Point.h"
#include "Enemy.h"
#include "Map.h"
#include "Shot.h"
#include "util.h"
#include "config.h"
#include "globals.h"
#include "gamestats.h"
#include "GUIFunctions.h"
#include "TextOutput.h"
#include "GUI.h"
#include "Label.h"

Enemy::Enemy(Map &map, const int level, const int maxHealth, const float speed, const int loot,
             const std::string spritePath) : map(map) {
    pos = map.path[0];
    pathIndex = 1;
    stat = {maxHealth, speed, loot};
    this->level = level;
    ID = 1;
    sprites.push_back(Sprite(pos, TILE_WIDTH - 6, TILE_WIDTH - 6, spritePath, true));
    health = getStat().maxHealth;
}

Enemy::~Enemy() {

}

void Enemy::update() {
    const auto targetPos = map.path[pathIndex];
    pos.moveTowards(targetPos, getStat().speed);
    if (distance(pos, targetPos) < 0.1) {
        pathIndex++;
        if (pathIndex == map.path.size()) {
            if (gamestats.lives == 0) { // TODO: implement at different location in code
                GUIFunctions::endGame();
            } else {
                gamestats.lives--;
                Mix_PlayChannel(-1, Cache::getSound("/audio/rip.wav"),0);
            }
            die();
        }
    }

    for (int i = 0; i < sprites.size(); i++) {
        sprites[i].pos = this->pos;
        sprites[i].rotation = atan2(targetPos.y - this->pos.y, targetPos.x - this->pos.x) * 180 / M_PI;
    }
}

void Enemy::drawHealthbar(int width, int height, int border) {
    struct rgbcolor {
        uint8_t r;
        uint8_t g;
        uint8_t b;
    };

    std::vector<rgbcolor> healthbarColors = {
            {255, 255, 0},
            {0,   255, 255},
            {0,   255, 0},
            {0,   0,   255},
            {255, 0,   0},
            {100, 0,   100},
    };

    auto corner = DisplayPoint((int) pos.x - width / 2 - border, (int) pos.y - 30 - border);
    auto inner = DisplayPoint((int) pos.x - width / 2, (int) pos.y - 30);
    int healthscale = (int) log10(std::max(1, getStat().maxHealth / 2));
    SDL_Rect rect;
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    // Draw transparent background
    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 150);
    rect.x = inner.x;
    rect.y = inner.y;
    rect.w = width;
    rect.h = height;
    SDL_RenderFillRect(renderer, &rect);

    // Draw thick black border
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    rect.x = inner.x - border;
    rect.y = inner.y - border;
    rect.w = border;
    rect.h = height + border * 2;
    SDL_RenderFillRect(renderer, &rect);
    rect.x = inner.x + width;
    rect.y = inner.y - border;
    rect.w = border;
    rect.h = height + border * 2;
    SDL_RenderFillRect(renderer, &rect);
    rect.x = inner.x - border;
    rect.y = inner.y - border;
    rect.w = width + border * 2;
    rect.h = border;
    SDL_RenderFillRect(renderer, &rect);
    rect.x = inner.x - border;
    rect.y = inner.y + height;
    rect.w = width + border * 2;
    rect.h = border;
    SDL_RenderFillRect(renderer, &rect);

    // Draw colored healthbar
    rgbcolor c = healthbarColors[healthscale % healthbarColors.size()];
    SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, 255);
    rect.x = inner.x;
    rect.y = inner.y;
    rect.w = width * health / getStat().maxHealth;
    rect.h = height;
    SDL_RenderFillRect(renderer, &rect);

    // Draw black separating bars
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 100);
    for (int i = 1; i < getStat().maxHealth / pow(10, healthscale); i++) {
        rect.x = inner.x + i * width / (getStat().maxHealth / pow(10, healthscale));
        rect.y = inner.y;
        rect.w = border;
        rect.h = height;
        SDL_RenderFillRect(renderer, &rect);
    }
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
}

void Enemy::hit(Tower &source, int damage) {
    health -= damage;
    if (health <= 0 && !dead) {
        die();
    }
}

void Enemy::die() {
    for (auto it = allGameObjects.begin(); it != allGameObjects.end(); it++) {
        if ((*it).get() == this) {
            (*it)->dead = true;
            gamestats.money += getStat().loot;
            gamestats.points += getStat().loot;
            Mix_PlayChannel(-1, Cache::getSound("/audio/burr.wav") ,0);
        }
        if ((*it)->ID == 3) { //delete Shot
            if (((Shot *) (*it).get())->target == this) {
                (*it)->dead = true;
            }
        }
    }
}

EnemyStats Enemy::getStat() {
    return {(int) (stat.maxHealth * pow(1.3, level)), (float) (stat.speed * pow(1.005, level)),
            (int) (stat.loot * (1 + 0.2 * level))};
}

void Enemy::draw() {
    if (health < getStat().maxHealth) {
        SDL_SetRenderTarget(renderer, destTextureMap);
        drawHealthbar();
    }
}
