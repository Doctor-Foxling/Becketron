#pragma once

#ifdef BT_PLATFORM_WINDOWS

extern Becketron::Application* Becketron::CreateApplication();

int main(int argc, char** argv)
{ 
	Becketron::Log::Init();
	BT_CORE_WARN("Initialized Log!");
	int a = 0;
	BT_INFO("Hello VAr={0}", a);

	auto app = Becketron::CreateApplication();
	app->Run();
	delete app;
}


#else
	#err or Becketron only supports Windows

#endif