#include "model.hpp"
#include "glm/glm.hpp"
#include "glad/glad.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <iomanip>

void Model::constructFromObj (const char* filePath)
{
    std::ifstream file;
    file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    std::stringstream objDataStream;

    try
    {
        file.open(filePath);
        objDataStream << file.rdbuf();
        file.close();
    }
    catch(const std::ifstream::failure &e)
    {
        std::cerr << "File Read Error for '" << filePath << "': " << e.what() << std::endl;
        exit(-1);
    }

    unsigned int vertexCount = 0;
    unsigned int surfaceNormalCount = 0;
    unsigned int uvCoordinateCount = 0;
    unsigned int faceCount = 0;

    std::stringstream vertexStream;
    std::stringstream surfaceNormalStream;
    std::stringstream uvCoordinateStream;
    std::stringstream faceStream;

    std::string line;

    while (std::getline(objDataStream, line))
    {

        if (line[0] == 'v' && line[1] == ' ')
        {
            vertexStream << line << "\n";
            ++vertexCount;
        }
        else if (line[0] == 'v' && line[1] == 'n' && line[2] == ' ')
        {
            surfaceNormalStream << line << "\n";
            ++surfaceNormalCount;
        }
        else if (line[0] == 'v' && line[1] == 't' && line[2] == ' ')
        {
            uvCoordinateStream << line << "\n";
            ++uvCoordinateCount;
        }
        else if (line[0] == 'f' && line[1] == ' ')
        {
            faceStream << line << "\n";
            ++faceCount;
        }
    }

    glm::vec3* vertices = (glm::vec3*) malloc(vertexCount * sizeof(glm::vec3));
    glm::vec3* surfaceNormals = (glm::vec3*) malloc(surfaceNormalCount * sizeof(glm::vec3));
    glm::vec2* uvCoordinates = (glm::vec2*) malloc(uvCoordinateCount * sizeof(glm::vec2));

    for (int i = 0; i < vertexCount; ++i)
    {
        std::string throwAwayID;
        vertexStream >> throwAwayID;
        vertexStream >> vertices[i].x;
        vertexStream >> vertices[i].y;
        vertexStream >> vertices[i].z;
    }

    for (int i = 0; i < surfaceNormalCount; ++i)
    {
        std::string throwAwayID;
        surfaceNormalStream >> throwAwayID;
        surfaceNormalStream >> surfaceNormals[i].x;
        surfaceNormalStream >> surfaceNormals[i].y;
        surfaceNormalStream >> surfaceNormals[i].z;
    }

    for (int i = 0; i < uvCoordinateCount; ++i)
    {
        std::string throwAwayID;
        uvCoordinateStream >> throwAwayID;
        uvCoordinateStream >> uvCoordinates[i].x;
        uvCoordinateStream >> uvCoordinates[i].y;
    }

    this->vertexDataCount = faceCount * VERTICES_PER_FACE;
    this->vertexDataSize = this->vertexDataCount * (VERTEX_SIZE + SURFACE_NORMAL_SIZE + UV_COORDINATE_SIZE);
    this->vertexData = (float*) malloc(this->vertexDataSize);

    for (int i = 0; i < faceCount; ++i) {

        std::string throwAwayID;
        faceStream >> throwAwayID;

        for (int j = 0; j < 3; ++j)
        {
            int vertexDataIndex = VERTEX_DATA_STRIDE * ((i * VERTICES_PER_FACE) + j);

            std::string vertexInfo;
            faceStream >> vertexInfo;

            int delimiterPosition = vertexInfo.find('/');
            int vertexIndex = std::stoi(vertexInfo.substr(0, delimiterPosition)) - 1;

            vertexInfo = vertexInfo.substr(delimiterPosition + 1);
            delimiterPosition = vertexInfo.find('/');
            int uvCoordinateIndex = std::stoi(vertexInfo.substr(0, delimiterPosition)) - 1;

            vertexInfo = vertexInfo.substr(delimiterPosition + 1);
            int surfaceNormalIndex = std::stoi(vertexInfo) - 1;

            this->vertexData[vertexDataIndex] = vertices[vertexIndex].x;
            this->vertexData[vertexDataIndex + 1] = vertices[vertexIndex].y;
            this->vertexData[vertexDataIndex + 2] = vertices[vertexIndex].z;

            this->vertexData[vertexDataIndex + 3] = surfaceNormals[surfaceNormalIndex].x;
            this->vertexData[vertexDataIndex + 4] = surfaceNormals[surfaceNormalIndex].y;
            this->vertexData[vertexDataIndex + 5] = surfaceNormals[surfaceNormalIndex].z;

            this->vertexData[vertexDataIndex + 6] = uvCoordinates[uvCoordinateIndex].x;
            this->vertexData[vertexDataIndex + 7] = uvCoordinates[uvCoordinateIndex].y;
        }
    }

    free(vertices);
    free(surfaceNormals);
    free(uvCoordinates);
}

Model::Model (const char* objFilePath)
{
    this->constructFromObj(objFilePath);

    glGenVertexArrays(1, &(this->vertexArray));
    glBindVertexArray(this->vertexArray);

    glGenBuffers(1, &(this->vertexBuffer));
    glBindBuffer(GL_ARRAY_BUFFER, this->vertexBuffer);

    glBufferData(GL_ARRAY_BUFFER, this->vertexDataSize, this->vertexData, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, VERTEX_DATA_STRIDE * sizeof(float), (void*)(0));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, VERTEX_DATA_STRIDE * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, VERTEX_DATA_STRIDE * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
}

Model::~Model ()
{
    free(this->vertexData);
}

const size_t Model::getVertexDataSize () const
{
    return this->vertexDataSize;
}

const float* const Model::getVertexData () const
{
    return this->vertexData;
}

const unsigned int Model::getVertexDataCount () const
{
    return this->vertexDataCount;
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
    glDrawArrays(GL_TRIANGLES, 0, this->vertexDataCount);
}

std::ostream& operator<<(std::ostream& os, const Model& model)
{
    const float* const vertices = model.getVertexData();

    for (int i = 0; i < model.getVertexDataCount(); ++i)
    {
        const int vertexIdx = i * VERTEX_DATA_STRIDE;

        for (int j = 0; j < VERTEX_DATA_STRIDE; ++j)
        {
            if (j == 0 || j == 3 || j == 6)
            {
                os << "[ ";
            }

            os << std::setw(10) << std::fixed << std::setprecision(7) << vertices[vertexIdx + j] << " ";

            if (j == 2 || j == 5 || j == 7)
            {
                os << "], ";
            }

            if (j == VERTEX_DATA_STRIDE - 1)
            {
                os << "\n";
            }
        }
    }

    os << model.getVertexDataCount() << " " << model.getVertexDataSize() << std::endl;

    return os;
}