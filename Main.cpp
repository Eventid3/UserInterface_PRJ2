#include "UserInterface.h"

#define PORT 6
#define BAUD 9600

int main()
{
	UserInterface UI(PORT, BAUD);

	UI.GetMenu();

	while (UI.isRunning())
	{
		UI.HandleInput();
	}
	//I was here
}