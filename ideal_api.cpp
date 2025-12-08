#include <core.hpp>
#include <rendering.hpp>
#include <input.hpp>
#include <input_glfw.hpp>

#include <GLFW/glfw3.h>
#include <glad/glad.h>

ShaderDiffuse diffuse_shader;
ShaderModel model_shader;
ShaderUniformColor uniform_shader;
// ShaderParticle particle_shader;

Math::Mat4 translate_mats[4];
Math::Mat4 animals_rot[4];

Renderer::Geometry pole;
Renderer::Geometry hexplane;
Renderer::Geometry animals[4];
Renderer::Geometry church;
Renderer::Geometry particle_geometry;

Renderer::Geometry quad;
DS::Vector<Math::Vec2> translations;

float saved_rot = 0.0f;
float saved_translation = 0.0f;
static int is_rotating = 1;
static int is_translating = 1;
bool emit = false;

#define MASTER_PROFILE "master"
#define MOVEMENT_PROFILE "movement"

// This should be a grouping of state
Camera camera;
bool mouse_captured = true;
float dt = 0;
float WIDTH = 900;
float HEIGHT = 900;
Random::Seed seed;

const int MAX_PARTICLES = 1000;
Particle particles[MAX_PARTICLES];
u32 next_available_particle_index;

void cbMasterProfile() {
    GLFWwindow* window = (GLFWwindow*)Input::glfw_window_instance;
    const bool SHIFT = Input::GetKey(Input::KEY_SHIFT, Input::PRESSED|Input::DOWN);

    if (Input::GetKeyPressed(Input::KEY_ESCAPE)) {
        glfwSetWindowShouldClose(window, true);
    }

    if (Input::GetKeyPressed(Input::KEY_R)) {
        diffuse_shader.compile();
        model_shader.compile();
        uniform_shader.compile();
    }

    if (Input::GetKeyPressed(Input::KEY_0)) {
        emit = !emit;
        model_shader.setEmissiveMaterial(emit);
    }

    if (SHIFT && Input::GetKeyPressed(Input::KEY_W)) {
        Input::ToggleProfile(MOVEMENT_PROFILE);
    }

    if (Input::GetKeyPressed(Input::KEY_L)) {
        Renderer::SetWireFrame(true);
    } else if (Input::GetKeyReleased(Input::KEY_L)) {
        Renderer::SetWireFrame(false);
    }

    if (Input::GetKeyPressed(Input::KEY_C)) {
        mouse_captured = !mouse_captured;
        glfwSetInputMode(window, GLFW_CURSOR, mouse_captured ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
    }

    if (Input::GetKeyPressed(Input::KEY_U)) {
        is_rotating = !is_rotating;
		is_translating = !is_translating;
    }
}

void cbMovementProfile() {
    GLFWwindow* window = (GLFWwindow*)Input::glfw_window_instance;

    if (Input::GetKey(Input::KEY_SPACE, Input::PRESSED|Input::DOWN)) {
        camera.processKeyboard(UP, dt);
    }

    if (Input::GetKey(Input::KEY_CTRL, Input::PRESSED|Input::DOWN)) {
        camera.processKeyboard(DOWN, dt);
    }

    if (Input::GetKey(Input::KEY_W, Input::PRESSED|Input::DOWN)) {
        camera.processKeyboard(FORWARD, dt); 
    }

    if (Input::GetKey(Input::KEY_A, Input::PRESSED|Input::DOWN)) {
        camera.processKeyboard(LEFT, dt); 
    }

    if (Input::GetKey(Input::KEY_S, Input::PRESSED|Input::DOWN)) {
        camera.processKeyboard(BACKWARD, dt); 
    }

    if (Input::GetKey(Input::KEY_D, Input::PRESSED|Input::DOWN)) {
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
    }
}

void display() {
    glClearColor(0.2f, 0.2f, 0.2f, 0);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    if (is_rotating == 1) {
        saved_rot = fmod(glfwGetTime() * 45.0f, 360);
    }

    Math::Mat4 rot_mat = Math::Mat4::Identity(); 
    rot_mat = Math::Mat4::Rotate(rot_mat, saved_rot, 0, 1, 0);

    Math::Mat4 perspective = Renderer::GetProjectionMatrix3D(WIDTH, HEIGHT, camera.zoom);
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

    quad.drawInstanced(&uniform_shader, 100);

    // LOG_WARN("Draw Call Count: %d\n", Renderer::GetDrawCallCount());

    // spawn particles
    const float PARTICLE_SPAWN_COUNT_PER_FRAME = 5;
    for (int i = 0; i < PARTICLE_SPAWN_COUNT_PER_FRAME; i++) {
        Particle p;
        // make this have some randomness Random::GenerateF32(&seed);
        p.position = Math::Vec3(0, 0, 0);
        p.velocity = Math::Vec3(0, 1, 0);

        particles[next_available_particle_index] = p;
        next_available_particle_index = (next_available_particle_index + 1) % MAX_PARTICLES;
    }

    // update and render particles
    for (int i = 0; i < MAX_PARTICLES; i++) {
        Particle* p = &particles[i];
        p->position += p->velocity.scale(dt);
    }

    // create model
    // particle_geometry.setVertexAttribute(8, ) // This will rebind
    //  particle_geometry.drawInstanced(&particle_shader, MAX_PARTICLES);

    Renderer::ClearTelemetry();
}

void init_pole_geometry() {
    DS::Vector<Renderer::Vertex> vertices = {
        Renderer::Vertex(Math::Vec3{-0.05f, +2.5f, +0.0f}, Math::Vec3{1, 1, 1}),
        Renderer::Vertex(Math::Vec3{-0.05f, -2.5f, +0.0f}, Math::Vec3{1, 1, 1}),
        Renderer::Vertex(Math::Vec3{+0.05f, +2.5f, +0.0f}, Math::Vec3{1, 1, 1}),

        Renderer::Vertex(Math::Vec3{+0.05f, +2.5f, +0.0f}, Math::Vec3{1, 1, 1}),
        Renderer::Vertex(Math::Vec3{-0.05f, -2.5f, +0.0f}, Math::Vec3{1, 1, 1}),
        Renderer::Vertex(Math::Vec3{+0.05f, -2.5f, +0.0f}, Math::Vec3{1, 1, 1}),
    };

    // TODO(Jovanni): can you derive these flags from the vertex data???
    pole = Renderer::Geometry(vertices);
}

void init_hexplane_geometry() {
    DS::Vector<Renderer::Vertex> vertices = {
        Renderer::Vertex(Math::Vec3{+0.0f, +0.0f, +0.0f}, Math::Vec3{1, 1, 1}),
        Renderer::Vertex(Math::Vec3{-0.5f, +0.0f, +1.0f}, Math::Vec3{1, 1, 1}),
        Renderer::Vertex(Math::Vec3{+0.5f, +0.0f, +1.0f}, Math::Vec3{1, 1, 1}),

        Renderer::Vertex(Math::Vec3{+0.0f, +0.0f, +0.0f}, Math::Vec3{1, 1, 1}),
        Renderer::Vertex(Math::Vec3{+0.5f, +0.0f, +1.0f}, Math::Vec3{1, 1, 1}),
        Renderer::Vertex(Math::Vec3{+1.0f, +0.0f, +0.0f}, Math::Vec3{1, 1, 1}),

        Renderer::Vertex(Math::Vec3{+0.0f, +0.0f, +0.0f}, Math::Vec3{1, 1, 1}),
        Renderer::Vertex(Math::Vec3{+1.0f, +0.0f, +0.0f}, Math::Vec3{1, 1, 1}),
        Renderer::Vertex(Math::Vec3{+0.5f, +0.0f, -1.0f}, Math::Vec3{1, 1, 1}),

        Renderer::Vertex(Math::Vec3{+0.0f, +0.0f, +0.0f}, Math::Vec3{1, 1, 1}),
        Renderer::Vertex(Math::Vec3{+0.5f, +0.0f, -1.0f}, Math::Vec3{1, 1, 1}),
        Renderer::Vertex(Math::Vec3{-0.5f, +0.0f, -1.0f}, Math::Vec3{1, 1, 1}),

        Renderer::Vertex(Math::Vec3{+0.0f, +0.0f, +0.0f}, Math::Vec3{1, 1, 1}),
        Renderer::Vertex(Math::Vec3{-0.5f, +0.0f, -1.0f}, Math::Vec3{1, 1, 1}),
        Renderer::Vertex(Math::Vec3{-1.0f, +0.0f, +0.0f}, Math::Vec3{1, 1, 1}),

        Renderer::Vertex(Math::Vec3{+0.0f, +0.0f, +0.0f}, Math::Vec3{1, 1, 1}),
        Renderer::Vertex(Math::Vec3{-1.0f, +0.0f, +0.0f}, Math::Vec3{1, 1, 1}),
        Renderer::Vertex(Math::Vec3{-0.5f, +0.0f, +1.0f}, Math::Vec3{1, 1, 1}),
    };

    hexplane = Renderer::Geometry(vertices);
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

    Input::Init();
    if (!Input::GLFW_SETUP(window)) {
        LOG_ERROR("Failed to setup GLFW\n");
        glfwTerminate();
        return -1;
    }
    // void GLFW_BIND_KEY_CALLBACK(GLFWkeyfun cb);
    // void GLFW_BIND_MOUSE_BUTTON_CALLBACK(GLFWmousebuttonfun cb);
    Input::GLFW_BIND_MOUSE_MOVE_CALLBACK(mouse);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        LOG_ERROR("Failed to initialize GLAD\n");
        glfwTerminate();
        return -1;
    }

    glfwSwapInterval(1);
    glfwSetInputMode(window, GLFW_CURSOR, mouse_captured ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
    
    glEnable(GL_MULTISAMPLE);
    Renderer::SetDepthTest(true);
    Renderer::SetStencilTest(true);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // glEnable(GL_FRAMEBUFFER_SRGB);

    diffuse_shader = ShaderDiffuse({"../../ShaderSource/Diffuse/diffuse.vert", "../../ShaderSource/Diffuse/diffuse.frag"});
    model_shader = ShaderModel({"../../ShaderSource/Model/model.vert", "../../ShaderSource/Model/model.frag"});
    uniform_shader = ShaderUniformColor({"../../ShaderSource/QuadInstance/quad.vert", "../../ShaderSource/QuadInstance/quad.frag"});

	init_pole_geometry();
    init_hexplane_geometry();

    animals[0] = Renderer::Geometry::Model("../../Models/cow.ply");
    animals_rot[0] = Math::Mat4::Rotate(animals_rot[0], 270, 0, 1, 0);
    translate_mats[0] = Math::Mat4::Translate(translate_mats[0], -2, 0, 0);

    animals[1] = Renderer::Geometry::Model("../../Models/hippo.ply");
    animals_rot[1] = Math::Mat4::Rotate(animals_rot[1], 90, 0, 1, 0);
    translate_mats[1] = Math::Mat4::Translate(translate_mats[1], 2, 0, 0);

    animals[2] = Renderer::Geometry::Model("../../Models/lion.ply");
    animals_rot[2] = Math::Mat4::Rotate(animals_rot[2], 0, 0, 1, 0);
    translate_mats[2] = Math::Mat4::Translate(translate_mats[2], 0, 0, 2);
    translate_mats[3] = Math::Mat4::Translate(translate_mats[3], 0, 0, -2);

    church = Renderer::Geometry::Model("../../Models/church.glb");

    DS::Vector<Renderer::Vertex> quad_vertices = {
        Math::Vec3(-0.05f, +0.05f, 0),
        Math::Vec3(+0.05f, -0.05f, 0),
        Math::Vec3(-0.05f, -0.05f, 0),

        Math::Vec3(-0.05f, +0.05f, 0),
        Math::Vec3(+0.05f, -0.05f, 0),
        Math::Vec3(+0.05f, +0.05f, 0),
    };

    quad = Renderer::Geometry(quad_vertices);

    int index = 0;
    float offset = 0.1f;
    translations = DS::Vector<Math::Vec2>(100);
    translations.resize(100);
    for (int y = -10; y < 10; y += 2) {
        for (int x = -10; x < 10; x += 2) {
            Math::Vec2 translation;
            translation.x = (float)x / 10.0f + offset;
            translation.y = (float)y / 10.0f + offset;
            translations[index++] = translation;
        }
    }

    quad.addInstanceVertexAttribute(8, translations);

    Input::CreateProfile(MASTER_PROFILE, cbMasterProfile);
    Input::CreateProfile(MOVEMENT_PROFILE, cbMovementProfile);

    camera = Camera(0, 0, 10);

    Random::Seed seed = Random::GenerateSeed(451);

    float previous = 0;
	while (!glfwWindowShouldClose(window)) {
        float current = glfwGetTime();
        dt = current - previous;
        previous = current;

        Input::Poll();

		display();

        LOG_DEBUG("Entropy: %f\n", Random::GenerateRange(&seed, 34.2f, 64.1f));

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