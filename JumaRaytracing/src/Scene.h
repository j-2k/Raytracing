#pragma once
#include <vector>
#include <glm/vec3.hpp>

struct Material {


	glm::vec3 Albedo{ 1.0f };
	float Roughness = 1.0f;
	float Metallic = 0.0f;

	glm::vec3 EmissionColor{ 0.0f };
	float EmissionPower = 0;

	glm::vec3 GetEmission() const 
	{
		return EmissionColor * EmissionPower;
	}
};

struct Sphere {
	glm::vec3 Position{ .0f,.0f,.0f };
	float Radius = .5f;

	int materialIndex = 0;
};

struct Scene {
	std::vector<Sphere> Spheres;
	std::vector<Material> Materials;
};