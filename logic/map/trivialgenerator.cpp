#include "trivialgenerator.h"
#include "diamondsquare.h"

void TrivialGenerator::Generate(Sector &s)
{
    DiamondSquare ds;
    Array2D<float> *a = ds.DsGenerate(RX, RY, 40);

    FORij
    {
        char c = (char)((((*a)[i][j]/2.f)) * (float)RZ);
        if(c >= RZ) c = RZ - 1;
        s.ground[i][j] = c;

        for(int k=c; k<RZ; k++)
            s.blocks[i][j][k]->id(1);
    }

    delete a;
}
