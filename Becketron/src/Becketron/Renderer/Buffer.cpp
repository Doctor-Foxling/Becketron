#include "btpch.h"
#include "Buffer.h"

#include  "Renderer.h"
#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Becketron {

	Ref<VertexBuffer> VertexBuffer::Create(uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:	BT_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:	return CreateRef<OpenGLVertexBuffer>(size);
		}

		BT_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Ref<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size)
	{
		// So this is where we decide which graphics API do we want to use
		// It will cast it to the base class so not returning a pointer to the 
		// derived class, which is what we want
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:	BT_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:	return CreateRef<OpenGLVertexBuffer>(vertices, size);
		}

		BT_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
	Ref<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:		BT_CORE_ASSERT(false, "RendererAPI::None is current not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:	return CreateRef<OpenGLIndexBuffer>(indices, size);
		}

		BT_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}