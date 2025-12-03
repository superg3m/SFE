#include <core.hpp>
#include <graphics.hpp>
#include <GLFW/glfw3.h>
#include <glad/glad.h>

ShaderDiffuse diffuse_shader;
ShaderModel model_shader;
Graphics::Geometry pole;
Graphics::Geometry hexplane;
Graphics::Geometry animals[4];
Math::Mat4 translate_mats[4];
Math::Mat4 animals_rot[4];

Graphics::Geometry church;

float saved_rot = 0.0f;
float saved_translation = 0.0f;
static int is_rotating = 1;
static int is_translating = 1;

#define MASTER_PROFILE "master"
#define MOVEMENT_PROFILE "movement"

Camera camera;
bool mouse_captured = true;
float dt = 0;
float WIDTH = 900;
float HEIGHT = 900;

void cbMasterProfile() {
    GLFWwindow* window = (GLFWwindow*)IOD::glfw_window_instance;
    const bool SHIFT = IOD::GetKey(IOD_KEY_SHIFT, IOD_InputState::PRESSED|IOD_InputState::DOWN);

    if (IOD::GetKey(IOD_KEY_ESCAPE, IOD_InputState::PRESSED)) {
        glfwSetWindowShouldClose(window, true);
    }

    if (IOD::GetKey(IOD_KEY_R, IOD_InputState::PRESSED)) {
        diffuse_shader.compile();
        model_shader.compile();
    }

    if (SHIFT && IOD::GetKey(IOD_KEY_W, IOD_InputState::PRESSED)) {
        IOD::ToggleProfile(MOVEMENT_PROFILE);
    }

    if (IOD::GetKey(IOD_KEY_L, IOD_InputState::PRESSED)) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    } else if (IOD::GetKey(IOD_KEY_L, IOD_InputState::RELEASED)) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    if (IOD::GetKey(IOD_KEY_C, IOD_InputState::PRESSED)) {
        mouse_captured = !mouse_captured;
        glfwSetInputMode(window, GLFW_CURSOR, mouse_captured ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
    }

    if (IOD::GetKey(IOD_KEY_U, IOD_InputState::PRESSED)) {
        is_rotating = !is_rotating;
		is_translating = !is_translating;
    }
}

void cbMovementProfile() {
    GLFWwindow* window = (GLFWwindow*)IOD::glfw_window_instance;

    if (IOD::GetKey(IOD_KEY_SPACE, IOD_InputState::PRESSED|IOD_InputState::DOWN)) {
        camera.processKeyboard(UP, dt);
    }

    if (IOD::GetKey(IOD_KEY_CTRL, IOD_InputState::PRESSED|IOD_InputState::DOWN)) {
        camera.processKeyboard(DOWN, dt);
    }

    if (IOD::GetKey(IOD_KEY_W, IOD_InputState::PRESSED|IOD_InputState::DOWN)) {
        camera.processKeyboard(FORWARD, dt); 
    }

    if (IOD::GetKey(IOD_KEY_A, IOD_InputState::PRESSED|IOD_InputState::DOWN)) {
        camera.processKeyboard(LEFT, dt); 
    }

    if (IOD::GetKey(IOD_KEY_S, IOD_InputState::PRESSED|IOD_InputState::DOWN)) {
        camera.processKeyboard(BACKWARD, dt); 
    }

    if (IOD::GetKey(IOD_KEY_D, IOD_InputState::PRESSED|IOD_InputState::DOWN)) {
        camera.processKeyboard(RIGHT, dt); 
    }
}

void mouse(GLFWwindow *window, double mouse_x, double mouse_y) {
    static float last_mouse_x = mouse_x;
    static float last_mouse_y = mouse_y;

    float xoffset = mouse_x - last_mouse_x;
    float yoffset = last_mouse_y - mouse_y;

    last_mouse_x = mouse_x;
    last_mouse_y = mouse_y;

    if (mouse_captured) { 
        camera.processMouseMovement(xoffset, yoffset);
        return;
    }
}

