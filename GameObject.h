#pragma once

#include "util.h"
#include "Sprite.h"

class GameObject{
public:
    GameObject();
    virtual ~GameObject();

    virtual void update() = 0;

    Point pos;
    std::vector<Sprite> sprites;
};
