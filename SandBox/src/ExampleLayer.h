#pragma once

#include "Becketron.h"

class ExampleLayer : public Becketron::Layer
{
public:
	ExampleLayer();

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	virtual void OnUpdate(Becketron::Timestep ts) override;
	virtual void OnImGuiRender() override;
	virtual void OnEvent(Becketron::Event& e) override;
private:
	Becketron::ShaderLibrary m_ShaderLibrary;
	Becketron::Ref<Becketron::Shader> m_Shader;
	Becketron::Ref<Becketron::VertexArray> m_VertexArray;

	Becketron::Ref<Becketron::Shader> m_FlatShader;
	Becketron::Ref<Becketron::VertexArray> m_SquareVA;

	Becketron::Ref<Becketron::Texture2D> m_Texture, m_MoonTexture;

	Becketron::OrthographicCameraController m_CameraController;
	glm::vec3 m_SquareColor = { 0.2f, 0.3f, 0.8f };
};