#ifndef MODEL_HPP
#define MODEL_HPP

#include "glm/glm.hpp"

#include <stddef.h>
#include <ostream>

#define VERTICES_PER_TRIANGLE 3
#define VERTEX_SIZE sizeof(float) * 3
#define NORMAL_SIZE sizeof(float) * 3
#define COLOR_SIZE sizeof(float) * 3

#define VERTEX_DATA_STRIDE 9

class Model
{
    private:

        unsigned int triangleCount;
        unsigned int vertexCount;

        size_t vertexDataSize;
        float* vertexData;

        glm::vec3 color;

    public:

        Model (const char* modelPath, glm::vec3 color);
        ~Model ();

        const unsigned int getTriangleCount () const;
        const unsigned int getVertexCount () const;

        const size_t getVertexDataSize () const;
        const float* const getVertexData () const;
        const glm::vec3 getColor () const;
};

std::ostream& operator<<(std::ostream& os, const Model& data);

#endif
