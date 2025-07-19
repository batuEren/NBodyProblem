#include "Sphere.h"
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>

Sphere::Sphere(const glm::vec3& position, float radius, const glm::vec4& color)
    : position(position), radius(radius), color(color) {}

void Sphere::setPosition(const glm::vec3& pos) {
    position = pos;
}

void Sphere::setRadius(float r) {
    radius = r;
}

void Sphere::setColor(const glm::vec4& col) {
    color = col;
}

glm::mat4 Sphere::getModelMatrix() const {
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position);
    model = glm::scale(model, glm::vec3(radius));
    return model;
}

SphereGeometry generateSphereGeometry(int latitudeSegments, int longitudeSegments) {
    SphereGeometry geometry;
    
    const float PI = 3.14159265359f;
    
    // Generate vertices
    for (int lat = 0; lat <= latitudeSegments; ++lat) {
        float theta = lat * PI / latitudeSegments;
        float sinTheta = sin(theta);
        float cosTheta = cos(theta);
        
        for (int lon = 0; lon <= longitudeSegments; ++lon) {
            float phi = lon * 2.0f * PI / longitudeSegments;
            float sinPhi = sin(phi);
            float cosPhi = cos(phi);
            
            // Vertex position (unit sphere)
            float x = cosPhi * sinTheta;
            float y = cosTheta;
            float z = sinPhi * sinTheta;
            
            geometry.vertices.push_back(x);
            geometry.vertices.push_back(y);
            geometry.vertices.push_back(z);
        }
    }
    
    // Generate indices
    for (int lat = 0; lat < latitudeSegments; ++lat) {
        for (int lon = 0; lon < longitudeSegments; ++lon) {
            int first = (lat * (longitudeSegments + 1)) + lon;
            int second = first + longitudeSegments + 1;
            
            // First triangle
            geometry.indices.push_back(first);
            geometry.indices.push_back(second);
            geometry.indices.push_back(first + 1);
            
            // Second triangle  
            geometry.indices.push_back(second);
            geometry.indices.push_back(second + 1);
            geometry.indices.push_back(first + 1);
        }
    }
    
    geometry.vertexCount = static_cast<unsigned int>(geometry.indices.size());
    
    // Create OpenGL buffers
    glGenVertexArrays(1, &geometry.VAO);
    glGenBuffers(1, &geometry.VBO);
    glGenBuffers(1, &geometry.EBO);
    
    glBindVertexArray(geometry.VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, geometry.VBO);
    glBufferData(GL_ARRAY_BUFFER, geometry.vertices.size() * sizeof(float), 
                 geometry.vertices.data(), GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry.EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, geometry.indices.size() * sizeof(unsigned int),
                 geometry.indices.data(), GL_STATIC_DRAW);
    
    // Position attribute (location = 0)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    glBindVertexArray(0);
    
    return geometry;
}

void cleanupSphereGeometry(SphereGeometry& geometry) {
    glDeleteVertexArrays(1, &geometry.VAO);
    glDeleteBuffers(1, &geometry.VBO);
    glDeleteBuffers(1, &geometry.EBO);
} 