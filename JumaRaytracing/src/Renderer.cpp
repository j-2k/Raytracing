#include "Renderer.h"
#include "Walnut/Random.h"

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

			m_ImageData[x + y * m_FinalImage->GetWidth()] = PerPixel(coord);

			//m_ImageData[i] = 0xff00ffff; //ABGR FORMAT (REVERSED RGBA!!!)
			//m_ImageData[i] = Walnut::Random::UInt();
			//m_ImageData[i] |= 0xff000000;
		}
	}

	m_FinalImage->SetData(m_ImageData);
}

uint32_t Renderer::PerPixel(glm::vec2 fragCoord)
{
	uint8_t r = (uint8_t)(fragCoord.x * 255.0f);
	uint8_t g = (uint8_t)(fragCoord.y * 255.0f);

	//solve for t : t is the magnitude of the ray (quadratic formula below)
	//  BELOW IS a			BELOW IS b			BELOW IS C
	// (bx^2 + by^2)t^2 + (2(axbx + ayby))t + (ax^2 + ay^2 - r^2) = 0
	//x & y is the coords/origin : r is the radius : a is ray origin : b is ray direction

	glm::vec3 rayOrigin(0.f,0.f,2.f);
	glm::vec3 rayDir(fragCoord.x, fragCoord.y, -1.0f);
	float radius = 0.5f;
	//rayDir = glm::normalize(rayDir);

	//final quadratic equation implemented
	float a = glm::dot(rayDir, rayDir);	//equivalent to float a =	rayDir.x * rayDir.x + rayDir.y * rayDir.y + rayDir.z * rayDir.z;
	float b = 2.0f * glm::dot(rayOrigin, rayDir);
	float c = glm::dot(rayOrigin, rayOrigin) - radius * radius;

	//finding out the # of solutions from the quadratic equation
	//quad formula discriminant = b^2 - 4ac
	float discriminant = b * b - 4.f * a * c;

	if (discriminant >= 0)
	{
		return 0xffff00ff;
	}

	return 0xff000000;

	/*
	//this will draw a float4(uv.xy,0,1) quad or "Hello World"
	return 0xff000000 | (g << 8) | r; //ABGR
	//BIT SHIFTING RIGHT TO LEFT!!! ABGR FORMAT!
	*/

}
