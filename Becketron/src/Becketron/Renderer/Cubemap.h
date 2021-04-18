#pragma once

#include "VertexArray.h"
#include "Shader.h"
#include "RenderCommand.h"
#include "Texture.h"

#include "Camera.h"

namespace Becketron {

	class Cubemap
	{
	public:
		static void Init();
		static void Shutdown();

		//static void DrawCubemap();
		static void RenderSkybox(const PerspectiveCamera& camera);
		static void RenderSkybox(const Camera& camera, const glm::mat4& transform);
		static unsigned int loadCubemap(std::vector<std::string>& faces);
	};
}