#pragma once

#include "Becketron/Renderer/Texture.h"

#include "Becketron/Renderer/Camera.h"

namespace Becketron {

	class Renderer3D
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const Camera& camera, const glm::mat4& transform);
		static void BeginScene(const OrthographicCamera& camera);  // TODO: Remove
		static void BeginScene(const PerspectiveCamera& camera);

		static void EndScene();
		static void Flush();

		// Primitives
		static void DrawCube(const glm::vec2& position, const glm::vec3& size, const glm::vec4& color);
		static void DrawCube(const glm::vec3& position, const glm::vec3& size, const glm::vec4& color);
		static void DrawCube(const glm::vec2& position, const glm::vec3& size, const Ref<Texture2D>& texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
		static void DrawCube(const glm::vec3& position, const glm::vec3& size, const Ref<Texture2D>& texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));

		static void DrawCube(const glm::mat4& transform, const glm::vec4& color);
		static void DrawCube(const glm::mat4& transform, const Ref<Texture2D>& texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));

		static void DrawRotatedCube(const glm::vec2& position, const glm::vec3& size, float rotation, const glm::vec4& color);
		static void DrawRotatedCube(const glm::vec3& position, const glm::vec3& size, float rotation, const glm::vec4& color);
		static void DrawRotatedCube(const glm::vec2& position, const glm::vec3& size, float rotation, const Ref<Texture2D>& texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
		static void DrawRotatedCube(const glm::vec3& position, const glm::vec3& size, float rotation, const Ref<Texture2D>& texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));

		static void DrawRotatedCube(const glm::mat4& transform, const glm::vec4& color);
		static void DrawRotatedCube(const glm::mat4& transform, const Ref<Texture2D>& texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));

		static void ShowLightCube(const PerspectiveCamera& camera);
		static void ShowLightCube(const glm::vec3& position, const glm::vec3& size, const glm::vec4& color, const Camera& camera, const glm::mat4& camTransform);
		static void ShowLightCube(const glm::mat4& transform, const glm::vec4& color, const Camera& camera, const glm::mat4& camTransform);
		static void DrawLightCube(const glm::vec3& position, const glm::vec3& size, const glm::vec4& color, const PerspectiveCamera& camera);
		static void DrawLightCube(const glm::vec3& position, const glm::vec3& size, const glm::vec4& color, const Camera& camera, const glm::mat4& camTransform);
		static void DrawLightCube(const glm::mat4& transform, const glm::vec4& color, const PerspectiveCamera& camera);
		static void DrawLightCube(const glm::mat4& transform, const glm::vec4& color, const Camera& camera, const glm::mat4& camTransform);

		static void CubeVertexPosInit();
		static void CubeTexCoordInit();
		static void CubeNormalCoordsInit();
		// TEmp
		static void LightVertexPosInit();
		struct LightInfo
		{
			glm::vec3 lightPos;
			glm::vec4 lightColor;
		};
		static LightInfo GetLightInformation();
		static void SetLightInformation();
		//Stats
		struct Statistics
		{
			uint32_t DrawCalls = 0;
			uint32_t CubeCount = 0;

			uint32_t GetTotalVertexCount() { return CubeCount * 24; }
			uint32_t GetTotalIndexCount() { return CubeCount * 36; }
		};
		static void ResetStats();
		static Statistics GetStats();
	private:
		static void FlushAndReset();

	};
}