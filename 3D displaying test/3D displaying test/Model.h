#pragma once

#include <string>
#include <vector>
#include <glm/glm.hpp>

struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec3 color;
};

class Model
{
public:
    explicit Model(const std::string& path);
    explicit Model(const std::vector<Vertex>& verticies);
    void bind() const;
    const std::vector<Vertex>& getVertices() const;
    void cleanup();
    glm::mat4 getModelMatrix() const noexcept;
    glm::mat4 setModelMatrix(const glm::mat4& modelToSet) noexcept;

private:
    void setupModel();
    void loadModel();

private:
    std::string _path;
    unsigned int _VAO, _VBO;
    std::vector<Vertex> _vertices;
    glm::mat4 _model;
};

// void setModelViewProjection(unsigned int shaderProgram, const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection);

