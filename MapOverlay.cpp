#include "Point.h"
#include "MapOverlay.h"
#include "config.h"
#include "globals.h"
#include "Map.h"
#include "GUIFunctions.h"
#include "Graphics.h"
#include "Tower.h"

MapOverlay::MapOverlay(const Point pos, const int width, const int height, void(*onClick)()) : GUIObject::GUIObject(defaultValue,     //MapOverlay does not need a specified identifier
                                                                                                                    true) { //argument in GUIObject("true") because always rendering MapOverlay into Map
    this->pos = pos;
    this->width = width;
    this->height = height;
    this->onClick = onClick;
}

MapOverlay::~MapOverlay() {

}

void MapOverlay::draw() {
    if (renderInMap) {
        SDL_SetRenderTarget(renderer, destTextureMap);
    } else {
        SDL_SetRenderTarget(renderer, destTextureGUI);
    }
    if (state == focused) {
        SDL_Rect rect;
        DisplayPoint corner = pos.snap().upperLeft();
        rect.x = corner.x;
        rect.y = corner.y;
        rect.w = TILE_WIDTH;
        rect.h = TILE_HEIGHT;
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderDrawRect(renderer, &rect);
    }
    if (GUIFunctions::currentPos == pos.snap() && GUIFunctions::currentTower != nullptr) {
        Graphics::drawTransparentCircle(DisplayPoint(pos.displayPoint().x + (int) (TILE_WIDTH / 2.0),
                                                     pos.displayPoint().y + (int) (TILE_HEIGHT / 2.0)),
                                        (int) GUIFunctions::currentTower->getStats().range);
    }
}

void MapOverlay::update() {
    if (mousePos.snap() == pos.snap() || clickedPos.snap() == pos.snap()) {
        state = focused;
    } else {
        state = unfocused;
    }
    if (mousePos.snap() == pos.snap() && mouseRelease) {
        if (onClick != nullptr) {
            onClick();
        }
    }
}