void display() {
    glClearColor(0.2f, 0.2f, 0.2f, 0);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    if(is_rotating == 1) {
        saved_rot = fmod(glfwGetTime() * 45.0f, 360);
    }

    Math::Mat4 rot_mat = Math::Mat4::Identity(); 
    rot_mat = Math::Mat4::Rotate(rot_mat, saved_rot, 0, 1, 0);

    float fov = camera.zoom;
    float aspect = WIDTH / HEIGHT;
    float near_plane = 0.1f;
    float far_plane = 200.0f;
    Math::Mat4 perspective = Math::Mat4::Perspective(fov, aspect, near_plane, far_plane);
    Math::Mat4 view = camera.getViewMatrix();

    diffuse_shader.setProjection(perspective);
    diffuse_shader.setView(view);

    model_shader.setProjection(perspective);
    model_shader.setView(view);

    for (int i = 0; i < 4; i++) {
        Math::Mat4 model = Math::Mat4::Identity();
        model = rot_mat * translate_mats[i] * rot_mat;
        diffuse_shader.setModel(model);
        pole.draw(&diffuse_shader);

        Math::Mat4 translate_mat = translate_mats[i];
        if (is_translating) {
            translate_mat.v[1].w = sin(glfwGetTime() * (1 + i)) - 0.5f;
        }

        model = rot_mat * translate_mat * animals_rot[i];
        diffuse_shader.setModel(model);
        if (i != 3) {
            animals[i].draw(&diffuse_shader);
        }
    }
 
    Math::Mat4 model = Math::Mat4::Identity();
    model = Math::Mat4::Scale(model, 2);
    model = rot_mat * model;
    model = Math::Mat4::Translate(model, 0, 2.5f, 0);
    diffuse_shader.setModel(model);
    hexplane.draw(&diffuse_shader);

    model = Math::Mat4::Identity();
    model = Math::Mat4::Scale(model, 2);
    model = rot_mat * model;
    model = Math::Mat4::Translate(model, 0, -2.5f, 0);
    diffuse_shader.setModel(model);
    hexplane.draw(&diffuse_shader);

    model = Math::Mat4::Identity();
    model = Math::Mat4::Scale(model, 5);
    model = Math::Mat4::Rotate(model, Math::Quat::FromEuler(90, 90, 0));
    model = rot_mat * model;
    model = Math::Mat4::Translate(model, 0, 5, 0);
    model_shader.setModel(model);
    church.draw(&model_shader); 
    
    glUseProgram(0);
}

void init_pole_geometry() {
    DS::Vector<Vertex> vertices = {
        Vertex(Math::Vec3{-0.05f, +2.5f, +0.0f}, Math::Vec3{1, 1, 1}),
        Vertex(Math::Vec3{-0.05f, -2.5f, +0.0f}, Math::Vec3{1, 1, 1}),
        Vertex(Math::Vec3{+0.05f, +2.5f, +0.0f}, Math::Vec3{1, 1, 1}),

        Vertex(Math::Vec3{+0.05f, +2.5f, +0.0f}, Math::Vec3{1, 1, 1}),
        Vertex(Math::Vec3{-0.05f, -2.5f, +0.0f}, Math::Vec3{1, 1, 1}),
        Vertex(Math::Vec3{+0.05f, -2.5f, +0.0f}, Math::Vec3{1, 1, 1}),
    };
    DS::Vector<unsigned int> indices = DS::Vector<unsigned int>();

    // TODO(Jovanni): can you derive these flags from the vertex data???
    VertexAttributeFlag flags = VertexAttributeFlag::aPosition | VertexAttributeFlag::aNormal;
    pole = Graphics::Geometry(flags, vertices, indices);
}

