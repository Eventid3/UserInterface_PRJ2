#include "UserInterface.h"

#include <fstream>
#include <string>
#include <thread>
#include <chrono>

void UserInterface::GetMenu() const
{
	std::cout << "*** USER INTERFACE***\n";
	std::cout << "OPTIONS:\n";
	std::cout << "Toggle led?  Enter 'a'\n";
	std::cout << "Read data?   Enter 'l'\n";
	std::cout << "Get menu?    Enter 'm'\n";
	std::cout << "Quit?        Enter 'q'\n";
	std::cout << std::endl;
}

void UserInterface::ReadLog() const
{
	std::string logLine;

	std::ifstream log("log.txt");

	while (std::getline(log, logLine)) 
	{
		std::cout << logLine << "\n";
	}

	log.close();
}

void UserInterface::PrintBuffer()
{
	for (int i = 0, l = m_Buffer.size(); i < l; i++)
	{
		std::cout << m_Buffer[i];
	}
	std::cout << std::endl;
}


void UserInterface::HandleInput()
{
	char input{}; // til input fra brugeren

	std::cout << "Input: ";
	std::cin >> input;

	switch (input)
	{
	case 'a':
	case 'A':
		m_Serial->SendData(&input, 1);
		break;
	case 'l':
	case 'L':
		m_Serial->SendData(&input, 1);
		m_Serial->ReadDataWaiting(); 
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
		m_Serial->ReadData(&m_Buffer, 10);
		PrintBuffer();
		break;
	case 'q':
	case 'Q':
		m_Running = false;
		break;
	case 'm':
	case 'M':
		GetMenu();
		break;
	default:
		break;
	}
}

