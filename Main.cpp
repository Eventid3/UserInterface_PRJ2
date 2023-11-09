#include "UserInterface.h"

#define PORT 6
#define BAUD 9600

int main()
{
	UserInterface UI(PORT, BAUD);

	UI.GetMenu();

	//Axel er sej, Andreas er mindre sej 

	while (UI.isRunning())
	{
		UI.HandleInput();
	}

}