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
		{
			Sphere b;
			b.Position = { 0, 0.5f, 0 };
			b.Radius = 0.3f;
			b.Albedo = { 0, 0.5f, 1.f };
			m_Scene.Spheres.push_back(b);
		}

		{
			Sphere a;
			a.Position = { 0, 0.0f, 0 };
			a.Radius = 0.5f;
			a.Albedo = { 1.f, 0.5f, 0.f };
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
		m_Camera.OnUpdate(ts);
	}

	virtual void OnUIRender() override
	{
		ImGui::Begin("Settings");
		ImGui::Text("Last render: %.3fms", m_LastRenderTime);
		ImGui::Text("FPS: %.1f", m_FPS);
		if (ImGui::Button("Render"))
		{
			Render();
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
			ImGui::ColorEdit3("Albedo",
				glm::value_ptr(sphere.Albedo), 0.1f);

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