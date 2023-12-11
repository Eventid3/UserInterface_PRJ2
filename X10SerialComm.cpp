#include "X10SerialComm.h"

#include <fstream>
#include <string>
#include <thread>
#include <chrono>
#include <iomanip>
#include <ctime>
#include <sstream>
#include <stdexcept>

X10SerialComm::X10SerialComm() 
{
	m_Buffer = { 0 }; // Init af bufferen til 0-taller
}

X10SerialComm::~X10SerialComm(){}

void X10SerialComm::LogEvents()
{
	/* Denne funktion sørger for at logge temperaturen til en 
	log.txt fil, efter en sat tid i m_SleepDuration */

	if (this)
	{
		// Åbner loggen i append-mode
		std::ofstream log(LOG, std::ios_base::app);

		{
			std::lock_guard<std::mutex> lock(m_Mutex);
			char c{ '1' };
			SendData(&c, 1);
			LoadRecievedDataToBuffer();
		}

		if (BufferToString().empty())
		{
			log.close();
			return;
		}

		log << GetCurrentDateTime() + ": Temperature: " + BufferToString() << "\n";

		for (auto i = 0; i < m_SleepDuration; ++i)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}

		log.close();
	}
}

std::string X10SerialComm::GetCurrentDateTime()
{
	/*
	Denne funktion returnerer en string med tid og dato i en pænt format
	*/
	// Antal af sekunder siden the epoch
	auto now = std::chrono::system_clock::now();

	// Konverter til time_t
	std::time_t currentTime = std::chrono::system_clock::to_time_t(now);

	// Initializer til tm format
	std::tm localTimeInfo;
	localtime_s(&localTimeInfo, &currentTime);

	// Formater tm'en som en string
	std::stringstream ss;
	ss << std::put_time(&localTimeInfo, "%d:%m:%Y %H:%M:%S");

	return ss.str();
}

std::string X10SerialComm::BufferToString()
{
	/*
	Returnerer dataen fra bufferen som en string
	*/
	std::string bufferString = "";

	for (size_t i = 0, l = m_Buffer.size(); i < l; i++)
	{
		bufferString += m_Buffer[i];
	}
	return bufferString;
}

void X10SerialComm::ReadTemp()
{
	/*
	Sender kommando til SystemControlleren 
	om at få den nuværende temperatur
	*/

	char command = '1';
	SendData(&command, 1);

	LoadRecievedDataToBuffer();
}

void X10SerialComm::ChangeThreshold(float newThresh)
{
	/*
	Sender kommando til SystemControlleren om at 
	ændre tærskelværdien for temperatursensoren
	*/

	char command = '2';
	std::lock_guard<std::mutex> lock(m_Mutex);
	SendData(&command, 1);

	char buffer[10];

	LoadRecievedDataToBuffer();

	snprintf(buffer, sizeof(buffer), "%f", newThresh);
	SendData(buffer, 10);
}

void X10SerialComm::GetCurrentThreshold()
{
	/*Sender kommando til SystemControlleren om at sende 
	den nuværende tærskelværdi for temperatursensoren*/
	char command = '3';

	SendData(&command, 1);
	LoadRecievedDataToBuffer();
}

void X10SerialComm::ClearBuffer()
{
	/*
	Skriver 0 til alle pladser i bufferen.
	*/
	m_Buffer.fill(0);
}

void X10SerialComm::LoadRecievedDataToBuffer()
{
	/*
	Læser dataen modtaget via UART til bufferen.
	*/

	ReadDataWaiting(); // Hvis der er noget gammelt data, så læs det
	ClearBuffer(); 
	// Lille delay inden dataen indlæses
	std::this_thread::sleep_for(std::chrono::milliseconds(50)); 
	ReadData(&m_Buffer, 10); 
}

void X10SerialComm::Quit()
{
	/*
	Denne funktion sætter sleep duration til 0, så den thread 
	der håndterer loggen afsluttes med det samme.
	*/
	m_SleepDuration = 0;
}
