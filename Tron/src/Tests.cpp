#include "Tests.h"
#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/type_ptr.hpp>

// coolision testing
#include "Becketron/Physics/BT_Physics/BoundingSphere.h"
#include "Becketron/Physics/BT_Physics/aabb.h"
#include "Becketron/Physics/BT_Physics/PhysicsObject.h"
#include "Becketron/Physics/BT_Physics/PhysicsEngine.h"

#include "PlayerScript.h"
#include "RigidbodyController.h"

// Temporary??
#include "Becketron/Renderer/Texture.h"
#include "examples/example1.h"
#include "examples/example2.h"

namespace Becketron {

	Tests::Tests()
		: Layer("Tests"), m_CameraController(1280.0f / 720.0f)
	{
	}

	void Tests::OnAttach()
	{
		m_CurrentLayer = new ExampleLayer1;
		Application::Get().PushLayer(m_CurrentLayer);
	}

	void Tests::OnDetach()
	{
		BT_PROFILE_FUNCTION();
	}

	void Tests::OnUpdate(Timestep ts)
	{
		
	}

	void Tests::OnImGuiRender()
	{
		BT_PROFILE_FUNCTION();
		
			static bool dockspaceOpen = true;
			static bool opt_fullscreen_persistant = true;
			bool opt_fullscreen = opt_fullscreen_persistant;
			static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

			// Using the ImGUiWindowFLags_NoDocking falg to make the parent window not dockable into,
			// since it could be confusing to have two docking targets within each others.
			ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
			if (opt_fullscreen)
			{
				ImGuiViewport* viewport = ImGui::GetMainViewport();
				ImGui::SetNextWindowPos(viewport->Pos);
				ImGui::SetNextWindowSize(viewport->Size);
				ImGui::SetNextWindowViewport(viewport->ID);
				ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
				ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
				window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
				window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
			}

			// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
			if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
				window_flags |= ImGuiWindowFlags_NoBackground;

			// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
			// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
			// all active  windows docked into it will lose their parent become undocked.
			// We cannot preserve the docking relatioshoip between an active window and an inactive docking, otherwise
			// any change of dockspace/setting would lead to widows being stuck in limbo and never being visible
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
			ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
			ImGui::PopStyleVar();

			if (opt_fullscreen)
				ImGui::PopStyleVar(2);

			// Dockspace
			ImGuiIO& io = ImGui::GetIO();
			if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
			{
				ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
				ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
			}

			if (ImGui::BeginMenuBar())
			{
				if (ImGui::BeginMenu("File"))
				{
					// Disabling fullscreen would allow the window to be moved to the front of other windows,
					// whcihc we can't undo at the moment without finer window depth/z control.
					// ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen_presistant);

					if (ImGui::MenuItem("Exit")) Application::Get().Close();
					ImGui::EndMenu();
				}

				ImGui::EndMenuBar();
			}

			/*
			*/
			ImGui::Begin("Stats");

			auto stats = Renderer2D::GetStats();
			ImGui::Text("Renderer2D Stats:");
			ImGui::Text("Draw Calls: %d", stats.DrawCalls);
			ImGui::Text("Quads: %d", stats.QuadCount);
			ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
			ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

			if (ImGui::TreeNode("Examples"))
			{
				static int selected = -1;

				if (m_LayerNumber != 1 && ImGui::Selectable("Example 1", true))
				{
					Application::Get().PopLayer(m_CurrentLayer);
					delete m_CurrentLayer;
					m_CurrentLayer = new ExampleLayer1;
					Application::Get().PushLayer(m_CurrentLayer);
					m_LayerNumber = 1;
				}

				if (m_LayerNumber != 2 && ImGui::Selectable("Example 2", true))
				{
					Application::Get().PopLayer(m_CurrentLayer);
					delete m_CurrentLayer;
					m_CurrentLayer = new ExampleLayer2;
					Application::Get().PushLayer(m_CurrentLayer);
					m_LayerNumber = 2;
				}
			
				ImGui::TreePop();
			}

			ImGui::End();

			ImGui::End();

			//ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
			
	}

	void Tests::OnEvent(Event& e)
	{
		m_CameraController.OnEvent(e);
	}
}