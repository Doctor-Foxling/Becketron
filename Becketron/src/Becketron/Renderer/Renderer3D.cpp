#include "btpch.h"
#include "Renderer3D.h"

#include "VertexArray.h"
#include "Shader.h"
#include "RenderCommand.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Becketron {

	struct CubeVertex
	{
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 TexCoord;
		// TODO: texid
		float TexIndex;
		float TilingFactor;
	};

	struct Renderer3DData
	{
		//static const uint32_t MaxCubes = 1000;
		static const uint32_t MaxTexCubes = 1000;
		//static const uint32_t MaxVertices = MaxCubes * 8;
		static const uint32_t MaxTexVertices = MaxTexCubes * 24;
		//static const uint32_t MaxIndices = MaxCubes * 36;
		static const uint32_t MaxTexIndices = MaxTexCubes * 36;
		static const uint32_t MaxTextureSlots = 32;  // TODO: RenderCaps

		Ref<VertexArray> CubeVertexArray;
		Ref<VertexBuffer> CubeVertexBuffer;
		Ref<Shader> TextureShader;
		Ref<Texture2D> WhiteTexture;

		uint32_t CubeIndexCount = 0;
		CubeVertex* CubeVertexBufferBase = nullptr;
		CubeVertex* CubeVertexBufferPtr = nullptr;

		std::array<Ref<Texture2D>, MaxTextureSlots> TextureSlots;
		uint32_t TextureSlotIndex = 1;

		//glm::vec4 CubeVertexPositions[8];
		
		glm::vec4 CubeVertexTexPositions[24];
		glm::vec2 CubeTextureCoordinates[24];

		Renderer3D::Statistics Stats;
	};

	static Renderer3DData s_Data;

	void Renderer3D::Init()
	{
		BT_PROFILE_FUNCTION();

		s_Data.CubeVertexArray = VertexArray::Create();

		s_Data.CubeVertexBuffer = VertexBuffer::Create(s_Data.MaxTexVertices * sizeof(CubeVertex));

		s_Data.CubeVertexBuffer->SetLayout({
				{ShaderDataType::Float3, "a_Position" },
				{ShaderDataType::Float4, "a_Color" },
				{ShaderDataType::Float2, "a_TexCoord"},
				{ShaderDataType::Float, "a_TexIndex"},
				{ShaderDataType::Float, "a_TilingFactor"}
			});
		s_Data.CubeVertexArray->AddVertexBuffer(s_Data.CubeVertexBuffer);

		s_Data.CubeVertexBufferBase = new CubeVertex[s_Data.MaxTexVertices];

		// ------------ Indices for normal cube --------------

		//uint32_t* cubeIndices = new uint32_t[s_Data.MaxIndices];

		//uint32_t offset = 0;
		//for (uint32_t i = 0; i < s_Data.MaxIndices; i += 36)
		//{
		//	// front
		//	cubeIndices[i + 0] = offset + 0;
		//	cubeIndices[i + 1] = offset + 1;
		//	cubeIndices[i + 2] = offset + 2;

		//	cubeIndices[i + 3] = offset + 2;
		//	cubeIndices[i + 4] = offset + 3;
		//	cubeIndices[i + 5] = offset + 0;

		//	// right
		//	cubeIndices[i + 6] = offset + 1;
		//	cubeIndices[i + 7] = offset + 5;
		//	cubeIndices[i + 8] = offset + 6;

		//	cubeIndices[i + 9] = offset + 6;
		//	cubeIndices[i + 10] = offset + 2;
		//	cubeIndices[i + 11] = offset + 1;

		//	// back
		//	cubeIndices[i + 12] = offset + 7;
		//	cubeIndices[i + 13] = offset + 6;
		//	cubeIndices[i + 14] = offset + 5;

		//	cubeIndices[i + 15] = offset + 5;
		//	cubeIndices[i + 16] = offset + 4;
		//	cubeIndices[i + 17] = offset + 7;

		//	// left
		//	cubeIndices[i + 18] = offset + 4;
		//	cubeIndices[i + 19] = offset + 0;
		//	cubeIndices[i + 20] = offset + 3;

		//	cubeIndices[i + 21] = offset + 3;
		//	cubeIndices[i + 22] = offset + 7;
		//	cubeIndices[i + 23] = offset + 4;

		//	// bottom
		//	cubeIndices[i + 24] = offset + 4;
		//	cubeIndices[i + 25] = offset + 5;
		//	cubeIndices[i + 26] = offset + 1;

		//	cubeIndices[i + 27] = offset + 1;
		//	cubeIndices[i + 28] = offset + 0;
		//	cubeIndices[i + 29] = offset + 4;

		//	// top
		//	cubeIndices[i + 30] = offset + 3;
		//	cubeIndices[i + 31] = offset + 2;
		//	cubeIndices[i + 32] = offset + 6;

		//	cubeIndices[i + 33] = offset + 6;
		//	cubeIndices[i + 34] = offset + 7;
		//	cubeIndices[i + 35] = offset + 3;

		//	offset += 8;
		//}

		//Ref<IndexBuffer> cubeIB = IndexBuffer::Create(cubeIndices, s_Data.MaxIndices);
		////s_Data.CubeVertexArray->SetIndexBuffer(cubeIB);
		//delete[] cubeIndices;

		// --------- Cube Indices for textured Cube -------

		uint32_t* cubeTexIndices = new uint32_t[s_Data.MaxTexIndices];
		// Re-using the offset variable
		uint32_t offset = 0;

		for (uint32_t i = 0; i < s_Data.MaxTexIndices; i += 36)
		{
			// front
			cubeTexIndices[i + 0] = offset + 0;
			cubeTexIndices[i + 1] = offset + 1;
			cubeTexIndices[i + 2] = offset + 2;

			cubeTexIndices[i + 3] = offset + 1;
			cubeTexIndices[i + 4] = offset + 3;
			cubeTexIndices[i + 5] = offset + 2;

			// right
			cubeTexIndices[i + 6] = offset + 4;
			cubeTexIndices[i + 7] = offset + 5;
			cubeTexIndices[i + 8] = offset + 6;

			cubeTexIndices[i + 9] = offset + 5;
			cubeTexIndices[i + 10] = offset + 7;
			cubeTexIndices[i + 11] = offset + 6;

			// back
			cubeTexIndices[i + 12] = offset + 8;
			cubeTexIndices[i + 13] = offset + 9;
			cubeTexIndices[i + 14] = offset + 10;

			cubeTexIndices[i + 15] = offset + 9;
			cubeTexIndices[i + 16] = offset + 11;
			cubeTexIndices[i + 17] = offset + 10;

			// left
			cubeTexIndices[i + 18] = offset + 12;
			cubeTexIndices[i + 19] = offset + 13;
			cubeTexIndices[i + 20] = offset + 14;

			cubeTexIndices[i + 21] = offset + 13;
			cubeTexIndices[i + 22] = offset + 15;
			cubeTexIndices[i + 23] = offset + 14;

			// bottom
			cubeTexIndices[i + 24] = offset + 16;
			cubeTexIndices[i + 25] = offset + 17;
			cubeTexIndices[i + 26] = offset + 18;

			cubeTexIndices[i + 27] = offset + 18;
			cubeTexIndices[i + 28] = offset + 19;
			cubeTexIndices[i + 29] = offset + 17;

			// top
			cubeTexIndices[i + 30] = offset + 20;
			cubeTexIndices[i + 31] = offset + 21;
			cubeTexIndices[i + 32] = offset + 22;

			cubeTexIndices[i + 33] = offset + 21;
			cubeTexIndices[i + 34] = offset + 23;
			cubeTexIndices[i + 35] = offset + 22;

			offset += 24;
		}


		Ref<IndexBuffer> cubeTexIB = IndexBuffer::Create(cubeTexIndices, s_Data.MaxTexIndices);
		s_Data.CubeVertexArray->SetIndexBuffer(cubeTexIB);
		delete[] cubeTexIndices;

		// --- done creating index buffers ----

		s_Data.WhiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		s_Data.WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

		int32_t samplers[s_Data.MaxTextureSlots];
		for (uint32_t i = 0; i < s_Data.MaxTextureSlots; i++)
			samplers[i] = i;

		s_Data.TextureShader = Shader::Create("assets/shaders/Texture.glsl");
		s_Data.TextureShader->Bind();
		s_Data.TextureShader->SetIntArray("u_Textures", samplers, s_Data.MaxTextureSlots);

		// Set all texture slots to 0
		s_Data.TextureSlots[0] = s_Data.WhiteTexture;

		// Normal Cube Vertices
		/*s_Data.CubeVertexPositions[0] = { -0.5f, -0.5f, 0.5f, 1.0f };
		s_Data.CubeVertexPositions[1] = { 0.5f, -0.5f, 0.5f, 1.0f };
		s_Data.CubeVertexPositions[2] = { 0.5f,  0.5f, 0.5f, 1.0f };
		s_Data.CubeVertexPositions[3] = { -0.5f,  0.5f, 0.5f, 1.0f };

		s_Data.CubeVertexPositions[4] = { -0.5f, -0.5f, -0.5f, 1.0f };
		s_Data.CubeVertexPositions[5] = { 0.5f, -0.5f, -0.5f, 1.0f };
		s_Data.CubeVertexPositions[6] = { 0.5f,  0.5f, -0.5f, 1.0f };
		s_Data.CubeVertexPositions[7] = { -0.5f,  0.5f, -0.5f, 1.0f };*/


		// --- Textured Cube Vertices 

		s_Data.CubeVertexTexPositions[0] =	{ -0.5f , -0.5f ,  0.5f, 1.0f  };
		s_Data.CubeVertexTexPositions[1] = { 0.5f , -0.5f ,  0.5f,   1.0f };
		s_Data.CubeVertexTexPositions[2] = { -0.5f ,  0.5f ,  0.5f,  1.0f };
		s_Data.CubeVertexTexPositions[3] = { 0.5f ,  0.5f ,  0.5f,   1.0f };

		s_Data.CubeVertexTexPositions[4] = { 0.5f , -0.5f ,  0.5f ,  1.0f };
		s_Data.CubeVertexTexPositions[5] = { 0.5f , -0.5f ,  -0.5f,  1.0f  };
		s_Data.CubeVertexTexPositions[6] = { 0.5f ,  0.5f ,  0.5f ,  1.0f };
		s_Data.CubeVertexTexPositions[7] = { 0.5f ,  0.5f ,  -0.5f,  1.0f  };

		s_Data.CubeVertexTexPositions[8] = { 0.5f , -0.5f , -0.5f ,  1.0f };
		s_Data.CubeVertexTexPositions[9] = { -0.5f , -0.5f , -0.5f , 1.0f };
		s_Data.CubeVertexTexPositions[10] = { 0.5f ,  0.5f , -0.5f,  1.0f  };
		s_Data.CubeVertexTexPositions[11] = { -0.5f ,  0.5f , -0.5f, 1.0f  };

		s_Data.CubeVertexTexPositions[12] = { -0.5f ,-0.5f ,-0.5f ,  1.0f};
		s_Data.CubeVertexTexPositions[13] = { -0.5f , -0.5f , 0.5f,  1.0f};
		s_Data.CubeVertexTexPositions[14] = { -0.5f ,  0.5f ,-0.5f,  1.0f};
		s_Data.CubeVertexTexPositions[15] = { -0.5f ,  0.5f , 0.5f,  1.0f};

		s_Data.CubeVertexTexPositions[16] = { -0.5f , -0.5f , -0.5f, 1.0f  };
		s_Data.CubeVertexTexPositions[17] = { 0.5f , -0.5f , -0.5f,  1.0f  };
		s_Data.CubeVertexTexPositions[18] = { -0.5f , -0.5f ,  0.5f, 1.0f  };
		s_Data.CubeVertexTexPositions[19] = { 0.5f , -0.5f ,  0.5f,  1.0f  };

		s_Data.CubeVertexTexPositions[20] = { -0.5f ,  0.5f ,  0.5f, 1.0f  };
		s_Data.CubeVertexTexPositions[21] = { 0.5f ,  0.5f ,  0.5f,  1.0f  };
		s_Data.CubeVertexTexPositions[22] = { -0.5f ,  0.5f , -0.5f, 1.0f  };
		s_Data.CubeVertexTexPositions[23] = { 0.5f ,  0.5f , -0.5f,  1.0f  };
									    
		// --- Cube Texture Coordinares
		s_Data.CubeTextureCoordinates[0] = 	{	0.0f, 0.0f	};
		s_Data.CubeTextureCoordinates[1] = 	{	0.33f, 0.0f	};
		s_Data.CubeTextureCoordinates[2] = 	{	0.0f, 0.5f	};
		s_Data.CubeTextureCoordinates[3] = 	{	0.33f, 0.5f	};
		
		s_Data.CubeTextureCoordinates[4] = 	{	0.0f, 0.5f	};
		s_Data.CubeTextureCoordinates[5] = 	{	0.33f, 0.5f	};
		s_Data.CubeTextureCoordinates[6] = 	{	0.0f, 1.0f	};
		s_Data.CubeTextureCoordinates[7] = 	{	0.33f, 1.0f	};
		
		s_Data.CubeTextureCoordinates[8] = 	{	0.66f, 0.5f	};
		s_Data.CubeTextureCoordinates[9] = 	{	1.0f, 0.5f	};
		s_Data.CubeTextureCoordinates[10] = {	0.66f, 1.0f	};
		s_Data.CubeTextureCoordinates[11] = {	1.0f, 1.0f	};
		
		s_Data.CubeTextureCoordinates[12] = {	0.66f, 0.0f	};
		s_Data.CubeTextureCoordinates[13] = {	1.0f, 0.0f	};
		s_Data.CubeTextureCoordinates[14] = {	0.66f, 0.5f	};
		s_Data.CubeTextureCoordinates[15] = {	1.0f, 0.5f	};
		
		s_Data.CubeTextureCoordinates[16] = {	0.33f, 0.0f	};
		s_Data.CubeTextureCoordinates[17] = {	0.66f, 0.0f	};
		s_Data.CubeTextureCoordinates[18] = {	0.33f, 0.5f	};
		s_Data.CubeTextureCoordinates[19] = {	0.66f, 0.5f	};
		
		s_Data.CubeTextureCoordinates[20] = {	0.33f, 0.5f	};
		s_Data.CubeTextureCoordinates[21] = {	0.66f, 0.5f	};
		s_Data.CubeTextureCoordinates[22] = {	0.33f, 1.0f	};
		s_Data.CubeTextureCoordinates[23] = {	0.66f, 1.0f	};
	}

	void Renderer3D::Shutdown()
	{
		BT_PROFILE_FUNCTION();

		delete[] s_Data.CubeVertexBufferBase;
	}

	void Renderer3D::BeginScene(const OrthographicCamera& camera)
	{
		BT_PROFILE_FUNCTION();

		s_Data.TextureShader->Bind();
		s_Data.TextureShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());

		s_Data.CubeIndexCount = 0;
		s_Data.CubeVertexBufferPtr = s_Data.CubeVertexBufferBase;

		s_Data.TextureSlotIndex = 1;
	}

	void Renderer3D::BeginScene(const Camera& camera, const glm::mat4& transform)
	{
		BT_PROFILE_FUNCTION();

		glm::mat4 viewProj = camera.GetProjection() * glm::inverse(transform);

		s_Data.TextureShader->Bind();
		s_Data.TextureShader->SetMat4("u_ViewProjection", viewProj);

		s_Data.CubeIndexCount = 0;
		s_Data.CubeVertexBufferPtr = s_Data.CubeVertexBufferBase;

		s_Data.TextureSlotIndex = 1;
	}

	void Renderer3D::BeginScene(const PerspectiveCamera& camera)
	{
		BT_PROFILE_FUNCTION();

		s_Data.TextureShader->Bind();
		s_Data.TextureShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());

		s_Data.CubeIndexCount = 0;
		s_Data.CubeVertexBufferPtr = s_Data.CubeVertexBufferBase;

		s_Data.TextureSlotIndex = 1;
	}

	void Renderer3D::EndScene()
	{
		BT_PROFILE_FUNCTION();

		// doing the casting to uint8_t to get an answer in bytes rather than the number of elements
		uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.CubeVertexBufferPtr - (uint8_t*)s_Data.CubeVertexBufferBase);
		s_Data.CubeVertexBuffer->SetData(s_Data.CubeVertexBufferBase, dataSize);

		Flush();
	}

	void Renderer3D::Flush()
	{
		if (s_Data.CubeIndexCount == 0)
			return;		// Nothing to Draw mate

		// Bind Textures
		for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++)
			s_Data.TextureSlots[i]->Bind(i);

		RenderCommand::DrawIndexed(s_Data.CubeVertexArray, s_Data.CubeIndexCount);
		s_Data.Stats.DrawCalls++;
	}

	void Renderer3D::FlushAndReset()
	{
		EndScene();

		s_Data.CubeIndexCount = 0;
		s_Data.CubeVertexBufferPtr = s_Data.CubeVertexBufferBase;

		s_Data.TextureSlotIndex = 1;
	}

	void Renderer3D::DrawCube(const glm::vec2& position, const glm::vec3& size, const glm::vec4& color)
	{
		DrawCube({ position.x, position.y, 0.0f }, size, color);
	}

	void Renderer3D::DrawCube(const glm::vec3& position, const glm::vec3& size, const glm::vec4& color)
	{
		BT_PROFILE_FUNCTION();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, size.z });

		DrawCube(transform, color);
	}

	void Renderer3D::DrawCube(const glm::mat4& transform, const glm::vec4& color)
	{
		BT_PROFILE_FUNCTION();

		constexpr size_t CubeVertexCount = 24;
		const float textureIndex = 0.0f;  // White Texture
		const float tilingFactor = 1.0f;

		if (s_Data.CubeIndexCount >= Renderer3DData::MaxTexIndices)
			FlushAndReset();

		for (size_t i = 0; i < CubeVertexCount; i++)
		{
			s_Data.CubeVertexBufferPtr->Position = transform * s_Data.CubeVertexTexPositions[i];
			s_Data.CubeVertexBufferPtr->Color = color + glm::vec4(i*0.1);
			s_Data.CubeVertexBufferPtr->TexCoord = s_Data.CubeTextureCoordinates[i];
			s_Data.CubeVertexBufferPtr->TexIndex = textureIndex;
			s_Data.CubeVertexBufferPtr->TilingFactor = tilingFactor;
			s_Data.CubeVertexBufferPtr++;
		}

		s_Data.CubeIndexCount += 36;

		s_Data.Stats.CubeCount++;
	}

	void Renderer3D::DrawCube(const glm::vec2& position, const glm::vec3& size, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
	{
		DrawCube({ position.x, position.y, 0.0f }, size, texture, tilingFactor, tintColor);
	}

	void Renderer3D::DrawCube(const glm::vec3& position, const glm::vec3& size, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
	{
		BT_PROFILE_FUNCTION();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, size.z });

		DrawCube(transform, texture, tilingFactor);
	}

	void Renderer3D::DrawCube(const glm::mat4& transform, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
	{
		BT_PROFILE_FUNCTION();

		constexpr size_t cubeVertexCount = 24;

		if (s_Data.CubeIndexCount >= Renderer3DData::MaxTexIndices)
			FlushAndReset();

		float textureIndex = 0.0f;
		for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++)
		{
			if (*s_Data.TextureSlots[i].get() == *texture.get())
			{
				textureIndex = (float)i;
				break;
			}
		}

		// If the texture submitted does not exist in the previous texture slots then:
		if (textureIndex == 0.0f)
		{
			if (s_Data.TextureSlotIndex >= Renderer3DData::MaxTextureSlots)
				FlushAndReset();

			textureIndex = (float)s_Data.TextureSlotIndex;
			s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
			s_Data.TextureSlotIndex++;
		}


		for (size_t i = 0; i < cubeVertexCount; i++)
		{
			s_Data.CubeVertexBufferPtr->Position = transform * s_Data.CubeVertexTexPositions[i];
			s_Data.CubeVertexBufferPtr->Color = tintColor;
			s_Data.CubeVertexBufferPtr->TexCoord = s_Data.CubeTextureCoordinates[i];
			s_Data.CubeVertexBufferPtr->TexIndex = textureIndex;
			s_Data.CubeVertexBufferPtr->TilingFactor = tilingFactor;
			s_Data.CubeVertexBufferPtr++;
		}

		s_Data.CubeIndexCount += 36;

		s_Data.Stats.CubeCount++;
	}

	void Renderer3D::DrawRotatedCube(const glm::vec2& position, const glm::vec3& size, float rotation, const glm::vec4& color)
	{
		DrawRotatedCube({ position.x, position.y, 0.0f }, size, rotation, color);
	}

	void Renderer3D::DrawRotatedCube(const glm::vec3& position, const glm::vec3& size, float rotation, const glm::vec4& color)
	{
		BT_PROFILE_FUNCTION();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, size.z });

		DrawCube(transform, color);
	}

	void Renderer3D::DrawRotatedCube(const glm::vec2& position, const glm::vec3& size, float rotation, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
	{
		DrawRotatedCube({ position.x, position.y, 0.0f }, size, rotation, texture, tilingFactor, tintColor);
	}

	void Renderer3D::DrawRotatedCube(const glm::vec3& position, const glm::vec3& size, float rotation, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
	{
		BT_PROFILE_FUNCTION();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, size.z });

		DrawCube(transform, texture, tilingFactor, tintColor);

	}

	void Renderer3D::ResetStats()
	{
		memset(&s_Data.Stats, 0, sizeof(Statistics));
	}

	Renderer3D::Statistics Renderer3D::GetStats()
	{
		return s_Data.Stats;
	}

}
