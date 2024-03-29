#include "EditorLayer.h"
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

namespace Becketron {

	EditorLayer::EditorLayer()
		: Layer("EditorLayer"), m_CameraController(1280.0f / 720.0f)
	{
	}

	void EditorLayer::OnAttach()
	{
		//Renderer3D::Init();
		BT_PROFILE_FUNCTION();
		m_CheckerboardTexture = Texture2D::Create("assets/textures/Chess_board.jpg");

		FramebufferSpecification fbSpec;
		fbSpec.Width = 1280;
		fbSpec.Height = 720;
		m_Framebuffer = Framebuffer::Create(fbSpec);

		m_ActiveScene = CreateRef<Scene>();

		// Entity
		auto square = m_ActiveScene->CreateEntity("Ground");
		square.AddComponent<SpriteRendererComponent>(glm::vec4{ 0.0f, 1.0f, 1.0f, 0.5f });
		auto& tc = square.GetComponent<TransformComponent>();
		tc.Translation = { 0.0f, -3.14f, 0.0f };
		tc.Scale = { 50.0f, 50.0f, 0.0f };
		tc.Rotation = { glm::radians(90.0f), 0.0f, 0.0f };

		auto redSquare = m_ActiveScene->CreateEntity("Red Square");
		redSquare.AddComponent<SpriteRendererComponent>(glm::vec4{ 1.0f, 0.0f, 0.0f, 1.0f });

		auto blueCube = m_ActiveScene->CreateEntity("Blue Cube");
		blueCube.AddComponent<CubeRendererComponent>(glm::vec4{ 0.0f, 0.4f, 1.0f, 1.0f });

		auto greenCube = m_ActiveScene->CreateEntity("Green Square");
		greenCube.AddComponent<CubeRendererComponent>(glm::vec4{ 0.0f, 1.0f, 0.4f, 1.0f });
		auto& greenCube_tc = greenCube.GetComponent<TransformComponent>();
		greenCube_tc.Translation = { 2.0f, 10.0f, 0.0f };
		
		greenCube.AddComponent<PhysXRigidDynamicComponent>(0.5, 0.5, 0.5, true);
		

		auto redCube = m_ActiveScene->CreateEntity("Red Cube");
		redCube.AddComponent<CubeRendererComponent>(glm::vec4{ 1.0f, 0.0f, 0.0f, 1.0f });

		auto& red_rigid = redCube.AddComponent<PhysXRigidDynamicComponent>(0.5f, 0.5f, 0.1f, true);
		redCube.AddComponent<NativeScriptComponent>().Bind<RigidbodyController>();
		

		auto mainLight = m_ActiveScene->CreateEntity("Main Light");
		mainLight.AddComponent<LightCubeComponent>(glm::vec4{ 1.0f, 1.0f, 1.0f, 1.0f });
		auto& mainLight_tc = mainLight.GetComponent<TransformComponent>();
		mainLight_tc.Translation = { 5.0f, 4.0f, 21.0f };

		//Ref<Texture2D> checkerboardTex = Texture2D::Create("assets/textures/Chess_board.jpg");
		
		for (int i = 1; i < 20; i++)
		{
			if (i / 5 == 0)
			{
			auto texCube = m_ActiveScene->CreateEntity("Tex Cube"+i);
			texCube.AddComponent<TexturedCubeComponent>(m_CheckerboardTexture, 1.0f, glm::vec4{ 1.0f, (1.0f+i)/i, 1.0f, 1.0f });
			auto& texCube_tc = texCube.GetComponent<TransformComponent>();
			texCube_tc.Translation = { (2.0f+i+i+i+i)/i, 10.0f+ 5.0f * i, 0.0f };
			texCube_tc.Scale = { 1.0f+i, (2.0f+i)/i, 3.0f };

			texCube.AddComponent<PhysXRigidDynamicComponent>(0.5f, 0.5f, 0.1f, true);
			}
		}

		m_SquareEntity = square;

		m_CameraEntity = m_ActiveScene->CreateEntity("Camera A");
		auto& cameraA = m_CameraEntity.AddComponent<CameraComponent>();
		auto& camA_tc = m_CameraEntity.GetComponent<TransformComponent>();
		camA_tc.Translation = { 0.0f, 5.0f, 40.0f };

		m_SecondCamera = m_ActiveScene->CreateEntity("Camera B");
		auto& cc = m_SecondCamera.AddComponent<CameraComponent>();
		cc.Primary = false;

		class CameraController : public ScriptableEntity
		{
		public:
			virtual void OnCreate() override
			{
				//auto& translation = GetComponent<TransformComponent>().Translation;
				//translation.x = rand() % 10 - 5.0f;
			}

			virtual void OnDestroy() override
			{
			}

			virtual void OnUpdate(Timestep ts) override
			{
				auto& translation = GetComponent<TransformComponent>().Translation;
				float speed = 5.0f;

				if (Input::IsKeyPressed(BT_KEY_A))
					translation.x -= speed * ts;
				if (Input::IsKeyPressed(BT_KEY_D))
					translation.x += speed * ts;
				if (Input::IsKeyPressed(BT_KEY_W))
					translation.y += speed * ts;
				if (Input::IsKeyPressed(BT_KEY_S))
					translation.y -= speed * ts;

			}
		};

		//class RigidBody : public PhysicsEntity
		//{
		//public:
		//	RigidBody(PhysicsObject phyObj)
		//		: m_physicsObject(phyObj)
		//	{}

		//	virtual void OnCreate() override
		//	{

		//	}

		//	virtual void OnDestroy() override
		//	{

		//	}

		//	virtual void OnUpdate(Timestep ts) override
		//	{
		//		m_physicsObject.Integrate(ts);
		//		auto& translation = GetComponent<TransformComponent>().Translation;
		//		auto& scale = GetComponent<TransformComponent>().Scale;

		//		//BT_TRACE("Rigid-body position x: {0}", translation.x);
		//		translation = m_physicsObject.GetPosition();
		//		float cubeSide = m_physicsObject.GetRadius() * glm::root_two<float>();
		//		scale = glm::vec3(cubeSide);
		//	}
		//private:
		//	PhysicsObject m_physicsObject;
		//};

		m_CameraEntity.AddComponent<NativeScriptComponent>().Bind<CameraController>();
		m_SecondCamera.AddComponent<NativeScriptComponent>().Bind<CameraController>();

		m_SceneHierarchyPanel.SetContext(m_ActiveScene);

		// Random Bounding SPhere test
	//	BoundingSphere::Test();
	}

