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
	m_Os << "************ USER INTERFACE ************\n\n";
	m_Os << "OPTIONS:\n";
	m_Os << "Read current temperature?       Enter '1'\n";
	m_Os << "Change temperature threshold?   Enter '2'\n";
	m_Os << "Read temperature threshold?     Enter '3'\n";
	m_Os << "Quit?                           Enter '0'\n";
	m_Os << std::endl;
}


void UserInterface::ResetUI(const std::string& message) const
{
	m_Os << CLEAR_SCREEN;
	GetMenu();
	std::cout << message;
}


std::string UserInterface::GetCurrentDateTime()
{
	// Time since epoch
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

	{
		std::lock_guard<std::mutex> lock(m_Mutex);
		char c{ '1' };
		m_Serial->SendData(&c, 1);
		LoadRecievedDataToBuffer();
	}

	log << GetCurrentDateTime() + ": Temperature: " + BufferToString() << "\n";

	for (auto i = 0; i < m_SleepDuration.count(); ++i)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}

	log.close();
}


void UserInterface::ReadLog() const
{
	std::string logLine;

	std::ifstream log(LOG);

	while (std::getline(log, logLine)) 
	{
		m_Os << logLine << "\n";
	}

	log.close();
}


void UserInterface::ChangeThreshold()
{
	float input{};
	m_Os << "Enter new temperature threshold: ";
	std::cin >> input;

	//TODO check for valid input

	char buffer[10];

	snprintf(buffer, sizeof(buffer), "%f", input);
	m_Os << buffer << std::endl;
	m_Serial->SendData(buffer, 10);
}


void UserInterface::LoadRecievedDataToBuffer()
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
		m_Os << m_Buffer[i];
	}
	m_Os << std::endl;
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

	m_Os << "Input: ";
	std::cin >> input;

	switch (input)
	{

	case '1': // temp reading
		m_Serial->SendData(&input, 1);

		LoadRecievedDataToBuffer();

		ResetUI("Current temperature: ");
		PrintBuffer();
		break;

	case '2': // Enter new threshold
		{
			std::lock_guard<std::mutex> lock(m_Mutex);
			m_Serial->SendData(&input, 1);
			ChangeThreshold();
		}

		ResetUI("New threshold set!\n");
		break;

	case '3': // Get Current threshold
		m_Serial->SendData(&input, 1);

		LoadRecievedDataToBuffer();

		ResetUI("Current temperature threshold: ");
		PrintBuffer();
		break;

	case '0': // Terminate program
		m_SleepDuration = std::chrono::milliseconds(0);
		m_Running = false;
		break;

	default: 
		ResetUI("Invalid input...\n");
		break;
	}
}

