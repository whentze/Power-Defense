#pragma once

struct Point{
    int xPos;
    int yPos;
};

class GameObject{
public:
    GameObject();
    virtual ~GameObject();

    virtual void update() = 0;

    Point pos;
    std::vector<Sprite> sprites;
};
