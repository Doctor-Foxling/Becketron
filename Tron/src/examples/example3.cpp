#include "example3.h"
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
#include "CameraControllerScript.h"
#include "RigidbodyController.h"

// Temporary??
#include "Becketron/Renderer/Texture.h"

namespace Becketron {

	ExampleLayer3::ExampleLayer3()
		: Layer("ExampleLayer3"), m_CameraController(1280.0f / 720.0f)
	{
	}

	void ExampleLayer3::OnAttach()
	{
		//Renderer3D::Init();
		BT_PROFILE_FUNCTION();
		m_CheckerboardTexture = Texture2D::Create("assets/textures/Chess_board.jpg");

		FramebufferSpecification fbSpec;
		fbSpec.Width = 1280;
		fbSpec.Height = 720;
		m_Framebuffer = Framebuffer::Create(fbSpec);

		m_ActiveScene = CreateRef<Scene>();


		std::vector<std::string> faces
		{
			"assets/textures/skybox7/right.png",
			"assets/textures/skybox7/left.png",
			"assets/textures/skybox7/top.png",
			"assets/textures/skybox7/bottom.png",
			"assets/textures/skybox7/front.png",
			"assets/textures/skybox7/back.png"
		};


		auto cubeMap = m_ActiveScene->CreateEntity("Cubemap");
		cubeMap.AddComponent<CubemapComponent>(faces, true);

		// Entity
		auto square = m_ActiveScene->CreateEntity("Ground");
		square.AddComponent<SpriteRendererComponent>(glm::vec4{ 1.0f, 0.0f, 1.0f, 0.5f });
		auto& tc = square.GetComponent<TransformComponent>();
		tc.Translation = { 0.0f, -3.14f, 0.0f };
		tc.Scale = { 50.0f, 50.0f, 0.0f };
		tc.Rotation = { glm::radians(90.0f), 0.0f, 0.0f };

		auto redSquare = m_ActiveScene->CreateEntity("Red Square");
		redSquare.AddComponent<SpriteRendererComponent>(glm::vec4{ 1.0f, 0.5f, 0.5f, 1.0f });

		auto blueCube = m_ActiveScene->CreateEntity("Blue Cube");
		blueCube.AddComponent<CubeRendererComponent>(glm::vec4{ 0.4f, 0.4f, 1.0f, 1.0f });

		auto greenCube = m_ActiveScene->CreateEntity("Green Square");
		greenCube.AddComponent<CubeRendererComponent>(glm::vec4{ 1.0f, 0.5f, 0.4f, 1.0f });
		auto& greenCube_tc = greenCube.GetComponent<TransformComponent>();
		greenCube_tc.Translation = { 2.0f, 10.0f, 0.0f };
		
		greenCube.AddComponent<PhysXRigidDynamicComponent>(0.5, 0.5, 0.5, true);
		

		auto redCube = m_ActiveScene->CreateEntity("Red Cube");
		redCube.AddComponent<CubeRendererComponent>(glm::vec4{ 0.2f, 0.2f, 0.2f, 1.0f });

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

	void ExampleLayer3::OnDetach()
	{
		BT_PROFILE_FUNCTION();
	}

	void ExampleLayer3::OnUpdate(Timestep ts)
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

	void ExampleLayer3::OnImGuiRender()
	{
		m_SceneHierarchyPanel.OnImGuiRender();

		ImGui::Begin("Viewport");

		m_ViewportFocused = ImGui::IsWindowFocused();
		m_ViewportHovered = ImGui::IsWindowHovered();
		Application::Get().GetImGuiLayer()->BlockEvents(!m_ViewportFocused || !m_ViewportHovered);

		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();

		m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

		uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
		ImGui::Image((void*)textureID, ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
		ImGui::End();
		//ImGui::PopStyleVar();
	}

	void ExampleLayer3::OnEvent(Event& e)
	{
		m_CameraController.OnEvent(e);
	}
}