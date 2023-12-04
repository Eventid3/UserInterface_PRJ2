#include "SerialCommunicator.h"

#include <fstream>
#include <string>
#include <thread>
#include <chrono>
#include <iomanip>
#include <ctime>
#include <sstream>

SerialCommunicator::SerialCommunicator() 
{
	m_Buffer = { 0 };
}

SerialCommunicator::~SerialCommunicator()
{
}

void SerialCommunicator::LogEvents()
{
	// Open log in append mode
	std::ofstream log(LOG, std::ios_base::app);

	{
		std::lock_guard<std::mutex> lock(m_Mutex);
		char c{ '1' };
		SendData(&c, 1);
		LoadRecievedDataToBuffer();
	}

	log << GetCurrentDateTime() + ": Temperature: " + BufferToString() << "\n";

	for (auto i = 0; i < m_SleepDuration.count(); ++i)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}

	log.close();
}

std::string SerialCommunicator::GetCurrentDateTime()
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

std::string SerialCommunicator::BufferToString()
{
	std::string bufferString = "";

	for (int i = 0, l = m_Buffer.size(); i < l; i++)
	{
		bufferString += m_Buffer[i];
	}
	return bufferString;
}

void SerialCommunicator::ReadTemp()
{
	char command = '1';
	SendData(&command, 1);

	LoadRecievedDataToBuffer();
}

void SerialCommunicator::ChangeThreshold(float newThresh)
{
	char command = '2';
	std::lock_guard<std::mutex> lock(m_Mutex);
	SendData(&command, 1);

	char buffer[10];
	
	snprintf(buffer, sizeof(buffer), "%f", newThresh);
	//m_Os << buffer << std::endl;
	SendData(buffer, 10);
}

void SerialCommunicator::GetCurrentThreshold()
{
	char command = '3';

	SendData(&command, 1);
	LoadRecievedDataToBuffer();
}

void SerialCommunicator::ClearBuffer()
{
	m_Buffer.fill(0);
}

void SerialCommunicator::LoadRecievedDataToBuffer()
{
	ReadDataWaiting();
	ClearBuffer();
	std::this_thread::sleep_for(std::chrono::milliseconds(50));
	ReadData(&m_Buffer, 10);
}

void SerialCommunicator::Quit()
{
	m_SleepDuration = std::chrono::milliseconds(0);
}
