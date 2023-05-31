#pragma once
#include "Walnut/Image.h"
#include <memory>
#include <glm/glm.hpp>
#include "Camera.h"
#include "Ray.h"
#include "Scene.h"

class Renderer
{
public:
	Renderer() = default;

	void OnResize(uint32_t width, uint32_t height);
	void Render(const Scene& scene, const Camera& camera);

	std::shared_ptr<Walnut::Image>GetFinalImage() const { return m_FinalImage; }

	private:
		glm::vec4 PerPixel(uint32_t x,uint32_t y); //another popular term called RayGen 

		struct HitPayload {
			float HitDistance;
			glm::vec3 WorldPosition;
			glm::vec3 WorldNormal;

			uint32_t ObjectIndex;
		};

		HitPayload TraceRay(const Ray& ray);
		HitPayload ClosestHit(const Ray& ray, float hitDistance, uint32_t objectIndex);
		HitPayload Miss(const Ray& ray);
private:
	std::shared_ptr<Walnut::Image> m_FinalImage;
	uint32_t* m_ImageData = nullptr;

	const Scene* m_ActiveScene = nullptr;
	const Camera* m_ActiveCamera = nullptr;
};