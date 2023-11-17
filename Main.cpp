#include <thread>
#include "UserInterface.h"

#define PORT 6
#define BAUD 9600

void handleInput(UserInterface* ui)
{
	ui->GetMenu();

	while (ui->isRunning())
	{
		ui->HandleInput();
	}
}

void logging(UserInterface* ui)
{
	while (ui->isRunning())
	{
		// Log events
		ui->LogEvents();
	}
}


int main()
{
	UserInterface UI(PORT, BAUD);

	std::thread inputThread(handleInput, &UI);
	std::thread logThread(logging, &UI);

	inputThread.join();
	logThread.join();

}