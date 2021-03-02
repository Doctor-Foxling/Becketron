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
	{
		BT_PROFILE_SCOPE("Renderer Prep");
		Becketron::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Becketron::RenderCommand::Clear();
	}

	{

		BT_PROFILE_SCOPE("Renderer Draw");
		Becketron::Renderer2D::BeginScene(m_CameraController.GetCamera());
		Becketron::Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.8f, 1.0f }, m_SquareColor);
		Becketron::Renderer2D::DrawQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, { 0.2f, 0.3f, 0.2f, 1.0f });
		Becketron::Renderer2D::DrawQuad({ 0.5f, 0.0f, -0.1f }, { 10.0f, 10.0f }, m_CheckerboardTexture);
		Becketron::Renderer::EndScene();
	}

	// TODO:: Add these funs - Shader::SetMat4, Shader::SetFloat4
	// std::dynamic_pointer_cast<Becketron::OpenGLShader>(m_FlatColorShader)->Bind();
	// std::dynamic_pointer_cast<Becketron::OpenGLShader>(m_FlatColorShader)->UploadUniformFloat4("u_Color", m_SquareColor);
}

void Sandbox2D::OnImGuiRender()
{
	BT_PROFILE_FUNCTION();

	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));

	ImGui::End();
}

void Sandbox2D::OnEvent(Becketron::Event& e)
{
	m_CameraController.OnEvent(e);
}