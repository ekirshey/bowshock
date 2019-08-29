#include <fstream>
#include <sstream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "spdlog/spdlog.h"

namespace bowshock {

namespace {
    void check_error(unsigned int shader, unsigned int type, const std::string message) {
        int success;
        char infoLog[512];
        glGetShaderiv(shader, type, &success);

        if (!success) {
            glGetShaderInfoLog(shader, 512, NULL, infoLog);
            spdlog::error("ERROR::{}: {}", message, infoLog);
        }
    }

    unsigned int compile_vertex_shader(std::string file) {
        // Load from file
        std::ifstream vf(file); // not worth fiddling with paths at this stage of the game
        std::stringstream buffer;
        buffer << vf.rdbuf();

        std::string vertexShaderSource = buffer.str();

        unsigned int vertexShader;
        vertexShader = glCreateShader(GL_VERTEX_SHADER);

        // c_str() gives us a const char*, but we need a non-const one
        char* src = const_cast<char*>(vertexShaderSource.c_str());
        GLint size = (GLint)vertexShaderSource.length();

        glShaderSource(vertexShader, 1, &src, NULL);
        glCompileShader(vertexShader);

        check_error(vertexShader, GL_COMPILE_STATUS, "SHADER::VERTEX::COMPILATION_FAILED");

        return vertexShader;
    }

    unsigned int compile_fragment_shader(std::string file) {
        // Load from file
        std::ifstream ff(file); // not worth fiddling with paths at this stage of the game
        std::stringstream buffer;
        buffer << ff.rdbuf();

        std::string fragmentShaderSource = buffer.str();

        unsigned int fragmentShader;
        fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

        // c_str() gives us a const char*, but we need a non-const one
        char* src = const_cast<char*>(fragmentShaderSource.c_str());
        GLint size = (GLint)fragmentShaderSource.length();

        glShaderSource(fragmentShader, 1, &src, NULL);
        glCompileShader(fragmentShader);

        check_error(fragmentShader, GL_COMPILE_STATUS, "SHADER::FRAGMENT::COMPILATION_FAILED");

        return fragmentShader;
    }

    unsigned int link_program(unsigned int vs, unsigned int fs) {
        unsigned int shaderProgram;
        shaderProgram = glCreateProgram();

        glAttachShader(shaderProgram, vs);
        glAttachShader(shaderProgram, fs);
        glLinkProgram(shaderProgram);

        check_error(shaderProgram, GL_LINK_STATUS, "SHADER::PROGRAM::LINK_FAILED");

        return shaderProgram;
    }

    unsigned int create_program(std::string vertex_file, std::string frag_file) {
        auto vertexShader = compile_vertex_shader(vertex_file);
        auto fragmentShader = compile_fragment_shader(frag_file);

        unsigned int shaderProgram = link_program(vertexShader, fragmentShader);

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        return shaderProgram;
    }
}

Shader::Shader(const std::string& vertex, const std::string& frag)
{
    id_ = create_program(vertex, frag);

    model_loc_ = get_uniform_location("model");
    view_proj_loc_ = get_uniform_location("viewProj");
    normal_loc_ = get_uniform_location("normalMat");
}

void Shader::set_mvp(const glm::mat4& model, const glm::mat4& view_proj)
{
    glUniformMatrix4fv(model_loc_, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(view_proj_loc_, 1, GL_FALSE, glm::value_ptr(view_proj));

    /*
        Calculate normal matrix here. Seems like a reasonable thing to do.
        Not sure if I'll ever want to user to be able to calculate this
        This is usually done in eye space and there's an inverseTranspose function
        in gtx
    */
    glm::mat3 normal_matrix = glm::transpose(glm::inverse(glm::mat3(model)));
    glUniformMatrix3fv(normal_loc_, 1, GL_FALSE, glm::value_ptr(normal_matrix));
}

void Shader::use()
{
    glUseProgram(id_);
}

unsigned int Shader::get_uniform_location(const std::string & name) const
{
    return glGetUniformLocation(id_, name.c_str());
}
}