#ifndef MATH_HPP
#define MATH_HPP

#include <glm/glm.hpp>

struct AABB {
    glm::vec2 min;
    glm::vec2 max;

    bool Intersects(const AABB& other) const {
        return !(max.x < other.min.x || min.x > other.max.x ||
                 max.y < other.min.y || min.y > other.max.y);
    }
};

#endif