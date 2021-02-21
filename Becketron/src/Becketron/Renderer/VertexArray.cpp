#include "btpch.h"
#include "VertexArray.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Becketron {

	VertexArray* VertexArray::Create()
	{
		switch(Renderer::GetAPI())
		{
			case RendererAPI::None:		BT_CORE_ASSERT(false, "RendererAPI::None Currently not supported!"); return nullptr;
			case RendererAPI::OpenGL:	return new OpenGLVertexArray();
		}

		BT_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}