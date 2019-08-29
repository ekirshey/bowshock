#pragma once
#include <string>
#include <glm/glm.hpp>

namespace bowshock {

class Shader {
public:
    Shader(const std::string& vertex, const std::string& frag);
    void set_mvp(const glm::mat4& model, const glm::mat4& view_proj);
    void use();
    unsigned int get_uniform_location(const std::string& name) const;

private:
    unsigned int id_;

    // Uniforms
    unsigned int model_loc_;
    unsigned int view_proj_loc_;
    unsigned int normal_loc_;

};

}
