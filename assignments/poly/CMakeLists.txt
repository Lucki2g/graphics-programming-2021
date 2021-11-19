# ---------------------------------------------------------------------------------
# Executable and target include/link libraries
# ---------------------------------------------------------------------------------

## set the variable "libraries" to hold the name of the libraries that we need
set(libraries glad glfw imgui)

if(APPLE)
    find_library(IOKIT_LIBRARY IOKit)
    find_library(COCOA_LIBRARY Cocoa)
    find_library(OPENGL_LIBRARY OpenGL)
    find_library(COREVIDEO_LIBRARY CoreVideo)

    list(APPEND libraries
            ${OPENGL_LIBRARY}
            ${COCOA_LIBRARY}
            ${IOKIT_LIBRARY}
            ${COREVIDEO_LIBRARY}
            )
endif()


## set target project
file(GLOB target_src "*.h" "*.cpp") # look for source files
file(GLOB target_shaders "shaders/*.vert" "shaders/*.frag") # look for shaders

set(output_file "poly")
add_executable(${output_file} ${target_src} ${target_shaders} engine/model.h engine/model_loader.h engine/entity_renderer.h shaders/static_shader.h entities/entity.h entities/camera.h entities/light.h util/obj_reader.h util/OBJ_loader.h generation/colour_generator.h config.h generation/height_map.h util/stb_image.h generation/FastNoiseLite.h util/guis/im_gui.h shaders/normal/normal_terrain_shader.h shaders/water/water_shader.h engine/water_renderer.h shaders/water/water_fbos.h util/guis/gui_texture.h engine/gui_renderer.h engine/master_renderer.h)

## set link libraries
target_link_libraries(${output_file} ${libraries})

## add local source directory to include paths
target_include_directories(${output_file} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})


## copy shaders folder to build folder
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/shaders DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/models DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

## copy shaders again every time the current target gets compiled
## (may require manual compilation if you modify shaders without modifying the C++ source)
add_custom_command(
        TARGET ${output_file} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/shaders ${CMAKE_CURRENT_BINARY_DIR}/shaders
        COMMENT "Copying shaders" VERBATIM
)
