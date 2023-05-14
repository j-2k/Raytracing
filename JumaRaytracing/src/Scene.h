#pragma once
#include <vector>
#include <glm/vec3.hpp>


struct Sphere {
	glm::vec3 Position{ .0f,.0f,.0f };
	float Radius = .5f;
	glm::vec3 Albedo{ 1.0f };
};

struct Scene {
	std::vector<Sphere> Spheres;
};