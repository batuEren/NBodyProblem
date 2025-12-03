cmake_minimum_required(VERSION 3.20)

project(NBodyProblem LANGUAGES C CXX)

# --- General compiler settings ---
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# --- Sources ---
set(SOURCES
    src/QuadTree.cpp
    src/BarnesHutCalculator.cpp
    src/Camera.cpp
    src/ForceCalculator.cpp
    src/glad.c
    src/GridGenerator.cpp
    src/Integrator.cpp
    src/MassObject.cpp
    src/MassObjectTracker.cpp
    src/main.cpp
    src/PhysicsEngine.cpp
    src/Sphere.cpp
    src/SphereRenderer.cpp

    include/imgui/imgui.cpp
    include/imgui/imgui_demo.cpp
    include/imgui/imgui_draw.cpp
    include/imgui/imgui_tables.cpp
    include/imgui/imgui_widgets.cpp
    include/imgui/backends/imgui_impl_glfw.cpp
    include/imgui/backends/imgui_impl_opengl3.cpp
)

# Optional: shaders just so they show up in IDEs
set(SHADERS
    src/vertex.glsl
    src/fragment.glsl
)

add_executable(NBodyProblem
    ${SOURCES}
    ${SHADERS}
)

# --- Includes ---
target_include_directories(NBodyProblem
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/include/imgui
        ${CMAKE_CURRENT_SOURCE_DIR}/include/glm
        ${CMAKE_CURRENT_SOURCE_DIR}/libs/glfw-3.4.bin.WIN64/glfw-3.4.bin.WIN64/include
)

# --- Preprocessor definitions ---
target_compile_definitions(NBodyProblem
    PRIVATE
        _CONSOLE
        $<$<CONFIG:Debug>:_DEBUG>
        $<$<CONFIG:Release>:NDEBUG>
)

# --- Compiler options per compiler ---
if (MSVC)
    target_compile_options(NBodyProblem PRIVATE /W3 /permissive-)
    target_compile_definitions(NBodyProblem PRIVATE WIN32)
else()
    # Nice warnings on gcc/clang
    target_compile_options(NBodyProblem PRIVATE -Wall -Wextra -Wpedantic)
endif()

# --- Link libraries per platform ---
if (WIN32)
    # Use prebuilt GLFW shipped in the repo
    set(GLFW_LIB_DIR
        ${CMAKE_CURRENT_SOURCE_DIR}/libs/glfw-3.4.bin.WIN64/glfw-3.4.bin.WIN64/lib-vc2022
    )

    target_link_directories(NBodyProblem PRIVATE ${GLFW_LIB_DIR})

    target_link_libraries(NBodyProblem
        PRIVATE
            glfw3
            opengl32
            user32
            gdi32
            shell32
    )

elseif(APPLE)
    # On macOS, install glfw via Homebrew: brew install glfw
    find_package(OpenGL REQUIRED)
    find_package(glfw3 3.3 REQUIRED)

    # Package name may vary; adjust glfw/glfw3::glfw if CMake errors
    target_link_libraries(NBodyProblem
        PRIVATE
            glfw       # or glfw3::glfw depending on your install
            OpenGL::GL
    )

elseif(UNIX)
    # On Linux, e.g.:
    #   sudo apt install libglfw3-dev mesa-common-dev
    find_package(OpenGL REQUIRED)
    find_package(glfw3 3.3 REQUIRED)

    # Package name may vary; adjust glfw/glfw3::glfw as needed
    target_link_libraries(NBodyProblem
        PRIVATE
            glfw       # or glfw3::glfw depending on distro
            OpenGL::GL
            pthread
            dl
    )
endif()

# --- Nice folder view in IDEs ---
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCES} ${SHADERS})
