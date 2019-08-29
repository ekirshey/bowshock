#include "config.h"
#include "INIReader.h"
#include "spdlog/spdlog.h"

namespace bowshock {

namespace {
    void sanitizePath(std::string& path) {
        if (path.back() != '/') {
            path += '/';
        }
    }
}

Config load_config(const std::string& ini) {
    Config config;
    INIReader reader(ini);

    if (reader.ParseError() < 0) {
        spdlog::error("Could not load file {}", ini);
        throw std::runtime_error("Could not load file " + ini);
    }

    config.window_title = reader.Get("video", "windowtitle", "default");
    config.width = reader.GetInteger("video", "width", 800);
    config.height = reader.GetInteger("video", "height", 600);
    config.vsync = reader.GetBoolean("video", "VSYNC", true);
    config.cursor = reader.GetBoolean("video", "cursor", true);
    config.fullscreen = reader.GetBoolean("video", "fullscreen", true);

    config.shader_path = reader.Get("paths", "shader_path", "");
    sanitizePath(config.shader_path);

    config.model_registry = reader.Get("paths", "model_registry", "");

    return config;
}

}