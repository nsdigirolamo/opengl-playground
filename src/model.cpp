#include "model.hpp"
#include "glm/glm.hpp"

#include <fstream>
#include <iostream>
#include <sstream>

Model::Model (const char* modelPath, glm::vec3 color)
{
    std::ifstream file;
    file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    std::stringstream stream;

    try
    {
        file.open(modelPath);
        stream << file.rdbuf();
        file.close();
    }
    catch(const std::ifstream::failure &e)
    {
        std::cerr << "Model '" << modelPath << "' File Read Error: " << e.what() << std::endl;
        exit(-1);
    }

    stream >> this->triangleCount;
    this->color = color;

    /**
     * An element of the vertex data consists of 9 floats.
     * - The first 3 floats are the vertex's 3D position vector.
     * - The second 3 floats are the vertex's 3D surface normal vector.
     * - The third 3 floats are the vertex's RGB color information.
     *
     * Every 3 vertices create a triangle.
     */

    this->vertexCount = this->triangleCount * VERTICES_PER_TRIANGLE;
    this->vertexDataSize = this->vertexCount * (VERTEX_SIZE + NORMAL_SIZE + COLOR_SIZE);
    this->vertexData = (float*)(malloc(this->vertexDataSize));

    // Filling position vector information.

    for (int i = 0; i < this->vertexCount; ++i)
    {
        const unsigned int vertexIdx = i * VERTEX_DATA_STRIDE;
        stream >> this->vertexData[vertexIdx];
        stream >> this->vertexData[vertexIdx + 1];
        stream >> this->vertexData[vertexIdx + 2];
    }

    // Filling surface normal vector information.

    for (int i = 0; i < this->triangleCount; ++i)
    {
        const int triangleIdx = i * VERTEX_DATA_STRIDE * VERTICES_PER_TRIANGLE;

        const glm::vec3 vertex0 = glm::vec3(
            this->vertexData[triangleIdx],
            this->vertexData[triangleIdx + 1],
            this->vertexData[triangleIdx + 2]
        );

        const glm::vec3 vertex1 = glm::vec3(
            this->vertexData[triangleIdx + 6],
            this->vertexData[triangleIdx + 7],
            this->vertexData[triangleIdx + 8]
        );

        const glm::vec3 vertex2 = glm::vec3(
            this->vertexData[triangleIdx + 12],
            this->vertexData[triangleIdx + 13],
            this->vertexData[triangleIdx + 14]
        );

        const glm::vec3 normal = glm::normalize(glm::cross(vertex1 - vertex0, vertex2 - vertex0));

        for (int j = 0; j < VERTICES_PER_TRIANGLE; ++j)
        {
            const int vertexIdx = triangleIdx + (j * VERTEX_DATA_STRIDE);

            this->vertexData[vertexIdx + 3] = normal.x;
            this->vertexData[vertexIdx + 4] = normal.y;
            this->vertexData[vertexIdx + 5] = normal.z;
        }
    }

    // Filling color information.

    for (int i = 0; i < this->vertexCount; ++i)
    {
        const unsigned int vertexIdx = i * VERTEX_DATA_STRIDE;
        this->vertexData[vertexIdx + 6] = this->color.x;
        this->vertexData[vertexIdx + 7] = this->color.y;
        this->vertexData[vertexIdx + 8] = this->color.z;
    }
}

Model::~Model ()
{
    free(this->vertexData);
}

const unsigned int Model::getTriangleCount () const
{
    return this->triangleCount;
}

const unsigned int Model::getVertexCount () const
{
    return this->vertexCount;
}

const size_t Model::getVertexDataSize () const
{
    return this->vertexDataSize;
}

const float* const Model::getVertexData () const
{
    return this->vertexData;
}

const glm::vec3 Model::getColor () const
{
    return this->color;
}

std::ostream& operator<<(std::ostream& os, const Model& model)
{
    const float* const vertices = model.getVertexData();

    for (int i = 0; i < model.getVertexCount(); ++i)
    {
        const int vertexIdx = i * VERTEX_DATA_STRIDE;

        os << vertices[vertexIdx] << " " << vertices[vertexIdx + 1] << " " << vertices[vertexIdx + 2];
        os << ", ";
        os << vertices[vertexIdx + 3] << " " << vertices[vertexIdx + 4] << " " << vertices[vertexIdx + 5];
        os << std::endl;
    }

    os << model.getTriangleCount() << " " << model.getVertexCount() << " " << model.getVertexDataSize();

    return os;
}