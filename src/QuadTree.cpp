#include "QuadTree.h"
#include <algorithm>
#include <cmath>

bool AABB::contains(const glm::vec2& p) const {
    return std::abs(p.x - center.x) <= halfSize.x &&
        std::abs(p.y - center.y) <= halfSize.y;
}

bool AABB::intersects(const AABB& other) const {
    return std::abs(center.x - other.center.x) <= (halfSize.x + other.halfSize.x) &&
        std::abs(center.y - other.center.y) <= (halfSize.y + other.halfSize.y);
}

QuadNode::QuadNode(const AABB& region) : bounds(region) {}

void QuadNode::subdivide() {
    glm::vec2 hs = bounds.halfSize * 0.5f;
    glm::vec2 c = bounds.center;
    nw = new QuadNode({ {c.x - hs.x, c.y + hs.y}, hs });
    ne = new QuadNode({ {c.x + hs.x, c.y + hs.y}, hs });
    sw = new QuadNode({ {c.x - hs.x, c.y - hs.y}, hs });
    se = new QuadNode({ {c.x + hs.x, c.y - hs.y}, hs });
}

bool QuadNode::insert(const MassObject& m) {
    const glm::vec2 p = m.getPosition();
    if (!bounds.contains(p)) return false;

    if (isLeaf() && body == nullptr) {
        body = &m;
        totalMass = m.getMass();
        com = p;
        return true;
    }

    if (isLeaf()) {
        subdivide();
        const MassObject* old = body;
        body = nullptr;
        insert(*old);
    }

    if (nw->insert(m)) return true;
    if (ne->insert(m)) return true;
    if (sw->insert(m)) return true;
    if (se->insert(m)) return true;
    return false;
}

void QuadNode::finalize() {
    if (isLeaf()) return;
    nw->finalize(); ne->finalize(); sw->finalize(); se->finalize();

    double msum = 0.0;
    glm::vec2 csum{0.0f, 0.0f};
    QuadNode* kids[4] = { nw, ne, sw, se };
    for (auto* k : kids) {
        if (k->totalMass > 0.0) {
            msum += k->totalMass;
            csum += k->com * static_cast<float>(k->totalMass);
        }
    }
    totalMass = msum;
    com = (msum > 0.0) ? (csum / static_cast<float>(msum)) : glm::vec2{ 0.0f, 0.0f };
}

static inline glm::vec2 pairForce(const glm::vec2& from, const glm::vec2& to, double mass, double G, double softening) {
    glm::vec2 r = to - from;
    double dist = std::max(static_cast<double>(glm::length(r)), softening);
    if (dist <= 0.0) return { 0.0f, 0.0f };
    glm::vec2 dir = glm::normalize(r);
    double mag = (G * mass) / (dist * dist);
    return dir * static_cast<float>(mag);
}

glm::vec2 QuadNode::computeForceAt(const glm::vec2& point, double theta, double G, double softening) const {
    if (totalMass <= 0.0) return { 0.0f, 0.0f };

    // Size over distance heuristic
    double s = static_cast<double>(bounds.halfSize.x + bounds.halfSize.y); // rough size
    double d = std::max(static_cast<double>(glm::length(com - point)), softening);

    if (isLeaf()) {
        if (body && (body->getPosition().x != point.x || body->getPosition().y != point.y)) {
            return pairForce(point, body->getPosition(), body->getMass() * G, 1.0, softening); // G folded into mass for reuse
        }
        return { 0.0f, 0.0f };
    }

    if ((s / d) < theta) {
        // Treat node as a single body at COM
        return pairForce(point, com, totalMass * G, 1.0, softening);
    }

    glm::vec2 f{0.0f, 0.0f};
    if (nw) f += nw->computeForceAt(point, theta, G, softening);
    if (ne) f += ne->computeForceAt(point, theta, G, softening);
    if (sw) f += sw->computeForceAt(point, theta, G, softening);
    if (se) f += se->computeForceAt(point, theta, G, softening);
    return f;
}