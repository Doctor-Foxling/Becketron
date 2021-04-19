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
		static void RenderSkybox(const PerspectiveCamera& camera, const std::vector<std::string>& faces);
		static void RenderSkybox(const Camera& camera, const glm::mat4& transform);
		static void ChangeSkybox(const std::vector<std::string>& faces);
		static unsigned int loadCubemap(const std::vector<std::string>& faces);
	};
}