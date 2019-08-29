#pragma once
#include "config.h"
#include "bs_window.h"
#include "model_container.h"
#include "texture_container.h"
#include <GLFW/glfw3.h>

namespace bowshock {
    /* 
        The engine is simply a loose collection
        of the important game objects

        It's a box of tools to leverage
        I'm not going to put any game loop or logic 
        within this class.

        I'n fact, you could build all these individually
        and not use the engine structure at all.

        The engine will bootstrap all the i/o and rendering
        but the rest is up to the user
    */
    struct Engine {

        Config config;
        BSWindow window;
        ModelContainer model_container;
        TextureContainer texture_container;

        Engine( const std::string& ini );
    };
}