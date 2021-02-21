#include "btpch.h"
#include "OpenGLRendererAPI.h"

#include <glad/glad.h>

namespace Becketron {

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
	void OpenGLRendererAPI::DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray)
	{
		glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
	}
}