#include "sfe_point_mass.hpp"
#include "sfe_force_generator.hpp"
#include "../Core/DataStructure/sfe_contiguous.hpp"


namespace Physics {
    struct ForceRegistery {
        struct PointMassForceRegistration {
            PointMass* pm;
            PointMassForceGenerator* fg;
        };

        void add(PointMass* pm, PointMassForceGenerator* fg) {
            this->registrations.push((PointMassForceRegistration){pm, fg});
        }
    
        void remove(PointMass* pm, PointMassForceGenerator* fg) {
            for (int i = 0; i < this->registrations.count(); i++) {
                const PointMassForceRegistration entry = this->registrations[i];
                if (pm == entry.pm && fg == entry.fg) {
                    this->registrations.unstableSwapbackRemove(i);
                }
            }
        }

        void updateForces(float dt) {
            for (const PointMassForceRegistration& entry : this->registrations) {
                entry.fg->updateForce(entry.pm, dt);
            }
        }

        DS::Vector<PointMassForceRegistration> registrations;
    };
}
