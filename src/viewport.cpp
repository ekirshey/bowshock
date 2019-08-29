#include "viewport.h"
#include <glad/glad.h>

#include <glm/gtc/matrix_transform.hpp>

namespace bowshock {

Viewport::Viewport(int width, int height, float z_near, float z_far)
    : width_(width)
    , height_(height)
    , z_near_(z_near)
    , z_far_(z_far)
{

}

glm::mat4 Viewport::projection(float fov) 
{
    return glm::perspective( glm::radians(fov), 
                             (float)width_ / height_, 
                             z_near_,
                             z_far_ );
}

void Viewport::resize(int width, int height)
{
    width_ = width;
    height_ = height;

    glViewport(0, 0, width_, height_);
}

}