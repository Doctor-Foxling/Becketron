#pragma once

#include "Core.h"

#include "Window.h"
#include "Becketron/LayerStack.h"
#include "Becketron/Events/Event.h"
#include "Becketron/Events/ApplicationEvent.h"

#include "Becketron/ImGui/ImGuiLayer.h"

#include "Becketron/Renderer/Shader.h"

namespace Becketron
{
	class BTRON_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		inline static Application& Get() { return *s_Instance; }
		inline Window& GetWindow() { return *m_Window; }
	private:
		bool OnWindowClose(WindowCloseEvent& e);

		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		LayerStack m_LayerStack;

		unsigned int m_VertexArray, m_VertexBuffer, m_IndexBuffer;
		std::unique_ptr<Shader> m_Shader;
		
	private:
		static Application* s_Instance;
	};

	// To be defined in client
	Application* CreateApplication();

}