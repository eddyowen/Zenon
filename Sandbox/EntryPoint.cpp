#include "Core/Application.h"

int main(int argc, char *argv[])
{
	zn::Application app{};
	if (app.Init())
	{
		app.Run();
	}

	return 0;
}