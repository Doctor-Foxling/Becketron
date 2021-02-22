#pragma once

#include <string>
#include <glm/glm.hpp>

namespace Becketron {

	class Shader
	{
	public:
		static Shader* CreateShader(const std::string& vertexSrc, const std::string& fragmentSrc);
		virtual ~Shader() {};

		virtual void Bind() const {};
		virtual void Unbind() const {};

		virtual void UploadUniformMat4(const std::string& name, const glm::mat4& matrix) {};
	};
}