#include "Core/Application.hpp"

int main(int argc, char *argv[])
{
    using namespace zn;
    
	SharedPtr<Application> app = CreateShared<Application>();
	if (app->Init("Sandbox", 1980, 1080))
	{
		app->Run();
	}

	return 0;
}