#pragma once
#include "X10SerialComm.h"
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

		m_Running = true;
	}

	// Destructor
	~UserInterface()
	{
		m_SerialComm->Close();
		delete m_SerialComm;
	}

	void Connect(int port, int baud);
	void Reconnect();
	void GetMenu() const;
	void ResetUI(const std::string& message) const;

	void PrintBuffer();

	bool isRunning() const { return m_Running; }

	void HandleInput();

	X10SerialComm* GetComm() { return m_SerialComm; }
private:
	X10SerialComm* m_SerialComm = nullptr;
	bool m_Running;
	std::ostream& m_Os;
};