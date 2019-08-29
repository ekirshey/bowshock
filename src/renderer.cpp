#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "spdlog/spdlog.h"

#include "renderer.h"

namespace bowshock {

namespace Renderer {
    void initialize() {
        glEnable(GL_DEPTH_TEST);

        // Gamma correction
        // Doing it in the shader right now for a bit more control
        //glEnable(GL_FRAMEBUFFER_SRGB);
    }

    void clear_screen(float red, float green, float blue) {
        glClearColor(red, green, blue, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_FRAMEBUFFER_SRGB);
    }

    void draw_shadows() {

    }

    void draw_scene() {

    }
}
}