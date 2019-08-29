#pragma once

#include <vector>
#include <glm/glm.hpp>
#include "mesh.h"
#include "texture_container.h"

namespace bowshock {

struct SceneInfo;
class Shader;

class Model {
public:
    Model(const SceneInfo& scene_info, TextureContainer& tc);

    void load_into_gpu();
    void draw(const Shader& shader);

private:
    std::string         path_;
    bool                in_gpu_;
    std::vector<Mesh>   meshes_;
    glm::mat4           transform_;
};

}