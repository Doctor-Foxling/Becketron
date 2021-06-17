#include "btpch.h"
#include "OpenGLTexture.h"

#include "stb_image.h"

namespace Becketron {

	OpenGLTexture2D::OpenGLTexture2D(uint32_t width, uint32_t height)
		: m_Width(width), m_Height(height)
	{
		BT_PROFILE_FUNCTION();

		m_InternalFormat = GL_RGBA8;
		m_DataFormat = GL_RGBA;

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Width, m_Height);

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}

	OpenGLTexture2D::OpenGLTexture2D(const std::string& path)
		: m_Path(path)
	{
		BT_PROFILE_FUNCTION();

		int width, height, channels;
		stbi_set_flip_vertically_on_load(1);
		stbi_uc* data = nullptr;
		{
			BT_PROFILE_SCOPE("stbi_load - OpenGLTexture2D::OpenGLTexture2D(const std::string&)");
			data = stbi_load(path.c_str(), &width, &height, &channels, 0);
		}
		BT_CORE_ASSERT(data, "Failed to load image!");
		m_Width = width;
		m_Height = height;

		GLenum internalFormat = 0, dataFormat = 0;
		if (channels == 4)
		{
			internalFormat = GL_RGBA8;
			dataFormat = GL_RGBA;
		}
		else if (channels == 3)
		{
			internalFormat = GL_RGB8;
			dataFormat = GL_RGB;
		}

		m_InternalFormat = internalFormat;
		m_DataFormat = dataFormat;

		BT_CORE_ASSERT(internalFormat & dataFormat, "Format not supported!");

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, internalFormat, m_Width, m_Height);

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		
		
		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, dataFormat, GL_UNSIGNED_BYTE, data);

		 //deallocate the memory used to store the data on the CPU
		stbi_image_free(data); 
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		BT_PROFILE_FUNCTION();

		glDeleteTextures(1, &m_RendererID);
	}

	void OpenGLTexture2D::SetData(void* data, uint32_t size)
	{
		BT_PROFILE_FUNCTION();

		uint32_t bpp = m_DataFormat == GL_RGBA ? 4 : 3;
		BT_CORE_ASSERT(size == m_Width * m_Height * bpp, "Data must b entire texture!");
		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);
	}

	void OpenGLTexture2D::Bind(uint32_t slot) const
	{
		BT_PROFILE_FUNCTION();

		glBindTextureUnit(slot, m_RendererID);
	}


	// ------/\| OpenGL Cubemap |/\----------------//
   // -------/\|----------------|/\---------------//
  // --------/\|----------------|/\--------------//

	OpenGLCubemap::OpenGLCubemap(uint32_t width, uint32_t height)
		: m_Width(width), m_Height(height)
	{
		BT_PROFILE_FUNCTION();

		m_InternalFormat = GL_RGBA8;
		m_DataFormat = GL_RGBA;

		glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Width, m_Height);

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	}

	OpenGLCubemap::OpenGLCubemap(const std::vector<std::string>& faces)
		: m_Faces(faces)
	{
		BT_PROFILE_FUNCTION();

		int width, height, channels;
		stbi_set_flip_vertically_on_load(1);
		stbi_uc* data = nullptr;

		//glTextureStorage2D(m_RendererID, 1, GL_RGB8, m_Width, m_Height);
		glGenTextures(1, &m_RendererID);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID);
		//glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &m_RendererID);

		//BT_CORE_ASSERT(internalFormat & dataFormat, "Format not supported!");

		for (unsigned int i = 0; i < faces.size(); i++)
		{
			BT_PROFILE_SCOPE("stbi_load - OpenGLCubemap::OpenGLCubemap(const std::vector<std::string>&)");
			unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &channels, 0);
			if (data)
			{
				//glTextureSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				//	0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
				//);

				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
					0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
				);
				stbi_image_free(data);
			}
			else
			{
				BT_TRACE("Cubemap tex failed to load at faces: {0}", faces[i]);
				stbi_image_free(data);
			}
		}

		// Probably need to take this section out -----
		BT_CORE_ASSERT(data, "Failed to load image!");
		m_Width = width;
		m_Height = height;

		GLenum internalFormat = 0, dataFormat = 0;
		if (channels == 4)
		{
			internalFormat = GL_RGBA8;
			dataFormat = GL_RGBA;
		}
		else if (channels == 3)
		{
			internalFormat = GL_RGB8;
			dataFormat = GL_RGB;
		}

		m_InternalFormat = internalFormat;
		m_DataFormat = dataFormat;
		// ----------------------------------------

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	}

	OpenGLCubemap::~OpenGLCubemap()
	{
		BT_PROFILE_FUNCTION();

		glDeleteTextures(1, &m_RendererID);
	}

	void OpenGLCubemap::SetData(void* data, uint32_t size)
	{
		BT_PROFILE_FUNCTION();

		uint32_t bpp = m_DataFormat == GL_RGBA ? 4 : 3;
		BT_CORE_ASSERT(size == m_Width * m_Height * bpp, "Data must b entire texture!");

		for (int face = 0; face < 6; ++face)
		{
			// face:
			// 0 = positive x face
			// 1 = negative x face
			// 2 = positive y face
			// 3 = negative y face
			// 4 = positive z face
			// 5 = negative z face

			glTextureSubImage3D(
				m_RendererID,
				0,      // only 1 level in example
				0,
				0,
				face,   // the offset to desired cubemap face, which offset goes to which face above
				m_Width,
				m_Height,
				1,      // depth how many faces to set, if this was 3 we'd set 3 cubemap faces at once
				GL_BGRA,
				GL_UNSIGNED_BYTE,
				data);
		}
	}

	void OpenGLCubemap::Bind(uint32_t slot) const
	{
		BT_PROFILE_FUNCTION();

		glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glBindTexture(GL_TEXTURE_CUBE_MAP, slot);
		//glBindTextureUnit(slot, m_RendererID);
	}

}