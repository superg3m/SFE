#include "sfe_point_mass.hpp"

namespace Physics {
    struct PointMassForceGenerator {
        virtual void updateForce(PointMass* pm, float dt) = 0;
    };
}