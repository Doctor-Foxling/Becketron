#include "btpch.h"
#include "Application.h"

#include "Becketron/Core/Log.h"
#include "Becketron/Core/Input.h"

#include "Becketron/Renderer/Renderer.h"

#include "Becketron/Core/KeyCodes.h"

#include  <GLFW/glfw3.h>

namespace Becketron {


	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		BT_PROFILE_FUNCTION();

		BT_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		/*m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));*/

		m_Window = Window::Create();
		m_Window->SetEventCallback(BT_BIND_EVENT_FN(Application::OnEvent));

		Renderer::Init();

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);

		
	}

	Application::~Application()
	{
		BT_PROFILE_FUNCTION();

		//Renderer::Shutdown();
	}

	void Application::PushLayer(Layer* layer)
	{
		BT_PROFILE_FUNCTION();

		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* layer)
	{
		BT_PROFILE_FUNCTION();

		m_LayerStack.PushOverlay(layer);
		layer->OnAttach();
	}

	void Application::OnEvent(Event& e)
	{
		BT_PROFILE_FUNCTION();

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BT_BIND_EVENT_FN(Application::OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(BT_BIND_EVENT_FN(Application::OnWindowResize));

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
		BT_PROFILE_FUNCTION();

		while (m_Running)
		{
			BT_PROFILE_SCOPE("RunLoop");

			float time = (float)glfwGetTime();
			// we can just assign it to float like that because
			// we have a constructor that takes a float, (implicit cast)
			Timestep timestep = time - m_LastFrameTime;
			m_LastFrameTime = time;

			if (!m_Minimized)
			{
				{
					BT_PROFILE_SCOPE("LayerStack - OnUpdate");

					for (Layer* layer : m_LayerStack)
						layer->OnUpdate(timestep); 
				}

				m_ImGuiLayer->Begin();
				{
					BT_PROFILE_SCOPE("LayerStack OnImGuiRender");

					for (Layer* layer : m_LayerStack)
						layer->OnImGuiRender();
				}
				m_ImGuiLayer->End();
			}
			m_Window->OnUpdate();
		}

		//glfwTerminate();
	}

	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		BT_PROFILE_FUNCTION();

		if (e.GetWidth() == 0 || e.GetHeight() == 0)
		{
			m_Minimized = true;
			return false;
		}

		m_Minimized = false;
		Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());

		return false;
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}
}
