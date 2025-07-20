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
    std::vector<float> gridVertices = generateGridVertices(10); // grid from -10 to +10
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
    
    // Add objects with interesting orbital-like initial velocities
    massTracker.addMassObject(MassObject(10.0, glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.0f)));     // Large central mass (stationary)
    //massTracker.addMassObject(MassObject(2.0, glm::vec2(5.0f, 0.0f), glm::vec2(0.0f, 2.0f)));      // Orbiting mass
    massTracker.addMassObject(MassObject(1.0, glm::vec2(-3.0f, 0.0f), glm::vec2(0.0f, -1.5f)));    // Counter-orbiting mass
    //massTracker.addMassObject(MassObject(1.5, glm::vec2(0.0f, 4.0f), glm::vec2(1.8f, 0.0f)));      // Another orbiting mass
    //massTracker.addMassObject(MassObject(0.8, glm::vec2(-2.0f, -2.0f), glm::vec2(1.0f, 1.0f)));    // Small mass with diagonal motion
    
    // Configure physics (you can experiment with these values)
    massTracker.getPhysicsEngine().setPhysicsTimestep(0.01); // 10ms physics timestep
    
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
