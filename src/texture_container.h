#pragma once

#include <string>
#include <unordered_map>

namespace bowshock {

enum class TextureType {
    DIFFUSE,
    SPECULAR,
    EMISSIVE
};

struct Texture {
    unsigned int id;
    std::string path;
    TextureType type;
};

class TextureContainer {
public:
    TextureContainer() {}

    Texture load_texture_from_file( std::string path,
                                    TextureType type);

private:
    std::vector<Texture> textures_;
};

}