cmake_minimum_required(VERSION 3.20)
project(NBodyProblem LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_EXAMPLE "Build the alternative example_with_sphere_renderer executable" ON)

# Paths
set(SRC_DIR "${CMAKE_SOURCE_DIR}/src")
set(INC_DIR "${CMAKE_SOURCE_DIR}/include")
set(GLFW_BUNDLE_ROOT "${CMAKE_SOURCE_DIR}/libs/glfw-3.4.bin.WIN64/glfw-3.4.bin.WIN64")

# Include dirs (GLM, ImGui, GLAD)
set(PROJ_INCLUDE_DIRS
    "${INC_DIR}"
    "${INC_DIR}/glad"
    "${INC_DIR}/glm"
    "${INC_DIR}/imgui"
    "${INC_DIR}/imgui/backends"
)

# Source lists
set(COMMON_CPP_SOURCES
    "${SRC_DIR}/BarnesHutCalculator.cpp"
    "${SRC_DIR}/Camera.cpp"
    "${SRC_DIR}/ForceCalculator.cpp"
    "${SRC_DIR}/GridGenerator.cpp"
    "${SRC_DIR}/Integrator.cpp"
    "${SRC_DIR}/MassObject.cpp"
    "${SRC_DIR}/MassObjectTracker.cpp"
    "${SRC_DIR}/PhysicsEngine.cpp"
    "${SRC_DIR}/QuadTree.cpp"
    "${SRC_DIR}/Sphere.cpp"
    "${SRC_DIR}/SphereRenderer.cpp"
)

set(GLAD_SOURCES
    "${SRC_DIR}/glad.c"
)

set(IMGUI_SOURCES
    "${INC_DIR}/imgui/imgui.cpp"
    "${INC_DIR}/imgui/imgui_draw.cpp"
    "${INC_DIR}/imgui/imgui_tables.cpp"
    "${INC_DIR}/imgui/imgui_widgets.cpp"
    "${INC_DIR}/imgui/backends/imgui_impl_glfw.cpp"
    "${INC_DIR}/imgui/backends/imgui_impl_opengl3.cpp"
)

# Platform OpenGL/GLFW
if (WIN32)
    # Use bundled GLFW on Windows
    set(GLFW_INCLUDE_DIR "${GLFW_BUNDLE_ROOT}/include")

    if (MSVC)
        set(GLFW_LIB_DIR "${GLFW_BUNDLE_ROOT}/lib-vc2022")
        # Prefer fully static multithreaded lib to avoid DLL deployment
        set(GLFW_LIB "${GLFW_LIB_DIR}/glfw3_mt.lib")
        set(PLATFORM_LIBS
            "${GLFW_LIB}"
            opengl32
            user32
            gdi32
            shell32
            ole32
            winmm
        )
    else()
        # MinGW
        set(GLFW_LIB_DIR "${GLFW_BUNDLE_ROOT}/lib-mingw-w64")
        set(GLFW_LIB "${GLFW_LIB_DIR}/libglfw3.a")
        set(PLATFORM_LIBS
            "${GLFW_LIB}"
            opengl32
            gdi32
        )
    endif()

    list(APPEND PROJ_INCLUDE_DIRS "${GLFW_INCLUDE_DIR}")
else()
    # Non-Windows: use system GLFW and OpenGL
    find_package(glfw3 REQUIRED)
    find_package(OpenGL REQUIRED)

    set(PLATFORM_LIBS
        glfw
        OpenGL::GL
    )

    if (UNIX AND NOT APPLE)
        list(APPEND PLATFORM_LIBS
            dl
            pthread
            X11
        )
    endif()
endif()

# Main executable
add_executable(NBodyProblem
    ${COMMON_CPP_SOURCES}
    ${GLAD_SOURCES}
    ${IMGUI_SOURCES}
    "${SRC_DIR}/main.cpp"
)

target_include_directories(NBodyProblem PRIVATE ${PROJ_INCLUDE_DIRS})
target_compile_definitions(NBodyProblem PRIVATE IMGUI_IMPL_OPENGL_LOADER_GLAD)
target_link_libraries(NBodyProblem PRIVATE ${PLATFORM_LIBS})

# Optional example executable (replaces main.cpp with example_with_sphere_renderer.cpp)
if (BUILD_EXAMPLE)
    add_executable(SphereExample
        ${COMMON_CPP_SOURCES}
        ${GLAD_SOURCES}
        ${IMGUI_SOURCES}
        "${SRC_DIR}/example_with_sphere_renderer.cpp"
    )
    target_include_directories(SphereExample PRIVATE ${PROJ_INCLUDE_DIRS})
    target_compile_definitions(SphereExample PRIVATE IMGUI_IMPL_OPENGL_LOADER_GLAD)
    target_link_libraries(SphereExample PRIVATE ${PLATFORM_LIBS})
endif()

# Organize in IDEs
source_group(TREE "${CMAKE_SOURCE_DIR}" FILES
    ${COMMON_CPP_SOURCES} ${GLAD_SOURCES} ${IMGUI_SOURCES}
    "${SRC_DIR}/main.cpp" "${SRC_DIR}/example_with_sphere_renderer.cpp"
)


