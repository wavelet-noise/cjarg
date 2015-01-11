#ifndef JTEXTURE_H
#define JTEXTURE_H
#include <string>
#include <GL/glew.h>
#include <glm/glm.hpp>

struct Texture {
public:
    GLuint textureId;
    std::string name;
    unsigned int height;
    unsigned int width;
    unsigned int zsize;

    Texture();
    Texture(GLuint id);
    ~Texture();

    //void Load(const QString &a, bool smooth = false, bool mip = false);
    //void Load(QImage *a, bool smooth = false, bool mip = false);
    void Empty(glm::vec2 &size, GLuint dim = GL_TEXTURE_2D, GLuint format = GL_RGBA);
    void CreateDepth(glm::vec2 &size);
    void IdOnly();
};

#endif // JTEXTURE_H
