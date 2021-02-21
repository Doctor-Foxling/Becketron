#include "btpch.h"
#include "Shader.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace Becketron
{
	Shader* Shader::CreateShader(const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::None: BT_CORE_ASSERT(false, "RendererAPI::None is not currently supported"); return nullptr;
		case RendererAPI::OpenGL: return new OpenGLShader(vertexSrc, fragmentSrc);
		}

		BT_CORE_ASSERT(false, "Unknown Renderer API");
		return nullptr;
	}
}
