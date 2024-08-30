#include "glad/glad.h"
#include "shader.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <material.hpp>

void Shader::checkCompilerErrors (unsigned int shader, std::string type)
{
    constexpr std::size_t infoLength = 1024;
    char info [infoLength];

    int success;

    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(shader, infoLength, NULL, info);
        std::cerr << "Shader Compilation Error of Type: " << type << std::endl << "Info: " << info;
    }
}

Shader::Shader (const char* vertexShaderPath, const char* fragmentShaderPath)
{
    std::string vertexShaderSource;
    std::string fragmentShaderSource;

    std::ifstream vertexShaderFile;
    std::ifstream fragmentShaderFile;

    vertexShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fragmentShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try
    {
        vertexShaderFile.open(vertexShaderPath);
        fragmentShaderFile.open(fragmentShaderPath);

        std::stringstream vertexShaderStream;
        std::stringstream fragmentShaderStream;

        vertexShaderStream << vertexShaderFile.rdbuf();
        fragmentShaderStream << fragmentShaderFile.rdbuf();

        vertexShaderFile.close();
        fragmentShaderFile.close();

        vertexShaderSource = vertexShaderStream.str();
        fragmentShaderSource = fragmentShaderStream.str();
    }
    catch (std::ifstream::failure &e)
    {
        std::cerr << "Shader File Read Error: " << e.what() << std::endl;
        exit(-1);
    }

    const char* vertexShaderString = vertexShaderSource.c_str();
    const char* fragmentShaderString = fragmentShaderSource.c_str();

    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderString, NULL);
    glCompileShader(vertexShader);
    this->checkCompilerErrors(vertexShader, "VERTEX");

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderString, NULL);
    glCompileShader(fragmentShader);
    this->checkCompilerErrors(fragmentShader, "FRAGMENT");

    this->programID = glCreateProgram();
    glAttachShader(this->programID, vertexShader);
    glAttachShader(this->programID, fragmentShader);
    glLinkProgram(this->programID);
    this->checkCompilerErrors(this->programID, "PROGRAM");

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void Shader::use ()
{
    glUseProgram(this->programID);
}

void Shader::setInt (const std::string &name, const int &num) const
{
    glUniform1i(glGetUniformLocation(this->programID, name.c_str()), num);
}

void Shader::setFloat (const std::string &name, const float &num) const
{
    glUniform1f(glGetUniformLocation(this->programID, name.c_str()), num);
}

void Shader::setMat4 (const std::string &name, const glm::mat4 &mat) const
{
    glUniformMatrix4fv(glGetUniformLocation(this->programID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::setVec3 (const std::string &name, const glm::vec3 &vec) const
{
    glUniform3fv(glGetUniformLocation(this->programID, name.c_str()), 1, &vec[0]);
}
