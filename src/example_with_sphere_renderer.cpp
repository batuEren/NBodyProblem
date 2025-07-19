// Alternative main.cpp example using SphereRenderer for easier sphere management
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "GridGenerator.h"
#include "SphereRenderer.h"
#include <iostream>
#include "Camera.h"

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
SphereRenderer sphereRenderer; // Global sphere renderer

void initializeCamera() {
    camera.setLookAt(glm::vec3(0.0f, 0.0f, 0.0f));
    camera.setRadius(25.0f);
    camera.setSphereCoords(glm::vec2(glm::pi<float>() / 4.0f, glm::pi<float>() / 6.0f));
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS || action == GLFW_REPEAT) {
        switch (key) {
        case GLFW_KEY_UP:
            camera.move(glm::vec3({ 0.0f, 0.0f, 1.0f }));
            break;
        case GLFW_KEY_DOWN:
            camera.move(glm::vec3({ 0.0f, 0.0f, -1.0f }));
            break;
        case GLFW_KEY_LEFT:
            camera.move(glm::vec3({ 1.0f, 0.0f, 0.0f }));
            break;
        case GLFW_KEY_RIGHT:
            camera.move(glm::vec3({ -1.0f, 0.0f, 0.0f }));
            break;
        case GLFW_KEY_W:
            camera.shiftSphereCoords(glm::vec2({ 0.0f, 0.05f }));
            break;
        case GLFW_KEY_A:
            camera.shiftSphereCoords(glm::vec2({ 0.05f, 0.0f }));
            break;
        case GLFW_KEY_S:
            camera.shiftSphereCoords(glm::vec2({ 0.0f, -0.05f}));
            break;
        case GLFW_KEY_D:
            camera.shiftSphereCoords(glm::vec2({ -0.05f, 0.0f }));
            break;
        // Dynamic sphere manipulation examples
        case GLFW_KEY_1:
            // Add a random sphere
            sphereRenderer.addSphere(
                glm::vec3((rand() % 20 - 10), (rand() % 20 - 10), (rand() % 20 - 10)), 
                1.0f + (rand() % 100) / 100.0f,
                glm::vec4((rand() % 100) / 100.0f, (rand() % 100) / 100.0f, (rand() % 100) / 100.0f, 1.0f)
            );
            std::cout << "Added random sphere. Total spheres: " << sphereRenderer.getSphereCount() << "\n";
            break;
        case GLFW_KEY_2:
            // Remove last sphere
            if (sphereRenderer.getSphereCount() > 0) {
                sphereRenderer.removeSphere(sphereRenderer.getSphereCount() - 1);
                std::cout << "Removed sphere. Total spheres: " << sphereRenderer.getSphereCount() << "\n";
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

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "Sphere Renderer Example", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD\n";
        return -1;
    }

    glViewport(0, 0, 800, 600);
    glEnable(GL_DEPTH_TEST);

    initializeCamera();

    // Create and compile shaders
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Setup grid
    std::vector<float> gridVertices = generateGridVertices(10);
    unsigned int gridVAO, gridVBO;
    glGenVertexArrays(1, &gridVAO);
    glGenBuffers(1, &gridVBO);
    glBindVertexArray(gridVAO);
    glBindBuffer(GL_ARRAY_BUFFER, gridVBO);
    glBufferData(GL_ARRAY_BUFFER, gridVertices.size() * sizeof(float), gridVertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);

    // Setup projection matrix
    glm::mat4 projection = glm::ortho(-25.0f, 25.0f, -20.0f, 20.0f, 1.0f, 100.0f);

    // Add some initial spheres
    sphereRenderer.addSphere(glm::vec3(0.0f, 0.0f, 0.0f), 2.0f, glm::vec4(1.0f, 0.2f, 0.2f, 1.0f));
    sphereRenderer.addSphere(glm::vec3(5.0f, 0.0f, 0.0f), 1.5f, glm::vec4(0.2f, 1.0f, 0.2f, 1.0f));
    sphereRenderer.addSphere(glm::vec3(-3.0f, 2.0f, 4.0f), 1.0f, glm::vec4(0.2f, 0.2f, 1.0f, 1.0f));

    std::cout << "Controls:\n";
    std::cout << "Arrow keys: Move camera\n";
    std::cout << "WASD: Rotate camera\n";
    std::cout << "1: Add random sphere\n";
    std::cout << "2: Remove last sphere\n";
    std::cout << "ESC: Exit\n\n";

    // Get uniform locations
    unsigned int modelLoc = glGetUniformLocation(shaderProgram, "model");
    unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
    unsigned int projLoc = glGetUniformLocation(shaderProgram, "projection");
    unsigned int colorLoc = glGetUniformLocation(shaderProgram, "objectColor");

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shaderProgram);

        // Update view matrix
        glm::mat4 view = glm::lookAt(camera.getPosition(), camera.getLookAt(), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

        // Draw grid
        glm::mat4 gridModel = glm::mat4(1.0f);
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(gridModel));
        glUniform4f(colorLoc, 0.8f, 0.8f, 0.8f, 1.0f);
        glBindVertexArray(gridVAO);
        glDrawArrays(GL_LINES, 0, gridVertices.size() / 3);

        // Draw all spheres with one simple call!
        sphereRenderer.render(shaderProgram, modelLoc, colorLoc);

        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
} 