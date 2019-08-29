#pragma once

#include <glm/glm.hpp>

namespace bowshock {
/* Multiple viewports can be assiociated with opengl
and I think that's how I would do
*/
class Viewport {
public:
    Viewport(int width, int height, float z_near, float z_far);

    glm::mat4 projection(float fov_deg);
    void resize(int width, int height);

private:
    int width_;
    int height_;
    float z_near_;
    float z_far_;
};


}