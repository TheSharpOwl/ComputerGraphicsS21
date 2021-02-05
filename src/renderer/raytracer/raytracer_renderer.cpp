#include "raytracer_renderer.h"

#include "utils/resource_utils.h"


void cg::renderer::ray_tracing_renderer::init()
{
	render_target = std::make_shared<cg::resource<cg::unsigned_color>>(
		settings->width, settings->height);

	camera = std::make_shared<cg::world::camera>();
	camera->set_height(static_cast<float>(settings->height));
	camera->set_width(static_cast<float>(settings->width));
	camera->set_position(float3{ settings->camera_position[0],
								 settings->camera_position[1],
								 settings->camera_position[2] });
	camera->set_theta(settings->camera_theta);
	camera->set_phi(settings->camera_phi);
	camera->set_angle_of_view(settings->camera_angle_of_view);
	camera->set_z_far(settings->camera_z_far);
	camera->set_z_near(settings->camera_z_near);

	raytracer = std::make_shared<cg::renderer::raytracer<cg::vertex, cg::unsigned_color>>();

	raytracer->set_render_target(render_target);
	raytracer->set_viewport(settings->width, settings->height);
}

void cg::renderer::ray_tracing_renderer::destroy() {}

void cg::renderer::ray_tracing_renderer::update() {}

void cg::renderer::ray_tracing_renderer::render()
{
	raytracer->clear_render_target({ 255, 255, 100 });

	raytracer->miss_shader =
		[](const ray& ray)
	{
		payload payload{};
		payload.t = -1.f;
		payload.color = { ray.direction.x / 0.5f + 0.5f, ray.direction.y / 0.5f + 0.5f,
						  ray.direction.z / 0.5f + 0.5f };
		//payload.color = { 200.f, 255.f, (ray.direction.y + 1.0f) * 0.5f };
		return payload;
	};

	raytracer->ray_generation(camera->get_position(),
		camera->get_direction(),
		camera->get_right(),
		camera->get_up());

	cg::utils::save_resource(*render_target, settings->result_path);
}