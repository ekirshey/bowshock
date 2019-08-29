#pragma once

#include <vector>
#include "model.h"
#include "texture_container.h"

namespace bowshock {

class Shader;

class ModelContainer {
public:
    ModelContainer();

    void add_model(const SceneInfo& sceneinfo, TextureContainer& tc);
    void load_model_into_gpu(unsigned int idx);
    void draw(unsigned int idx, 
              const Shader& shader);
private:
    std::string modelpath_;
    std::vector<Model> models_;

};

}