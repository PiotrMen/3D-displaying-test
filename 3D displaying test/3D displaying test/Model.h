#pragma once

#include <string>
#include <vector>
#include <glm/glm.hpp>

class Model
{
public:
    Model(const std::string& path) : _path(path){}
    //void setupModel(unsigned int shaderProgram, int width, int height);
    //void renderModel(unsigned int shaderProgram);
    //void cleanupModel();
    void loadModel();
    const std::vector<float>& getVertices() const;

private:
    std::string _path;
    std::vector<float> vertices;
    //unsigned int VBO, VAO;
};

void setModelViewProjection(unsigned int shaderProgram, const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection);

