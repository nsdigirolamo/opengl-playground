#ifndef MODEL_HPP
#define MODEL_HPP

#include "glm/glm.hpp"

#include <stddef.h>
#include <ostream>

#define VERTICES_PER_FACE 3
#define VERTEX_SIZE sizeof(float) * 3
#define SURFACE_NORMAL_SIZE sizeof(float) * 3

#define VERTEX_DATA_STRIDE 6

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
        glm::vec3 color;

        Model (const char* modelPath, glm::vec3 color);
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
