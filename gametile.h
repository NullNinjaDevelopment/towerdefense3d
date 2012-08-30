#ifndef GAMETILE_H
#define GAMETILE_H

class GameTile
{
public:
    GameTile(float, float, float, float, float, float);

    // color values (red green blue alpha)
    float r, g, b, a;
    // position values (x and y coordinates)
    float x, y;
};

#endif // GAMETILE_H
