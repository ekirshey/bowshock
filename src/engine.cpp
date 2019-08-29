#include "engine.h"
#include "spdlog/spdlog.h"
#include "assimp_loader.h"
#include "renderer.h"

namespace bowshock {
Engine::Engine( const std::string& ini ) 
    : config( load_config( ini ) )
    , window( config )
{
    Renderer::initialize();

    // Load all model information
    AssimpLoader::load( config.model_registry, 
                        model_container, 
                        texture_container,
                        aiProcess_Triangulate | 
                        aiProcess_FlipUVs | 
                        aiProcess_GenNormals);
}

}