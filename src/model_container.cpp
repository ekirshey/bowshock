#include "model_container.h"
#include "shader.h"

namespace bowshock {

ModelContainer::ModelContainer() {

}

void ModelContainer::add_model(const SceneInfo& sceneinfo, TextureContainer& tc)
{
    models_.push_back( Model( sceneinfo, tc ) );
}

void ModelContainer::load_model_into_gpu(unsigned int idx) {
    models_[idx].load_into_gpu();
}

void ModelContainer::draw(unsigned int idx,
                          const Shader& shader)
{
    // Make sure the model exists/loaded
    if (idx < models_.size()) {
        models_[idx].load_into_gpu();	// I need to create an actual asset loader instead of checking each frame
        models_[idx].draw(shader);
    }
}

}