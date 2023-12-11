#pragma once

#include <string>
#include <array>
#include <mutex>
#include "Serial.h"

#define LOG "log.txt"


class X10SerialComm : public CSerial
{
public:
	X10SerialComm();
	~X10SerialComm();

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
	int m_SleepDuration{ 3000 }; // time between logs
};