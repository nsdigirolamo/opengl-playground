#ifndef MODEL_HPP
#define MODEL_HPP

#include "glm/glm.hpp"

#include <stddef.h>
#include <ostream>
#include "material.hpp"

#define VERTICES_PER_FACE 3
#define VERTEX_SIZE sizeof(float) * 3
#define SURFACE_NORMAL_SIZE sizeof(float) * 3
#define UV_COORDINATE_SIZE sizeof(float) * 2

#define VERTEX_DATA_STRIDE 8

class Model
{
    private:

        float* vertexData;
        size_t vertexDataSize;
        unsigned int vertexDataCount;

        unsigned int vertexArray;
        unsigned int vertexBuffer;

        void constructFromObj (const char* filePath);

    public:

        glm::vec3 position;
        Material material;
        glm::vec3 scale;

        Model (const char* objFilePath);
        Model (const char* objFilePath, glm::vec3 position, Material material, glm::vec3 scale);
        ~Model ();

        const size_t getVertexDataSize () const;
        const float* const getVertexData () const;
        const unsigned int getVertexDataCount () const;

        void bindVertexArray () const;
        void bindVertexBuffer () const;
        void drawVertexArray () const;
};

std::ostream& operator<<(std::ostream& os, const Model& data);

#endif
