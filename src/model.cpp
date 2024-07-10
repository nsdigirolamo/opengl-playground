#include "model.hpp"

#include <fstream>
#include <iostream>
#include <sstream>

Model::Model (const char* modelPath)
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
    this->vertexCount = this->triangleCount * 3;
    this->vertexDataSize = this->vertexCount * 3 * sizeof(float);

    this->vertexData = (float*)(malloc(this->vertexDataSize));

    for (int i = 0; i < this->vertexDataSize; ++i)
    {
        stream >> this->vertexData[i];
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

std::ostream& operator<<(std::ostream& os, const Model& model)
{

    const float* const vertices = model.getVertexData();

    for (int i = 0; i < model.getVertexDataSize(); i += 3)
    {
        os << vertices[i] << " " << vertices[i + 1] << " " << vertices[i + 2] << std::endl;
    }

    os << model.getTriangleCount() << " " << model.getVertexCount() << " " << model.getVertexDataSize();

    return os;
}