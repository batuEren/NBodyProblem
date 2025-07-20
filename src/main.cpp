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

    // Viewport
    glViewport(0, 0, 800, 600);
    glEnable(GL_DEPTH_TEST);

    // Initialize camera for better visibility
    initializeCamera();

    //Camera Projection - increased bounds and moved near plane back
    glm::mat4 projection = glm::ortho(-25.0f, 25.0f, -20.0f, 20.0f, 1.0f, 100.0f);

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

    // 0. copy our vertices array in a buffer for OpenGL to use
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // 1. then set the vertex attributes pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // 2. use our shader program when we want to render an object
    glUseProgram(shaderProgram);
    // 3. now draw the object 
    //someOpenGLFunctionThatDrawsOurTriangle();


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

    //create grid
    std::vector<float> gridVertices = generateGridVertices(20, 0.5f); // grid from -10 to +10
    unsigned int gridVAO, gridVBO;

    glGenVertexArrays(1, &gridVAO);
    glGenBuffers(1, &gridVBO);

    glBindVertexArray(gridVAO);
    glBindBuffer(GL_ARRAY_BUFFER, gridVBO);
    glBufferData(GL_ARRAY_BUFFER, gridVertices.size() * sizeof(float), gridVertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);

    // Create sphere geometry (shared by all spheres)
    SphereGeometry sphereGeometry = generateSphereGeometry(32, 32);

    // Create mass object tracker and add some example mass objects with initial velocities
    MassObjectTracker massTracker;
    globalMassTracker = &massTracker; // Set global pointer for keyboard callbacks
    
    // Create realistic solar system example with proper astronomical units
    // All masses in solar masses (M☉), distances in AU, velocities in AU/year
    // 
    // REAL-WORLD ORBITAL VELOCITIES (for G = 39.478 AU³/(M☉·year²)):
    // Formula: v = √(G*M/r) for circular orbit around central mass M
    // - Earth at 1 AU: v = √(39.478*1.0/1.0) = 6.28 AU/year ≈ 29.8 km/s
    // - Jupiter at 5.2 AU: v = √(39.478*1.0/5.2) = 2.76 AU/year ≈ 13.1 km/s
    // - Mars at 1.52 AU: v = √(39.478*1.0/1.52) = 5.07 AU/year ≈ 24.1 km/s
    
    // Sun (1 solar mass at origin, stationary)
    massTracker.addMassObject(MassObject(1.0, glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.0f)));
    
    // Using real orbital velocities for G = 39.478
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
    std::cout << "Physics: G = 39.478 AU³/(M☉·year²) [REAL VALUE]" << std::endl;
    std::cout << "Objects created:" << std::endl;
    std::cout << "- Sun: 1.0 M☉ at origin (stationary)" << std::endl;
    std::cout << "- Jupiter: 0.001 M☉ at 5.2 AU (v = 2.76 AU/year ≈ 13.1 km/s)" << std::endl;
    std::cout << "- Earth: 3×10⁻⁶ M☉ at 1.0 AU (v = 6.28 AU/year ≈ 29.8 km/s)" << std::endl;
    std::cout << "- Mars: 3.2×10⁻⁷ M☉ at 1.52 AU (v = 5.07 AU/year ≈ 24.1 km/s)" << std::endl;
    std::cout << "- Asteroid: 10⁻¹⁰ M☉ at 2.8 AU (v = 3.76 AU/year ≈ 17.9 km/s)" << std::endl;
    std::cout << "Physics timestep: 0.0001 years (≈0.88 hours) [Fine timestep for stability]" << std::endl;
    std::cout << "Controls: SPACE=physics on/off, E=Euler, V=Verlet" << std::endl;
    std::cout << "Note: Using REAL orbital mechanics - Earth completes orbit in 1 year!" << std::endl;
    std::cout << "=========================================" << std::endl;

    // Variables for timing and energy monitoring
    double lastTime = glfwGetTime();
    double physicsTime = 0.0;
    double initialEnergy = massTracker.getPhysicsEngine().calculateTotalEnergy(massTracker.getMassObjects());
    double energyCheckTimer = 0.0;

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        
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

        // Draw triangle (optional - you might want to remove this)
        glm::mat4 model = glm::mat4(1.0f);
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniform4f(colorLoc, 1.0f, 0.5f, 0.2f, 1.0f);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
    }

    // Cleanup
    cleanupSphereGeometry(sphereGeometry);

    glfwTerminate();
    return 0;
}
