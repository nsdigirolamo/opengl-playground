#include "model.hpp"
#include "glm/glm.hpp"
#include "glad/glad.h"

#include <fstream>
#include <iostream>
#include <sstream>

void Model::fillVertexPositions (std::stringstream& vertexPositions)
{
    for (int i = 0; i < this->vertexCount; ++i)
    {
        const unsigned int vertexIdx = i * VERTEX_DATA_STRIDE;
        vertexPositions >> this->vertexData[vertexIdx];
        vertexPositions >> this->vertexData[vertexIdx + 1];
        vertexPositions >> this->vertexData[vertexIdx + 2];
    }
}

void Model::fillVertexSurfaceNormals ()
{
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
}

void Model::fillVertexColors ()
{
    for (int i = 0; i < this->vertexCount; ++i)
    {
        const unsigned int vertexIdx = i * VERTEX_DATA_STRIDE;
        this->vertexData[vertexIdx + 6] = this->color.x;
        this->vertexData[vertexIdx + 7] = this->color.y;
        this->vertexData[vertexIdx + 8] = this->color.z;
    }
}

Model::Model (const char* modelPath, glm::vec3 color)
    : color(color)
{
    std::ifstream file;
    file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    std::stringstream modelData;

    try
    {
        file.open(modelPath);
        modelData << file.rdbuf();
        file.close();
    }
    catch(const std::ifstream::failure &e)
    {
        std::cerr << "Model '" << modelPath << "' File Read Error: " << e.what() << std::endl;
        exit(-1);
    }

    modelData >> this->triangleCount;

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

    this->fillVertexPositions(modelData);
    this->fillVertexSurfaceNormals();
    this->fillVertexColors();

    glGenVertexArrays(1, &(this->vertexArray));
    glBindVertexArray(this->vertexArray);

    glGenBuffers(1, &(this->vertexBuffer));
    glBindBuffer(GL_ARRAY_BUFFER, this->vertexBuffer);

    glBufferData(GL_ARRAY_BUFFER, this->vertexDataSize, this->vertexData, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, VERTEX_DATA_STRIDE * sizeof(float), (void*)(0));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, VERTEX_DATA_STRIDE * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, VERTEX_DATA_STRIDE * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
}

Model::~Model ()
{
    free(this->vertexData);
}

void Model::bindVertexArray () const
{
    glBindVertexArray(this->vertexArray);
}

void Model::bindVertexBuffer () const
{
    glBindBuffer(GL_ARRAY_BUFFER, this->vertexBuffer);
}

void Model::drawVertexArray () const
{
    glDrawArrays(GL_TRIANGLES, 0, this->vertexCount);
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