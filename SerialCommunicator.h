#pragma once

#include <string>
#include <array>
#include <mutex>
#include "Serial.h"

#define LOG "log.txt"


class SerialCommunicator : public CSerial
{
public:
	SerialCommunicator();
	~SerialCommunicator();

	void LogEvents();

	std::string GetCurrentDateTime();

	std::string BufferToString();

	void ReadTemp();
	void ChangeThreshold(float newThresh);
	void GetCurrentThreshold();

	void ClearBuffer();
	void LoadRecievedDataToBuffer();

	std::array<char,10>* GetBuffer(){ return &m_Buffer; }

	void Quit();
private:
	std::array<char, 10> m_Buffer; //buffer for recieved data
	std::mutex m_Mutex; // mutex for handeling multithreading
	std::chrono::milliseconds m_SleepDuration{ 1000 }; // time between logs
};