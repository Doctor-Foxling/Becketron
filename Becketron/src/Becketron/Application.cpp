#include "btpch.h"
#include "Application.h"

#include "Becketron/Log.h"
#include "Becketron/Input.h"

#include "Becketron/Renderer/Renderer.h"

#include "Becketron/KeyCodes.h"

#include  <GLFW/glfw3.h>

namespace Becketron {

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		BT_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);

		
	}

	Application::~Application()
	{
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* layer)
	{
		m_LayerStack.PushOverlay(layer);
		layer->OnAttach();
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));

		//dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT_FN(OnKeyEvent));

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); )
		{
			(*--it)->OnEvent(e);
			if (e.Handled)
				break;
		}
	}
	
	void Application::Run()
	{
		while (m_Running)
		{
			float time = (float)glfwGetTime();
			// we can just assign it to float like that because
			// we have a constructor that takes a float, (implicit cast)
			Timestep timestep = time - m_LastFrameTime;
			m_LastFrameTime = time;

			for (Layer* layer : m_LayerStack)
				layer->OnUpdate(timestep); 

			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack)
				layer->OnImGuiRender();
			m_ImGuiLayer->End();

			m_Window->OnUpdate();
		}

		//glfwTerminate();
	}

	/*bool Application::OnKeyEvent(KeyPressedEvent& e)
	{
		if (Input::IsKeyPressed(BT_KEY_UP))
		{
			m_camPos.y += 0.1f; return true;
		}
		else if (Input::IsKeyPressed(BT_KEY_DOWN))
		{
			m_camPos.y -= 0.1f;	return true;
		}
		else if (Input::IsKeyPressed(BT_KEY_LEFT))
		{
			m_camPos.x -= 0.1f; return true;
		}
		else if (Input::IsKeyPressed(BT_KEY_RIGHT))
		{
			m_camPos.x += 0.1f; return true;
		}
		
		return false;
	}*/

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}
}
