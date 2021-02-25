#include "btpch.h"
#include "VertexArray.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Becketron {

	Ref<VertexArray> VertexArray::Create()
	{
		switch(Renderer::GetAPI())
		{
			case RendererAPI::API::None:	BT_CORE_ASSERT(false, "RendererAPI::None Currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:	return std::make_shared<OpenGLVertexArray>();
		}

		BT_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}