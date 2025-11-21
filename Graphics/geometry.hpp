#include "glad/glad.h"
#include "vertex.hpp"

#include "../Common/common.hpp"
#include "../Math/math.hpp"

#include "material.hpp"
#include "../DataStructure/ds.hpp"

namespace Graphics {
	struct Geometry {
		GLuint VAO = 0; 
		GLuint VBO = 0; 
		GLuint EBO = 0;
		GLuint vertex_count;
		GLuint index_count;
		GLenum draw_type; // GL_TRIANGLES, GL_LINES, ...

		u32 base_vertex  = 0; // offset to next vertex in the vertex buffer
		u32 base_index   = 0; // offset to next index in the index buffer
		Material material;
		Math::AABB aabb;

		Geometry* next;

		Geometry();
		Geometry(VertexAttributeFlag flags, const DS::Vector<Vertex>& vertices, const DS::Vector<unsigned int>& indices, GLenum draw_type = GL_TRIANGLES);

		static Geometry Quad();
		static Geometry AABB();
		static Geometry Cube();
		static Geometry Sphere(int segments);
		static Geometry Model(const char* path);

		void setup(VertexAttributeFlag flags, const DS::Vector<Vertex>& vertices, const DS::Vector<unsigned int>& indicies);

		private:
			// void loadMeshFromScene(const std::string &path, const aiScene* scene);
			// void loadMeshFromData(const std::vector<Vertex> &vertices, const std::vector<unsigned int> &indices, VertexAttributeFlag flags);
			// MeshEntry processMesh(aiMesh *mesh, const aiScene *scene, GM_Matrix4 absolute_transform);
			// void processNode(aiNode* node, const aiScene* scene, GM_Matrix4 parent_transform);
	};
}