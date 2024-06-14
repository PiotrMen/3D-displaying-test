#pragma once

#include <string>
#include <vector>
#include <glm/glm.hpp>

class Model
{
public:
    Model(const std::string& path) : _path(path), _VAO(0), _VBO(0) {}
    void setupModel();
    void loadModel();
    void bind() const;
    const std::vector<float>& getVertices() const;
    void cleanup();
private:
    std::string _path;
    unsigned int _VAO, _VBO;
    std::vector<float> _vertices;
};

// void setModelViewProjection(unsigned int shaderProgram, const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection);

