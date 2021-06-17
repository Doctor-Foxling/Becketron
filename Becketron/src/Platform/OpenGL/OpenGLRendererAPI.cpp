#include "btpch.h"
#include "OpenGLRendererAPI.h"

#include <glad/glad.h>

namespace Becketron {

	void OpenGLMessageCallback(
		unsigned source, unsigned type, unsigned id, unsigned severity,
		int length, const char* message, const void* userParam)
	{
		switch (severity)
		{
		case GL_DEBUG_SEVERITY_HIGH:		BT_CORE_CRITICAL(message); return;
		case GL_DEBUG_SEVERITY_MEDIUM:		BT_CORE_ERROR(message); return;
		case GL_DEBUG_SEVERITY_LOW:			BT_CORE_WARN(message); return;
		case GL_DEBUG_SEVERITY_NOTIFICATION:BT_CORE_TRACE(message); return;
		}

		BT_CORE_ASSERT(false, "Unknown severity level!");
	}
	void OpenGLRendererAPI::Init()
	{
		BT_PROFILE_FUNCTION();

#ifdef BT_DEBUG
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(OpenGLMessageCallback, nullptr);

		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);
#endif

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_DEPTH_TEST);
	}

	void OpenGLRendererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		glViewport(x, y, width, height);
	}

	void OpenGLRendererAPI::SetClearColor(const glm::vec4& color)
	{
		// glm gives us multiple aliases for the same variables
		// and so we can do somma like: color.b and get the blue (3rd) element of a color variable
		glClearColor(color.r, color.g, color.b, color.a);
	}
	void OpenGLRendererAPI::Clear()
	{
		// Temporary because you don't always want to clear the color and the depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLRendererAPI::DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount)
	{
		uint32_t count = indexCount ? indexCount : vertexArray->GetIndexBuffer()->GetCount();
		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void OpenGLRendererAPI::DrawCubemap()
	{
		glDrawArrays(GL_TRIANGLES, 0, 36);
		//glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	}

	void OpenGLRendererAPI::SetDepthMask(bool depthOn)
	{
		glDepthMask(depthOn);
	}
}