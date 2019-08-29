#include <assimp/scene.h>
#include <glm/gtc/type_ptr.hpp>
#include "model.h"
#include "assimp_loader.h"
#include "spdlog/spdlog.h"
#include "shader.h"

namespace bowshock {

namespace {
    glm::mat4 aiMatrix4x4ToGlm(const aiMatrix4x4& from) {
        glm::mat4 to;
        //the a,b,c,d in assimp is the row ; the 1,2,3,4 is the column
        to[0][0] = from.a1; to[1][0] = from.a2; to[2][0] = from.a3; to[3][0] = from.a4;
        to[0][1] = from.b1; to[1][1] = from.b2; to[2][1] = from.b3; to[3][1] = from.b4;
        to[0][2] = from.c1; to[1][2] = from.c2; to[2][2] = from.c3; to[3][2] = from.c4;
        to[0][3] = from.d1; to[1][3] = from.d2; to[2][3] = from.d3; to[3][3] = from.d4;
        return to;
    }
}

Model::Model(const SceneInfo& scene_info, TextureContainer& tc) 
    : path_(scene_info.filepath )
    , in_gpu_( false )
{
    // Build meshes
    auto scene = scene_info.scene;
    auto root = scene->mRootNode;

    for (unsigned int i = 0; i < root->mNumChildren; i++) {
        auto child = root->mChildren[i];
        if (child->mNumMeshes > 0) {
            transform_ = aiMatrix4x4ToGlm(child->mTransformation);
        }
    }

    if (scene->HasMeshes()) {
        auto lastslash = path_.rfind('/') + 1;
        for (unsigned int i = 0; i < scene->mNumMeshes; i++) 
        {
            meshes_.push_back( Mesh(path_.substr(0, lastslash), 
                                    tc, 
                                    scene->mMeshes[i], 
                                    scene) );
        }
    }

    spdlog::info("Done model processing!");
}

void Model::load_into_gpu() 
{
    if (!in_gpu_) {
        in_gpu_ = true;
        for (int i = 0; i < meshes_.size(); i++) {
            meshes_[i].load_into_gpu();
        }
    }
}

void Model::draw(const Shader& shader)
{
    if (!in_gpu_) {
        // Load the model into memory
        load_into_gpu();
    }

    for (int i = 0; i < meshes_.size(); i++) 
    {
        auto& me = meshes_[i];
        me.draw(shader);
    }
}

}