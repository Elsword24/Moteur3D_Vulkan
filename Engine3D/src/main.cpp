#include "vulkan.h"

//int main()
//{
//	//step0();
//	return 0;
//}

int main()
{
	VulkanRAII app;

	try
	{
		app.run();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}