#include "btpch.h"
#include "RendererAPI.h"
#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Becketron {

	RendererAPI::API RendererAPI::s_API = RendererAPI::API::OpenGL;

	Scope<RendererAPI> RendererAPI::Create()
	{
		switch (s_API)
		{
		case RendererAPI::API::None:	BT_CORE_ASSERT(false, "RendererAPI::None not currently supported!"); return nullptr;
		case RendererAPI::API::OpenGL:	return CreateScope<OpenGLRendererAPI>();
		}

		BT_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}
}