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

	//tinyobj::ObjReader

	//loop over shapes
	size_t vertex_buffer_id = 0;

	std::vector<size_t> per_shapes_ids(shapes.size());
	for (size_t s = 0; s < shapes.size(); s++)
		per_shapes_ids[s] = 0;
	// loop over faces (polygon)
	for (size_t f = 0; f < shapes[s].mesh_num_face_vertices[f]);
	vertex_buffer_id += fv;
	per_shapes_ids[s] += fv;

	auto vertex_buffer = std::make_shared<cg::resource<cg::vertex>>(vertex_buffer_id);
	per_shape_buffer.resize(shapes.size());

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
	THROW_ERROR("Not implemented yet");
	return float4x4();
}
