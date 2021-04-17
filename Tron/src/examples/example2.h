#pragma once

#include "Becketron.h"
#include "Panels/SceneHierarchyPanel.h"

namespace Becketron
{
	extern physx::PxTransform glmToPhysxTransform(const glm::mat4& transform);
	extern physx::PxRigidDynamic* CreateRigidDynamic(physx::PxTransform trans);

	class ExampleLayer2 : public Layer
	{
	public:
		ExampleLayer2();
		virtual ~ExampleLayer2() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		void OnUpdate(Timestep ts) override;
		virtual void OnImGuiRender() override;
		void OnEvent(Event& e) override;
	private:
		OrthographicCameraController m_CameraController;

		// Temp
		Ref<VertexArray> m_SquareVA;
		Ref<Shader> m_FlatColorShader;
		Ref<Framebuffer> m_Framebuffer;

		Ref<Scene> m_ActiveScene;
		Entity m_SquareEntity;
		Entity m_CameraEntity;
		Entity m_SecondCamera;

		bool m_PrimaryCamera = true;

		Ref<Texture2D> m_CheckerboardTexture;

		bool m_ViewportFocused = false, m_ViewportHovered = false;
		glm::vec2 m_ViewportSize = { 0.0f, 0.0f };

		//Panels
		SceneHierarchyPanel m_SceneHierarchyPanel;
	};
}