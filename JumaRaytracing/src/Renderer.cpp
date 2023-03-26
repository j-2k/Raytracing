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

	//this will draw a float4(uv.xy,0,1) quad or "Hello World"
	return 0xff000000 | (g << 8) | r; //ABGR
	//BIT SHIFTING RIGHT TO LEFT!!! ABGR FORMAT!

}
