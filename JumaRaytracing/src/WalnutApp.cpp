#include "Walnut/Application.h"
#include "Walnut/EntryPoint.h"

#include "Walnut/Image.h"
#include "Walnut/Timer.h"

#include "Renderer.h"
#include "Camera.h"

using namespace Walnut;

class ExampleLayer : public Walnut::Layer
{
public:
	ExampleLayer() : m_Camera(45.0f, 1, 100) {}
	

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
		renderer.Render(m_Camera);

		m_LastRenderTime = timer.ElapsedMillis();
		m_FPS = 1 / (m_LastRenderTime / 1000);
	}

private:
	Camera m_Camera;

	Renderer renderer;

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