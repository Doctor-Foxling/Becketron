#include "btpch.h"
#include "Application.h"

#include "Becketron/Core/Log.h"
#include "Becketron/Core/Input.h"

#include "Becketron/Renderer/Renderer.h"
#include "Becketron/Physics/PhysX/PhysXManager.h"

#include "Becketron/Core/KeyCodes.h"

#include  <GLFW/glfw3.h>

namespace Becketron {


	Application* Application::s_Instance = nullptr;

	Application::Application(const std::string& name)
	{
		BT_PROFILE_FUNCTION();

		BT_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this; // setting to the current instance of application

		/*m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));*/

		m_Window = Window::Create(WindowProps(name));
		m_Window->SetEventCallback(BT_BIND_EVENT_FN(Application::OnEvent));

		// Initializing the Renderer
		Renderer::Init();
		PhysXManager::Init();

		// Creating the main GUI layer and pushing it as an overlay
		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);
	}

	Application::~Application()
	{
		BT_PROFILE_FUNCTION();

		PhysXManager::Shutdown();
		//Renderer::Shutdown();
	}

	void Application::PushLayer(Layer* layer)
	{
		BT_PROFILE_FUNCTION();

		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PopLayer(Layer* layer)
	{
		BT_PROFILE_FUNCTION();

		m_LayerStack.PopLayer(layer);
	}

	void Application::PushOverlay(Layer* layer)
	{
		BT_PROFILE_FUNCTION();

		m_LayerStack.PushOverlay(layer);
		layer->OnAttach();
	}

	void Application::PopOverlay(Layer* layer)
	{
		BT_PROFILE_FUNCTION();

		m_LayerStack.PopOverlay(layer);
	}

	void Application::Close()
	{
		m_Running = false;
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
			if (e.Handled)
				break;
			(*--it)->OnEvent(e);
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

			/* Creating a copy because the using the erase function (see layerStack) on a given vector 
			   invalidates the iterators, so using the vetor object directly to iterate over doesn't work */ 
			std::vector<Layer*>& localLayers = m_LayerStack.getLayers();
			size_t stackSize = localLayers.size();

			if (!m_Minimized)
			{
				{
					BT_PROFILE_SCOPE("LayerStack - OnUpdate");
					int layerCount = 0;
					//for (Layer* layer : m_LayerStack)
					for (Layer* layer : localLayers)
					{
						layerCount++;
						if(layerCount < stackSize)
							layer->OnUpdate(timestep); 
					}
				}

				m_ImGuiLayer->Begin();
				{
					BT_PROFILE_SCOPE("LayerStack OnImGuiRender");

					//for (Layer* layer : m_LayerStack)
					for (Layer* layer : localLayers)
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
