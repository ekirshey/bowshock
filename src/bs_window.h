#pragma once
#include <string>
#include <functional>

struct GLFWwindow;

namespace bowshock {

struct Config;

class BSWindow {
public:
    BSWindow(const Config& config);
    ~BSWindow();

    bool is_closing();
    bool is_key_pressed(int key);

    void update();
    void close();

    inline void resize_window(int width, int height)
    {
        resize_cb_(width, height);
    }

    inline void mouse_scroll(double xoffset, double yoffset)
    {
        scroll_cb_(xoffset, yoffset);
    }

    inline void mouse_move(double xpos, double ypos) 
    {
        mouse_cb_(xpos, ypos);
    }

    void register_resize_cb(const std::function< void(int, int) >& f);
    void register_scroll_cb(const std::function< void(double, double) >& f);
    void register_mouse_cb( const std::function< void(double, double) >& f);
    
private:
    int width_;
    int height_;

    std::function<void(int, int)> resize_cb_;
    std::function<void(double, double)> scroll_cb_;
    std::function<void(double, double)> mouse_cb_;

    GLFWwindow* window_;
};


}