#pragma once

#include "utils/error_handler.h"

#include <algorithm>
#include <linalg.h>
#include <vector>


using namespace linalg::aliases;

namespace cg
{
template<typename T>
class resource
{
public:
	resource(size_t size);
	resource(size_t x_size, size_t y_size);
	~resource();

	const T* get_data();
	T& item(size_t item);
	T& item(size_t x, size_t y);

	size_t get_size_in_bytes() const;
	size_t get_number_of_elements() const;
	size_t get_stride() const;

private:
	std::vector<T> data;
	size_t item_size = sizeof(T);
	size_t stride;
};
template<typename T>
inline resource<T>::resource(size_t size)
{
	data.resize(size);
	stride = 0;
}
template<typename T>
inline resource<T>::resource(size_t x_size, size_t y_size)
{
	data.resize(x_size * y_size);
	stride = x_size;
}
template<typename T>
inline resource<T>::~resource()
{

}
template<typename T>
inline const T* resource<T>::get_data()
{
	return data.data();
}
template<typename T>
inline T& resource<T>::item(size_t item)
{
	return data.at(item);
}
template<typename T>
inline T& resource<T>::item(size_t x, size_t y)
{
	return data.at(stride * y + x);
}
template<typename T>
inline size_t resource<T>::get_size_in_bytes() const
{
	return data.size() * item_size;
}
template<typename T>
inline size_t resource<T>::get_number_of_elements() const
{
	return data.size();
}

template<typename T>
inline size_t resource<T>::get_stride() const
{
	return stride;
}
struct color
{
	static color from_float3(const float3& in)
	{
		THROW_ERROR("Not implemented yet");
		return color();
	};
	float r;
	float g;
	float b;
};

struct unsigned_color
{
	static unsigned_color from_color(const color& color)
	{
		THROW_ERROR("Not implemented yet");
		return unsigned_color();
	};
	float3 to_float3()
	{
		THROW_ERROR("Not implemented yet");
		return float3();
	};
	unsigned char r;
	unsigned char g;
	unsigned char b;
};


struct vertex
{
	float x;
	float y;
	float z;

	float nx;
	float ny;
	float nz;

	float ambient_r;
	float ambient_g;
	float ambient_b;

	float diffuse_r;
	float diffuse_g;
	float diffuse_b;

	float emissive_r;
	float emissive_g;
	float emissive_b;


	inline static vertex vertex::interpolate_bary(
		const vertex& v1, const vertex& v2, const vertex& v3, float u, float v, float w)
	{
		vertex result;

		result.x = v1.x * u + v2.x * v + v3.x * w;
		result.y = v1.y * u + v2.y * v + v3.y * w;
		result.z = v1.z * u + v2.z * v + v3.z * w;

		result.nx = v1.nx * u + v2.nx * v + v3.nx * w;
		result.ny = v1.ny * u + v2.ny * v + v3.ny * w;
		result.nz = v1.nz * u + v2.nz * v + v3.nz * w;

		result.ambient_r = v1.ambient_r * u + v2.ambient_r * v + v3.ambient_r * w;
		result.ambient_g = v1.ambient_g * u + v2.ambient_g * v + v3.ambient_g * w;
		result.ambient_b = v1.ambient_b * u + v2.ambient_b * v + v3.ambient_b * w;

		result.diffuse_r = v1.diffuse_r * u + v2.diffuse_r * v + v3.diffuse_r * w;
		result.diffuse_g = v1.diffuse_g * u + v2.diffuse_g * v + v3.diffuse_g * w;
		result.diffuse_b = v1.diffuse_b * u + v2.diffuse_b * v + v3.diffuse_b * w;

		result.emissive_r = v1.emissive_r * u + v2.emissive_r * v + v3.emissive_r * w;
		result.emissive_g = v1.emissive_g * u + v2.emissive_g * v + v3.emissive_g * w;
		result.emissive_b = v1.emissive_b * u + v2.emissive_b * v + v3.emissive_b * w;

		return result;
	}
};

} // namespace cg