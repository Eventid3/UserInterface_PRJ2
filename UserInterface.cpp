#include "UserInterface.h"

#include <fstream>
#include <string>
#include <thread>
#include <chrono>
#include <iomanip>
#include <ctime>
#include <sstream>

#define CLEAR_SCREEN "\033[2J\033[1;1H" 


void UserInterface::Connect(int port, int baud)
{
	/*
	Åbner en port til UART communikation.
	*/
	if (m_SerialComm != nullptr)
		delete m_SerialComm;

	m_SerialComm = new X10SerialComm();

	if (!m_SerialComm->Open(port, baud))
	{
		m_Os << "Could not open COM" << port << std::endl;
		delete m_SerialComm;
		m_SerialComm = nullptr;
	}
	else
	{
		m_Os << "Serial communication opened on Port " << port << std::endl;
	}
}

void UserInterface::Reconnect()
{
	/*
	Denne funktion bruges til at reconnecte til en UART enhed hvis 
	der ikke er indtastet den rigtige COM port og/eller baud-rate
	*/

	int port, baud;
	m_Os << "Enter new port: ";
	std::cin >> port;

	m_Os << "Enter new baudrate: ";
	std::cin >> baud;

	Connect(port, baud);
}

void UserInterface::GetMenu() const
{
	/*
	Printer menuen til brugeren.
	*/
	m_Os << "************ USER INTERFACE ************\n\n";
	m_Os << "OPTIONS:\n";
	m_Os << "Read current temperature?       Enter '1'\n";
	m_Os << "Change temperature threshold?   Enter '2'\n";
	m_Os << "Read temperature threshold?     Enter '3'\n";
	m_Os << "Reconnect serial deivce?        Enter '9'\n";
	m_Os << "Quit?                           Enter '0'\n";
	m_Os << std::endl;
}


void UserInterface::ResetUI(const std::string& message) const
{
	/*
	Resetter UI'en så der ikke scrolles ned i konsolvinduet i det uendelige.
	*/
	m_Os << CLEAR_SCREEN;
	GetMenu();
	std::cout << message;
}



void UserInterface::PrintBuffer()
{
	/*
	Printer bufferen i SerialComm'en ud.
	*/
	for (size_t i = 0, l = m_SerialComm->GetBuffer()->size(); i < l; i++)
	{
		m_Os << (*m_SerialComm->GetBuffer())[i];
	}
	m_Os << std::endl;
}


void UserInterface::HandleInput()
{
	char input{}; // Til input fra brugeren

	m_Os << "Input: ";
	std::cin >> input;

	switch (input)
	{

	case '1': // temperatur læsning
		m_SerialComm->ReadTemp();

		ResetUI("Current temperature: ");
		PrintBuffer();
		break;

	case '2': // Indtast en ny tærskelværdi for temperatursensoren | UseCase 2
		float newThresh;
		m_Os << "Enter new temperature threshold: ";
		std::cin >> newThresh;
		m_SerialComm->ChangeThreshold(newThresh);
		ResetUI("Threshold set to: ");

		m_SerialComm->LoadRecievedDataToBuffer();
		PrintBuffer();
		break;

	case '3': // Aflæs den nuværende tærskelværdi for temperatursensoren
		m_SerialComm->GetCurrentThreshold();

		ResetUI("Current temperature threshold: ");
		PrintBuffer();
		break;

	case '9': // Reconnect til et UART device
		int port, baud;
		m_Os << "Enter new port: ";
		std::cin >> port;

		m_Os << "Enter new baudrate: ";
		std::cin >> baud;

		ResetUI("");
		Connect(port, baud);

		break;

	case '0': // Afslut programmet
		m_SerialComm->Quit();
		m_Running = false;
		break;

	default: 
		ResetUI("Invalid input...\n");
		break;
	}
}

