#pragma once

#include <assimp/scene.h>
#include "mesh.h"
#include "texture_container.h"
#include "spdlog/spdlog.h"
#include "shader.h"

namespace bowshock {

Mesh::Mesh(const std::string& modelpath,
           TextureContainer& tc,
           const aiMesh* ai_m,
           const aiScene* scene)
    : vertices_{ ai_m->mNumVertices }
    , ambient_(1.0f, 1.0f, 1.0f)
    , diffuse_(1.0f, 1.0f, 1.0f)
    , specular_(1.0f, 1.0f, 1.0f)
{
    // Walk through each of the ai_m's vertices
    for (unsigned int i = 0; i < ai_m->mNumVertices; i++) {
        Vertex vertex;
        glm::vec3 vector; // We declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
                          // Positions
        vector.x = ai_m->mVertices[i].x;
        vector.y = ai_m->mVertices[i].y;
        vector.z = ai_m->mVertices[i].z;
        vertex.position = vector;

        // Normals
        if (ai_m->HasNormals()) {
            vector.x = ai_m->mNormals[i].x;
            vector.y = ai_m->mNormals[i].y;
            vector.z = ai_m->mNormals[i].z;
            vertex.normal = vector;
        }

        // Texture Coordinates
        if (ai_m->mTextureCoords[0]) { // Does the mesh contain texture coordinates?
            glm::vec2 vec;
            // A vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
            // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
            vec.x = ai_m->mTextureCoords[0][i].x;
            vec.y = ai_m->mTextureCoords[0][i].y;
            vertex.texcoords = vec;
        }
        else {
            vertex.texcoords = glm::vec2(0.0f, 0.0f);
        }

        vertices_[i] = vertex;
    }

    // Now walk through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
    for (unsigned int i = 0; i < ai_m->mNumFaces; i++) {
        aiFace face = ai_m->mFaces[i];
        // Retrieve all indices of the face and store them in the indices vector
        for (unsigned int j = 0; j < face.mNumIndices; j++) {
            indices_.push_back(face.mIndices[j]);
        }
    }

    // process materials
    aiMaterial* material = scene->mMaterials[ai_m->mMaterialIndex];
    load_material_textures_(modelpath, tc, material);

}

Mesh::Mesh(const std::vector<Vertex>& vertices,
           const std::vector<GLuint>& indices,
           const std::vector<Texture>& textures,
           float shininess)
    : vertices_(vertices)
    , indices_(indices)
    , textures_(textures)
    , ambient_(1.0f, 1.0f, 1.0f)
    , diffuse_(1.0f, 1.0f, 1.0f)
    , specular_(1.0f, 1.0f, 1.0f)
    , shininess_(shininess)
{

}

void Mesh::load_into_gpu() {
    // Create buffers/arrays
    glGenVertexArrays(1, &VAO_);
    glGenBuffers(1, &VBO_);
    if (indices_.size() > 0) {
        glGenBuffers(1, &EBO_);
    }

    glBindVertexArray(VAO_);
    // Load data into vertex buffers
    glBindBuffer(GL_ARRAY_BUFFER, VBO_);
    glBufferData(GL_ARRAY_BUFFER, vertices_.size() * sizeof(Vertex), &vertices_[0], GL_STATIC_DRAW);

    if (indices_.size() > 0) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_.size() * sizeof(GLuint), &indices_[0], GL_STATIC_DRAW);
    }

    // Set the vertex attribute pointers
    // Vertex Positions
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    // Vertex Normals
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(1);
    // Vertex Texture Coords
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texcoords));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}

Mesh::~Mesh() {
    unload_from_gpu();
}

void Mesh::unload_from_gpu() {
    glDeleteVertexArrays(1, &VAO_);
    glDeleteBuffers(1, &VBO_);

    if (indices_.size() > 0) {
        glDeleteBuffers(1, &EBO_);
    }
}

void Mesh::draw(const Shader& shader) {
    unsigned int tct = 0;

    for (auto& t : textures_) {
        glActiveTexture(GL_TEXTURE0 + tct);
        if (t.type == TextureType::DIFFUSE) {
            glUniform1i(shader.get_uniform_location("material.diffuse"), tct);
        }
        else if (t.type == TextureType::SPECULAR) {
            glUniform1i(shader.get_uniform_location("material.specular"), tct);
        }
        else if (t.type == TextureType::EMISSIVE) {
            glUniform1i(shader.get_uniform_location("material.emission"), tct);
        }
        glBindTexture(GL_TEXTURE_2D, t.id);
        tct++;
    }

    // According to the spec, if the location is -1 then the call is silently ignored
    // If the uniform location is not found, then -1 is returned.
    // So effectively, if there is no uniform of the specified name, nothing happens
    glUniform3fv(shader.get_uniform_location("material.diffuse_color"), 1, &diffuse_[0]);
    glUniform3fv(shader.get_uniform_location("material.ambient_color"), 1, &ambient_[0]);
    glUniform1f(shader.get_uniform_location("material.shininess"), 32.0f);

    glBindVertexArray(VAO_);

    if ( indices_.size() > 0 ) {
        glDrawElements(GL_TRIANGLES, (GLsizei)indices_.size(), GL_UNSIGNED_INT, 0);
    }
    else {
        glDrawArrays(GL_TRIANGLES, 0, (GLsizei)vertices_.size());
    }

    glBindVertexArray(0);
}

void Mesh::load_material_textures_(const std::string& modelpath,
                                   TextureContainer& tc,
                                   aiMaterial *mat)
{
    aiString name;
    mat->Get(AI_MATKEY_NAME, name);

    spdlog::debug("Processing material {}", name.C_Str());

    aiColor4D a_ambient;
    mat->Get(AI_MATKEY_COLOR_AMBIENT, a_ambient);

    ambient_.r = a_ambient.r;
    ambient_.g = a_ambient.g;
    ambient_.b = a_ambient.b;

    aiColor4D a_diffuse;
    mat->Get(AI_MATKEY_COLOR_DIFFUSE, a_diffuse);

    diffuse_.r = a_diffuse.r;
    diffuse_.g = a_diffuse.g;
    diffuse_.b = a_diffuse.b;

    aiColor4D a_specular;
    mat->Get(AI_MATKEY_COLOR_SPECULAR, a_specular);

    specular_.r = a_specular.r;
    specular_.g = a_specular.g;
    specular_.b = a_specular.b;

    mat->Get(AI_MATKEY_SHININESS, shininess_);

    aiTextureType type = aiTextureType_DIFFUSE;
    TextureType type_name = TextureType::DIFFUSE;
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
        aiString str;
        mat->GetTexture(type, i, &str);
        std::string texpath(str.C_Str());
        std::string path = modelpath + texpath;
        spdlog::info("Loading diffuse texture: {}", path);
        textures_.push_back(tc.load_texture_from_file(path, type_name));
    }

    type = aiTextureType_SPECULAR;
    type_name = TextureType::SPECULAR;
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
        aiString str;
        mat->GetTexture(type, i, &str);
        std::string texpath(str.C_Str());
        std::string path = modelpath + texpath;
        spdlog::info("Loading specular texture: {}", path);
        textures_.push_back(tc.load_texture_from_file(path, type_name));
    }

    type = aiTextureType_EMISSIVE;
    type_name = TextureType::EMISSIVE;
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
        aiString str;
        mat->GetTexture(type, i, &str);
        std::string texpath(str.C_Str());
        std::string path = modelpath + texpath;
        spdlog::info("Loading specular texture: {}", path);
        textures_.push_back(tc.load_texture_from_file(path, type_name));
    }
}

}