	void EditorLayer::OnDetach()
	{
		BT_PROFILE_FUNCTION();
	}

	void EditorLayer::OnUpdate(Timestep ts)
	{
		BT_PROFILE_FUNCTION();

		// Resize 
		if (Becketron::FramebufferSpecification spec = m_Framebuffer->GetSpecification();
			m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f && // zero sized frambuffer is invalid
			(spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y))
		{
			m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			m_CameraController.OnResize(m_ViewportSize.x, m_ViewportSize.y);

			m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		}


		// Update
		if (m_ViewportFocused)
			m_CameraController.OnUpdate(ts);

		// Render
		Renderer2D::ResetStats();
		m_Framebuffer->Bind();
		RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		RenderCommand::Clear();

		// Update scene
		m_ActiveScene->OnUpdate(ts);

		m_Framebuffer->Unbind();

		// TODO:: Add these funs - Shader::SetMat4, Shader::SetFloat4
		// std::dynamic_pointer_cast<OpenGLShader>(m_FlatColorShader)->Bind();
		// std::dynamic_pointer_cast<OpenGLShader>(m_FlatColorShader)->UploadUniformFloat4("u_Color", m_SquareColor);
	}

	void EditorLayer::OnImGuiRender()
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

			m_SceneHierarchyPanel.OnImGuiRender();

			ImGui::Begin("Stats");

			auto stats = Renderer2D::GetStats();
			ImGui::Text("Renderer2D Stats:");
			ImGui::Text("Draw Calls: %d", stats.DrawCalls);
			ImGui::Text("Quads: %d", stats.QuadCount);
			ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
			ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

			if (ImGui::TreeNode("Selection State: Single Selection"))
			{
				static int selected = -1;
				for (int n = 0; n < 5; n++)
				{
					char buf[32];
					sprintf(buf, "Object %d", n);
					if (ImGui::Selectable(buf, selected == n))
					{
						Application::Get().PushOverlay(new ExampleLayer1);
						selected = n;
					}
				}
				ImGui::TreePop();
			}

			ImGui::End();

			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
			ImGui::Begin("Viewport");

			m_ViewportFocused = ImGui::IsWindowFocused();
			m_ViewportHovered = ImGui::IsWindowHovered();
			Application::Get().GetImGuiLayer()->BlockEvents(!m_ViewportFocused || !m_ViewportHovered);

			ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
			
			m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

			uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
			ImGui::Image((void*)textureID, ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
			ImGui::End();
			ImGui::PopStyleVar();

			ImGui::End();
	}

	void EditorLayer::OnEvent(Event& e)
	{
		m_CameraController.OnEvent(e);
	}
}