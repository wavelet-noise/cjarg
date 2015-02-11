#include "sector.h"
#include "glm/gtx/transform.hpp"
#include "sge/textureatlas.h"
#include <mutex>
#include "sge/logger.h"
#include <thread>
#include <chrono>
#include "logic/base/database.h"

Sector::Sector() :
    mesh(),
    creatures(),
    items(),
    blocks()
{
    Init();
}

Sector::Sector(const Point &p) :
    offset(p),
    mesh(),
    creatures(),
    items(),
    blocks()
{
    Init();
}

Sector::~Sector()
{
    FORijk
    {
        delete blocks[i][j][k];
    }
}

void Sector::blockId(const Point3 &p, Jid i)
{
    blocks[p.x][p.y][p.z]->id(i);
}

Jid Sector::blockId(const Point3 &p)
{
    return blocks[p.x][p.y][p.z]->id();
}

void Sector::Init()
{
    FORijk
    {
        blocks[i][j][k] = new Block();
    }
}

Block *Sector::block(const Point3 &p)
{
    return blocks[p.x][p.y][p.z];
}

inline void get_uvs(Jtex apos, float &q, float &w, float &qq, float &ww)
{
    qq = 32 / 2048.f;
    ww = 32 / 2048.f;
    int inrow = 2048 / 32;
    q = (apos % inrow) * qq;
    w = (apos / inrow) * ww;
    qq += q; ww += w;
}

