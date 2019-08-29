#pragma once

#include <glm/glm.hpp>

namespace bowshock {

class Camera {
public:
    Camera(glm::vec3 pos,
           glm::vec3 front,
           glm::vec3 up,
           float fov,
           float speed);

    glm::mat4 lookat() const;

    // Normalized direction and delta_time
    void move(glm::vec3 norm_dir, float delta_time);
    void rotate(float yaw, float pitch);
    void zoom(float amount);

    // Some getters
    float fov() const; 
    glm::vec3 pos() const;
    glm::vec3 front() const;

private:
    glm::vec3 pos_;
    glm::vec3 front_;
    glm::vec3 up_;
    float fov_;
    float speed_;
};

}
