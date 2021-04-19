#include "Sandbox3D.h"
#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//#include "Becketron/Physics/PhysicsEngine.h"
//#include <PhysX/include/PhysX/PxPhysicsAPI.h>

//#include "Becketron/Physics/PhysX/PhysXScene.h"



Sandbox3D::Sandbox3D()
	: Layer("Sandbox3D"), m_CameraController(glm::radians(45.0f), 1280.0f / 720.0f, 0.01f, 1000.0f, true)
{
}

void Sandbox3D::OnAttach()
{
	BT_PROFILE_FUNCTION();
	m_CheckerboardTexture = Becketron::Texture2D::Create("assets/textures/Chess_board.jpg");
	//static physx::PxPhysics* phys;

}

void Sandbox3D::OnDetach()
{
	BT_PROFILE_FUNCTION();
}

void Sandbox3D::OnUpdate(Becketron::Timestep ts)
{
	BT_PROFILE_FUNCTION();

	// Update
	m_CameraController.OnUpdate(ts);

	// Render
	Becketron::Renderer3D::ResetStats();
	{
		BT_PROFILE_SCOPE("Renderer Prep");
		Becketron::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.5f, 1 });
		Becketron::RenderCommand::Clear();
	}

	std::vector<std::string> faces
	{
		"assets/textures/skybox2/right.png",
		"assets/textures/skybox2/left.png",
		"assets/textures/skybox2/top.png",
		"assets/textures/skybox2/bottom.png",
		"assets/textures/skybox2/front.png",
		"assets/textures/skybox2/back.png"
	};

	Becketron::Cubemap::RenderSkybox(m_CameraController.GetCamera());


	{
		static float rotation = 0.0f;
		rotation += ts * 50.0f;

		BT_PROFILE_SCOPE("Renderer Draw");

		Becketron::Renderer3D::BeginScene(m_CameraController.GetCamera());
		//Becketron::Renderer3D::DrawRotatedQuad({ -1.0f, 0.0f }, { 0.8f, 1.0f }, -45.0f, m_SquareColor);
		//m_CameraController.GetCamera().SetPosition(m_camPos);

		//Becketron::Renderer3D::DrawQuad({ -1.0f, 0.0f }, { 0.8f, 1.0f }, m_SquareColor);
		Becketron::Renderer3D::DrawCube({ 0.5f, -0.5f, -5.0f }, { 0.5f, 0.5f, 0.5f }, m_SquareColor);

		glm::vec3 Rotation = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::mat4 rotation_mat = glm::rotate(glm::mat4(1.0f), Rotation.x, { 1, 0, 0 })
			* glm::rotate(glm::mat4(1.0f), Rotation.y, { 0, 1, 0 })
			* glm::rotate(glm::mat4(1.0f), Rotation.z, { 0, 0, 1 });

		glm::vec3 Translation = glm::vec3(0.5f, 0.7f, -5.0f);
		glm::vec3 Scale = glm::vec3(1.0f);

		glm::mat4 Transform = glm::translate(glm::mat4(1.0f), Translation)
			* rotation_mat
			* glm::scale(glm::mat4(1.0f), Scale);

		Becketron::Renderer3D::DrawCube(Transform, m_CheckerboardTexture, 1.0f);

		//Becketron::Renderer3D::DrawCube({ 0.5f, 0.7f, -5.0f }, { 1.0f, 1.0f, 1.0f }, m_CheckerboardTexture, 1.0f);


		//Becketron::Renderer2D::EndScene();

		//Becketron::Renderer2D::BeginScene(m_CameraController.GetCamera());
		/*for (float y = -5.0f; y < 5.0f; y += 0.5f)
		{
			for (float x = -5.0f; x < 5.0f; x += 0.5f)
			{
				glm::vec4 color = { (x + 5.0f) / 10.0f, 0.4f, (y + 5.0f) / 10.0f, 0.7f };
				Becketron::Renderer2D::DrawQuad({ x, y }, { 0.45f, 0.45f }, color);
			}
		}*/

		Becketron::Renderer3D::EndScene();
		
		Becketron::Renderer3D::ShowLightCube(m_CameraController.GetCamera());

		Becketron::Renderer2D::BeginScene(m_CameraController.GetCamera());
		Becketron::Renderer2D::DrawQuad({ -0.75f, -0.25f, -7.0f }, { 0.5f, 0.5f, 5.0f }, m_SquareColor);
		Becketron::Renderer2D::DrawRotatedQuad({ 0.0f, -0.8f, -4.0f }, { 1.0f, 1.0f, 10.f }, rotation, m_CheckerboardTexture, 0.5f, Becketron::BT_axis::x);

		//m_lightColor = Becketron::Renderer3D::GetDataByRef();

		Becketron::Renderer2D::EndScene();


	}
	// TODO:: Add these funs - Shader::SetMat4, Shader::SetFloat4
	// std::dynamic_pointer_cast<Becketron::OpenGLShader>(m_FlatColorShader)->Bind();
	// std::dynamic_pointer_cast<Becketron::OpenGLShader>(m_FlatColorShader)->UploadUniformFloat4("u_Color", m_SquareColor);
}

void Sandbox3D::OnImGuiRender()
{
	BT_PROFILE_FUNCTION();

	ImGui::Begin("Settings");

	auto stats = Becketron::Renderer3D::GetStats();
	ImGui::Text("Renderer2D Stats:");
	ImGui::Text("Draw Calls: %d", stats.DrawCalls);
	ImGui::Text("Cubess: %d", stats.CubeCount);
	ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
	ImGui::Text("Indices: %d", stats.GetTotalIndexCount());
	ImGui::SliderFloat3("Cam Position %d", &m_camPos.x, 30.0f, -30.0f);

	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));

	uint32_t textureID = m_CheckerboardTexture->GetRendererID();
	ImGui::Image((void*)textureID, ImVec2{ 56.0f, 56.0f });
	ImGui::End();
}

void Sandbox3D::OnEvent(Becketron::Event& e)
{
	m_CameraController.OnEvent(e);
}