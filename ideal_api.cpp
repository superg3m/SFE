#include <Core/core.hpp>
#include <Graphics/graphics.hpp>
#include <GLFW/glfw3.h>

static ShaderModel model_shader;
static Graphics::Geometry pole;
static Graphics::Geometry hexplane;
static Graphics::Geometry* animals[4];

Math::Matrix4 translate_mats[4];
Math::Matrix4 animals_rot[4];

float saved_rot = 0.0f;
float saved_translation = 0.0f;

static int is_rotating = 1;
static int is_translating = 1;

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

    model_shader.use();
    model_shader.setProjection(persepctive);
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
        if (animals[i] != NULL) {
            animals[i]->draw();
        }
    }

    float modelview[16];
    float scale_mat[16];
    mat4f_scale_new(scale_mat, 2, 2, 2);

    float translate_mat_top[16];
    mat4f_translate_new(translate_mat_top, 0, 2.5f, 0);

    float translate_mat_bot[16];
    mat4f_translate_new(translate_mat_bot, 0, -2.5f, 0);

    mat4f_mult_mat4f_many(modelview, viewMat, translate_mat_top, rot_mat, scale_mat, NULL);
    glUniformMatrix4fv(kuhl_get_uniform("ModelView"), 1, 0, modelview);
    kuhl_geometry_draw(&hexplane);

    mat4f_mult_mat4f_many(modelview, viewMat, translate_mat_bot, rot_mat, scale_mat, NULL);
    glUniformMatrix4fv(kuhl_get_uniform("ModelView"), 1, 0, modelview);
    kuhl_geometry_draw(&hexplane);
    kuhl_errorcheck();
    
    glUseProgram(0);
}

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

int main(int argc, char** argv) {
	kuhl_ogl_init(&argc, argv, -1, -1, 32, 4);
	glfwSetKeyCallback(kuhl_get_window(), keyboard);
	// glfwSetFramebufferSizeCallback(window, reshape);

	program = kuhl_create_program("triangle-shade.vert", "triangle-shade.frag");
	glUseProgram(program);
	kuhl_errorcheck();
    glUseProgram(0);

	init_pole_geometry(&pole, program);
    init_hexplane_geometry(&hexplane, program);

    animals[0] = kuhl_load_model("./models/merry/cow.ply", NULL, program, NULL);
    mat4f_rotateAxis_new(animals_rot[0], 270, 0.0f, 1.0f, 0.0f);
    mat4f_translate_new(translate_mats[0], -2, 0, 0);

    animals[1] = kuhl_load_model("./models/merry/hippo.ply", NULL, program, NULL);
    mat4f_translate_new(translate_mats[1], 2, 0, 0);
    mat4f_rotateAxis_new(animals_rot[1], 90, 0.0f, 1.0f, 0.0f);

    animals[2] = kuhl_load_model("./models/merry/lion.ply", NULL, program, NULL);
    mat4f_translate_new(translate_mats[2], 0, 0, 2);
    mat4f_rotateAxis_new(animals_rot[2], 0, 0.0f, 1.0f, 0.0f);  

    animals[3] = NULL;
    mat4f_translate_new(translate_mats[3], 0, 0, -2);
    

	float initCamPos[3]  = {0,0,10}; // location of camera
	float initCamLook[3] = {0,0,0}; // a point the camera is facing at
	float initCamUp[3]   = {0,1,0}; // a vector indicating which direction is up
	viewmat_init(initCamPos, initCamLook, initCamUp);
	
	while(!glfwWindowShouldClose(kuhl_get_window())) {
		display();
		kuhl_errorcheck();

		/* process events (keyboard, mouse, etc) */
		glfwPollEvents();
	}

	exit(EXIT_SUCCESS);
}
