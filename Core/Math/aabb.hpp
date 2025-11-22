#pragma once

#include <Math/vector.hpp>

namespace Math {
    struct AABB {
        Vector3 min;
        Vector3 max;

        AABB();
        AABB(Vector3 min, Vector3 max);
        AABB(float min_x, float min_y, float min_z, float max_x, float max_y, float max_z);

        Vector3 getCenter();
        Vector3 getExtents();

        static AABB FromCenterExtents(Vector3 center, Vector3 extents);
        static bool Intersection(AABB aabb, Vector3 p0, Vector3 p1);
    };
}