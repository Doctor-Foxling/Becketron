#include "Sandbox2D.h"
#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>




Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f)
{
}

void Sandbox2D::OnAttach()
{
	BT_PROFILE_FUNCTION();
	m_CheckerboardTexture = Becketron::Texture2D::Create("assets/textures/Chess_board.jpg");

	
}

void Sandbox2D::OnDetach()
{
	BT_PROFILE_FUNCTION();
}

void Sandbox2D::OnUpdate(Becketron::Timestep ts)
{
	BT_PROFILE_FUNCTION();

	// Update
		m_CameraController.OnUpdate(ts);

	// Render
		Becketron::Renderer2D::ResetStats();
	{
		BT_PROFILE_SCOPE("Renderer Prep");
		Becketron::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Becketron::RenderCommand::Clear();
	}

	{
		static float rotation = 0.0f;
		rotation += ts * 50.0f;

		BT_PROFILE_SCOPE("Renderer Draw");
		Becketron::Renderer2D::BeginScene(m_CameraController.GetCamera());
		Becketron::Renderer2D::DrawRotatedQuad({ -1.0f, 0.0f }, { 0.8f, 1.0f }, -45.0f , m_SquareColor);
		Becketron::Renderer2D::DrawQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, { 0.2f, 0.3f, 0.2f, 1.0f });
		Becketron::Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.8f, 1.0f }, m_SquareColor);
		Becketron::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 20.0f, 20.0f }, m_CheckerboardTexture, 10.0f);
		Becketron::Renderer2D::DrawRotatedQuad({ 0.0f, -0.8f, 0.0f }, { 1.0f, 1.0f }, rotation, m_CheckerboardTexture, 0.5f);
		//Becketron::Renderer2D::EndScene();

		//Becketron::Renderer2D::BeginScene(m_CameraController.GetCamera());
		for (float y = -5.0f; y < 5.0f; y += 0.5f)
		{
			for (float x = -5.0f; x < 5.0f; x += 0.5f)
			{
				glm::vec4 color = { (x + 5.0f) / 10.0f, 0.4f, (y + 5.0f) / 10.0f, 0.7f };
				Becketron::Renderer2D::DrawQuad({ x, y }, { 0.45f, 0.45f }, color);
			}
		}
		Becketron::Renderer2D::EndScene();
	}

	// TODO:: Add these funs - Shader::SetMat4, Shader::SetFloat4
	// std::dynamic_pointer_cast<Becketron::OpenGLShader>(m_FlatColorShader)->Bind();
	// std::dynamic_pointer_cast<Becketron::OpenGLShader>(m_FlatColorShader)->UploadUniformFloat4("u_Color", m_SquareColor);
}

void Sandbox2D::OnImGuiRender()
{
	BT_PROFILE_FUNCTION();

	ImGui::Begin("Settings");

	auto stats = Becketron::Renderer2D::GetStats();
	ImGui::Text("Renderer2D Stats:");
	ImGui::Text("Draw Calls: %d", stats.DrawCalls);
	ImGui::Text("Quads: %d", stats.QuadCount);
	ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
	ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));

	uint32_t textureID = m_CheckerboardTexture->GetRendererID();
	ImGui::Image((void*)textureID, ImVec2{ 56.0f, 56.0f });
	ImGui::End();
}

void Sandbox2D::OnEvent(Becketron::Event& e)
{
	m_CameraController.OnEvent(e);
}