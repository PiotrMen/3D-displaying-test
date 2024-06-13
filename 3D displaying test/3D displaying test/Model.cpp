#include "Model.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <iostream>
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


void Model::loadModel()
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(this->_path, aiProcess_Triangulate | aiProcess_FlipUVs);
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
        return;
    }

    aiMesh* mesh = scene->mMeshes[0];
    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        this->_vertices.push_back(mesh->mVertices[i].x);
        this->_vertices.push_back(mesh->mVertices[i].y);
        this->_vertices.push_back(mesh->mVertices[i].z);
    }
}

const std::vector<float>& Model::getVertices() const
{
    return this->_vertices;
}

void Model::setupModel()
{
    // Generate and bind vertex array and buffer objects
    glGenVertexArrays(1, &this->_VAO);
    glGenBuffers(1, &this->_VBO);

    glBindVertexArray(this->_VAO);

    glBindBuffer(GL_ARRAY_BUFFER, this->_VBO);
    glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(float), _vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0); // Unbind VAO to prevent accidental modifications
}

void Model::bind() const
{
    glBindVertexArray(this->_VAO);
}

void Model::cleanup() {
    glDeleteVertexArrays(1, &this->_VAO);
    glDeleteBuffers(1, &this->_VBO);
}


void setModelViewProjection(unsigned int shaderProgram, const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection)
{
    unsigned int modelLoc = glGetUniformLocation(shaderProgram, "model");
    unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
    unsigned int projectionLoc = glGetUniformLocation(shaderProgram, "projection");

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
}