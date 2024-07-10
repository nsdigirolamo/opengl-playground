#ifndef MODEL_HPP
#define MODEL_HPP

#include <stddef.h>
#include <ostream>

class Model
{
    private:

        unsigned int triangleCount;
        unsigned int vertexCount;

        size_t verticesSize;
        float* vertices;

    public:

        Model (const char* modelPath);
        ~Model ();

        const unsigned int getTriangleCount () const;
        const unsigned int getVertexCount () const;

        const size_t getVerticesSize () const;
        const float* const getVertices () const;
};

std::ostream& operator<<(std::ostream& os, const Model& data);

#endif
