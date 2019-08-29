#include "bs_time.h"
#include <GLFW/glfw3.h>

namespace bowshock {
    
float get_time()
{
    return (float)glfwGetTime();
}

}