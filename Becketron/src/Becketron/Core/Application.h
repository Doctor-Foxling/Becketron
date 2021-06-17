#pragma once

#include "Core.h"

#include "Becketron/Core/Timestep.h"

#include "Window.h"
#include "Becketron/Core/LayerStack.h"
#include "Becketron/Events/Event.h"
#include "Becketron/Events/ApplicationEvent.h"

#include "Becketron/ImGui/ImGuiLayer.h"

#include "Becketron/Renderer/Shader.h"
#include "Becketron/Renderer/Buffer.h"
#include "Becketron/Renderer/VertexArray.h"

int main(int argc, char** argv);

namespace Becketron
{
	class BTRON_API Application
	{
	public:
		Application(const std::string& name = "Becketron App");
		virtual ~Application();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PopLayer(Layer* layer);
		void PushOverlay(Layer* layer);
		void PopOverlay(Layer* layer);

		inline Window& GetWindow() { return *m_Window; }

		void Close();

		ImGuiLayer* GetImGuiLayer() { return m_ImGuiLayer; }

		inline static Application& Get() { return *s_Instance; }
	private:
		void Run();
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);

	private:
		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		bool m_Minimized = false;
		LayerStack m_LayerStack;
		float m_LastFrameTime = 0.0f;
	private:
		static Application* s_Instance;
		friend int ::main(int argc, char** argv);  // refer to #188
 	};

	// To be defined in client
	Application* CreateApplication();

}