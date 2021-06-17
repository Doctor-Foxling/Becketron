#pragma once

#include "Becketron.h"

class Sandbox3D : public Becketron::Layer
{
public:
	Sandbox3D();
	virtual ~Sandbox3D() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	virtual void OnUpdate(Becketron::Timestep ts) override;
	virtual void OnImGuiRender() override;
	virtual void OnEvent(Becketron::Event& e) override;

private:
	Becketron::PerspectiveCameraController m_CameraController;
	glm::vec3 m_camPos = glm::vec3(0.0f);

	// Temp
	Becketron::Ref<Becketron::VertexArray> m_SquareVA;
	Becketron::Ref<Becketron::Shader> m_FlatColorShader;

	Becketron::Ref<Becketron::Texture2D> m_CheckerboardTexture;
	Becketron::Ref<Becketron::Texture2D> m_GroundTexture;

	struct ProfileResult
	{
		const char* Name;
		float Time;
	};

	std::vector<ProfileResult> m_ProfileResults;

	glm::vec4 m_SquareColor = { 0.2f, 0.5f, 0.5f, 1.0f };
};