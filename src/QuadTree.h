#pragma once
#include <glm/glm.hpp>
#include <vector>
#include "MassObject.h"

struct AABB {
    glm::vec2 center;
    glm::vec2 halfSize; 
    bool contains(const glm::vec2& p) const;
    bool intersects(const AABB& other) const;
};

class QuadNode {
public:
    QuadNode(const AABB& region);

    bool insert(const MassObject& body);
    void subdivide();

    // Aggregate properties
    double totalMass = 0.0;
    glm::vec2 com = { 0.0f, 0.0f }; // center of mass (AU)

    const MassObject* body = nullptr;

    QuadNode* nw = nullptr;
    QuadNode* ne = nullptr;
    QuadNode* sw = nullptr;
    QuadNode* se = nullptr;

    AABB bounds;

    bool isLeaf() const { return !nw; }

    // Build aggregation after inserts
    void finalize();

    // Accumulate force contribution to a point using Barnes–Hut criterion
    glm::vec2 computeForceAt(const glm::vec2& point, double theta, double G, double softening) const;
};