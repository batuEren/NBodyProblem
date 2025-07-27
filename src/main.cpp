#include <glad/glad.h>     // Include before GLFW!
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "GridGenerator.h"
#include "Sphere.h"
#include <iostream>
#include <algorithm>
#include "Camera.h"
#include "MassObjectTracker.h"

// Dear ImGui includes
#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"


const char* vertexShaderSource = R"(#version 460 core
layout(location = 0) in vec3 aPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}
)";

const char* fragmentShaderSource = R"(
#version 460 core
out vec4 FragColor;

uniform vec4 objectColor;

void main() {
    FragColor = objectColor;
}
)";

Camera camera;
MassObjectTracker* globalMassTracker = nullptr; // For keyboard callbacks

// Basic ImGui interface function
void createBasicGUI() {
    // Create a window
    ImGui::Begin("N-Body Simulation");
    
    // Display some text
    ImGui::Text("Welcome to N-Body Physics Simulation by Batu Eren!");
    //ImGui::Text("This is a basic ImGui interface.");
    
    // Add a separator line
    ImGui::Separator();
    
    // Static variable to store button click count
    static int buttonClickCount = 0;
    
    // Create a button
    //if (ImGui::Button("Click Me!")) {
    //    buttonClickCount++;
    //    std::cout << "Button clicked! Count: " << buttonClickCount << std::endl;
    //}
    
    // Display the click count
    //ImGui::Text("Button has been clicked %d times", buttonClickCount);
    
    int intVal = 10;
    ImGui::SliderInt("Time Speed", &intVal, 1, 100);

    // Add some simulation info
    ImGui::Separator();
    ImGui::Text("Controls:");
    ImGui::BulletText("SPACE - Toggle physics on/off");
    ImGui::BulletText("E - Switch to Euler integrator");
    ImGui::BulletText("V - Switch to Verlet integrator");
    ImGui::BulletText("WASD - Camera rotation");
    ImGui::BulletText("Arrow keys - Camera movement");
    
    ImGui::End();
}

void initializeCamera() {
    // Set camera for better grid visibility
    camera.setLookAt(glm::vec3(0.0f, 0.0f, 0.0f));  // Look at center
    camera.setRadius(25.0f);  // Move camera further back
    camera.setSphereCoords(glm::vec2(glm::pi<float>() / 4.0f, glm::pi<float>() / 6.0f));  // Better angle
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS || action == GLFW_REPEAT) {
        switch (key) {
        case GLFW_KEY_UP:
            std::cout << "Up arrow pressed\n";
            camera.move(glm::vec3({ 0.0f, 0.0f, 1.0f }));
            break;
        case GLFW_KEY_DOWN:
            std::cout << "Down arrow pressed\n";
            camera.move(glm::vec3({ 0.0f, 0.0f, -1.0f }));
            break;
        case GLFW_KEY_LEFT:
            std::cout << "Left arrow pressed\n";
            camera.move(glm::vec3({ 1.0f, 0.0f, 0.0f }));
            break;
        case GLFW_KEY_RIGHT:
            std::cout << "Right arrow pressed\n";
            camera.move(glm::vec3({ -1.0f, 0.0f, 0.0f }));
            break;
        // To Do, Rotate around the Look at Pos
        case GLFW_KEY_W:
            std::cout << "Up arrow pressed\n";
            camera.shiftSphereCoords(glm::vec2({ 0.0f, 0.05f }));
            break;
        case GLFW_KEY_A:
            std::cout << "Down arrow pressed\n";
            camera.shiftSphereCoords(glm::vec2({ 0.05f, 0.0f }));
            break;
        case GLFW_KEY_S:
            std::cout << "Left arrow pressed\n";
            camera.shiftSphereCoords(glm::vec2({ 0.0f, -0.05f}));
            break;
        case GLFW_KEY_D:
            std::cout << "Right arrow pressed\n";
            camera.shiftSphereCoords(glm::vec2({ -0.05f, 0.0f }));
            break;
        case GLFW_KEY_SPACE:
            // Toggle physics on/off
            if (globalMassTracker) {
                bool currentState = globalMassTracker->getPhysicsEngine().isEnabled();
                globalMassTracker->setPhysicsEnabled(!currentState);
                std::cout << "Physics " << (!currentState ? "enabled" : "disabled") << "\n";
            }
            break;
        case GLFW_KEY_R:
            // Reset simulation (you could implement this)
            std::cout << "Reset simulation (not implemented yet)\n";
            break;
        case GLFW_KEY_E:
            // Switch to Euler integrator
            if (globalMassTracker) {
                globalMassTracker->switchToEulerIntegrator();
            }
            break;
        case GLFW_KEY_V:
            // Switch to Verlet integrator
            if (globalMassTracker) {
                globalMassTracker->switchToVerletIntegrator();
            }
            break;
        case GLFW_KEY_ESCAPE:
            glfwSetWindowShouldClose(window, true);
            break;
        }
    }
}


