#include <iostream>
#include <sstream>
#include <fstream>
#include <assimp/scene.h>           // Output data structure
#include <nlohmann/json.hpp>
#include "assimp_loader.h"
#include "spdlog/spdlog.h"
#include "model_container.h"
#include "texture_container.h"

namespace bowshock {

namespace {
    std::string read_file(std::string file) {
        // Open file
        std::ifstream ifstrm(file);
        if (!ifstrm.is_open()) {
            return "";
        }

        // Read file into buffer
        std::stringstream buffer;
        buffer << ifstrm.rdbuf();

        // Make a std::string and fill it with the contents of buffer
        std::string file_content = buffer.str();

        return file_content;
    }
}

namespace AssimpLoader {
	void load(std::string model_registry, 
              ModelContainer& mc, 
              TextureContainer& tc,
              unsigned int flags) 
    {
		using json = nlohmann::json;
		json j = json::parse(read_file(model_registry));
		auto registry = j.at("registry");
		std::string rootmodelpath = j.at("root_model_path");
        
        spdlog::info("Loading models from {}", rootmodelpath);
		std::vector<std::string> loadedscenes;
		for (int i = 0; i < registry.size(); i++) {
			std::string model_path = registry[i].at("model_path");
			std::string fullpath = rootmodelpath + model_path;
            spdlog::info("Model Path: {} ", fullpath);
            
			// Check if scene has already been loaded
			bool existingscene = false;
			for (auto p : loadedscenes) {
				if (p == fullpath) {
					existingscene = true;
					break;
				}
			}
			if (existingscene) {
				continue;
			}

			Assimp::Importer importer;
			const aiScene* scene = importer.ReadFile(fullpath, flags);
			// Check for errors
			if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) { // if is Not Zero
				std::cerr << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
			}
			else {
				SceneInfo sceneinfo(fullpath, scene);
                mc.add_model(sceneinfo, tc);
			}
		}
        
	}
}

}