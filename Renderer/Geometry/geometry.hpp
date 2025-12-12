#pragma once

#include <glad/glad.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <Common/common.hpp>
#include <DataStructure/ds.hpp>
#include <Math/math.hpp>
#include <Geometry/material.hpp>
#include <Geometry/vertex.hpp>

#include <renderer_state.hpp>

#define MAX_BONES 128

typedef struct ShaderBase ShaderBase;
namespace Renderer  {
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
		GLuint VAO = 0; 
		GLuint VBO = 0; 
		GLuint EBO = 0;
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

		// TODO(Jovanni): I really would like to simplify this
		template<SupportedVertexAttributeType T>
		void addVertexAttribute(int location, const T& value) {
			RUNTIME_ASSERT_MSG(
				!this->vertex_attribute_locations.has(location),
				"Location already assigned"
			);

			int max_attribs;
			glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &max_attribs);
			RUNTIME_ASSERT_MSG(max_attribs > location, "Location outside range");

			Renderer::BindVAO(this->VAO);

			glDisableVertexAttribArray(location);
			if constexpr (std::is_same_v<T, float>){
				glVertexAttrib1f(location, value);
				this->vertex_attribute_locations.put(location, true);
			} else if constexpr (std::is_same_v<T, Math::Vec2>){
				glVertexAttrib2f(location, value.x, value.y);
				this->vertex_attribute_locations.put(location, true);
			} else if constexpr (std::is_same_v<T, Math::Vec3>){
				glVertexAttrib3f(location, value.x, value.y, value.z);
				this->vertex_attribute_locations.put(location, true);
			} else if constexpr (std::is_same_v<T, Math::Vec4>){
				glVertexAttrib4f(location, value.x, value.y, value.z, value.w);
				this->vertex_attribute_locations.put(location, true);
			} else if constexpr (std::is_same_v<T, int>){
				glVertexAttribI1i(location, value);
			} else if constexpr (std::is_same_v<T, Math::IVec4>){
				glVertexAttribI4i(location, value.x, value.y, value.z, value.w);
				this->vertex_attribute_locations.put(location, true);
			} else if constexpr (std::is_same_v<T, Math::Mat4>) {
				for (int i = 0; i < 4; i++) {
					RUNTIME_ASSERT_MSG(max_attribs > location + i,
						"Matrix would overflow available attributes"
					);

					glDisableVertexAttribArray(location + i);
					glVertexAttrib4fv(location + i, &value[i][0]);
					this->vertex_attribute_locations.put(location + i, true);
				}
			}
		}

		// TODO(Jovanni): I would like to clean this up
		template<SupportedVertexAttributeType T>
		void addVertexAttribute(int location, const DS::Vector<T>& values) {
			RUNTIME_ASSERT_MSG(
				!this->vertex_attribute_locations.has(location),
				"You are trying to use a location that has already been assigned"
			);

			int attribute_count;
			glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &attribute_count);
			RUNTIME_ASSERT_MSG(attribute_count > location,
				"You are trying to use a location that is outside of the max vertex attributes: %d",
				attribute_count
			);

			constexpr bool is_matrix = std::is_same_v<T, Math::Mat4>;
			constexpr bool is_int     = std::is_same_v<T, int> || std::is_same_v<T, Math::IVec4>;
			constexpr GLenum gl_type  = is_int ? GL_INT : GL_FLOAT;
			constexpr int component_count = (
				std::is_same_v<T, bool>        ? 1  :
				std::is_same_v<T, int>         ? 1  :
				std::is_same_v<T, float>       ? 1  :
				std::is_same_v<T, Math::Vec2>  ? 2  :
				std::is_same_v<T, Math::Vec3>  ? 3  :
				std::is_same_v<T, Math::Vec4>  ? 4  :
				std::is_same_v<T, Math::IVec4> ? 4  :
				std::is_same_v<T, Math::Mat4>  ? 16 : 0
			);

			Renderer::BindVAO(this->VAO);

			unsigned int vbo;
			glGenBuffers(1, &vbo);
			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glBufferData(GL_ARRAY_BUFFER, values.count() * sizeof(T), (const void*)values.data(), GL_STATIC_DRAW);

			constexpr GLsizei stride = sizeof(T);
			if constexpr (is_matrix) {
				for (int i = 0; i < 4; i++) {
					RUNTIME_ASSERT_MSG(attribute_count > location + i,
						"You are trying to use a location that is outside of the max vertex attributes: %d",
						attribute_count
					);

					glEnableVertexAttribArray(location + i);
					if constexpr (is_int) {
						glVertexAttribIPointer(location + i, 4, gl_type, stride, (void*)(sizeof(Math::IVec4) * i));
					} else {
						glVertexAttribPointer(location + i, 4, gl_type, false, stride, (void*)(sizeof(Math::Vec4) * i));
					}

					this->vertex_attribute_locations.put(location + i, true);
				}
			} else {
				glEnableVertexAttribArray(location);
				if constexpr (is_int) {
					glVertexAttribIPointer(location, component_count, gl_type, stride, nullptr);
				} else {
					glVertexAttribPointer(location, component_count, gl_type, false, stride, nullptr);
				}

				this->vertex_attribute_locations.put(location, true);
			}
		}

		template<SupportedVertexAttributeType T>
		void addInstanceVertexAttribute(int location, const DS::Vector<T>& values) {
			RUNTIME_ASSERT_MSG(
				!this->vertex_attribute_locations.has(location),
				"You are trying to use a location that has already been assigned"
			);

			int attribute_count;
			glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &attribute_count);
			RUNTIME_ASSERT_MSG(attribute_count >= location,
				"You are trying to use a location that is outside of the max vertex attributes: %d",
				attribute_count
			);

			constexpr bool is_matrix = std::is_same_v<T, Math::Mat4>;
			constexpr bool is_int     = std::is_same_v<T, int> || std::is_same_v<T, Math::IVec4>;
			constexpr GLenum gl_type  = is_int ? GL_INT : GL_FLOAT;
			constexpr int component_count = (
				std::is_same_v<T, bool>        ? 1  :
				std::is_same_v<T, int>         ? 1  :
				std::is_same_v<T, float>       ? 1  :
				std::is_same_v<T, Math::Vec2>  ? 2  :
				std::is_same_v<T, Math::Vec3>  ? 3  :
				std::is_same_v<T, Math::Vec4>  ? 4  :
				std::is_same_v<T, Math::IVec4> ? 4  :
				std::is_same_v<T, Math::Mat4>  ? 16 : 0
			);

			Renderer::BindVAO(this->VAO);

			unsigned int instance_vbo;
			glGenBuffers(1, &instance_vbo);
			glBindBuffer(GL_ARRAY_BUFFER, instance_vbo);
			glBufferData(GL_ARRAY_BUFFER, values.count() * sizeof(T), values.data(), GL_STATIC_DRAW);

			constexpr GLsizei stride = sizeof(T);
			if constexpr (is_matrix) {
				for (int i = 0; i < 4; i++) {
					RUNTIME_ASSERT_MSG(attribute_count > location + i,
						"You are trying to use a location that is outside of the max vertex attributes: %d",
						attribute_count
					);

					glEnableVertexAttribArray(location + i);

					if constexpr (is_int) {
						glVertexAttribIPointer(location + i, 4, gl_type, stride, (void*)(sizeof(Math::IVec4) * i));
					} else {
						glVertexAttribPointer(location + i, 4, gl_type, GL_FALSE, stride, (void*)(sizeof(Math::Vec4) * i));
					}

					glVertexAttribDivisor(location + i, 1);
					this->vertex_attribute_locations.put(location + i, true);
				}
			} else {
				glEnableVertexAttribArray(location);

				if constexpr (is_int) {
					glVertexAttribIPointer(location, component_count, gl_type, stride, nullptr);
				} else {
					glVertexAttribPointer(location, component_count, gl_type, GL_FALSE, stride, nullptr);
				}

				glVertexAttribDivisor(location, 1);
				this->vertex_attribute_locations.put(location, true);
			}
		}

		void draw(const ShaderBase* shader);
		void drawInstanced(const ShaderBase* shader, int instance_count);

		private:
			DS::Vector<Vertex> vertices; // only the root will have vertices
			DS::Vector<GLuint> indices;  // only the root will have indices
			DS::Vector<Material> materials;
			DS::Hashmap<int, bool> vertex_attribute_locations;

			void setup(bool should_destory_data = true);

			void loadMeshFromData(const DS::Vector<Vertex> &vertices, const DS::Vector<unsigned int> &indices);
			void loadMeshFromScene(const char *path, u64 path_length, const aiScene* scene);
			Geometry* processNode(Geometry* root, aiNode* node, const aiScene* scene, Math::Mat4 parent_transform);
			void processAssimpMesh(Geometry* root, aiMesh* ai_mesh,  const aiScene* scene, Math::Mat4 parent_transform);
	};
}