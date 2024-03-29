#pragma once

#include "Becketron/Renderer/Texture.h"

#include <glad/glad.h>

namespace Becketron {

	class OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D(uint32_t width, uint32_t height);
		OpenGLTexture2D(const std::string& path);
		virtual ~OpenGLTexture2D();

		virtual uint32_t GetWidth() const override { return m_Width; }
		virtual uint32_t GetHeight() const override { return m_Height; }
		virtual uint32_t GetRendererID() const override { return m_RendererID; }

		virtual void SetData(void* data, uint32_t size) override;

		virtual void Bind(uint32_t slot = 0) const override;

		virtual bool operator==(const Texture& other) const override
		{
			return m_RendererID == ((OpenGLTexture2D&)other).m_RendererID;
		}
	private:
		std::string m_Path;
		uint32_t m_Width, m_Height;
		uint32_t m_RendererID;
		GLenum m_InternalFormat, m_DataFormat;
	};

	class OpenGLCubemap : public CubemapTex
	{
	public:
		OpenGLCubemap(uint32_t width, uint32_t height);
		OpenGLCubemap(const std::vector<std::string>& faces);
		virtual ~OpenGLCubemap();

		virtual uint32_t GetWidth() const override { return m_Width; }
		virtual uint32_t GetHeight() const override { return m_Height; }
		virtual uint32_t GetRendererID() const override { return m_RendererID; }

		virtual void SetData(void* data, uint32_t size) override;

		virtual void Bind(uint32_t slot = 0) const override;

		virtual bool operator==(const Texture& other) const override
		{
			return m_RendererID == ((OpenGLCubemap&)other).m_RendererID;
		}
	private:
		std::vector<std::string> m_Faces;
		uint32_t m_Width, m_Height;
		uint32_t m_RendererID;
		GLenum m_InternalFormat, m_DataFormat;
	};
}