void init_hexplane_geometry() {
    DS::Vector<Vertex> vertices = {
        Vertex(Math::Vec3{+0.0f, +0.0f, +0.0f}, Math::Vec3{1, 1, 1}),
        Vertex(Math::Vec3{-0.5f, +0.0f, +1.0f}, Math::Vec3{1, 1, 1}),
        Vertex(Math::Vec3{+0.5f, +0.0f, +1.0f}, Math::Vec3{1, 1, 1}),

        Vertex(Math::Vec3{+0.0f, +0.0f, +0.0f}, Math::Vec3{1, 1, 1}),
        Vertex(Math::Vec3{+0.5f, +0.0f, +1.0f}, Math::Vec3{1, 1, 1}),
        Vertex(Math::Vec3{+1.0f, +0.0f, +0.0f}, Math::Vec3{1, 1, 1}),

        Vertex(Math::Vec3{+0.0f, +0.0f, +0.0f}, Math::Vec3{1, 1, 1}),
        Vertex(Math::Vec3{+1.0f, +0.0f, +0.0f}, Math::Vec3{1, 1, 1}),
        Vertex(Math::Vec3{+0.5f, +0.0f, -1.0f}, Math::Vec3{1, 1, 1}),

        Vertex(Math::Vec3{+0.0f, +0.0f, +0.0f}, Math::Vec3{1, 1, 1}),
        Vertex(Math::Vec3{+0.5f, +0.0f, -1.0f}, Math::Vec3{1, 1, 1}),
        Vertex(Math::Vec3{-0.5f, +0.0f, -1.0f}, Math::Vec3{1, 1, 1}),

        Vertex(Math::Vec3{+0.0f, +0.0f, +0.0f}, Math::Vec3{1, 1, 1}),
        Vertex(Math::Vec3{-0.5f, +0.0f, -1.0f}, Math::Vec3{1, 1, 1}),
        Vertex(Math::Vec3{-1.0f, +0.0f, +0.0f}, Math::Vec3{1, 1, 1}),

        Vertex(Math::Vec3{+0.0f, +0.0f, +0.0f}, Math::Vec3{1, 1, 1}),
        Vertex(Math::Vec3{-1.0f, +0.0f, +0.0f}, Math::Vec3{1, 1, 1}),
        Vertex(Math::Vec3{-0.5f, +0.0f, +1.0f}, Math::Vec3{1, 1, 1}),
    };
    DS::Vector<unsigned int> indices = DS::Vector<unsigned int>();

    VertexAttributeFlag flags = VertexAttributeFlag::aPosition | VertexAttributeFlag::aNormal;
    hexplane = Graphics::Geometry(flags, vertices, indices);
}

