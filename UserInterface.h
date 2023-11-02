#pragma once
#include "Serial.h"

#include <iostream>

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

		m_Running = true;
	}

	~UserInterface()
	{
		m_Serial->Close();
		delete m_Serial;
	}

	void GetMenu() const;
	void ReadLog() const;

	bool isRunning() const { return m_Running; }

	void HandleInput();

	CSerial* Serial() { return m_Serial; }
private:
	CSerial* m_Serial;

	bool m_Running;

};