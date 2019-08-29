#include "camera.h"
#include <glm/gtc/matrix_transform.hpp>

namespace bowshock {

Camera::Camera(glm::vec3 pos,
               glm::vec3 front,
               glm::vec3 up,
               float fov,
               float speed)
    : pos_(pos)
    , front_(front)
    , up_(up)
    , fov_(fov)
    , speed_(speed)
{
}

glm::mat4 Camera::lookat() const {
    return glm::lookAt(pos_, pos_ + front_, up_);
}

void Camera::move(glm::vec3 norm_dir, float delta_time) {
    float curr_speed = speed_ * delta_time;

    if ( norm_dir.x != 0.0f )
        pos_ += norm_dir.x * glm::normalize(glm::cross(front_, up_)) * curr_speed;

    if ( norm_dir.y != 0.0f )
        pos_ += norm_dir.y * curr_speed * up_;
         
    if ( norm_dir.z != 0.0f )
        pos_ += norm_dir.z * curr_speed * front_;
}

void Camera::rotate(float yaw, float pitch)
{
    front_.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
    front_.y = sin(glm::radians(pitch));
    front_.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
}

void Camera::zoom(float amount)
{
    if (fov_ >= 1.0f && fov_ <= 45.0f)
        fov_ -= amount;
    if (fov_ <= 1.0f)
        fov_ = 1.0f;
    if (fov_ >= 45.0f)
        fov_ = 45.0f;
}

float Camera::fov() const
{
    return fov_;
}

glm::vec3 Camera::pos() const
{
    return pos_;
}

glm::vec3 Camera::front() const
{
    return front_;
}

}