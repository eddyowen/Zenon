#include "Core/Application.h"

int main(int argc, char *argv[])
{
    using namespace zn;
    
	SharedPtr<Application> app = CreateShared<Application>();
	if (app->Init())
	{
		app->Run();
	}

	return 0;
}