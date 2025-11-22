#include <Core/core.hpp>
#include <Graphics/graphics.hpp>
#include <GLFW/glfw3.h>
#include <glad/glad.h>

static ShaderModel model_shader;
static Graphics::Geometry pole;
static Graphics::Geometry hexplane;
static Graphics::Geometry animals[4];
Math::Matrix4 translate_mats[4];
Math::Matrix4 animals_rot[4];

float saved_rot = 0.0f;
float saved_translation = 0.0f;
static int is_rotating = 1;
static int is_translating = 1;

Camera camera;
float WIDTH = 800;
float HEIGHT = 600;

// NOTE(Jovanni):
// This handles like -1 mod 5 = 4
uint32_t math_mod(int a, int b) {
    int rem = a % b;
    return (rem < 0) ? (rem + b) : rem;
}

void keyboard(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if(action == GLFW_PRESS) {
		return;
    }

	if(key == GLFW_KEY_SPACE) {
		is_rotating = !is_rotating;
	}

	if(key == GLFW_KEY_U) {
		is_translating = !is_translating;
    }
}

void display() {
    if(is_rotating == 1) {
        saved_rot = fmod(glfwGetTime() * 45.0f, 360);
    }

    Math::Matrix4 rot_mat = Math::Matrix4::Identity(); 
    Math::Matrix4::Rotate(rot_mat, saved_rot, 0, 1, 0);

    float fov = camera.zoom;
    float aspect = WIDTH / HEIGHT;
    float near_plane = 0.1f;
    float far_plane = 1000.0f;
    Math::Matrix4 perspective = Math::Matrix4::Perspective(fov, aspect, near_plane, far_plane);
    Math::Matrix4 view = camera.getViewMatrix();

    model_shader.use();
    model_shader.setProjection(perspective);
    model_shader.setView(view);
    for (int i = 0; i < 4; i++) {
        Math::Matrix4 model = rot_mat * translate_mats[i];
        model_shader.setModel(model);
        pole.draw();

        Math::Matrix4 translate_mat = translate_mats[i];
        if (is_translating) {
            float x = translate_mat.v[0].w;
            float y = sin(glfwGetTime() * (1 + i)) - 0.5f;
            float z = translate_mat.v[2].w;
            Math::Matrix4::Translate(translate_mat, x, y, z);
        }

        model = rot_mat * translate_mat * animals_rot[i];
        model_shader.setModel(model);
        if (i != 3) {
            animals[i].draw();
        }
    }
 
    Math::Matrix4 model = Math::Matrix4::Identity();
    model = Math::Matrix4::Scale(model, 2);
    model = rot_mat * model;
    model = Math::Matrix4::Translate(model, 0, 2.5f, 0);
    model_shader.setModel(model);
    hexplane.draw();

    model = Math::Matrix4::Identity();
    model = Math::Matrix4::Scale(model, 2);
    model = rot_mat * model;
    model = Math::Matrix4::Translate(model, 0, -2.5f, 0);
    model_shader.setModel(model);
    hexplane.draw();
    
    glUseProgram(0);
}

