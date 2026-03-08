#include <cstdint>
#include <stdexcept>
#include <iostream>

#include "Engine.hpp"

constexpr uint32_t WIDTH = 800;
constexpr uint32_t HEIGHT = 600;

int main()
{
	
	try
	{
		EngineQVY Game("RailShooter", WIDTH, HEIGHT);
		Game.RunGameLoop();
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}