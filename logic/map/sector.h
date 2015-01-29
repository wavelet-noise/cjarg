#ifndef SECTOR_H
#define SECTOR_H
#define RX 16
#define RY 16
#define RZ 16
#define RXYZ RX*RY*RZ
#include "block.h"
#include <glm/glm.hpp>
#include "point.h"
#include "../entity/creature.h"
#include "../entity/item.h"
#define FORijk for(int i=0; i<RX; i++) for(int j=0; j<RY; j++) for (int k=0; k<RZ; k++)
#define FORij for(int i=0; i<RX; i++) for(int j=0; j<RY; j++)

struct Sector
{
    Sector();
    Sector(const Point &p);
    ~Sector();
    void blockId(const Point3 &p, Jid i);
    Jid blockId(const Point3 &p);

    Block *block(const Point3 &p);

    Block *blocks[RX][RY][RZ];
    unsigned char ground[RX][RY];
    Point offset;
    std::vector<Creature*> creatures;
    std::vector<Item*> items;
private:

    friend struct Generator;
    friend class Level;
};

#endif // SECTOR_H
