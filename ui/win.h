#ifndef Win_h__
#define Win_h__
#include <glm/glm.hpp>
#include "ui/component.h"
#include <vector>
#include <string>
#include <memory>
#include "sge/textgeometry.h"

class Win : public WContainer
{
public:
    Win(void);
    Win(glm::vec2 &p, glm::vec2 &s);
    Win(glm::vec2 &p, glm::vec2 &s, glm::vec4 &col);
    ~Win();
    virtual void Draw() const;
    void Update();
    glm::vec2 GlobalPos() const;
    glm::vec2 drag_point;
    bool dragged = false;

    glm::vec2 size;
    glm::vec2 pos;
    glm::vec4 col;
    std::vector<std::unique_ptr<WComponent>> Items;
    std::unique_ptr<TextGeometry> text;
};

#endif // Win_h__

