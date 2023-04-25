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
}

void Renderer::Render()
{
	//Main Renderer / Rendering Function
	for (uint32_t y = 0; y < m_FinalImage->GetHeight(); y++)
	{
		for (uint32_t x = 0; x < m_FinalImage->GetWidth(); x++)
		{
			glm::vec2 coord = {
				(float)x / (float)m_FinalImage->GetWidth(),
				(float)y / (float)m_FinalImage->GetHeight()
			};

			//remapping coords to -1 to 1 was 0 - 1
			coord = coord * 2.0f - 1.0f;

			glm::vec4 color = PerPixel(coord);
			color = glm::clamp(color, glm::vec4(0.0f), glm::vec4(1.0f));
			m_ImageData[x + y * m_FinalImage->GetWidth()] = Utils::ConvertVec4ToRGBA32Bit(color);
		}
	}

	m_FinalImage->SetData(m_ImageData);
}

glm::vec4 Renderer::PerPixel(glm::vec2 fragCoord)
{
	//solve for t : t is the magnitude of the ray (quadratic formula below)
	//  BELOW IS a			BELOW IS b			BELOW IS C
	// (bx^2 + by^2)t^2 + (2(axbx + ayby))t + (ax^2 + ay^2 - r^2) = 0
	//x & y is the coords/origin : r is the radius : a is ray origin : b is ray direction

	glm::vec3 rayOrigin(0.0f,0.0f,1.5f);
	glm::vec3 rayDir(fragCoord.x, fragCoord.y, -1.0f);
	float radius = 0.5f;
	//rayDir = glm::normalize(rayDir);

	//final quadratic equation implemented
	float a = glm::dot(rayDir, rayDir);	//equivalent to float a =	rayDir.x * rayDir.x + rayDir.y * rayDir.y + rayDir.z * rayDir.z;
	float b = 2.0f * glm::dot(rayOrigin, rayDir);
	float c = glm::dot(rayOrigin, rayOrigin) - radius * radius;

	//finding out the # of solutions from the quadratic equation
	//quad formula discriminant = b^2 - 4ac
	float discriminant = b * b - 4.0f * a * c;

	//quad formula full = -b +- sqrt(discriminant) / 2a

	if (discriminant < 0)
	{
		return glm::vec4(0, 0, 0, 1);
	}

	float t0 = (-b + glm::sqrt(discriminant)) / (2.0f * a);
	float closeHitLength = (-b - glm::sqrt(discriminant)) / (2.0f * a);//t1
	glm::vec3 h0 = rayOrigin + rayDir * t0;
	glm::vec3 hitPos1 = rayOrigin + rayDir * closeHitLength;//h1
	glm::vec3 normals = glm::normalize(hitPos1);
	//normals = normals * 0.5f + 0.5f; //remapping to 0 - 1

	glm::vec3 lightDir = glm::normalize(glm::vec3(-1, -1, -1));
	float dotNL = glm::max(glm::dot(normals,-lightDir), 0.0f);

	glm::vec3 sphereColor  (1,0.5f,0);
	sphereColor *= dotNL;//normals;
	return glm::vec4(sphereColor, 1);
}
