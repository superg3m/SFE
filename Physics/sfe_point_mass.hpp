#include "../Core/Math/sfe_math.hpp"
namespace Physics {
    const Math::Vec3 EARTH_GRAVITY = Math::Vec3(0, -9.81f, 0); // NOTE(Jovanni): Acceleration due to gravity
    
    struct PointMass {
        float velocity_dampening; // NOTE(Jovanni): 0 - 1

        Math::Vec3 position;
        Math::Vec3 velocity; // m/s
        Math::Vec3 acceleration; // m/s^2

        float getMass() {
            return this->mass;
        }
        void setMass(float mass) {
            this->mass = mass;
            this->inverse_mass = 1.0f / this->mass;
        }

        void addForce(Math::Vec3 force) {
            this->force += force;
        }

        void integrate(float dt) {
            this->position += this->velocity.scale(dt);

            Math::Vec3 resulting_acceleration = this->acceleration;
            resulting_acceleration += force.scale(this->inverse_mass);

            this->velocity += resulting_acceleration.scale(dt);
            this->velocity = this->velocity.scale(powf(this->velocity_dampening, dt));

            this->force = Math::Vec3(0);
        }

    private:
        float mass; // In kg
        float inverse_mass; // In 1/kg

        Math::Vec3 force; // Forces to be applied to the next physics iteration, zeroed at each integration step
    };
}