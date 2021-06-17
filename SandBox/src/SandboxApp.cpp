#include <Becketron.h>
#include <Becketron/Core/EntryPoint.h>

#include <imgui/imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Sandbox3D.h"
#include "ExampleLayer.h"

#include "Becketron/Renderer/Shader.h"

class Sandbox : public Becketron::Application
{
public:
	Sandbox()
	{
		//PushLayer(new ExampleLayer());
		PushLayer(new Sandbox3D());
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