/*
void init_pole_geometry(kuhl_geometry *geo, GLuint program){
    #define SG_VERTS_COUNT 6
    #define SG_POS_COUNT 3
    #define SG_NORM_COUNT 3

    kuhl_geometry_new(
        geo, program, SG_VERTS_COUNT, 
	    GL_TRIANGLES
    );

    GLfloat vertexPositions[SG_VERTS_COUNT * SG_POS_COUNT] = {
        -0.05f, +2.5f, +0.0f,
        -0.05f, -2.5f, +0.0f,
        +0.05f, +2.5f, +0.0f,

        +0.05f, +2.5f, +0.0f,
        -0.05f, -2.5f, +0.0f,
        +0.05f, -2.5f, +0.0f,
    };

	kuhl_geometry_attrib(
        geo, vertexPositions,
	    SG_POS_COUNT,
	    "in_Position",
	    KG_WARN
    );

    GLfloat normVert[SG_VERTS_COUNT * SG_NORM_COUNT] = {
        1, 1, 1,
        1, 1, 1,
        1, 1, 1,

        1, 1, 1,
        1, 1, 1,
        1, 1, 1
    };

	kuhl_geometry_attrib(
        geo, normVert,
	    SG_NORM_COUNT,
	    "in_Normal",
	    KG_WARN
    );

    #undef SG_VERTS_COUNT
    #undef SG_POS_COUNT
    #undef SG_NORM_COUNT
}

void init_hexplane_geometry(kuhl_geometry *geo, GLuint program){
    #define SG_VERTS_COUNT 18
    #define SG_POS_COUNT 3
    #define SG_NORM_COUNT 3

    kuhl_geometry_new(
        geo, program, SG_VERTS_COUNT, 
	    GL_TRIANGLES
    );

    GLfloat vertexPositions[SG_VERTS_COUNT * SG_POS_COUNT] = {
        +0.0f, +0.0f, +0.0f,
        -0.5f, +0.0f, +1.0f,
        +0.5f, +0.0f, +1.0f,

        +0.0f, +0.0f, +0.0f,
        +0.5f, +0.0f, +1.0f,
        +1.0f, +0.0f, +0.0f,

        +0.0f, +0.0f, +0.0f,
        +1.0f, +0.0f, +0.0f,
        +0.5f, +0.0f, -1.0f,

        +0.0f, +0.0f, +0.0f,
        +0.5f, +0.0f, -1.0f,
        -0.5f, +0.0f, -1.0f,

        +0.0f, +0.0f, +0.0f,
        -0.5f, +0.0f, -1.0f,
        -1.0f, +0.0f, +0.0f,

        +0.0f, +0.0f, +0.0f,
        -1.0f, +0.0f, +0.0f,
        -0.5f, +0.0f, +1.0f,
    };

	kuhl_geometry_attrib(
        geo, vertexPositions,
	    SG_POS_COUNT,
	    "in_Position",
	    KG_WARN
    );

    GLfloat normVert[SG_VERTS_COUNT * SG_NORM_COUNT] = {
        1, 1, 1,
        1, 1, 1,
        1, 1, 1,
        
        1, 1, 1,
        1, 1, 1,
        1, 1, 1,

        1, 1, 1,
        1, 1, 1,
        1, 1, 1,

        1, 1, 1,
        1, 1, 1,
        1, 1, 1,

        1, 1, 1,
        1, 1, 1,
        1, 1, 1,

        1, 1, 1,
        1, 1, 1,
        1, 1, 1,
    };

	kuhl_geometry_attrib(
        geo, normVert,
	    SG_NORM_COUNT,
	    "in_Normal",
	    KG_WARN
    );

    #undef SG_VERTS_COUNT
    #undef SG_POS_COUNT
    #undef SG_NORM_COUNT
}
*/

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

    model_shader = ShaderModel({""});

	// init_pole_geometry(&pole, program);
    // init_hexplane_geometry(&hexplane, program);

    animals[0] = Graphics::Geometry::Model("./models/merry/cow.ply");
    animals_rot[0] = Math::Matrix4::Rotate(animals_rot[0], 270, 0, 1, 0);
    translate_mats[0] = Math::Matrix4::Translate(translate_mats[0], -2, 0, 0);

    animals[1] = Graphics::Geometry::Model("./models/merry/hippo.ply");
    animals_rot[1] = Math::Matrix4::Rotate(animals_rot[1], 90, 0, 1, 0);
    translate_mats[1] = Math::Matrix4::Translate(translate_mats[1], 2, 0, 0);

    animals[2] = Graphics::Geometry::Model("./models/merry/lion.ply");
    animals_rot[2] = Math::Matrix4::Rotate(animals_rot[2], 0, 0, 1, 0);
    translate_mats[2] = Math::Matrix4::Translate(translate_mats[2], 0, 0, 2);
    translate_mats[3] = Math::Matrix4::Translate(translate_mats[3], 0, 0, -2);

    camera = Camera(0, 0, 10);
	while(!glfwWindowShouldClose(window)) {
		display();
		glfwPollEvents();
	}

	exit(EXIT_SUCCESS);
}
