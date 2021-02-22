#pragma once

#include "Core.h"

#include "Becketron/Core/Timestep.h"

#include "Window.h"
#include "Becketron/LayerStack.h"
#include "Becketron/Events/Event.h"
#include "Becketron/Events/ApplicationEvent.h"

#include "Becketron/ImGui/ImGuiLayer.h"

#include "Becketron/Renderer/Shader.h"
#include "Becketron/Renderer/Buffer.h"
#include "Becketron/Renderer/VertexArray.h"

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

	private:
		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		LayerStack m_LayerStack;
		float m_LastFrameTime = 0.0f;
	private:
		static Application* s_Instance;
	};

	// To be defined in client
	Application* CreateApplication();

}