#pragma once
#include "SerialCommunicator.h"
#include <iostream>
#include <array>
#include <mutex>
#include <condition_variable>

class UserInterface
{
public:
	// Constructor
	UserInterface(int port, int baud, std::ostream& os) : m_Os(os)
	{
		Connect(port, baud);

		//m_Buffer = { 0 };
		m_Running = true;
	}

	~UserInterface()
	{
		m_SerialComm->Close();
		delete m_SerialComm;
	}

	void Connect(int port, int baud);
	void Reconnect();
	void GetMenu() const;
	void ResetUI(const std::string& message) const;
	//void LogEvents();
	//void ReadLog() const;
	//std::string GetCurrentDateTime();

	void PrintBuffer();
	//void ClearBuffer();
	//std::string BufferToString();

	bool isRunning() const { return m_Running; }

	void HandleInput();
	//void ChangeThreshold();

	//void LoadRecievedDataToBuffer();

	//CSerial* Serial() { return m_Serial; }
	SerialCommunicator* GetComm() { return m_SerialComm; }
private:
	SerialCommunicator* m_SerialComm = nullptr;
	//CSerial* m_Serial = nullptr; // UART communication
	bool m_Running;
	//std::array<char, 10> m_Buffer; //buffer for recieved data
	//std::mutex m_Mutex; // mutex for handeling multithreading
	//std::chrono::milliseconds m_SleepDuration{ 1000 }; // time between logs

	std::ostream& m_Os;
};