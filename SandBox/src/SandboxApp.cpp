#include <Becketron.h>

#include "imgui/imgui.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Platform/OpenGL/OpenGLShader.h"

class ExampleLayer : public Becketron::Layer
{
public:
	ExampleLayer()
		: Layer("Example"), m_Camera(-1.6, 1.6f, -0.9f, 0.9f)
	{
		m_VertexArray.reset(Becketron::VertexArray::Create());

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.5f, 0.8f, 1.0f,
			 0.5f, -0.5f, 0.0f,	0.0f, 0.4f, 0.7f, 1.0f,
			 0.0f, 0.5f, 0.0f,	1.0f, 1.0f, 0.0f, 1.0f,
		};

		Becketron::Ref<Becketron::VertexBuffer> vertexBuffer;
		// -- reset is kinda like an alternative to make_unique (lookitup)
		vertexBuffer.reset(Becketron::VertexBuffer::Create(vertices, sizeof(vertices)));

		Becketron::BufferLayout layout = {
			{ Becketron::ShaderDataType::Float3, "a_Position" },
			{ Becketron::ShaderDataType::Float4, "a_Color"},
		};

		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);

		unsigned int indices[3] = { 0, 1, 2 };
		Becketron::Ref<Becketron::IndexBuffer> indexBuffer;
		indexBuffer.reset(Becketron::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(indexBuffer);

		m_SquareVA.reset(Becketron::VertexArray::Create());

		float squareVertices[5 * 4] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			  0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
			 -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
		};

		Becketron::Ref<Becketron::VertexBuffer> squareVB;
		squareVB.reset(Becketron::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));

		Becketron::BufferLayout squareVBLayout = {
			{ Becketron::ShaderDataType::Float3, "a_Position" },
			{ Becketron::ShaderDataType::Float2, "a_TextCoord"}
		};
		squareVB->SetLayout(squareVBLayout);
		m_SquareVA->AddVertexBuffer(squareVB);

		unsigned int squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		Becketron::Ref<Becketron::IndexBuffer> squareIB;
		squareIB.reset(Becketron::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
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

		m_Shader.reset(Becketron::Shader::CreateShader(vertexSrc, fragmentSrc));

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

		m_FlatShader.reset(Becketron::Shader::CreateShader(flatShader_vertexSrc, flatShader_fragmentSrc));

		std::string textureShader_vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec2 a_TexCoord;			

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec2 v_TexCoord;

			void main()
			{
				v_TexCoord = a_TexCoord;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
			}		
		)";

		std::string textureShader_fragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;
			
			in vec2 v_TexCoord;			

			uniform sampler2D u_Texture;

			void main()
			{
				color = texture(u_Texture, v_TexCoord); 
			}		
		)";

		m_TextureShader.reset(Becketron::Shader::CreateShader(textureShader_vertexSrc, textureShader_fragmentSrc));

		m_Texture = Becketron::Texture2D::Create("assets/textures/paint_splash.jpg");

		std::dynamic_pointer_cast<Becketron::OpenGLShader>(m_TextureShader)->Bind();
		std::dynamic_pointer_cast<Becketron::OpenGLShader>(m_TextureShader)->UploadUniformInt(
			"u_Texture", 0);

	}

	void OnUpdate(Becketron::Timestep ts) override
	{
		BT_TRACE("Delta time: {0}s ({1}ms)", ts.GetSeconds(), ts.GetMilliseconds());

		Becketron::RenderCommand::SetClearColor({ 0.2f, 0.2f, 0.2f, 1.0f });
		Becketron::RenderCommand::Clear();

		if (Becketron::Input::IsKeyPressed(BT_KEY_UP))
		{
			m_camPos.y += m_camMovSpeed * ts; 
		}
		else if (Becketron::Input::IsKeyPressed(BT_KEY_DOWN))
		{
			m_camPos.y -= m_camMovSpeed * ts;
		}
		if (Becketron::Input::IsKeyPressed(BT_KEY_LEFT))
		{
			m_camPos.x -= m_camMovSpeed * ts;
		}
		else if (Becketron::Input::IsKeyPressed(BT_KEY_RIGHT))
		{
			m_camPos.x += m_camMovSpeed * ts;
		}

		if (Becketron::Input::IsKeyPressed(BT_KEY_A))
			m_camRotation += m_camRotSpeed * ts;
		else if (Becketron::Input::IsKeyPressed(BT_KEY_D))
			m_camRotation -= m_camRotSpeed * ts;

		m_Camera.SetPosition(m_camPos);
		m_Camera.SetRotation(m_camRotation);

		Becketron::Renderer::BeginScene(m_Camera);

		//making scale static since we don't need to calculate it all the time
		static glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		std::dynamic_pointer_cast<Becketron::OpenGLShader>(m_FlatShader)->Bind();
		std::dynamic_pointer_cast<Becketron::OpenGLShader>(m_FlatShader)->UploadUniformFloat3(
			"u_Color", m_SquareColor);

		for (int y = 0; y < 20; y++)
		{
			for (int x = 0; x < 20; x++)
			{
				glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
				Becketron::Renderer::Submit(m_FlatShader, m_SquareVA, transform);
			}
		}

		m_Texture->Bind();
		glm::mat4 scale2 = glm::scale(glm::mat4(1.0f), glm::vec3(1.5f));
		Becketron::Renderer::Submit(m_TextureShader, m_SquareVA, scale2);

		//Becketron::Renderer::Submit(m_Shader, m_VertexArray);
		Becketron::Renderer::EndScene();

	}

	virtual void OnImGuiRender() override
	{
		ImGui::Begin("Settings");
		ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));
		ImGui::End();
	}

	void OnEvent(Becketron::Event& event) override
	{
		//Becketron::EventDispatcher dispatcher(event);
		//dispatcher.Dispatch<Becketron::KeyPressedEvent>(BT_BIND_EVENT_FN(ExampleLayer::OnKeyPressedEvent));
	}

	bool OnKeyPressedEvent(Becketron::KeyPressedEvent& event)
	{
		switch (event.GetKeyCode())
		{
			case BT_KEY_UP: m_camPos.y += 0.1f; return false;
			case BT_KEY_DOWN: m_camPos.y -= 0.1f; return false;
			case BT_KEY_RIGHT: m_camPos.x += 0.1f; return false;
			case BT_KEY_LEFT: m_camPos.x -= 0.1f; return false; 
		}
		return false;
	}


private:
	Becketron::Ref<Becketron::Shader> m_Shader;
	Becketron::Ref<Becketron::Shader> m_FlatShader, m_TextureShader;
	Becketron::Ref<Becketron::VertexArray> m_VertexArray;

	Becketron::Ref<Becketron::VertexArray> m_SquareVA;

	Becketron::Ref<Becketron::Texture2D> m_Texture;

	Becketron::OrthographicCamera m_Camera;
	glm::vec3 m_camPos = { 0.0f, 0.0f, 0.0f };
	float m_camMovSpeed = 5.0f;

	float m_camRotSpeed = 180.0f;
	float m_camRotation = 0.0f;

	glm::vec3 m_SquareColor = { 0.2f, 0.3f, 0.8f };
};

class Sandbox : public Becketron::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
	}

	~Sandbox()
	{

	}

};

Becketron::Application* Becketron::CreateApplication()
{
	return new Sandbox();
}



/*
int main()
{
	Sandbox* sandbox = new Sandbox();
	sandbox->Run();
	delete sandbox;
}
*/