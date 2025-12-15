#pragma once

#include <glad/glad.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "../../Core/DataStructure/sfe_ds.hpp"
#include "../../Core/Math/sfe_math.hpp"
#include "../GPUBuffer/sfe_gpu_buffer.hpp"
#include "../VertexArray/sfe_vertex_array.hpp"
#include "../Core/Common/sfe_common.hpp"
#include "../sfe_renderer.hpp"
#include "sfe_material.hpp"
#include "sfe_vertex.hpp"

#define MAX_BONES 128

typedef struct ShaderBase ShaderBase;
namespace GFX  {
    template<typename T>
    concept SupportedVertexAttributeType = (
        std::is_same_v<T, bool> ||
        std::is_same_v<T, int> ||
        std::is_same_v<T, float> || 
        std::is_same_v<T, Math::Vec2> ||
        std::is_same_v<T, Math::Vec3> ||
        std::is_same_v<T, Math::Vec4> ||
        std::is_same_v<T, Math::IVec4> ||
        std::is_same_v<T, Math::Mat4>
    );

	struct Geometry {
		VertexArray VAO; 
		GPUBuffer VBO; 
		GPUBuffer EBO;
		GLuint vertex_count = 0;
		GLuint index_count = 0;
		GLenum draw_type = GL_TRIANGLES; // GL_TRIANGLES, GL_LINES, ...

		GLuint total_vertex_count = 0;

		u32 base_vertex  = 0; // offset to next vertex in the vertex buffer
		u32 base_index   = 0; // offset to next index in the index buffer

		Material material;
		Math::AABB aabb;
		Geometry* next = nullptr;
		bool wireframe = false;

		Geometry();
		Geometry(const DS::Vector<Vertex>& vertices, GLenum draw_type = GL_TRIANGLES);
		Geometry(const DS::Vector<Vertex>& vertices, const DS::Vector<unsigned int>& indices, GLenum draw_type = GL_TRIANGLES);

		static Geometry Quad();
		static Geometry Quad(int width, int height); // tessellated quad
		static Geometry AABB();
		static Geometry Cube();
		static Geometry Sphere(int segments);
		static Geometry Model(const char* path);

		void draw(ShaderBase* shader);
		void drawInstanced(ShaderBase* shader, int instance_count);

		private:
			DS::Vector<Vertex> vertices; // only the root will have vertices
			DS::Vector<GLuint> indices;  // only the root will have indices
			DS::Vector<Material> materials;

			void setup(bool should_destory_data = true);
			void loadMeshFromData(const DS::Vector<Vertex> &vertices, const DS::Vector<unsigned int> &indices);
			void loadMeshFromScene(const char *path, u64 path_length, const aiScene* scene);
			Geometry* processNode(Geometry* root, aiNode* node, const aiScene* scene, Math::Mat4 parent_transform);
			void processAssimpMesh(Geometry* root, aiMesh* ai_mesh,  const aiScene* scene, Math::Mat4 parent_transform);
	};
}