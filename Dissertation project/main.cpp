
#include "src/Simulation.h"

int main()
{

	Simulation simulation;
	if (!simulation.init())
	{
		std::cerr << "Failed to initialise simulation";
		return 1;
	}
	simulation.run();

	return 0;
}
