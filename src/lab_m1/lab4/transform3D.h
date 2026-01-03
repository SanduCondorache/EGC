#pragma once

#include <cmath>

namespace transform3D {
// Translate matrix
inline glm::mat4 Translate(float translateX, float translateY,
                           float translateZ) {
    return glm::transpose(glm::mat4(1, 0, 0, translateX, 0, 1, 0, translateY, 0,
                                    0, 1, translateZ, 0, 0, 0, 1));
}

// Scale matrix
inline glm::mat4 Scale(float scaleX, float scaleY, float scaleZ) {
    return glm::transpose(glm::mat4(scaleX, 0, 0, 0, 0, scaleY, 0, 0, 0, 0,
                                    scaleZ, 0, 0, 0, 0, 1));
}

// Rotate matrix relative to the OZ axis
inline glm::mat4 RotateOZ(float radians) {
    float cs = glm::cos(radians);
    float sn = glm::sin(radians);
    return glm::transpose(
        glm::mat4(cs, -sn, 0, 0, sn, cs, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1));
}

// Rotate matrix relative to the OY axis
inline glm::mat4 RotateOY(float radians) {
    float cs = glm::cos(radians);
    float sn = glm::sin(radians);
    return glm::transpose(
        glm::mat4(cs, 0, sn, 0, 0, 1, 0, 0, -sn, 0, cs, 0, 0, 0, 0, 1));
}

// Rotate matrix relative to the OX axis
inline glm::mat4 RotateOX(float radians) {
    float cs = glm::cos(radians);
    float sn = glm::sin(radians);
    return glm::transpose(
        glm::mat4(1, 0, 0, 0, 0, cs, -sn, 0, 0, sn, cs, 0, 0, 0, 0, 1));
}
} // namespace transform3D
