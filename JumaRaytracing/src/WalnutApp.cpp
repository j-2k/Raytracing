#include "Walnut/Application.h"
#include "Walnut/EntryPoint.h"

#include "Walnut/Image.h"
#include "Walnut/Timer.h"

#include "Renderer.h"
#include "Camera.h"

#include <glm/gtc/type_ptr.hpp>

using namespace Walnut;

class ExampleLayer : public Walnut::Layer
{
public:
	ExampleLayer() : m_Camera(45.0f, 1, 100) {
		//m_Scene.Spheres.push_back(Sphere{ {0,0,0},0.5f,{1,0,1} });//pos,rad,col
		
		Material& purpSphere = m_Scene.Materials.emplace_back();
		purpSphere.Albedo = { 1,0,1 };
		purpSphere.Roughness = 0;
		Material& greenSphere = m_Scene.Materials.emplace_back();
		greenSphere.Albedo = { 0.1f,0.9f,0.1f };
		greenSphere.Roughness = 0.1f;
		{
			Sphere b;
			b.Position = { 0, 0, 0 };
			b.Radius = 1;
			b.materialIndex = 0;
			m_Scene.Spheres.push_back(b);
		}

		{
			Sphere a;
			a.Position = { 0, -51, 0 };
			a.Radius = 50;
			a.materialIndex = 1;
			m_Scene.Spheres.push_back(a);
		}


		/*
		{
			Sphere c;
			c.Position = { 0, 0.5f, 0.5f };
			c.Radius = 0.2f;
			c.Albedo = { 0.2f, 1.f, 0.2f };
			m_Scene.Spheres.push_back(c);
		}
		*/
	}
	


	virtual void OnUpdate(float ts) override
	{
		if (m_Camera.OnUpdate(ts))
		{
			renderer.ResetFrameIndex();//auto resetting frame index when moved
		}
	}

	virtual void OnUIRender() override
	{
		ImGui::Begin("Settings");
		ImGui::Text("Last render: %.3fms", m_LastRenderTime);
		ImGui::Text("FPS: %.1f", m_FPS);
		ImGui::Text("Multithreading Randomness: MAKE SURE THE RANDOM FUNCTION IN THE PERPIXEL FUNCTION IS MULTITHREADED,");
		ImGui::Text("src > Renderer.cpp > Renderer::PerPixel() > Walnut::Random / CTRL + LCLICK Random & enter the random files (CPP & H)");
		ImGui::Text("prefix 'thread_local' to RandomEngine in both the random cpp & h files/ MOCK RANDOM CPP & H FILES ARE IN THE SRC FILES COPY THEM");

		if (ImGui::Button("Render"))
		{
			Render();
		}

		ImGui::Checkbox("Accumulate", &renderer.GetSettings().Accumulate);

		if (ImGui::Button("Reset"))
		{
			renderer.ResetFrameIndex();
		}

		ImGui::End();

		ImGui::Begin("Scene");
		for (size_t i = 0; i < m_Scene.Spheres.size(); i++)
		{
			ImGui::PushID(i);

			Sphere& sphere = m_Scene.Spheres[i];
			ImGui::DragFloat3("Position",
				glm::value_ptr(sphere.Position), 0.1f);
			ImGui::DragFloat("Radius",
				&sphere.Radius, 0.1f);
			ImGui::DragInt("Material", &sphere.materialIndex,
				1.0f, 0, (int)m_Scene.Materials.size()-1);

			ImGui::Separator();

			ImGui::PopID();
		}

		for (size_t i = 0; i < m_Scene.Materials.size(); i++)
		{
			ImGui::PushID(i);

			Material& material = m_Scene.Materials[i];

			ImGui::ColorEdit3("Albedo",
				glm::value_ptr(material.Albedo), 0.1f);
			ImGui::DragFloat("Roughness",
				&material.Roughness, 0.05f,0.0f,1.0f);
			ImGui::DragFloat("Metallic",
				&material.Metallic, 0.05f, 0.0f, 1.0f);

			ImGui::Separator();

			ImGui::PopID();
		}
		ImGui::End();

		//ImGui::ShowDemoWindow();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
		ImGui::Begin("Viewport");
		
		m_ViewportWidth = ImGui::GetContentRegionAvail().x;
		m_ViewportHeight = ImGui::GetContentRegionAvail().y;
		
		auto image = renderer.GetFinalImage();
		if (image)
		{
			ImGui::Image(image->GetDescriptorSet(), {
						(float)image->GetWidth(),
						(float)image->GetHeight()
			}, 
			ImVec2(0,1), ImVec2(1,0));
		}


		ImGui::End();
		ImGui::PopStyleVar();

		Render();
	}

	void Render()
	{

		Timer timer;

		//Call resize first then the renderer render function
		renderer.OnResize(m_ViewportWidth, m_ViewportHeight);
		m_Camera.OnResize(m_ViewportWidth, m_ViewportHeight);
		renderer.Render(m_Scene,m_Camera);

		m_LastRenderTime = timer.ElapsedMillis();
		m_FPS = 1 / (m_LastRenderTime / 1000);
	}

private:
	Camera m_Camera;

	Renderer renderer;

	Scene m_Scene;

	uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;



	float m_LastRenderTime = 0;
	float m_FPS = 0;
};

Walnut::Application* Walnut::CreateApplication(int argc, char** argv)
{
	Walnut::ApplicationSpecification spec;
	spec.Name = "Juma's First Raytracing Project";

	Walnut::Application* app = new Walnut::Application(spec);
	app->PushLayer<ExampleLayer>();
	app->SetMenubarCallback([app]()
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Exit"))
			{
				app->Close();
			}
			ImGui::EndMenu();
		}
	});
	return app;
}