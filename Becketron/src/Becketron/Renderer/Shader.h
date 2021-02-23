#pragma once

#include <string>

namespace Becketron {

	class Shader
	{
	public:
		static Shader* CreateShader(const std::string& vertexSrc, const std::string& fragmentSrc);
		virtual ~Shader() {} ;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
	};
}