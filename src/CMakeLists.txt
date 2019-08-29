
SET( BOWSHOCK_HDR 
    ${PROJECT_SOURCE_DIR}/src/assimp_loader.h
    ${PROJECT_SOURCE_DIR}/src/mesh.h
    ${PROJECT_SOURCE_DIR}/src/model.h
    ${PROJECT_SOURCE_DIR}/src/shader.h
    ${PROJECT_SOURCE_DIR}/src/model_container.h
    ${PROJECT_SOURCE_DIR}/src/texture_container.h
	${PROJECT_SOURCE_DIR}/src/bs_window.h
	${PROJECT_SOURCE_DIR}/src/camera.h
	${PROJECT_SOURCE_DIR}/src/viewport.h
	${PROJECT_SOURCE_DIR}/src/engine.h
	${PROJECT_SOURCE_DIR}/src/config.h
	${PROJECT_SOURCE_DIR}/src/renderer.h
	${PROJECT_SOURCE_DIR}/src/bs_math.h
	${PROJECT_SOURCE_DIR}/src/bs_time.h
)

SET( BOWSHOCK_SRC 
	# Third party
    ${GLAD_SOURCE_DIR}/glad.c
	${INIH_SRC_DIR}/INIReader.cpp
	${INIH_SRC_DIR}/ini.c
    
    #SOURCE
    ${PROJECT_SOURCE_DIR}/src/assimp_loader.cpp
    ${PROJECT_SOURCE_DIR}/src/mesh.cpp
    ${PROJECT_SOURCE_DIR}/src/model.cpp
    ${PROJECT_SOURCE_DIR}/src/shader.cpp
    ${PROJECT_SOURCE_DIR}/src/model_container.cpp
    ${PROJECT_SOURCE_DIR}/src/texture_container.cpp
	${PROJECT_SOURCE_DIR}/src/bs_window.cpp
	${PROJECT_SOURCE_DIR}/src/camera.cpp
	${PROJECT_SOURCE_DIR}/src/viewport.cpp
	${PROJECT_SOURCE_DIR}/src/engine.cpp
	${PROJECT_SOURCE_DIR}/src/config.cpp
	${PROJECT_SOURCE_DIR}/src/renderer.cpp
	${PROJECT_SOURCE_DIR}/src/bs_time.cpp
)

include_directories(
	${GLAD_INCLUDE_DIR}
	${KHR_INCLUDE_DIR}
    ${GLFW_INCLUDE_DIR}
	${SPDLOG_INCLUDE_DIR}
	${STB_INCLUDE_DIR}
	${GLM_INCLUDE_DIR}
	${ASSIMP_INCLUDE_DIR}
	${INIH_INCLUDE_DIR}
    ${BOWSHOCK_HDR}
)

add_library(bowshock
    ${BOWSHOCK_SRC} 
	${BOWSHOCK_HDR}
)

target_link_libraries( bowshock PRIVATE nlohmann_json nlohmann_json::nlohmann_json )

add_custom_command(TARGET bowshock
        POST_BUILD
        COMMAND python ${PROJECT_SOURCE_DIR}/copy_deps.py ${PROJECT_SOURCE_DIR}
)
        