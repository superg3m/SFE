#include <core.hpp>
#include <graphics.hpp>
#include <GLFW/glfw3.h>
#include <glad/glad.h>

ShaderDiffuse diffuse_shader;
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

Camera camera;
float WIDTH = 900;
float HEIGHT = 900;

void keyboard(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if(action == GLFW_PRESS) {
		return;
    }

    if (key == GLFW_KEY_ESCAPE) {
        glfwSetWindowShouldClose(window, true);
    }

	if(key == GLFW_KEY_SPACE) {
		is_rotating = !is_rotating;
	}

	if(key == GLFW_KEY_U) {
		is_translating = !is_translating;
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
    float far_plane = 1000.0f;
    Math::Mat4 perspective = Math::Mat4::Perspective(fov, aspect, near_plane, far_plane);
    Math::Mat4 view = camera.getViewMatrix();

    diffuse_shader.use();
    diffuse_shader.setProjection(perspective);
    diffuse_shader.setView(view);
    for (int i = 0; i < 4; i++) {
        Math::Mat4 model = Math::Mat4::Identity();
        model = rot_mat * translate_mats[i] * rot_mat;
        diffuse_shader.setModel(model);
        pole.draw();

        Math::Mat4 translate_mat = translate_mats[i];
        if (is_translating) {
            translate_mat.v[1].w = sin(glfwGetTime() * (1 + i)) - 0.5f;
        }

        model = rot_mat * translate_mat * animals_rot[i];
        diffuse_shader.setModel(model);
        if (i != 3) {
            animals[i].draw();
        }
    }
 
    Math::Mat4 model = Math::Mat4::Identity();
    model = Math::Mat4::Scale(model, 2);
    model = rot_mat * model;
    model = Math::Mat4::Translate(model, 0, 2.5f, 0);
    diffuse_shader.setModel(model);
    hexplane.draw();

    model = Math::Mat4::Identity();
    model = Math::Mat4::Scale(model, 2);
    model = rot_mat * model;
    model = Math::Mat4::Translate(model, 0, -2.5f, 0);
    diffuse_shader.setModel(model);
    hexplane.draw();

    model = Math::Mat4::Identity();
    model = Math::Mat4::Scale(model, 5);
    model = Math::Mat4::Rotate(model, Math::Quat::FromEuler(90, 90, 0));
    model = rot_mat * model;
    model = Math::Mat4::Translate(model, 0, 5, 0);
    diffuse_shader.setModel(model);
    church.draw(); 
    
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
	glfwSetKeyCallback(window, keyboard);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        LOG_ERROR("Failed to initialize GLAD\n");
        glfwTerminate();
        return -1;
    }

    glfwSwapInterval(1);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_STENCIL_TEST);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    glEnable(GL_FRAMEBUFFER_SRGB);

    Memory::GeneralAllocator allocator = Memory::GeneralAllocator();
    Memory::bindAllocator(&allocator);

    diffuse_shader = ShaderDiffuse({"../../ShaderSource/Diffuse/diffuse.vert", "../../ShaderSource/Diffuse/diffuse.frag"});

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

    camera = Camera(0, 0, 10);
	while(!glfwWindowShouldClose(window)) {
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