int main() {
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        return -1;
    }

    // Tell GLFW we want OpenGL 3.3 Core Profile
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create window
    GLFWwindow* window = glfwCreateWindow(800, 600, "Modern OpenGL", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);


    // Load OpenGL functions using GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD\n";
        return -1;
    }

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330 core");

    // Viewport
    glViewport(0, 0, 800, 600);
    glEnable(GL_DEPTH_TEST);

    // Initialize camera for better visibility
    initializeCamera();

    //Camera Projection - increased bounds and moved near plane back
    //glm::mat4 projection = glm::ortho(-25.0f, 25.0f, -20.0f, 20.0f, 1.0f, 100.0f);
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 25.0f / 20.0f, 1.0f, 100.0f);


    glm::mat4 model = glm::mat4(1.0f);


    float vertices[] = {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f
    };


    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);

    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    int  success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }


    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glUseProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);


    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glUseProgram(shaderProgram);


    unsigned int VAO;
    glGenVertexArrays(1, &VAO);

    // 1. bind Vertex Array Object
    glBindVertexArray(VAO);
    // 2. copy our vertices array in a buffer for OpenGL to use
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // 3. then set our vertex attributes pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);


    // Create sphere geometry (shared by all spheres)
    SphereGeometry sphereGeometry = generateSphereGeometry(32, 32);

    // Create grid VAO and VBO (will be updated each frame)
    unsigned int gridVAO, gridVBO;
    glGenVertexArrays(1, &gridVAO);
    glGenBuffers(1, &gridVBO);
    
    glBindVertexArray(gridVAO);
    glBindBuffer(GL_ARRAY_BUFFER, gridVBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);

    // Create mass object tracker and add some example mass objects with initial velocities
    MassObjectTracker massTracker;
    globalMassTracker = &massTracker; // Set global pointer for keyboard callbacks
    
    // Create realistic solar system example with proper astronomical units
    // All masses in solar masses (MS), distances in AU, velocities in AU/year
    // 
    // REAL-WORLD ORBITAL VELOCITIES (for G = 39.478 AU^3/(MS·year²)):
    // Formula: v = sqrt(G*M/r) for circular orbit around central mass M
    // - Earth at 1 AU: v = √(39.478*1.0/1.0) = 6.28 AU/year ≈ 29.8 km/s
    // - Jupiter at 5.2 AU: v = sqrt(39.478*1.0/5.2) = 2.76 AU/year ≈ 13.1 km/s
    // - Mars at 1.52 AU: v = sqrt(39.478*1.0/1.52) = 5.07 AU/year ≈ 24.1 km/s
    
    // Sun (1 solar mass at origin, stationary)
    massTracker.addMassObject(MassObject(1.0, glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.0f)));
    
    // Using real orbital velocities
    // Jupiter (0.001 solar masses at 5.2 AU with real orbital velocity)
    massTracker.addMassObject(MassObject(0.001, glm::vec2(5.2f, 0.0f), glm::vec2(0.0f, 2.76f)));
    
    // Earth (3×10⁻⁶ solar masses at 1 AU with real orbital velocity) 
    massTracker.addMassObject(MassObject(3e-6, glm::vec2(1.0f, 0.0f), glm::vec2(0.0f, 6.28f)));
    
    // Mars (3.2×10⁻⁷ solar masses at 1.52 AU with real orbital velocity)
    massTracker.addMassObject(MassObject(3.2e-7, glm::vec2(1.52f, 0.0f), glm::vec2(0.0f, 5.07f)));
    
    // Large asteroid (10⁻¹⁰ solar masses in asteroid belt at 2.8 AU)
    massTracker.addMassObject(MassObject(1e-10, glm::vec2(2.8f, 0.0f), glm::vec2(0.0f, 3.76f)));
    
    // Configure physics timestep for real astronomical values (needs smaller timestep)
    massTracker.getPhysicsEngine().setPhysicsTimestep(0.0001); // 0.0001 years ≈ 0.88 hours
    
    // Display system information
    std::cout << "=== N-Body Solar System Simulation ===" << std::endl;
    std::cout << "Units: AU (distance), Solar Masses (mass), Years (time)" << std::endl;
    std::cout << "Physics: G = 39.478 AU³/(MS·year²) [REAL VALUE]" << std::endl;
    std::cout << "Objects created:" << std::endl;
    std::cout << "- Sun: 1.0 MS at origin (stationary)" << std::endl;
    std::cout << "- Jupiter: 0.001 MS at 5.2 AU (v = 2.76 AU/year ≈ 13.1 km/s)" << std::endl;
    std::cout << "- Earth: 3×10⁻⁶ MS at 1.0 AU (v = 6.28 AU/year ≈ 29.8 km/s)" << std::endl;
    std::cout << "- Mars: 3.2×10⁻⁷ MS at 1.52 AU (v = 5.07 AU/year ≈ 24.1 km/s)" << std::endl;
    std::cout << "- Asteroid: 10⁻¹⁰ MS at 2.8 AU (v = 3.76 AU/year ≈ 17.9 km/s)" << std::endl;
    std::cout << "Physics timestep: 0.0001 years (≈0.88 hours) [Fine timestep for stability]" << std::endl;
    std::cout << "Controls: SPACE=physics on/off, E=Euler, V=Verlet" << std::endl;
    std::cout << "=========================================" << std::endl;

    // Variables for timing and energy monitoring
    double lastTime = glfwGetTime();
    double physicsTime = 0.0;
    double initialEnergy = massTracker.getPhysicsEngine().calculateTotalEnergy(massTracker.getMassObjects());
    double energyCheckTimer = 0.0;

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        
        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        
        // Calculate time delta for physics
        double currentTime = glfwGetTime();
        double deltaTime = currentTime - lastTime;
        lastTime = currentTime;
        
        // Update physics (limit to prevent huge time steps)
        double clampedDeltaTime = std::min(deltaTime, 0.02); // Max 20ms per frame
        massTracker.updatePhysics(clampedDeltaTime);
        physicsTime += clampedDeltaTime;
        
        // Monitor energy conservation (print every 2 seconds)
        energyCheckTimer += clampedDeltaTime;
        if (energyCheckTimer >= 2.0) {
            double currentEnergy = massTracker.getPhysicsEngine().calculateTotalEnergy(massTracker.getMassObjects());
            double energyChange = ((currentEnergy - initialEnergy) / initialEnergy) * 100.0;
            std::cout << "Energy change: " << energyChange << "% (Time: " << physicsTime << "s)\n";
            energyCheckTimer = 0.0;
        }
        
        // Create ImGui interface
        createBasicGUI();
        
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shaderProgram);

        glm::mat4 view = glm::lookAt(
            camera.getPosition(), // camera position
            camera.getLookAt(), // look at
            glm::vec3(0.0f, 1.0f, 0.0f)  // up vector
        );

        unsigned int modelLoc = glGetUniformLocation(shaderProgram, "model");
        unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
        unsigned int projLoc = glGetUniformLocation(shaderProgram, "projection");
        unsigned int colorLoc = glGetUniformLocation(shaderProgram, "objectColor");

        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

        // Update grid vertices based on current gravitational field
        std::vector<float> gridVertices = generateGridVertices(20, 0.5f, massTracker.getMassObjects());
        
        // Update the existing grid buffer with new data
        glBindBuffer(GL_ARRAY_BUFFER, gridVBO);
        glBufferData(GL_ARRAY_BUFFER, gridVertices.size() * sizeof(float), gridVertices.data(), GL_DYNAMIC_DRAW);

        // Draw grid
        glm::mat4 gridModel = glm::mat4(1.0f);
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(gridModel));
        glUniform4f(colorLoc, 0.8f, 0.8f, 0.8f, 1.0f);
        glBindVertexArray(gridVAO);
        glDrawArrays(GL_LINES, 0, gridVertices.size() / 3);

        // Draw spheres from mass objects
        glBindVertexArray(sphereGeometry.VAO);
        std::vector<Sphere> spheres = massTracker.getAllSpheres();
        for (const auto& sphere : spheres) {
            glm::mat4 sphereModel = sphere.getModelMatrix();
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(sphereModel));
            
            const glm::vec4& sphereColor = sphere.getColor();
            glUniform4f(colorLoc, sphereColor.r, sphereColor.g, sphereColor.b, sphereColor.a);
            
            glDrawElements(GL_TRIANGLES, sphereGeometry.vertexCount, GL_UNSIGNED_INT, 0);
        }

        // Render ImGui
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    // Cleanup ImGui
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    // Cleanup
    cleanupSphereGeometry(sphereGeometry);
    glDeleteVertexArrays(1, &gridVAO);
    glDeleteBuffers(1, &gridVBO);

    glfwTerminate();
    return 0;
}
