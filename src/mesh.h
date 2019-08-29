#pragma once
#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <assimp/material.h>
#include <glad/glad.h>
#include "texture_container.h"

struct aiMesh;
struct aiScene;
struct aiMaterial;

namespace bowshock {

class TextureContainer;
class Shader;

struct Vertex {
    // Position
    glm::vec3 position;
    // Normal
    glm::vec3 normal;
    // TexCoords
    glm::vec2 texcoords;
};

class Mesh {
public:
    // Load from assimp
    Mesh(const std::string& modelpath, 
         TextureContainer& tc, 
         const aiMesh* ai_m, 
         const aiScene* scene);

    Mesh(const std::vector<Vertex>& vertices,
        const std::vector<GLuint>& indices,
        const std::vector<Texture>& textures,
        float shininess);

    ~Mesh();

    void load_into_gpu();
    void unload_from_gpu();

    void draw(const Shader& shader );

private:
    std::vector<Vertex> vertices_;
    std::vector<GLuint> indices_;
    std::vector<Texture> textures_;
    GLuint VAO_;
    GLuint VBO_;
    GLuint EBO_;

    glm::vec3 ambient_;
    glm::vec3 diffuse_;
    glm::vec3 specular_;
    float shininess_;

    void load_material_textures_(const std::string& modelpath, 
                                 TextureContainer& tc, 
                                 aiMaterial *mat);
};

}
