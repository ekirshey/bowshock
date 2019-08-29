#pragma once
#include <string>

namespace bowshock {
    /*
        Configuration structure initialzed from
        ini file
    */
    struct Config {
        std::string window_title;
        std::string shader_path;
        std::string model_registry;

        bool fullscreen = false;
        bool vsync = false;
        bool cursor = false;

        int width = 800;
        int height = 600;

    };

    Config load_config(const std::string& ini);
}
