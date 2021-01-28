#define TINYOBJLOADER_IMPLEMENTATION

#include "model.h"

#include "utils/error_handler.h"

#include <linalg.h>


using namespace linalg::aliases;
using namespace cg::world;

cg::world::model::model() {}

cg::world::model::~model() {}

void cg::world::model::load_obj(const std::filesystem::path& model_path)
{
	tinyobj::ObjReaderConfig reader_config;
	reader_config.mtl_search_path = model_path.parent_path().string();
	reader_config.triangulate = true;

	tinyobj::ObjReader reader;

	if (!reader.ParseFromFile(model_path.string(), reader_config) && !reader.Error().empty())
	{
		THROW_ERROR(reader.Error());
	}

	attrib = reader.GetAttrib();
	shapes = reader.GetShapes();
	materials = reader.GetMaterials();

	//loop over shapes
	size_t vertex_buffer_id = 0;
	std::vector<size_t> per_shape_ids(shapes.size());
	for (size_t s = 0; s < shapes.size(); s++)
	{
		per_shape_ids[s] = 0;
		// Loop over faces(polygon)
		for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++)
		{
			int fv = shapes[s].mesh.num_face_vertices[f];
			vertex_buffer_id += fv;
			per_shape_ids[s] += fv;
		}
	}

	vertex_buffer = std::make_shared<cg::resource<cg::vertex>>(vertex_buffer_id);
	per_shape_buffer.resize(shapes.size());

	for (size_t s = 0; s < shapes.size(); s++)
	{
		per_shape_buffer[s] =
			std::make_shared<cg::resource<cg::vertex>>(per_shape_ids[s]);
	}

	// Loop over shapes
	vertex_buffer_id = 0;
	for (size_t s = 0; s < shapes.size(); s++)
	{
		size_t per_shape_id = 0;
		// Loop over faces(polygon)
		size_t index_offset = 0;
		for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++)
		{
			int fv = shapes[s].mesh.num_face_vertices[f];

			// Loop over vertices in the face.
			float3 normal;

			if (shapes[s].mesh.indices[index_offset].normal_index < 0)
			{
				auto a_id = shapes[s].mesh.indices[index_offset + 0];
				auto b_id = shapes[s].mesh.indices[index_offset + 1];
				auto c_id = shapes[s].mesh.indices[index_offset + 2];

				float3 a{ attrib.vertices[3 * a_id.vertex_index + 0],
						  attrib.vertices[3 * a_id.vertex_index + 1],
						  attrib.vertices[3 * a_id.vertex_index + 2] };

				float3 b{ attrib.vertices[3 * b_id.vertex_index + 0],
						  attrib.vertices[3 * b_id.vertex_index + 1],
						  attrib.vertices[3 * b_id.vertex_index + 2] };

				float3 c{ attrib.vertices[3 * c_id.vertex_index + 0],
						  attrib.vertices[3 * c_id.vertex_index + 1],
						  attrib.vertices[3 * c_id.vertex_index + 2] };

				normal = normalize(cross(b - a, c - a));
			}


			for (size_t v = 0; v < fv; v++)
			{

				// access to vertex
				tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];

				cg::vertex vertex = {};
				vertex.x = attrib.vertices[3 * idx.vertex_index + 0];
				vertex.y = attrib.vertices[3 * idx.vertex_index + 1];
				vertex.z = attrib.vertices[3 * idx.vertex_index + 2];

				if (idx.normal_index >= 0)
				{
					vertex.nx = attrib.normals[3 * idx.normal_index + 0];
					vertex.ny = attrib.normals[3 * idx.normal_index + 1];
					vertex.nz = attrib.normals[3 * idx.normal_index + 2];
				}
				else
				{
					vertex.nx = normal.x;
					vertex.ny = normal.y;
					vertex.nz = normal.z;
				}

				if (materials.size() > 0)
				{
					auto material = materials[shapes[s].mesh.material_ids[f]];
					vertex.ambient_r = material.ambient[0];
					vertex.ambient_g = material.ambient[1];
					vertex.ambient_b = material.ambient[2];

					vertex.diffuse_r = material.diffuse[0];
					vertex.diffuse_g = material.diffuse[1];
					vertex.diffuse_b = material.diffuse[2];

					vertex.emissive_r = material.emission[0];
					vertex.emissive_g = material.emission[1];
					vertex.emissive_b = material.emission[2];
				}

				vertex_buffer->item(vertex_buffer_id++) = vertex;
				per_shape_buffer[s]->item(per_shape_id++) = vertex;

				 tinyobj::real_t red = attrib.colors[3*idx.vertex_index+0];
				 tinyobj::real_t green = attrib.colors[3*idx.vertex_index+1];
				 tinyobj::real_t blue = attrib.colors[3*idx.vertex_index+2];
			}
			index_offset += fv;

			// per-face material
			//shapes[s].mesh.material_ids[f];
		}
	}

}

std::shared_ptr<cg::resource<cg::vertex>> cg::world::model::get_vertex_buffer() const
{
	return vertex_buffer;
}

std::vector<std::shared_ptr<cg::resource<cg::vertex>>>
	cg::world::model::get_per_shape_buffer() const
{
	return per_shape_buffer;
}


const float4x4 cg::world::model::get_world_matrix() const
{
	return float4x4({ { 1.0f, 0.0f, 0.0f, 0.0f },
					  { 0.0f, 1.0f, 0.0f, 0.0f },
					  { 0.0f, 0.0f, 1.0f, 0.0f },
					  { 0.0f, 0.0f, 0.0f, 1.0f } });
}
