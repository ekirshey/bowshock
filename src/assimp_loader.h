#pragma once
#include <string>
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/postprocess.h>     // Post processing flags

namespace bowshock {

struct SceneInfo {
    std::string filepath;
    const aiScene* scene;
    SceneInfo(std::string p, const aiScene* s)
        : filepath(p)
        , scene(s)
    {}
};

class ModelContainer;
class TextureContainer;

namespace AssimpLoader {
    void load(std::string model_registry,
        ModelContainer& mc,
        TextureContainer& tc,
        unsigned int flags);
};

}