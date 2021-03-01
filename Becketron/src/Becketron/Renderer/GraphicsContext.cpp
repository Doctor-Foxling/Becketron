#include "btpch.h"
#include "Becketron/Renderer/GraphicsContext.h"

#include "Becketron/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLContext.h"

namespace Becketron {

	Scope<GraphicsContext> GraphicsContext::Create(void* window)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:	BT_CORE_ASSERT(false, "RendererAPI::None not supported"); return nullptr;
			case RendererAPI::API::OpenGL:	return CreateScope<OpenGLContext>(static_cast<GLFWwindow*>(window));
		}

		BT_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}
}