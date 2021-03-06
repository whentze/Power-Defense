#include "Symbol.h"
#include "Sprite.h"
#include "Point.h"
#include "config.h"
#include "globals.h"
#include "GUIFunctions.h"
#include "Graphics.h"
#include "Tower.h"
#include "BasicTower.h"
#include "NailGun.h"
#include "Cache.h"
#include "SplashTower.h"


Symbol::Symbol(const eGUI identifier, const bool isTranparent, const bool renderInMap) : GUIObject::GUIObject(identifier, renderInMap) {
    this->isTransparent = isTranparent;
}

Symbol::Symbol(const eGUI identifier, std::vector<std::string> paths, const GridPoint pos, void (*onCLick)()) : GUIObject::GUIObject(identifier) {
    this->onClick = onCLick;
    isActivated = false;
    this->pos = GridPoint(pos.x + MAP_WIDTH, pos.y).center();
    isTransparent = false;
    for (auto element: paths) {
        sprites.push_back(Sprite(GridPoint(MAP_WIDTH + pos.x, pos.y).center(), TILE_WIDTH, TILE_HEIGHT, element, renderInMap));
    }
}

Symbol::~Symbol() {

}

void Symbol::update() {
    GUIObject::update(); //TODO: use abstraction to reduce redundancy
}

void Symbol::draw() {
    if (isActivated) {
        if(renderInMap){
            SDL_SetRenderTarget(renderer, destTextureMap);
        }else{
            SDL_SetRenderTarget(renderer, destTextureGUI);
        }
        if (isTransparent) {
            for (auto element: sprites) {
                SDL_SetTextureAlphaMod(element.texture, 200);
                SDL_SetTextureBlendMode(element.texture, SDL_BLENDMODE_BLEND);
                element.draw();//TODO: fix TowerBase is shining through
                //SDL_SetTextureBlendMode(element.texture, SDL_BLENDMODE_NONE);
                SDL_SetTextureAlphaMod(element.texture, 255);
            }
        } else {
            for (auto element: sprites) {
                element.draw();
            }
        }

        int range = 0;
        if (GUIFunctions::currentPos == pos.snap()) {
            switch (GUIFunctions::currentTowerType) {
                case basicTower:
                    range = (int) BasicTower::stat[0].range;
                    break;
                case nailGun:
                    range = (int) NailGun::stat[0].range;
                    break;
                case splashTower:
                    range = (int) SplashTower::stat[0].range;
                    break;
            }
            Graphics::drawTransparentCircle(pos.displayPoint(), range);
        }
    }
}
