#include <Becketron.h>

class Sandbox : public Becketron::Application
{
public:
	Sandbox()
	{

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