#include "raytracer_renderer.h"

#include "utils/resource_utils.h"


void cg::renderer::ray_tracing_renderer::init()
{
	render_target = std::make_shared<cg::resource<cg::unsigned_color>>(
		settings->width, settings->height);

	model = std::make_shared<cg::world::model>();
	model->load_obj(settings->model_path);

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

	shadow_raytracer = std::make_shared<cg::renderer::raytracer<cg::vertex, cg::unsigned_color>>();
	raytracer->set_render_target(render_target);
	raytracer->set_viewport(settings->width, settings->height);

	raytracer->set_per_shape_vertex_buffer(model->get_per_shape_buffer());

	lights.push_back({ float3{ 0, 1.58f, -0.03f }, float3{ 0.78f, 0.78f, 0.78f } });
}

void cg::renderer::ray_tracing_renderer::destroy() {}

void cg::renderer::ray_tracing_renderer::update() {}

inline float smoothstep(float edge0, float edge1, float x) 
{
	x = std::clamp((x - edge0) / (edge1 - edge0), 0.f, 1.f);
	return x * x * (3 - 2 * x);
}

void cg::renderer::ray_tracing_renderer::render()
{
	raytracer->clear_render_target({ 255, 255, 100 });

	raytracer->miss_shader =
		[](const ray& ray)
	{
		payload payload;
		float3 ground = float3(0.8f, 0.7f, 0.7f);
		float3 sky = float3(77.f / 255.f, 174.f / 255.f, 219.f / 255.f);
		float t = smoothstep(0.f, 0.5f, ray.direction.y + 0.5f);
		float3 color = ground * (1 - t) + sky * t;
		payload.color = cg::color::from_float3(color);

		payload.t = t;

		// payload.t = -1.f;
		/*payload.color = { ray.direction.x * 0.5f + 0.5f, ray.direction.y * 0.5f + 0.5f,
						  ray.direction.z * 0.5f + 0.5f };*/

		//payload.color = { 200.f, 255.f, (ray.direction.y + 1.0f) * 0.5f };
		return payload;
	};

	raytracer->closest_hit_shader = [&](const ray& ray, payload& payload,
									   const triangle<cg::vertex>& triangle)
	{
		float3 result_color = triangle.emissive;

		float3 position = ray.position + ray.direction * payload.t;
		float3 normal = payload.bary.x * triangle.na +
						payload.bary.y * triangle.nb + payload.bary.z * triangle.nc;

		for(auto& light : lights)
		{
			cg::renderer::ray to_light(position, light.position - position);

			auto shadow_payload =
				shadow_raytracer->trace_ray(to_light, 1, length(light.position - position));

			if (shadow_payload.t == -1.f)
			{
				result_color += triangle.diffuse * light.color *
								std::max(dot(normal, to_light.direction), 0.f);
			}
		}
		payload.color = cg::color::from_float3(result_color);
		return payload;
	};

	

	shadow_raytracer->miss_shader = [](const ray& ray) {
		payload payload{};
		payload.t = -1.f;
		return payload;
	};

	shadow_raytracer->any_hit_shader = [](const ray& ray, payload& payload,
										const triangle<cg::vertex>& triangle) {
		return payload;
	};

	raytracer->build_acceleration_structure();

	shadow_raytracer->acceleration_structures = raytracer->acceleration_structures;

	raytracer->ray_generation(camera->get_position(),
		camera->get_direction(),
		camera->get_right(),
		camera->get_up());

	cg::utils::save_resource(*render_target, settings->result_path);
}
