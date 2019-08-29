#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "spdlog/spdlog.h"
#include "bs_window.h"
#include "config.h"

namespace bowshock {
namespace {
    void framebuffer_size_callback(GLFWwindow* window, int width, int height)
    {
        BSWindow *bs_window = static_cast<BSWindow*>(glfwGetWindowUserPointer(window));

        bs_window->resize_window(width, height);
    }

    void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) 
    {
        BSWindow *bs_window = static_cast<BSWindow*>(glfwGetWindowUserPointer(window));

        bs_window->mouse_scroll(xoffset, yoffset);
    }

    void mouse_callback(GLFWwindow* window, double xpos, double ypos)
    {
        BSWindow *bs_window = static_cast<BSWindow*>(glfwGetWindowUserPointer(window));

        bs_window->mouse_move(xpos, ypos);
    }
}

BSWindow::BSWindow(const Config& config) 
    : width_ (config.width)
    , height_(config.height)
    , resize_cb_( [](int, int) {} )
    , scroll_cb_( [](double, double) {} )
    , mouse_cb_( [](double, double) {} )
{
    // glfw: initialize and configure
    // ------------------------------
    if (!glfwInit()) {
        spdlog::error("GLFW Initialization failed");
        throw std::exception();
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window_ = glfwCreateWindow( width_, 
                                height_, 
                                config.window_title.c_str(), 
                                NULL, NULL);
    if ( window_ == NULL )
    {
        spdlog::error("Failed to create GLFW window");
        glfwTerminate();
        throw std::exception();
    }

    glfwMakeContextCurrent(window_);

    glfwSetWindowUserPointer(window_, this);

    glfwSetFramebufferSizeCallback(window_, framebuffer_size_callback);
    glfwSetCursorPosCallback(window_, mouse_callback);
    glfwSetScrollCallback(window_, scroll_callback);

    // Probably put this in the ini
    // capture mouse
    if (config.cursor)
    {
        glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
    else
    {
        glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }

    // I'm doing this here for now since it's associated with glfw
    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        spdlog::error("Failed to initialize GLAD");
        throw std::exception();
    }
}

BSWindow::~BSWindow() {
    glfwMakeContextCurrent(window_);
}

bool BSWindow::is_closing() {
    return glfwWindowShouldClose(window_);
}

void BSWindow::close() 
{
    glfwSetWindowShouldClose(window_, true);
}

bool BSWindow::is_key_pressed(int key)
{
    return (glfwGetKey(window_, key) == GLFW_PRESS);
}

void BSWindow::update()
{
    glfwSwapBuffers(window_);
    glfwPollEvents();
}

void BSWindow::register_mouse_cb(const std::function< void(double, double) >& f)
{
    mouse_cb_ = f;
}

void BSWindow::register_scroll_cb(const std::function< void(double, double) >& f)
{
    scroll_cb_ = f;
}

void BSWindow::register_resize_cb(const std::function< void(int, int) >& f)
{
    resize_cb_ = f;
}

}