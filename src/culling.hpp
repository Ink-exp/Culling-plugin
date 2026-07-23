#ifndef CULLING_HPP
#define CULLING_HPP

#include <cmath>

struct Vector3 {
    float x, y, z;

    float distance_squared(const Vector3& target) const {
        float dx = x - target.x;
        float dy = y - target.y;
        float dz = z - target.z;
        return (dx * dx) + (dy * dy) + (dz * dz);
    }
};

// Determines if an entity or particle emitter is outside rendering threshold
inline bool is_culled(const Vector3& position, const Vector3& camera_position, float max_render_distance) {
    float threshold = max_render_distance * max_render_distance;
    return position.distance_squared(camera_position) > threshold;
}

#endif // CULLING_HPP

