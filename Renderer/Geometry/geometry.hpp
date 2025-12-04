#include <glad/glad.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <Common/common.hpp>
#include <DataStructure/ds.hpp>
#include <Math/math.hpp>
#include <Geometry/material.hpp>
#include <Geometry/vertex.hpp>

#define MAX_BONES 128

typedef struct ShaderBase ShaderBase;
namespace Renderer  {
	
	struct Geometry {
		GLuint VAO = 0; 
		GLuint VBO = 0; 
		GLuint EBO = 0;
		GLuint vertex_count = 0;
		GLuint index_count = 0;
		GLenum draw_type = GL_TRIANGLES; // GL_TRIANGLES, GL_LINES, ...

		u32 base_vertex  = 0; // offset to next vertex in the vertex buffer
		u32 base_index   = 0; // offset to next index in the index buffer

		Material material;
		Math::AABB aabb;
		Geometry* next = nullptr;
		Geometry* root = nullptr;

		Geometry();
		Geometry(VertexAttributeFlag flags, const DS::Vector<Vertex>& vertices, const DS::Vector<unsigned int>& indices, GLenum draw_type = GL_TRIANGLES);

		static Geometry Quad();
		static Geometry AABB();
		static Geometry Cube();
		static Geometry Sphere(int segments);
		static Geometry Model(const char* path);

		void draw(const ShaderBase* shader);

		private:
			DS::Vector<Vertex> vertices; // only the root will have vertices
			DS::Vector<GLuint> indices;  // only the root will have indices
			DS::Vector<Material> materials;

			void setup(VertexAttributeFlag flags, bool should_destory_data = true);

			void loadMeshFromData(const DS::Vector<Vertex> &vertices, const DS::Vector<unsigned int> &indices, VertexAttributeFlag flags);
			void loadMeshFromScene(const char *path, u64 path_length, const aiScene* scene);
			Geometry* processNode(Geometry* root, aiNode* node, const aiScene* scene, Math::Mat4 parent_transform);
			void processAssimpMesh(Geometry* root, aiMesh* ai_mesh,  const aiScene* scene, Math::Mat4 parent_transform);
	};
}