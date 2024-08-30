#include "texture.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"

#include <filesystem>
#include <iostream>

Texture::Texture (const char* textureFilePath)
{
    glGenTextures(1, &(this->texture));
    glBindTexture(GL_TEXTURE_2D, this->texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, channelCount;

    stbi_set_flip_vertically_on_load(true);
    stbi_uc* imageData = stbi_load(
        std::filesystem::path(textureFilePath).c_str(),
        &width, 
        &height,
        &channelCount,
        STBI_rgb_alpha
    );

    if (imageData) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cerr << "Failed to load texture at '" << textureFilePath << "': Are you sure the path is correct?" << std::endl;
        exit(-1);
    }

    stbi_image_free(imageData);
}

GLuint Texture::getID () {
    return this->texture;
}