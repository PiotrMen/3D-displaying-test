#pragma once
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <iostream>
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shaders.h"

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
    ~Model();
    void bind() const;
    const std::vector<Vertex>& getVertices() const;
    glm::mat4 getModelMatrix() const noexcept;
    glm::mat4 setModelMatrix(const glm::mat4& modelToSet) noexcept;

    glm::vec3 getModelPos()const;
    void translate(const glm::vec3& translationVector);
    void rotate(float angle, const glm::vec3& axisDirecton);
    void scale(const glm::vec3& scalingRatio);

private:
    void cleanup();
    void setupModel();
    void loadModel();
private:
    glm::vec3 _modelPos;
    std::string _path;
    unsigned int _VAO, _VBO;
    std::vector<Vertex> _vertices;
    glm::mat4 _model;
};

class Cube : public Model
{
public:
    Cube() : Model(createCubeVertices()) {}
    Cube(const glm::vec3& translationVector, const glm::vec3& scalingRatio);
    ~Cube() {}
private:
    std::vector<Vertex> createCubeVertices();
};