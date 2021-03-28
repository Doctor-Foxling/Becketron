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
		glm::vec3 Normal;
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

		// Lighting
		Ref<VertexArray> lightVertexArray;
		Ref<VertexBuffer> lightVertexBuffer;
		Ref<Shader> lightShader;
		float lightVertices[24 * 4];
		glm::vec3 lightPos = glm::vec3(3.0f, -1.0f, -1.0f);
		// Temporary
		glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 0.4f, 1.0f);

		uint32_t CubeIndexCount = 0;
		CubeVertex* CubeVertexBufferBase = nullptr;
		CubeVertex* CubeVertexBufferPtr = nullptr;

		std::array<Ref<Texture2D>, MaxTextureSlots> TextureSlots;
		uint32_t TextureSlotIndex = 1;

		//glm::vec4 CubeVertexPositions[8];
		
		// Vertex positions for the textured cube (which is the only type we're using)
		glm::vec4 CubeVertexTexPositions[24];
		glm::vec2 CubeTextureCoordinates[24];
		glm::vec3 CubeNormalCoordinates[24];

		Renderer3D::Statistics Stats;
	};

	static Renderer3DData s_Data3D;

	//Temp
	static Renderer3D::LightInfo lightInfo;

	void SetLightInformation()
	{
		lightInfo.lightPos = s_Data3D.lightPos;
		lightInfo.lightColor = s_Data3D.lightColor;
	}

	Renderer3D::LightInfo GetLightInformation()
	{
		lightInfo.lightPos = s_Data3D.lightPos;
		lightInfo.lightColor = s_Data3D.lightColor;

		return lightInfo;
	}

	void Renderer3D::Init()
	{
		BT_PROFILE_FUNCTION();

		// --- Textured Cube Vertices 

		CubeVertexPosInit();

		// --- Cube Texture Coordinares

		CubeTexCoordInit();

		// --- Cube Normal Coordinated

		CubeNormalCoordsInit();

		// ---- Light Cube Vertices

		LightVertexPosInit();

		// VAO For batch rendering the cubes
		s_Data3D.CubeVertexArray = VertexArray::Create();

		s_Data3D.CubeVertexBuffer = VertexBuffer::Create(s_Data3D.MaxTexVertices * sizeof(CubeVertex));

		s_Data3D.CubeVertexBuffer->SetLayout({
				{ShaderDataType::Float3, "a_Position" },
				{ShaderDataType::Float4, "a_Color" },
				{ShaderDataType::Float2, "a_TexCoord"},
				{ShaderDataType::Float, "a_TexIndex"},
				{ShaderDataType::Float, "a_TilingFactor"},
				{ShaderDataType::Float3, "a_Normal"}
			});
		s_Data3D.CubeVertexArray->AddVertexBuffer(s_Data3D.CubeVertexBuffer);

		s_Data3D.CubeVertexBufferBase = new CubeVertex[s_Data3D.MaxTexVertices];


		// VAO For Light_Cube
		s_Data3D.lightVertexArray = VertexArray::Create();

		s_Data3D.lightVertexBuffer = VertexBuffer::Create(s_Data3D.lightVertices, sizeof(s_Data3D.lightVertices));

		s_Data3D.lightVertexBuffer->SetLayout({ 
				{ ShaderDataType::Float4, "a_Position"} });

		s_Data3D.lightVertexArray->AddVertexBuffer(s_Data3D.lightVertexBuffer);

		// --------- Cube Indices for textured Cube -------

		uint32_t* cubeTexIndices = new uint32_t[s_Data3D.MaxTexIndices];
		// Re-using the offset variable
		uint32_t offset = 0;

		for (uint32_t i = 0; i < s_Data3D.MaxTexIndices; i += 36)
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

		// Using the 1st cube indices for lightIndices
		uint32_t* lightTexIndices = new uint32_t[36];

		for (int i = 0; i < 36; i++)
		{
			lightTexIndices[i] = cubeTexIndices[i];
		}

		Ref<IndexBuffer> cubeTexIB = IndexBuffer::Create(cubeTexIndices, s_Data3D.MaxTexIndices);
		s_Data3D.CubeVertexArray->SetIndexBuffer(cubeTexIB);
		delete[] cubeTexIndices;

		Ref<IndexBuffer> lightTexIB = IndexBuffer::Create(lightTexIndices, 36);
		s_Data3D.lightVertexArray->SetIndexBuffer(lightTexIB);
		delete[] lightTexIndices;

		// --- done cube index buffers ----

		s_Data3D.WhiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		s_Data3D.WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

		int32_t samplers[s_Data3D.MaxTextureSlots];
		for (uint32_t i = 0; i < s_Data3D.MaxTextureSlots; i++)
			samplers[i] = i;

		s_Data3D.TextureShader = Shader::Create("assets/shaders/Texture.glsl");
		s_Data3D.TextureShader->Bind();
		s_Data3D.TextureShader->SetIntArray("u_Textures", samplers, s_Data3D.MaxTextureSlots);
		s_Data3D.TextureShader->SetFloat4("lightColor", s_Data3D.lightColor);
		s_Data3D.TextureShader->SetFloat3("lightPos", s_Data3D.lightPos);

		s_Data3D.lightShader = Shader::Create("assets/shaders/Light.glsl");

		// Set all texture slots to 0
		s_Data3D.TextureSlots[0] = s_Data3D.WhiteTexture;

		SetLightInformation();
	}

	void Renderer3D::CubeVertexPosInit()
	{
		// Putting this here to clean up the Init() function

		//Front
		s_Data3D.CubeVertexTexPositions[0] = { -0.5f , -0.5f ,  0.5f, 1.0f };
		s_Data3D.CubeVertexTexPositions[1] = { 0.5f , -0.5f ,  0.5f,   1.0f };
		s_Data3D.CubeVertexTexPositions[2] = { -0.5f ,  0.5f ,  0.5f,  1.0f };
		s_Data3D.CubeVertexTexPositions[3] = { 0.5f ,  0.5f ,  0.5f,   1.0f };

		//Right
		s_Data3D.CubeVertexTexPositions[4] = { 0.5f , -0.5f ,  0.5f ,  1.0f };
		s_Data3D.CubeVertexTexPositions[5] = { 0.5f , -0.5f ,  -0.5f,  1.0f };
		s_Data3D.CubeVertexTexPositions[6] = { 0.5f ,  0.5f ,  0.5f ,  1.0f };
		s_Data3D.CubeVertexTexPositions[7] = { 0.5f ,  0.5f ,  -0.5f,  1.0f };

		// Back
		s_Data3D.CubeVertexTexPositions[8] = { 0.5f , -0.5f , -0.5f ,  1.0f };
		s_Data3D.CubeVertexTexPositions[9] = { -0.5f , -0.5f , -0.5f , 1.0f };
		s_Data3D.CubeVertexTexPositions[10] = { 0.5f ,  0.5f , -0.5f,  1.0f };
		s_Data3D.CubeVertexTexPositions[11] = { -0.5f ,  0.5f , -0.5f, 1.0f };

		// Left
		s_Data3D.CubeVertexTexPositions[12] = { -0.5f ,-0.5f ,-0.5f ,  1.0f };
		s_Data3D.CubeVertexTexPositions[13] = { -0.5f , -0.5f , 0.5f,  1.0f };
		s_Data3D.CubeVertexTexPositions[14] = { -0.5f ,  0.5f ,-0.5f,  1.0f };
		s_Data3D.CubeVertexTexPositions[15] = { -0.5f ,  0.5f , 0.5f,  1.0f };

		// Bottom
		s_Data3D.CubeVertexTexPositions[16] = { -0.5f , -0.5f , -0.5f, 1.0f };
		s_Data3D.CubeVertexTexPositions[17] = { 0.5f , -0.5f , -0.5f,  1.0f };
		s_Data3D.CubeVertexTexPositions[18] = { -0.5f , -0.5f ,  0.5f, 1.0f };
		s_Data3D.CubeVertexTexPositions[19] = { 0.5f , -0.5f ,  0.5f,  1.0f };

		// Top
		s_Data3D.CubeVertexTexPositions[20] = { -0.5f ,  0.5f ,  0.5f, 1.0f };
		s_Data3D.CubeVertexTexPositions[21] = { 0.5f ,  0.5f ,  0.5f,  1.0f };
		s_Data3D.CubeVertexTexPositions[22] = { -0.5f ,  0.5f , -0.5f, 1.0f };
		s_Data3D.CubeVertexTexPositions[23] = { 0.5f ,  0.5f , -0.5f,  1.0f };
	}

	void Renderer3D::CubeTexCoordInit()
	{
		// Just to clean up the Init() fucntion

		s_Data3D.CubeTextureCoordinates[0] = { 0.0f, 0.0f };
		s_Data3D.CubeTextureCoordinates[1] = { 0.33f, 0.0f };
		s_Data3D.CubeTextureCoordinates[2] = { 0.0f, 0.5f };
		s_Data3D.CubeTextureCoordinates[3] = { 0.33f, 0.5f };

		s_Data3D.CubeTextureCoordinates[4] = { 0.0f, 0.5f };
		s_Data3D.CubeTextureCoordinates[5] = { 0.33f, 0.5f };
		s_Data3D.CubeTextureCoordinates[6] = { 0.0f, 1.0f };
		s_Data3D.CubeTextureCoordinates[7] = { 0.33f, 1.0f };

		s_Data3D.CubeTextureCoordinates[8] = { 0.66f, 0.5f };
		s_Data3D.CubeTextureCoordinates[9] = { 1.0f, 0.5f };
		s_Data3D.CubeTextureCoordinates[10] = { 0.66f, 1.0f };
		s_Data3D.CubeTextureCoordinates[11] = { 1.0f, 1.0f };

		s_Data3D.CubeTextureCoordinates[12] = { 0.66f, 0.0f };
		s_Data3D.CubeTextureCoordinates[13] = { 1.0f, 0.0f };
		s_Data3D.CubeTextureCoordinates[14] = { 0.66f, 0.5f };
		s_Data3D.CubeTextureCoordinates[15] = { 1.0f, 0.5f };

		s_Data3D.CubeTextureCoordinates[16] = { 0.33f, 0.0f };
		s_Data3D.CubeTextureCoordinates[17] = { 0.66f, 0.0f };
		s_Data3D.CubeTextureCoordinates[18] = { 0.33f, 0.5f };
		s_Data3D.CubeTextureCoordinates[19] = { 0.66f, 0.5f };

		s_Data3D.CubeTextureCoordinates[20] = { 0.33f, 0.5f };
		s_Data3D.CubeTextureCoordinates[21] = { 0.66f, 0.5f };
		s_Data3D.CubeTextureCoordinates[22] = { 0.33f, 1.0f };
		s_Data3D.CubeTextureCoordinates[23] = { 0.66f, 1.0f };
	}

	void Renderer3D::CubeNormalCoordsInit()
	{
		// Front
		s_Data3D.CubeNormalCoordinates[0] =  { 0.0f, 0.0f, 1.0f };
		s_Data3D.CubeNormalCoordinates[1] =  { 0.0f, 0.0f, 1.0f };
		s_Data3D.CubeNormalCoordinates[2] =  { 0.0f, 0.0f, 1.0f };
		s_Data3D.CubeNormalCoordinates[3] =  { 0.0f, 0.0f, 1.0f };

		// Right
		s_Data3D.CubeNormalCoordinates[4] =  { 1.0f, 0.0f, 0.0f };
		s_Data3D.CubeNormalCoordinates[5] =  { 1.0f, 0.0f, 0.0f };
		s_Data3D.CubeNormalCoordinates[6] =  { 1.0f, 0.0f, 0.0f };
		s_Data3D.CubeNormalCoordinates[7] =  { 1.0f, 0.0f, 0.0f };

		// Back
		s_Data3D.CubeNormalCoordinates[8] =  { 0.0f, 0.0f,-1.0f };
		s_Data3D.CubeNormalCoordinates[9] =  { 0.0f, 0.0f,-1.0f };
		s_Data3D.CubeNormalCoordinates[10] = { 0.0f, 0.0f,-1.0f };
		s_Data3D.CubeNormalCoordinates[11] = { 0.0f, 0.0f,-1.0f };

		// Left
		s_Data3D.CubeNormalCoordinates[12] = {-1.0f, 0.0f, 0.0f };
		s_Data3D.CubeNormalCoordinates[13] = {-1.0f, 0.0f, 0.0f };
		s_Data3D.CubeNormalCoordinates[14] = {-1.0f, 0.0f, 0.0f };
		s_Data3D.CubeNormalCoordinates[15] = {-1.0f, 0.0f, 0.0f };

		// Bottom
		s_Data3D.CubeNormalCoordinates[16] = { 0.0f,-1.0f, 0.0f };
		s_Data3D.CubeNormalCoordinates[17] = { 0.0f,-1.0f, 0.0f };
		s_Data3D.CubeNormalCoordinates[18] = { 0.0f,-1.0f, 0.0f };
		s_Data3D.CubeNormalCoordinates[19] = { 0.0f,-1.0f, 0.0f };

		// Top
		s_Data3D.CubeNormalCoordinates[20] = { 0.0f, 1.0f, 0.0f };
		s_Data3D.CubeNormalCoordinates[21] = { 0.0f, 1.0f, 0.0f };
		s_Data3D.CubeNormalCoordinates[22] = { 0.0f, 1.0f, 0.0f };
		s_Data3D.CubeNormalCoordinates[23] = { 0.0f, 1.0f, 0.0f };
	}


	void Renderer3D::LightVertexPosInit()
	{
		float vertices[24 * 4] = {
			-0.5f, -0.5f, 0.5f, 1.0f ,
			 0.5f, -0.5f, 0.5f, 1.0f ,
			- 0.5f, 0.5f, 0.5f, 1.0f ,
			  0.5f, 0.5f, 0.5f, 1.0f ,

			 0.5f, -0.5f, 0.5f, 1.0f,
			0.5f, -0.5f, -0.5f, 1.0f,
			  0.5f, 0.5f, 0.5f, 1.0f,
			 0.5f, 0.5f, -0.5f, 1.0f,

			0.5f, -0.5f, -0.5f, 1.0f,
		  - 0.5f, -0.5f, -0.5f, 1.0f,
			 0.5f, 0.5f, -0.5f, 1.0f,
		   - 0.5f, 0.5f, -0.5f, 1.0f,

		   - 0.5f, -0.5f, -0.5f, 1.0f,
			- 0.5f, -0.5f, 0.5f, 1.0f,
			- 0.5f, 0.5f, -0.5f, 1.0f,
		     - 0.5f, 0.5f, 0.5f, 1.0f,

			- 0.5f, -0.5f, -0.5f, 1.0f,
			0.5f, -0.5f, -0.5f, 1.0f  ,
			- 0.5f, -0.5f, 0.5f, 1.0f ,
			0.5f, -0.5f, 0.5f, 1.0f	  ,

			- 0.5f, 0.5f, 0.5f, 1.0f ,
			0.5f, 0.5f, 0.5f, 1.0f	 ,
			- 0.5f, 0.5f, -0.5f, 1.0f,
			0.5f, 0.5f, -0.5f, 1.0f	
		};

		for (int i = 0; i < (24 * 4); i++)
			s_Data3D.lightVertices[i] = vertices[i];
	}

	void Renderer3D::Shutdown()
	{
		BT_PROFILE_FUNCTION();

		delete[] s_Data3D.CubeVertexBufferBase;
	}

	void Renderer3D::BeginScene(const OrthographicCamera& camera)
	{
		BT_PROFILE_FUNCTION();

		s_Data3D.CubeVertexArray->Bind();
		s_Data3D.CubeVertexArray->GetIndexBuffer()->Bind();

		s_Data3D.TextureShader->Bind();
		s_Data3D.TextureShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());

		s_Data3D.CubeIndexCount = 0;
		s_Data3D.CubeVertexBufferPtr = s_Data3D.CubeVertexBufferBase;

		s_Data3D.TextureSlotIndex = 1;
	}

	void Renderer3D::BeginScene(const Camera& camera, const glm::mat4& transform)
	{
		BT_PROFILE_FUNCTION();

		glm::mat4 viewProj = camera.GetProjection() * glm::inverse(transform);


		s_Data3D.CubeVertexArray->Bind();
		s_Data3D.CubeVertexArray->GetIndexBuffer()->Bind();

		s_Data3D.TextureShader->Bind();
		s_Data3D.TextureShader->SetMat4("u_ViewProjection", viewProj);

		s_Data3D.CubeIndexCount = 0;
		s_Data3D.CubeVertexBufferPtr = s_Data3D.CubeVertexBufferBase;

		s_Data3D.TextureSlotIndex = 1;
	}

	void Renderer3D::BeginScene(const PerspectiveCamera& camera)
	{
		BT_PROFILE_FUNCTION();

		s_Data3D.CubeVertexArray->Bind();
		s_Data3D.CubeVertexArray->GetIndexBuffer()->Bind();

		s_Data3D.TextureShader->Bind();
		s_Data3D.TextureShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());

		s_Data3D.CubeIndexCount = 0;
		s_Data3D.CubeVertexBufferPtr = s_Data3D.CubeVertexBufferBase;

		s_Data3D.TextureSlotIndex = 1;
	}

	void Renderer3D::EndScene()
	{
		BT_PROFILE_FUNCTION();

		// doing the casting to uint8_t to get an answer in bytes rather than the number of elements
		uint32_t dataSize = (uint32_t)((uint8_t*)s_Data3D.CubeVertexBufferPtr - (uint8_t*)s_Data3D.CubeVertexBufferBase);
		s_Data3D.CubeVertexBuffer->SetData(s_Data3D.CubeVertexBufferBase, dataSize);

		Flush();
	}

	void Renderer3D::Flush()
	{
		if (s_Data3D.CubeIndexCount == 0)
			return;		// Nothing to Draw mate

		// Bind Textures
		for (uint32_t i = 0; i < s_Data3D.TextureSlotIndex; i++)
			s_Data3D.TextureSlots[i]->Bind(i);

		RenderCommand::DrawIndexed(s_Data3D.CubeVertexArray, s_Data3D.CubeIndexCount);
		s_Data3D.Stats.DrawCalls++;

		s_Data3D.CubeVertexArray->Unbind();
		s_Data3D.CubeVertexArray->GetIndexBuffer()->Unbind();
		s_Data3D.TextureShader->Unbind();

		// Lighting (Temp most likely) involves extra draw call and binding of buffers
		//DrawLightCube();
	}

	void Renderer3D::ShowLightCube(const PerspectiveCamera& camera)
	{
		s_Data3D.lightVertexArray->Bind();
		s_Data3D.lightVertexArray->GetIndexBuffer()->Bind();

		DrawLightCube(s_Data3D.lightPos, glm::vec3(2.0f), s_Data3D.lightColor, camera);

		RenderCommand::DrawIndexed(s_Data3D.lightVertexArray, 36);
		s_Data3D.Stats.DrawCalls++;

		s_Data3D.lightVertexArray->Unbind();
		s_Data3D.lightVertexArray->GetIndexBuffer()->Unbind();
		s_Data3D.lightShader->Unbind();
	}

	void Renderer3D::FlushAndReset()
	{
		EndScene();

		s_Data3D.CubeIndexCount = 0;
		s_Data3D.CubeVertexBufferPtr = s_Data3D.CubeVertexBufferBase;

		s_Data3D.TextureSlotIndex = 1;
	}

	// Light Cube (Temp maybe)

	void Renderer3D::DrawLightCube(const glm::vec3& position, const glm::vec3& size, const glm::vec4& color, const PerspectiveCamera& camera)
	{
		BT_PROFILE_FUNCTION();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, size.z });

		DrawLightCube(transform, color, camera);
	}

	void Renderer3D::DrawLightCube(const glm::mat4& transform, const glm::vec4& color, const PerspectiveCamera& camera)
	{
		BT_PROFILE_FUNCTION();

		s_Data3D.lightShader->Bind();
		s_Data3D.lightShader->SetMat4("u_Transform", transform);
		s_Data3D.lightShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
		s_Data3D.lightShader->SetFloat4("u_Color", color);
		
		s_Data3D.Stats.CubeCount++;
	}

	// -----------------

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

		if (s_Data3D.CubeIndexCount >= Renderer3DData::MaxTexIndices)
			FlushAndReset();

		for (size_t i = 0; i < CubeVertexCount; i++)
		{
			s_Data3D.CubeVertexBufferPtr->Position = transform * s_Data3D.CubeVertexTexPositions[i];
			s_Data3D.CubeVertexBufferPtr->Color = color;
			s_Data3D.CubeVertexBufferPtr->TexCoord = s_Data3D.CubeTextureCoordinates[i];
			s_Data3D.CubeVertexBufferPtr->TexIndex = textureIndex;
			s_Data3D.CubeVertexBufferPtr->TilingFactor = tilingFactor;
			s_Data3D.CubeVertexBufferPtr->Normal = s_Data3D.CubeNormalCoordinates[i];
			s_Data3D.CubeVertexBufferPtr++;
		}

		s_Data3D.CubeIndexCount += 36;

		s_Data3D.Stats.CubeCount++;
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

		if (s_Data3D.CubeIndexCount >= Renderer3DData::MaxTexIndices)
			FlushAndReset();

		float textureIndex = 0.0f;
		for (uint32_t i = 1; i < s_Data3D.TextureSlotIndex; i++)
		{
			if (*s_Data3D.TextureSlots[i].get() == *texture.get())
			{
				textureIndex = (float)i;
				break;
			}
		}

		// If the texture submitted does not exist in the previous texture slots then:
		if (textureIndex == 0.0f)
		{
			if (s_Data3D.TextureSlotIndex >= Renderer3DData::MaxTextureSlots)
				FlushAndReset();

			textureIndex = (float)s_Data3D.TextureSlotIndex;
			s_Data3D.TextureSlots[s_Data3D.TextureSlotIndex] = texture;
			s_Data3D.TextureSlotIndex++;
		}


		for (size_t i = 0; i < cubeVertexCount; i++)
		{
			s_Data3D.CubeVertexBufferPtr->Position = transform * s_Data3D.CubeVertexTexPositions[i];
			s_Data3D.CubeVertexBufferPtr->Color = tintColor;
			s_Data3D.CubeVertexBufferPtr->TexCoord = s_Data3D.CubeTextureCoordinates[i];
			s_Data3D.CubeVertexBufferPtr->TexIndex = textureIndex;
			s_Data3D.CubeVertexBufferPtr->TilingFactor = tilingFactor;
			s_Data3D.CubeVertexBufferPtr->Normal = s_Data3D.CubeNormalCoordinates[i];
			s_Data3D.CubeVertexBufferPtr++;
		}

		s_Data3D.CubeIndexCount += 36;

		s_Data3D.Stats.CubeCount++;
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
		memset(&s_Data3D.Stats, 0, sizeof(Statistics));
	}

	Renderer3D::Statistics Renderer3D::GetStats()
	{
		return s_Data3D.Stats;
	}

}
