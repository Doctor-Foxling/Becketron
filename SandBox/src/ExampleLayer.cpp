#include "ExampleLayer.h"

#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

ExampleLayer::ExampleLayer()
	: Layer("Example"), m_CameraController(1280.0f / 720.0f, true)
{
	m_VertexArray = Becketron::VertexArray::Create();

	float vertices[3 * 7] = {
		-0.5f, -0.5f, 0.0f, 0.0f, 0.5f, 0.8f, 1.0f,
			0.5f, -0.5f, 0.0f,	0.0f, 0.4f, 0.7f, 1.0f,
			0.0f, 0.5f, 0.0f,	1.0f, 1.0f, 0.0f, 1.0f,
	};

	Becketron::Ref<Becketron::VertexBuffer> vertexBuffer;
	// -- reset is kinda like an alternative to make_unique (lookitup)
	vertexBuffer = Becketron::VertexBuffer::Create(vertices, sizeof(vertices));

	Becketron::BufferLayout layout = {
		{ Becketron::ShaderDataType::Float3, "a_Position" },
		{ Becketron::ShaderDataType::Float4, "a_Color"},
	};

	vertexBuffer->SetLayout(layout);
	m_VertexArray->AddVertexBuffer(vertexBuffer);

	uint32_t indices[3] = { 0, 1, 2 };
	Becketron::Ref<Becketron::IndexBuffer> indexBuffer;
	indexBuffer = Becketron::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));
	m_VertexArray->SetIndexBuffer(indexBuffer);

	m_SquareVA = Becketron::VertexArray::Create();

	float squareVertices[5 * 4] = {
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
	};

	Becketron::Ref<Becketron::VertexBuffer> squareVB;
	squareVB = Becketron::VertexBuffer::Create(squareVertices, sizeof(squareVertices));

	Becketron::BufferLayout squareVBLayout = {
		{ Becketron::ShaderDataType::Float3, "a_Position" },
		{ Becketron::ShaderDataType::Float2, "a_TextCoord"}
	};
	squareVB->SetLayout(squareVBLayout);
	m_SquareVA->AddVertexBuffer(squareVB);

	unsigned int squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
	Becketron::Ref<Becketron::IndexBuffer> squareIB;
	squareIB = Becketron::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));
	m_SquareVA->SetIndexBuffer(squareIB);

	std::string vertexSrc = R"(
		#version 330 core
			
		layout(location = 0) in vec3 a_Position;
		layout(location = 1) in vec4 a_Color;
			
		uniform mat4 u_ViewProjection;
		uniform mat4 u_Transform;

		out vec3 v_Position;
		out vec4 v_Color;			

		void main()
		{
			v_Position = a_Position;
			v_Color = a_Color;
			gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
		}		
	)";

	std::string fragmentSrc = R"(
		#version 330 core
			
		layout(location = 0) out vec4 color;			

		in vec3 v_Position;			
		in vec4 v_Color;			

		void main()
		{
			color = vec4(v_Position * 0.5 + 0.5, 1.0);
			color = v_Color;
		}		
	)";

	m_Shader = Becketron::Shader::Create("Shader_1", vertexSrc, fragmentSrc);

	std::string flatShader_vertexSrc = R"(
		#version 330 core
			
		layout(location = 0) in vec3 a_Position;
			
		uniform mat4 u_ViewProjection;
		uniform mat4 u_Transform;

		out vec3 v_Position;

		void main()
		{
			v_Position = a_Position;
			gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
		}		
	)";

	std::string flatShader_fragmentSrc = R"(
		#version 330 core
			
		layout(location = 0) out vec4 color;
			
		in vec3 v_Position;

		uniform vec3 u_Color;

		void main()
		{
			color = vec4(u_Color, 1.0);
		}
	)";

	m_FlatShader = Becketron::Shader::Create("flat_Shader", flatShader_vertexSrc, flatShader_fragmentSrc);

	auto textureShader = m_ShaderLibrary.Load("assets/shaders/Texture.glsl");

	m_Texture = Becketron::Texture2D::Create("assets/textures/paint_splash.jpg");
	m_MoonTexture = Becketron::Texture2D::Create("assets/textures/moon.png");

	textureShader->Bind();
	textureShader->SetInt("u_Texture", 0);
}

void ExampleLayer::OnAttach()
{
}

void ExampleLayer::OnDetach()
{
}

void ExampleLayer::OnUpdate(Becketron::Timestep ts) 
{
	
	//BT_TRACE("Delta time: {0}s ({1}ms)", ts.GetSeconds(), ts.GetMilliseconds());

	//Update
	m_CameraController.OnUpdate(ts);

	// REnder
	Becketron::RenderCommand::SetClearColor({ 0.2f, 0.2f, 0.2f, 1.0f });
	Becketron::RenderCommand::Clear();

	Becketron::Renderer::BeginScene(m_CameraController.GetCamera());

	//making scale static since we don't need to calculate it all the time
	static glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

	m_FlatShader->Bind();
	m_FlatShader->SetInt("u_Texture", 0);

	for (int y = 0; y < 20; y++)
	{
		for (int x = 0; x < 20; x++)
		{
			glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
			glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
			Becketron::Renderer::Submit(m_FlatShader, m_SquareVA, transform);
		}
	}

	auto textureShader = m_ShaderLibrary.Get("Texture");

	m_Texture->Bind();
	glm::mat4 scale2 = glm::scale(glm::mat4(1.0f), glm::vec3(1.5f));
	Becketron::Renderer::Submit(textureShader, m_SquareVA, scale2);
	m_MoonTexture->Bind();
	Becketron::Renderer::Submit(textureShader, m_SquareVA, scale2);

	//Becketron::Renderer::Submit(m_Shader, m_VertexArray);
	Becketron::Renderer::EndScene();

}

void ExampleLayer::OnImGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));
	ImGui::End();
}

void ExampleLayer::OnEvent(Becketron::Event& e)
{
	m_CameraController.OnEvent(e);
}