int main(int argc, char** argv) {
    Memory::GeneralAllocator allocator = Memory::GeneralAllocator();
    Memory::bindAllocator(&allocator);

	glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 4);

    #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr);
    if (window == nullptr) {
        LOG_ERROR("Failed to create GLFW window\n");
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    // glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    // glfwSetScrollCallback(window, scroll_callback);
	// glfwSetKeyCallback(window, keyboard);
	// glfwSetCursorPosCallback(window, mouse);

    IOD::Init();
    if (!IOD_GLFW_SETUP(window)) {
        LOG_ERROR("Failed to setup IOD_GLFW\n");
        glfwTerminate();
        return -1;
    }
    // void IOD_GLFW_BIND_KEY_CALLBACK(GLFWkeyfun cb);
    // void IOD_GLFW_BIND_MOUSE_BUTTON_CALLBACK(GLFWmousebuttonfun cb);
    IOD_GLFW_BIND_MOUSE_MOVE_CALLBACK(mouse);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        LOG_ERROR("Failed to initialize GLAD\n");
        glfwTerminate();
        return -1;
    }

    glfwSwapInterval(1);
    glfwSetInputMode(window, GLFW_CURSOR, mouse_captured ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
    
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_STENCIL_TEST);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    glEnable(GL_FRAMEBUFFER_SRGB);

    diffuse_shader = ShaderDiffuse({"../../ShaderSource/Diffuse/diffuse.vert", "../../ShaderSource/Diffuse/diffuse.frag"});
    model_shader = ShaderModel({"../../ShaderSource/Model/model.vert", "../../ShaderSource/Model/model.frag"});

	init_pole_geometry();
    init_hexplane_geometry();

    animals[0] = Graphics::Geometry::Model("../../Models/cow.ply");
    animals_rot[0] = Math::Mat4::Rotate(animals_rot[0], 270, 0, 1, 0);
    translate_mats[0] = Math::Mat4::Translate(translate_mats[0], -2, 0, 0);

    animals[1] = Graphics::Geometry::Model("../../Models/hippo.ply");
    animals_rot[1] = Math::Mat4::Rotate(animals_rot[1], 90, 0, 1, 0);
    translate_mats[1] = Math::Mat4::Translate(translate_mats[1], 2, 0, 0);

    animals[2] = Graphics::Geometry::Model("../../Models/lion.ply");
    animals_rot[2] = Math::Mat4::Rotate(animals_rot[2], 0, 0, 1, 0);
    translate_mats[2] = Math::Mat4::Translate(translate_mats[2], 0, 0, 2);
    translate_mats[3] = Math::Mat4::Translate(translate_mats[3], 0, 0, -2);

    church = Graphics::Geometry::Model("../../Models/church.glb");
    // church = Graphics::Geometry::Model("../../Models/backpack/backpack.obj");

    IOD::CreateProfile(MASTER_PROFILE, cbMasterProfile);
    IOD::CreateProfile(MOVEMENT_PROFILE, cbMovementProfile);

    camera = Camera(0, 0, 10);
    float previous = 0;
	while(!glfwWindowShouldClose(window)) {
        float current = glfwGetTime();
        dt = current - previous;
        previous = current;

        IOD::Poll();

		display();

		glfwPollEvents();
        glfwSwapBuffers(window);
	}

	exit(EXIT_SUCCESS);
}

/*
Math::Vec3 getCirclePoint(Math::Vec3 center, float radius, float theta) {
    float rad = DEGREES_TO_RAD(theta);

    float offset_x = cos(rad) * radius;
    float offset_z = sin(rad) * radius;

    return Math::Vec3(center.x + offset_x, center.y, center.z + offset_z);
}


void draw_circle() {
    Math::Vec3 CENTER = Math::Vec3(0);
    const float RADIUS = 1.0f;
    const int SEGMENT_COUNT = 360;
    const float DELTA_THETA = 1 / SEGMENT_COUNT;

    for (int i = 0; i < SEGMENT_COUNT; i++) { 
        Math::Vec3 square_point = getSquarePoint(CENTER, DELTA_THETA * i, RADIUS);
        Math::Vec3 circle_point = getCirclePoint(CENTER, DELTA_THETA * i, RADIUS);
        uv.x = ((DELTA_THETA * i) / SEGMENT_COUNT); // you can divide by 4 to make it on each the sides
        uv.y = 0 to 1; // if you are botton triangle you are 0.0f, if you are top triangle you are 1.0f

        // derive the quad segmenet from center
        Math::Vec3 p0; // BL (Bottom Left)
        Math::Vec3 p1; // TL
        Math::Vec3 p2; // BR
        Math::Vec3 p3; // TR
        Math::Vec3 p4; // TL
        Math::Vec3 p5; // BR

        Math::Vec3 quad_normal = Math::Vec3::Cross(p1 - p0, p2 - p0);

        // Math::Vec3 closest = Math::Vec3::Closest(square_point, circle_point);

        Math::Vec2 normalized_camera_front = Math::Vec2(camera.front.x, camera.front.z).normalize();
        Math::Vec2 normalized_segment_normal = Math::Vec2(normal.x, normal.z).normalize();
        float camera_normal_dot = Math::Vec2::Dot(normalized_camera_front, normalized_segment_normal);
        bool front_facing = (camera_normal_dot >= -1.0f) && (camera_normal_dot <= -0.8f);
        // draw_slice(p0, )
    }
}
*/