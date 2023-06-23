#include "Renderer.h"
#include "Walnut/Random.h"

namespace Utils
{
	static uint32_t ConvertVec4ToRGBA32Bit(const glm::vec4 col)
	{
		uint8_t r = (uint8_t)(col.r * 255.0f);
		uint8_t g = (uint8_t)(col.g * 255.0f);
		uint8_t b = (uint8_t)(col.b * 255.0f);
		uint8_t a = (uint8_t)(col.a * 255.0f);

		//rgba abgr
		uint32_t finalCol = (a << 24) | (b << 16) | (g << 8) | r;
		return finalCol;
	}
}

void Renderer::OnResize(uint32_t width, uint32_t height)
{
	if (m_FinalImage)
	{
		if (m_FinalImage->GetHeight() == height && m_FinalImage->GetWidth() == width)
		{
			return;
		}

		m_FinalImage->Resize(width, height);
	}
	else
	{
		m_FinalImage = std::make_shared<Walnut::Image>(width, height,
			Walnut::ImageFormat::RGBA);
	}

	delete[] m_ImageData;
	m_ImageData = new uint32_t[width * height];

	delete[] m_AccumulationData;
	m_AccumulationData = new glm::vec4[width * height];
}

void Renderer::Render(const Scene& scene, const Camera& camera)
{
	m_ActiveScene = &scene;
	m_ActiveCamera = &camera;

	if (m_FrameIndex == 1)//start with setting all memory @ accumulation data to 0 since its our first frame
	{
		memset(m_AccumulationData, 0, 
			m_FinalImage->GetHeight() * m_FinalImage->GetWidth() * sizeof(glm::vec4));
	}

	//Main Renderer / Rendering Function
	for (uint32_t y = 0; y < m_FinalImage->GetHeight(); y++)
	{
		for (uint32_t x = 0; x < m_FinalImage->GetWidth(); x++)
		{
			glm::vec4 color = PerPixel(x,y);
			m_AccumulationData[x + y * m_FinalImage->GetWidth()] += color;//add color to accumulated data (without averaging the color would be really bright)

			glm::vec4 accumulatedCol = m_AccumulationData[x + y * m_FinalImage->GetWidth()];	
			accumulatedCol /= (float)m_FrameIndex;//Averaging the accumulated color data

			accumulatedCol = glm::clamp(accumulatedCol, glm::vec4(0.0f), glm::vec4(1.0f));//clamp range to 0 1
			m_ImageData[x + y * m_FinalImage->GetWidth()] = Utils::ConvertVec4ToRGBA32Bit(accumulatedCol); //1byte/8bit per color channel
		}
	}

	m_FinalImage->SetData(m_ImageData);

	if (m_Settings.Accumulate)
	{m_FrameIndex++;}
	else 
	{m_FrameIndex = 1;}
}

glm::vec4 Renderer::PerPixel(uint32_t x, uint32_t y)
{
	Ray ray;
	ray.origin = m_ActiveCamera->GetPosition();
	ray.direction = m_ActiveCamera->GetRayDirections()[x + y * m_FinalImage->GetWidth()];

	glm::vec3 color(0);
	float multiplier = 1.0f;

	int rayBounces = 6;
	for (int i = 0; i < rayBounces; i++)
	{
		Renderer::HitPayload payload = TraceRay(ray);
		if (payload.HitDistance < 0.0f)
		{
			glm::vec3 skyColor = glm::vec3(0.1f, 0.5f, 0.9f);
			color += skyColor * multiplier;
			break;
		}

		glm::vec3 lightDir = glm::normalize(glm::vec3(-1, -1, -1));
		float dotNL = glm::max(glm::dot(payload.WorldNormal, -lightDir), 0.0f);//dot normal & light

		const Sphere& sphere = m_ActiveScene->Spheres[payload.ObjectIndex];
		const Material& material = m_ActiveScene->Materials[sphere.materialIndex];

		glm::vec3 sphereColor = material.Albedo;
		sphereColor *= dotNL;//normals;
		color += sphereColor * multiplier;

		multiplier *= 0.6f;

		ray.origin = payload.WorldPosition + payload.WorldNormal * 0.0001f;
		ray.direction = glm::reflect(ray.direction, payload.WorldNormal
			+ material.Roughness * Walnut::Random::Vec3(-0.5f,0.5f));
	}
	return glm::vec4(color, 1);
}

Renderer::HitPayload Renderer::ClosestHit(const Ray& ray, float hitDistance, int objectIndex)
{
	Renderer::HitPayload payload;
	payload.HitDistance = hitDistance;
	payload.ObjectIndex = objectIndex;


	const Sphere& closestSphere = m_ActiveScene->Spheres[objectIndex];

	glm::vec3 origin = ray.origin - closestSphere.Position;
	payload.WorldPosition = origin + ray.direction * hitDistance;//h1
	payload.WorldNormal = glm::normalize(payload.WorldPosition);//h1
	//float t0 = (-b + glm::sqrt(discriminant)) / (2.0f * a);
	//float closeHitLength = (-b - glm::sqrt(discriminant)) / (2.0f * a);//t1
	//glm::vec3 h0 = newOrigin + ray.direction * t0;
	//glm::vec3 normals = glm::normalize(h0);
	//normals = normals * 0.5f + 0.5f; //remapping to 0 - 1

	payload.WorldPosition += closestSphere.Position;
	return payload;
}

Renderer::HitPayload Renderer::Miss(const Ray& ray)
{
	Renderer::HitPayload payload;
	payload.HitDistance = -1.0f;//-1.0f;

	return payload;
}

Renderer::HitPayload Renderer::TraceRay(const Ray& ray)
{
	//(bx^2  +  by^2)t^2 + (2(axbx  +  ayby))t + (ax^2  +  ay^2  -  r^2) = 0
	//a = ray origin
	//b = ray direction
	//r = radius
	//t = hit distance /mag

	int closestSphere = -1;
	float hitDist = std::numeric_limits<float>::max();
	for (size_t i = 0; i < m_ActiveScene->Spheres.size(); i++)
	{
		const Sphere& sphere = m_ActiveScene->Spheres[i];

		glm::vec3 newOrigin = ray.origin - sphere.Position;

		//final quadratic equation implemented
		float a = glm::dot(ray.direction, ray.direction);	//equivalent to float a =	rayDir.x * rayDir.x + rayDir.y * rayDir.y + rayDir.z * rayDir.z;
		float b = 2.0f * glm::dot(newOrigin, ray.direction);
		float c = glm::dot(newOrigin, newOrigin) - sphere.Radius * sphere.Radius;

		//finding out the # of solutions from the quadratic equation
		//quad formula discriminant = b^2 - 4ac
		float discriminant = b * b - 4.0f * a * c;

		//quad formula full = -b +- sqrt(discriminant) / 2a

		if (discriminant < 0)
		{
			continue;
		}


		//float t0 = (-b + glm::sqrt(discriminant)) / (2.0f * a);
		float closestT = (-b - glm::sqrt(discriminant)) / (2.0f * a);//t1
		if (closestT > 0.0f && closestT < hitDist)
		{
			hitDist = closestT;
			closestSphere = (int)i;
		}
	}

	if (closestSphere < 0)
	{
		return Miss(ray);
	}

	return ClosestHit(ray, hitDist, closestSphere);
}
