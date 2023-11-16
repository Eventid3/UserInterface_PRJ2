#pragma once
#include "Serial.h"

#include <iostream>
#include <array>
#include <mutex>
#include <condition_variable>

class UserInterface
{
public:
	// Constructor
	UserInterface(int port, int baud)
	{
		m_Serial = new CSerial();

		if (!m_Serial->Open(port, baud))
		{
			std::cout << "Could not open COM" << port << std::endl;
			delete m_Serial;
			exit(-1);
		}
		else
		{
			std::cout << "Serial communication opened on Port " << port << std::endl;
		}

		m_Buffer = { 0 };
		m_Running = true;
	}

	~UserInterface()
	{
		m_Serial->Close();
		delete m_Serial;
	}

	void GetMenu() const;
	void LogEvents();
	void ReadLog() const;

	void PrintBuffer();
	void ClearBuffer();
	std::string BufferToString();

	bool isRunning() const { return m_Running; }

	void HandleInput();
	void ChangeThreshold();

	void LoadTempToBuffer();

	CSerial* Serial() { return m_Serial; }

private:
	CSerial* m_Serial;

	bool m_Running;

	std::array<char, 10> m_Buffer;

	std::mutex mutex_;
	std::condition_variable condition_;
	bool pauseLogEvents_ = false;

};