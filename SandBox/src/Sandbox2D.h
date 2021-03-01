#pragma once

#include "Becketron.h"

class Sandbox2D : public Becketron::Layer
{
public:
	Sandbox2D();
	virtual ~Sandbox2D() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	virtual void OnUpdate(Becketron::Timestep ts) override;
	virtual void OnImGuiRender() override;
	virtual void OnEvent(Becketron::Event& e) override;

private:
	Becketron::OrthographicCameraController m_CameraController;

	// Temp
	Becketron::Ref<Becketron::VertexArray> m_SquareVA;
	Becketron::Ref<Becketron::Shader> m_FlatColorShader;

	Becketron::Ref<Becketron::Texture2D> m_CheckerboardTexture;

	struct ProfileResult
	{
		const char* Name;
		float Time;
	};
	
	std::vector<ProfileResult> m_ProfileResults;

	glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };
};