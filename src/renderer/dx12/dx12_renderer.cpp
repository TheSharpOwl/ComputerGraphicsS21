#include "dx12_renderer.h"

#include "utils/com_error_handler.h"
#include "utils/window.h"

#include <filesystem>


void cg::renderer::dx12_renderer::init()
{
	// set the default values
	rtv_descriptor_size = 0;
	view_port = CD3DX12_VIEWPORT(0.f, 0.f,
		static_cast<float>(settings->width), static_cast<float>(settings->height));
	scissor_rect = CD3DX12_RECT(0, 0, static_cast<LONG>(settings->width)
		, static_cast<LONG>(settings->height));
	vertex_buffer_view = {};

	constant_buffer_data_begin = nullptr;
	frame_index = 0;

	for (size_t i = 0; i < frame_number; i++)
	{
		fence_values[i] = 0;
	}

	//Loading the model
	model = std::make_shared<cg::world::model>();
	model->load_obj(settings->model_path);

	// camera preparation
	camera = std::make_shared<cg::world::camera>();
	camera->set_height(static_cast<float>(settings->height));
	camera->set_width(static_cast<float>(settings->width));

	camera->set_position( float3{settings->camera_position[0],
								 settings->camera_position[1],
								 settings->camera_position[2]});

	camera->set_theta(settings->camera_theta);
	camera->set_phi(settings->camera_phi);

	camera->set_angle_of_view(settings->camera_angle_of_view);
	camera->set_z_far(settings->camera_z_far);
	camera->set_z_near(settings->camera_z_near);

	world_view_projection =
		camera->get_dxm_view_matrix() * camera->get_dxm_projection_matrix();

	load_pipeline();
	load_assets();
}

void cg::renderer::dx12_renderer::destroy()
{
	wait_for_gpu();
	CloseHandle(fence_event);
}

void cg::renderer::dx12_renderer::update()
{
	world_view_projection = camera->get_dxm_view_matrix() * camera->get_dxm_projection_matrix();
	memcpy(constant_buffer_data_begin, &world_view_projection, sizeof(world_view_projection));
}

void cg::renderer::dx12_renderer::render()
{
	populate_command_list();

	ID3D12CommandList* command_lists[] = { command_list.Get() };
	command_queue->ExecuteCommandLists(_countof(command_lists), command_lists);

	THROW_IF_FAILED(swap_chain->Present(0, 0));

	move_to_next_frame();

}

void cg::renderer::dx12_renderer::load_pipeline()
{
	THROW_ERROR("Not implemented yet")
}

void cg::renderer::dx12_renderer::load_assets()
{
	THROW_ERROR("Not implemented yet")
}

void cg::renderer::dx12_renderer::populate_command_list()
{
	THROW_ERROR("Not implemented yet")
}


void cg::renderer::dx12_renderer::move_to_next_frame()
{
	THROW_ERROR("Not implemented yet")
}

void cg::renderer::dx12_renderer::wait_for_gpu()
{
	THROW_ERROR("Not implemented yet")
}
