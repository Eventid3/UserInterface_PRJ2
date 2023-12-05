#include <thread>
#include "UserInterface.h"

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
		if (ui->GetComm())
			ui->GetComm()->LogEvents();
	}
}


int main()
{
	UserInterface UI(6,9600,std::cout);

	std::thread inputThread(handleInput, &UI);
	std::thread logThread(logging, &UI);

	inputThread.join();
	logThread.join();

}