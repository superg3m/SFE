// ACTIVE_VAO
// WIRE_FRAME_MODE
// projection_mode (3d or 2d)

// the goal would be to render 3d stuff then render a small 2d square

struct Renderer {
    unsigned int VAO;

    void bindVAO();

    void toggleWireFrame(bool toggle);
    void toggleWireFrame();
};