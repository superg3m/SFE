/*
struct Entity {
    Math::Vec3 position;
    Math::Quat orientation;
    Math::Vec3 scale;
    Geometry geometry;

    void setPosition(Math::Vec3 p);
    void setPosition(float x, float y, float z);

    void setOrientation(Math::Quat orientation);
    void setEulerAngles(Math::Vec3 euler);
    void setEulerAngles(float theta_x, float theta_y, float theta_z);

    void setScale(float scalar);
    void setScale(Math::Vec3 s);
    void setScale(float scale_x, float scale_y, float scale_z);

    Math::Mat4 getTransform();
    Math::Mat4 getAABBTransform();
    GM_AABB getAABB();

    void update(dt);
    void draw(ShaderBase* shader);
};
*/