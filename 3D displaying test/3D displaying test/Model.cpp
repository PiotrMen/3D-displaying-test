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
    const aiScene* scene = importer.ReadFile(this->_path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
        return;
    }

    aiMesh* mesh = scene->mMeshes[0];
    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
		Vertex vertex;
		vertex.position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
		if (mesh->HasNormals()) {
			vertex.normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
		}

		vertex.color = glm::vec3(1.0f, 1.0f, 1.0f); // Initial color (white), will be recalculated in the vertex shader
		this->_vertices.emplace_back(vertex);
    }
}

const std::vector<Vertex>& Model::getVertices() const
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
    glBufferData(GL_ARRAY_BUFFER, this->_vertices.size() * sizeof(Vertex), this->_vertices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));

    glEnableVertexAttribArray(0);

    glBindVertexArray(0); // Unbind VAO to prevent accidental modifications
}

Model::Model(const std::string& path) : _path(path), _VAO(0), _VBO(0), _model(glm::mat4(1.f))
{
    this->loadModel();
    this->setupModel();
}

Model::Model(const std::vector<Vertex>& verticies) : _vertices(verticies)
{
    this->setupModel();
}

void Model::bind() const
{
    glBindVertexArray(this->_VAO);
}

void Model::cleanup() {
    glDeleteVertexArrays(1, &this->_VAO);
    glDeleteBuffers(1, &this->_VBO);
}

glm::mat4 Model::getModelMatrix() const noexcept
{
    return this->_model;
}

glm::mat4 Model::setModelMatrix(const glm::mat4& modelToSet) noexcept
{
    return this->_model = modelToSet;
}
