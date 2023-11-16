#include "UserInterface.h"

#include <fstream>
#include <string>
#include <thread>
#include <chrono>

#define CLEAR_SCREEN "\033[2J\033[1;1H" 
#define LOG ""

void UserInterface::GetMenu() const
{
	std::cout << "************ USER INTERFACE ************\n\n";
	std::cout << "OPTIONS:\n";
	std::cout << "Toggle led?                     Enter '1'\n";
	std::cout << "Read current temperature?       Enter '2'\n";
	std::cout << "Change temperature threshold?   Enter '3'\n";
	std::cout << "Read temperature threshold?     Enter '4'\n";
	std::cout << "Quit?                           Enter '0'\n";
	std::cout << std::endl;
}

void UserInterface::LogEvents()
{
	std::ofstream log("log.txt");
	{
		std::unique_lock<std::mutex> lock(mutex_);
		condition_.wait(lock, [&]() { return !pauseLogEvents_; });
	}

	char c{ '2' };
	m_Serial->SendData(&c, 1);
	LoadTempToBuffer();
	std::string logLine = "Temperature: " + BufferToString();

	log << logLine << "\n";

	std::this_thread::sleep_for(std::chrono::milliseconds(3000));

	log.close();
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

void UserInterface::LoadTempToBuffer()
{	
	m_Serial->ReadDataWaiting();
	ClearBuffer();
	std::this_thread::sleep_for(std::chrono::milliseconds(50));
	m_Serial->ReadData(&m_Buffer, 10);
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

std::string UserInterface::BufferToString()
{
	std::string bufferString = "";

	for (int i = 0, l = m_Buffer.size(); i < l; i++)
	{
		bufferString += m_Buffer[i];
	}
	return bufferString;
}


void UserInterface::HandleInput()
{
	char input{}; // til input fra brugeren

	std::cout << "Input: ";
	std::cin >> input;

	switch (input)
	{
	case '1': //LED toggle til debugging
		m_Serial->SendData(&input, 1);

		std::cout << CLEAR_SCREEN;
		GetMenu();
		std::cout << "LED Toggled\n";
		break;
	case '2': // temp reading
		m_Serial->SendData(&input, 1);
		LoadTempToBuffer();

		std::cout << CLEAR_SCREEN;
		GetMenu();
		std::cout << "Current temperature: ";
		PrintBuffer();
		break;
	case '3':
		{
		std::lock_guard<std::mutex> lock(mutex_);
		pauseLogEvents_ = true;
		condition_.notify_one();
		}
		m_Serial->SendData(&input, 1);
		ChangeThreshold();

		{
			std::lock_guard<std::mutex> lock(mutex_);
			pauseLogEvents_ = false;
			condition_.notify_one();
		}

		std::cout << CLEAR_SCREEN;
		GetMenu();
		std::cout << "Threshold set\n";
		break;
	case '4':
		m_Serial->SendData(&input, 1);
		m_Serial->ReadDataWaiting();
		ClearBuffer();
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
		m_Serial->ReadData(&m_Buffer, 10);

		std::cout << CLEAR_SCREEN;
		GetMenu();
		std::cout << "Current temperature threshold: ";
		PrintBuffer();
		break;
	case '0':
		m_Running = false;
		break;
	default:
		std::cout << CLEAR_SCREEN;
		GetMenu();
		std::cout << "Invalid input...\n";
		break;
	}
}

