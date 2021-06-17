#include "btpch.h"
#include "Texture.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLTexture.h"

namespace Becketron {

	Ref<Texture2D> Texture2D::Create(uint32_t width, uint32_t height)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:	BT_CORE_ASSERT(false, "RendererAPI::None not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:	return CreateRef<OpenGLTexture2D>(width, height);
		}

		BT_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}

	Ref<Texture2D> Texture2D::Create(const std::string& path)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:	BT_CORE_ASSERT(false, "RendererAPI::None not currently supported!"); return nullptr;
		case RendererAPI::API::OpenGL:	return CreateRef<OpenGLTexture2D>(path);
		}

		BT_CORE_ASSERT(false, "Unknown Renderer API");
		return nullptr;
	}

	Ref<CubemapTex> CubemapTex::Create(uint32_t width, uint32_t height)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:	BT_CORE_ASSERT(false, "RendererAPI::None not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:	return CreateRef<OpenGLCubemap>(width, height);
		}

		BT_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}


	Ref<CubemapTex> CubemapTex::Create(const std::vector<std::string>& faces)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:	BT_CORE_ASSERT(false, "RendererAPI::None not currently supported!"); return nullptr;
		case RendererAPI::API::OpenGL:	return CreateRef<OpenGLCubemap>(faces);
		}

		BT_CORE_ASSERT(false, "Unknown Renderer API");
		return nullptr;
	}
}