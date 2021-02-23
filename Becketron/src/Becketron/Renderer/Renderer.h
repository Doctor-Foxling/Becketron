#pragma once
#include "RenderCommand.h"

#include "Camera.h"
#include "Shader.h"

namespace Becketron {

	class Renderer
	{
	public:
		static void BeginScene(OrthographicCamera& camera);
		static void EndScene();

		static void Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform = glm::mat4(1.0f));

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
	private:
		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix;
		};

		// temp.. since this would essential live for the entire duration of our renderer
		static SceneData* m_SceneData;
	};
}