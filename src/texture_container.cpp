#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "spdlog/spdlog.h"

#include "texture_container.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace bowshock {

namespace {
    unsigned int load_texture(std::string path,
                              TextureType type,
                              unsigned int clamp,
                              unsigned int min_filter,
                              unsigned int max_filter)
    {
        // Texture loading
        int width, height, nrComponents;
        unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrComponents, 0);

        unsigned int texture;
        glGenTextures(1, &texture);
        if (data) {
            GLenum internal_format;
            GLenum format;
            // Gamma correct the internal format for diffuse textures
            // Make sure they're in sRGB format
            if (nrComponents == 1) {
                format = GL_RED;
                internal_format = GL_RED;
            }
            else if (nrComponents == 3) {
                format = GL_RGB;
                if (type == TextureType::DIFFUSE) {
                    internal_format = GL_SRGB;
                }
                else {
                    internal_format = GL_RGB;
                }
            }
            else if (nrComponents == 4) {
                format = GL_RGBA;
                if (type == TextureType::DIFFUSE) {
                    internal_format = GL_SRGB_ALPHA;
                }
                else {
                    internal_format = GL_RGBA;
                }
            }

            glBindTexture(GL_TEXTURE_2D, texture);
            glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);

            // set the texture wrapping/filtering options (on the currently bound texture object)
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, clamp);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, clamp);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, max_filter);

        }
        else {
            spdlog::error("Failed to load container.jpg");
        }

        stbi_image_free(data);

        return texture;
    }
}

Texture TextureContainer::load_texture_from_file( std::string path,
                                                  TextureType type )
{
    Texture texture;

    for (auto& t : textures_) {
        if (t.path == path) {
            return t;
        }
    }

    texture.path = path;
    texture.type = type;
    texture.id = load_texture(path,
                              type,
                              GL_REPEAT,
                              GL_LINEAR_MIPMAP_LINEAR,
                              GL_LINEAR);

    textures_.emplace_back(texture);

    return texture;
}

}
