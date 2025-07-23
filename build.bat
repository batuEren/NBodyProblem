@echo off
echo Building NBody Problem...

rem Using MinGW (if available)
g++ -g -std=c++17 ^
    -I./include ^
    -I./libs/glfw-3.4.bin.WIN64/glfw-3.4.bin.WIN64/include ^
    src/main.cpp ^
    src/glad.c ^
    src/Camera.cpp ^
    src/GridGenerator.cpp ^
    src/MassObject.cpp ^
    src/Sphere.cpp ^
    src/SphereRenderer.cpp ^
    src/ForceCalculator.cpp ^
    src/PhysicsEngine.cpp ^
    src/MassObjectTracker.cpp ^
    src/Integrator.cpp ^
    include/imgui/imgui.cpp ^
    include/imgui/imgui_demo.cpp ^
    include/imgui/imgui_draw.cpp ^
    include/imgui/imgui_tables.cpp ^
    include/imgui/imgui_widgets.cpp ^
    include/imgui/backends/imgui_impl_glfw.cpp ^
    include/imgui/backends/imgui_impl_opengl3.cpp ^
    -L./libs/glfw-3.4.bin.WIN64/glfw-3.4.bin.WIN64/lib-mingw-w64 ^
    -lglfw3 -lopengl32 -lgdi32 ^
    -o NBodyProblem.exe

if %ERRORLEVEL% EQU 0 (
    echo Build successful! Running program...
    echo.
    NBodyProblem.exe
) else (
    echo Build failed!
    echo.
    echo Make sure you have MinGW installed and in your PATH
    echo Alternatively, try using Visual Studio or the build-msvc task
    pause
) 