Write-Host "Building NBody Problem..." -ForegroundColor Green

# Using MinGW (if available)
$sourceFiles = @(
    "src/main.cpp",
    "src/glad.c", 
    "src/Camera.cpp",
    "src/GridGenerator.cpp",
    "src/MassObject.cpp",
    "src/Sphere.cpp",
    "src/SphereRenderer.cpp",
    "src/ForceCalculator.cpp",
    "src/PhysicsEngine.cpp",
    "src/MassObjectTracker.cpp",
    "src/Integrator.cpp",
    "include/imgui/imgui.cpp",
    "include/imgui/imgui_demo.cpp",
    "include/imgui/imgui_draw.cpp",
    "include/imgui/imgui_tables.cpp",
    "include/imgui/imgui_widgets.cpp",
    "include/imgui/backends/imgui_impl_glfw.cpp",
    "include/imgui/backends/imgui_impl_opengl3.cpp"
)

$includes = @(
    "-I./include",
    "-I./libs/glfw-3.4.bin.WIN64/glfw-3.4.bin.WIN64/include"
)

$libPath = "-L./libs/glfw-3.4.bin.WIN64/glfw-3.4.bin.WIN64/lib-mingw-w64"
$libs = "-lglfw3 -lopengl32 -lgdi32"

$buildCommand = "g++ -g -std=c++17 $($includes -join ' ') $($sourceFiles -join ' ') $libPath $libs -o NBodyProblem.exe"

Write-Host "Command: $buildCommand" -ForegroundColor Yellow
Write-Host ""

try {
    Invoke-Expression $buildCommand
    
    if ($LASTEXITCODE -eq 0) {
        Write-Host "Build successful! Running program..." -ForegroundColor Green
        Write-Host ""
        ./NBodyProblem.exe
    } else {
        Write-Host "Build failed!" -ForegroundColor Red
        Write-Host "Make sure you have MinGW installed and in your PATH" -ForegroundColor Yellow
        Write-Host "Alternatively, try using Visual Studio" -ForegroundColor Yellow
    }
} catch {
    Write-Host "Error: $_" -ForegroundColor Red
    Write-Host "Make sure you have MinGW (g++) installed and in your PATH" -ForegroundColor Yellow
}

Write-Host "Press any key to continue..." -ForegroundColor Cyan
$null = $Host.UI.RawUI.ReadKey("NoEcho,IncludeKeyDown") 