#include "UserInterface.h"

#include <fstream>
#include <string>
#include <thread>
#include <chrono>
#include <iomanip>
#include <ctime>
#include <sstream>

#define CLEAR_SCREEN "\033[2J\033[1;1H" 
#define LOG "log.txt"


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

std::string UserInterface::GetCurrentDateTime()
{
	// Get the current time point
	auto now = std::chrono::system_clock::now();

	// Convert it to time_t
	std::time_t currentTime = std::chrono::system_clock::to_time_t(now);

	// Initialize a struct tm with the local time
	std::tm localTimeInfo;
	localtime_s(&localTimeInfo, &currentTime);

	// Format the time as a string
	std::stringstream ss;
	ss << std::put_time(&localTimeInfo, "%d:%m:%Y %H:%M:%S");

	return ss.str();
}

void UserInterface::LogEvents()
{
	// Open log in append mode
	std::ofstream log(LOG, std::ios_base::app);
	
	// lock thread if 
	{
		std::unique_lock<std::mutex> lock(m_Mutex);
		m_Condition.wait(lock, [&]() { return !m_PauseLogEvents; });
		if (!m_Running)
			return;  // Break out of the loop immediately
	}

	char c{ '2' };
	m_Serial->SendData(&c, 1);
	LoadTempToBuffer();

	std::string logLine = GetCurrentDateTime() + ": Temperature: " + BufferToString();
	
	log << logLine << "\n";

	for (auto i = 0; i < m_SleepDuration.count(); ++i)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		{
			std::unique_lock<std::mutex> lock(m_Mutex);
			m_Condition.wait_for(lock, std::chrono::milliseconds(1), [&]() { return !m_PauseLogEvents; });
			if (!m_Running) {
				return;  // Break out of the loop immediately
			}
		}
	}

	log.close();
}

void UserInterface::ReadLog() const
{
	std::string logLine;

	std::ifstream log(LOG);

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
		std::lock_guard<std::mutex> lock(m_Mutex);
		m_PauseLogEvents = true;
		m_Condition.notify_one();
		}
		m_Serial->SendData(&input, 1);
		ChangeThreshold();

		{
			std::lock_guard<std::mutex> lock(m_Mutex);
			m_PauseLogEvents = false;
			m_Condition.notify_one();
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
		m_SleepDuration = std::chrono::milliseconds(0);
		m_Running = false;
		break;
	default:
		std::cout << CLEAR_SCREEN;
		GetMenu();
		std::cout << "Invalid input...\n";
		break;
	}
}

