#include "btpch.h"
#include "Cubemap.h"

#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>
#include <GLAD/glad.h>
#include <stb_image/stb_image.h>

namespace Becketron
{
	struct CubeVertex
	{
		glm::vec3 Position;
	};

    struct CubemapData
    {
        /*
        Ref<VertexArray>	CubeVertexArray;
        Ref<VertexBuffer>	CubeVertexBuffer;
        Ref<Shader>			CubemapShader;
        Ref<CubemapTex>		Texture;
        */

        unsigned int skyboxVAO;
        unsigned int skyboxVBO;
        unsigned int cubemapTexture;
        Ref<Shader>  skyboxShader;
        
    };

    CubemapData m_Data;

	void Cubemap::Init()
	{
		BT_PROFILE_FUNCTION();

        float skyboxVertices[] = {
            // positions          
            -1.0f,  1.0f, -1.0f,
            -1.0f, -1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,
             1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

             1.0f, -1.0f, -1.0f,
             1.0f, -1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

            -1.0f,  1.0f, -1.0f,
             1.0f,  1.0f, -1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
             1.0f, -1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
             1.0f, -1.0f,  1.0f
        };

        /*
		m_Data.CubeVertexArray = VertexArray::Create();
		m_Data.CubeVertexBuffer = VertexBuffer::Create(skyboxVertices, sizeof(CubeVertex));

		m_Data.CubeVertexBuffer->SetLayout({
			{ShaderDataType::Float3, "a_Position"}
			});
		m_Data.CubeVertexArray->AddVertexBuffer(m_Data.CubeVertexBuffer);

		m_Data.CubemapShader = Shader::Create("assets/shaders/Skybox.glsl");
		m_Data.CubemapShader->Bind();
		m_Data.CubemapShader->SetInt("u_Skybox", 0);
        */

        glGenVertexArrays(1, &m_Data.skyboxVAO);
        glGenBuffers(1, &m_Data.skyboxVBO);
        glBindVertexArray(m_Data.skyboxVAO);
        glBindBuffer(GL_ARRAY_BUFFER, m_Data.skyboxVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);



        std::vector<std::string> faces
        {
            "assets/textures/skybox/right.jpg",
            "assets/textures/skybox/left.jpg",
            "assets/textures/skybox/top.jpg",
            "assets/textures/skybox/bottom.jpg",
            "assets/textures/skybox/front.jpg",
            "assets/textures/skybox/back.jpg"
        };

        m_Data.cubemapTexture = loadCubemap(faces);

        m_Data.skyboxShader = Shader::Create("assets/shaders/Skybox.glsl");

        m_Data.skyboxShader->Bind();
        m_Data.skyboxShader->SetInt("u_Skybox", 0);
        
       // m_Data.Texture = CubemapTex::Create(faces);

	}

    void Cubemap::RenderSkybox(const PerspectiveCamera& camera)
    {
        BT_PROFILE_FUNCTION();

        // draw skybox as last
        glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
        m_Data.skyboxShader->Bind();
        glm::mat4 view = glm::mat4(glm::mat3(camera.GetViewMatrix())); // remove translation from the view matrix
        m_Data.skyboxShader->SetMat4("view", view);
        m_Data.skyboxShader->SetMat4("projection", camera.GetProjectionMatrix());
        // skybox cube
        glBindVertexArray(m_Data.skyboxVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, m_Data.cubemapTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        glDepthFunc(GL_LESS); // set depth function back to default
       
        /*
        //RenderCommand::SetDepthMask(false);

        m_Data.CubeVertexArray->Bind();

        m_Data.CubemapShader->Bind();
        m_Data.CubemapShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
        //m_Data.CubemapShader->SetFloat3("lightPos", s_Data3D.lightPos);
        //m_Data.CubemapShader->SetFloat4("lightColor", s_Data3D.lightColor);
        //m_Data.CubemapShader->SetFloat3("u_ViewPos", camera.GetPosition());
        
        m_Data.Texture->Bind(m_Data.Texture->GetRendererID());

        RenderCommand::DrawCubemap();
        //RenderCommand::SetDepthMask(true);

        m_Data.CubeVertexArray->Unbind();
        m_Data.CubemapShader->Unbind();
        */
    }

    void Cubemap::RenderSkybox(const Camera& camera, const glm::mat4& transform)
    {
        BT_PROFILE_FUNCTION();

        // draw skybox as last
        glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
        m_Data.skyboxShader->Bind();
        glm::mat4 view = glm::inverse(transform);
        view = glm::mat4(glm::mat3(view)); // remove translation from the view matrix
        m_Data.skyboxShader->SetMat4("view", view);
        m_Data.skyboxShader->SetMat4("projection", camera.GetProjection());
        // skybox cube
        glBindVertexArray(m_Data.skyboxVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, m_Data.cubemapTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        glDepthFunc(GL_LESS); // set depth function back to default
    }

    unsigned int Cubemap::loadCubemap(std::vector<std::string>& faces)
    {
        unsigned int textureID;
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

        int width, height, nrChannels;
        for (unsigned int i = 0; i < faces.size(); i++)
        {
            unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
            if (data)
            {
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
                stbi_image_free(data);
            }
            else
            {
                std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
                stbi_image_free(data);
            }
        }
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        return textureID;
    }
}
