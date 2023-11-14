#include "UserInterface.h"

#include <fstream>
#include <string>
#include <thread>
#include <chrono>

void UserInterface::GetMenu() const
{
	std::cout << "*** USER INTERFACE***\n";
	std::cout << "OPTIONS:\n";
	std::cout << "Toggle led?                     Enter '1'\n";
	std::cout << "Read current temperature?       Enter '2'\n";
	std::cout << "Change temperature threshold?   Enter '3'\n";
	std::cout << "Read temperature threshold?     Enter '4'\n";
	std::cout << "Get menu?                       Enter '9'\n";
	std::cout << "Quit?                           Enter '0'\n";
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

void UserInterface::ChangeThreshold()
{
	float input{};
	std::cout << "Enter new temperature threshold: ";
	std::cin >> input;

	//TODO check for valid input

	char buffer[10];

	snprintf(buffer, sizeof(buffer), "%f", input);
	std::cout << buffer << std::endl;
	m_Serial->SendData(buffer, 10);
}

void UserInterface::PrintBuffer()
{
	for (int i = 0, l = m_Buffer.size(); i < l; i++)
	{
		std::cout << m_Buffer[i];
	}
	std::cout << std::endl;
}

void UserInterface::ClearBuffer()
{
	m_Buffer.fill(0);
}


void UserInterface::HandleInput()
{
	char input{}; // til input fra brugeren

	std::cout << "Input: ";
	std::cin >> input;

	switch (input)
	{
	case '1':
		m_Serial->SendData(&input, 1);
		break;
	case '2':
		m_Serial->SendData(&input, 1);
		m_Serial->ReadDataWaiting(); 
		ClearBuffer();
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
		m_Serial->ReadData(&m_Buffer, 10);
		PrintBuffer();
		break;
	case '3':
		m_Serial->SendData(&input, 1);
		ChangeThreshold();
		break;
	case '4':
		m_Serial->SendData(&input, 1);
		m_Serial->ReadDataWaiting();
		ClearBuffer();
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
		m_Serial->ReadData(&m_Buffer, 10);
		PrintBuffer();
		break;
	case '9':
		GetMenu();
		break;
	case '0':
		m_Running = false;
		break;
	default:
		break;
	}
}

