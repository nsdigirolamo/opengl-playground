#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include "glad/glad.h"

class Texture
{   
    private:

        GLuint texture;

    public:

        Texture (const char* textureFilePath);

        GLuint getID ();
};

#endif