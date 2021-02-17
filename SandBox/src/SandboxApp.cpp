#include <Becketron.h>

#include "imgui/imgui.h"

class ExampleLayer : public Becketron::Layer
{
public:
	ExampleLayer()
		: Layer("Example")
	{
	}

	void OnUpdate() override
	{
		//BT_INFO("ExampleLayer::Update");
		if (Becketron::Input::IsKeyPressed(BT_KEY_TAB))
			BT_WARN("Tab key is pressed");
	}

	virtual void OnImGuiRender() override
	{
		ImGui::Begin("Test");
		ImGui::Text("Hello World");
		ImGui::End();
	}

	void OnEvent(Becketron::Event& event) override
	{
		//BT_TRACE("{0}", event);
	}
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