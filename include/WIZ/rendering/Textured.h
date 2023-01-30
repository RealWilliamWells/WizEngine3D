//
// Created by william on 30/01/23.
//

#ifndef WIZENGINE3D_TEXTURED_H
#define WIZENGINE3D_TEXTURED_H

#include "GL/glew.h"

namespace wiz {
    class Textured;
}

class wiz::Textured {
    unsigned int texture;

    unsigned char *data;
    int width;
    int height;
    int nrChannels;

public:
    Textured(const char *textureFile);

    void useTexture();
};

#endif //WIZENGINE3D_TEXTURED_H