void Sector::Rebuild(std::shared_ptr<Material> mat_, std::shared_ptr<BasicJargShader> basic_)
{
    mesh.World = glm::translate(glm::mat4(1), glm::vec3(offset.x * RX, offset.y * RY, 0));
    mesh.material = mat_;
    mesh.shader = basic_;

    mesh.Vertices.reserve(50000);
    mesh.Indices.reserve(50000);

    int c = 0;
    FORijk
    {
        // 1 y -
        // 2 x +
        // 3 x -
        // 4 y +
        // 5 z -
        // 6 z +

        Jtex apos = blocks[i][j][k]->id();
        if(!apos) continue;
        float qq,ww,q,w;

        if(j == 0 || blocks[i][j - 1][k]->id() == 0)
        {
            get_uvs(database::instance()->block_db[blocks[i][j][k]->id()]->tex[StaticBlock::SIDE_BACK],
                    q,w,qq,ww);

            mesh.Vertices.push_back(VertPosNormTanBiTex({0,1,0},{i,     j,     k    }, {q, ww}));
            mesh.Vertices.push_back(VertPosNormTanBiTex({0,1,0},{i + 1, j,     k    }, {qq, ww}));
            mesh.Vertices.push_back(VertPosNormTanBiTex({0,1,0},{i + 1, j,     k + 1}, {qq, w}));
            mesh.Vertices.push_back(VertPosNormTanBiTex({0,1,0},{i    , j,     k + 1}, {q, w}));

            mesh.Indices.push_back(c + 0);
            mesh.Indices.push_back(c + 1);
            mesh.Indices.push_back(c + 2);

            mesh.Indices.push_back(c + 2);
            mesh.Indices.push_back(c + 3);
            mesh.Indices.push_back(c + 0);
            c+=4;
        }

        if(i == RX - 1 || blocks[i+1][j][k]->id() == 0)
        {
            get_uvs(database::instance()->block_db[blocks[i][j][k]->id()]->tex[StaticBlock::SIDE_RIGHT],
                    q,w,qq,ww);

            mesh.Vertices.push_back(VertPosNormTanBiTex({-1,0,0},{i + 1, j,     k + 1}, {q, w}));
            mesh.Vertices.push_back(VertPosNormTanBiTex({-1,0,0},{i + 1, j,     k    }, {q, ww}));
            mesh.Vertices.push_back(VertPosNormTanBiTex({-1,0,0},{i + 1, j + 1, k    }, {qq, ww}));
            mesh.Vertices.push_back(VertPosNormTanBiTex({-1,0,0},{i + 1, j + 1, k + 1}, {qq, w}));

            mesh.Indices.push_back(c + 0);
            mesh.Indices.push_back(c + 1);
            mesh.Indices.push_back(c + 2);

            mesh.Indices.push_back(c + 2);
            mesh.Indices.push_back(c + 3);
            mesh.Indices.push_back(c + 0);
            c+=4;
        }

        if(i == 0 || blocks[i - 1][j][k]->id() == 0)
        {
            get_uvs(database::instance()->block_db[blocks[i][j][k]->id()]->tex[StaticBlock::SIDE_LEFT],
                    q,w,qq,ww);

            mesh.Vertices.push_back(VertPosNormTanBiTex({1,0,0},{i    , j,     k    }, {qq, ww}));
            mesh.Vertices.push_back(VertPosNormTanBiTex({1,0,0},{i    , j,     k + 1}, {qq, w}));
            mesh.Vertices.push_back(VertPosNormTanBiTex({1,0,0},{i    , j + 1, k + 1}, {q, w}));
            mesh.Vertices.push_back(VertPosNormTanBiTex({1,0,0},{i    , j + 1, k    }, {q, ww}));

            mesh.Indices.push_back(c + 0);
            mesh.Indices.push_back(c + 1);
            mesh.Indices.push_back(c + 2);

            mesh.Indices.push_back(c + 2);
            mesh.Indices.push_back(c + 3);
            mesh.Indices.push_back(c + 0);
            c+=4;
        }

        if(j == RY - 1 || blocks[i][j+1][k]->id() == 0)
        {
            get_uvs(database::instance()->block_db[blocks[i][j][k]->id()]->tex[StaticBlock::SIDE_FRONT],
                    q,w,qq,ww);

            mesh.Vertices.push_back(VertPosNormTanBiTex({0,-1,0},{i    , j + 1, k    }, {qq, ww}));
            mesh.Vertices.push_back(VertPosNormTanBiTex({0,-1,0},{i    , j + 1, k + 1}, {qq, w}));
            mesh.Vertices.push_back(VertPosNormTanBiTex({0,-1,0},{i + 1, j + 1, k + 1}, {q, w}));
            mesh.Vertices.push_back(VertPosNormTanBiTex({0,-1,0},{i + 1, j + 1, k    }, {q, ww}));

            mesh.Indices.push_back(c + 0);
            mesh.Indices.push_back(c + 1);
            mesh.Indices.push_back(c + 2);

            mesh.Indices.push_back(c + 2);
            mesh.Indices.push_back(c + 3);
            mesh.Indices.push_back(c + 0);
            c+=4;
        }

        if(k == 0 || blocks[i][j][k - 1]->id() == 0)
        if(k != 0)
        {
            get_uvs(database::instance()->block_db[blocks[i][j][k]->id()]->tex[StaticBlock::SIDE_BOTTOM],
                    q,w,qq,ww);

            mesh.Vertices.push_back(VertPosNormTanBiTex({0,0,1},{i + 1, j    , k    }, {q, w}));
            mesh.Vertices.push_back(VertPosNormTanBiTex({0,0,1},{i    , j    , k    }, {qq, w}));
            mesh.Vertices.push_back(VertPosNormTanBiTex({0,0,1},{i    , j + 1, k    }, {qq, ww}));
            mesh.Vertices.push_back(VertPosNormTanBiTex({0,0,1},{i + 1, j + 1, k    }, {q, ww}));

            mesh.Indices.push_back(c + 0);
            mesh.Indices.push_back(c + 1);
            mesh.Indices.push_back(c + 2);

            mesh.Indices.push_back(c + 2);
            mesh.Indices.push_back(c + 3);
            mesh.Indices.push_back(c + 0);
            c+=4;
        }

        if(k == RZ - 1 || blocks[i][j][k + 1]->id() == 0)
        {
            get_uvs(database::instance()->block_db[blocks[i][j][k]->id()]->tex[StaticBlock::SIDE_TOP],
                    q,w,qq,ww);

            mesh.Vertices.push_back(VertPosNormTanBiTex({0,0,-1},{i    , j    , k + 1}, {qq, w}));
            mesh.Vertices.push_back(VertPosNormTanBiTex({0,0,-1},{i + 1, j    , k + 1}, {q, w}));
            mesh.Vertices.push_back(VertPosNormTanBiTex({0,0,-1},{i + 1, j + 1, k + 1}, {q, ww}));
            mesh.Vertices.push_back(VertPosNormTanBiTex({0,0,-1},{i    , j + 1, k + 1}, {qq, ww}));

            mesh.Indices.push_back(c + 0);
            mesh.Indices.push_back(c + 1);
            mesh.Indices.push_back(c + 2);

            mesh.Indices.push_back(c + 2);
            mesh.Indices.push_back(c + 3);
            mesh.Indices.push_back(c + 0);
            c+=4;
        }
    }

    facecount = mesh.Indices.size() / 3;
    vertcount = mesh.Vertices.size();

    state = UNBINDED;
}

