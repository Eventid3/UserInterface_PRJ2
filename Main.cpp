#include <thread>
#include "UserInterface.h"

#define PORT 6
#define BAUD 9600


int main()
{
	UserInterface UI(PORT, BAUD);

	std::thread inputThread([&]()
		{
			UI.GetMenu();

			while (UI.isRunning())
			{
				UI.HandleInput();
			}
		});

	std::thread logThread([&]()
		{
			while (UI.isRunning())
			{
				// Log events
				UI.LogEvents();
			}
		});

	inputThread.join();
	logThread.join();

}