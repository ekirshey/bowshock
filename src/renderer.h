#pragma once

namespace bowshock {

struct Scene {

};

namespace Renderer {
    void initialize();
    void clear_screen(float red, float green, float blue);

    // Create a scene structure
    // Take in the model container and scene
    // Most likely have a draw abstraction at the engine level
    void draw_shadows();
    void draw_scene();
}

}
