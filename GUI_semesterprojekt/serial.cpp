#include "serial.h"
#include <iostream>
#include<string>

Serial::Serial()
{
    memset(&m_OverlappedRead, 0, sizeof(OVERLAPPED));
    memset(&m_OverlappedWrite, 0, sizeof(OVERLAPPED));
    m_hIDComDev = NULL;
    m_bOpened = false;
}
Serial::~Serial()
{

}
int Serial::ReadDataWaiting(void)
{

   if (!m_bOpened || m_hIDComDev == NULL) return(0);

   DWORD dwErrorFlags;
   COMSTAT ComStat;

   ClearCommError(m_hIDComDev, &dwErrorFlags, &ComStat);

   return((int)ComStat.cbInQue);
   // qDebug()<<"Test aprroved";
   // std::string TEST =" 23";
   // return (TEST);

}

BOOL Serial::Open(int nPort, int nBaud)
{

    if (m_bOpened) return(TRUE);

    wchar_t szPort[15];
    wchar_t szComParams[50];
    DCB dcb;

    wsprintf(szPort, L"COM%d", nPort);
    m_hIDComDev = CreateFile(szPort, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL);
    if (m_hIDComDev == NULL) return(FALSE);

    memset(&m_OverlappedRead, 0, sizeof(OVERLAPPED));
    memset(&m_OverlappedWrite, 0, sizeof(OVERLAPPED));

    COMMTIMEOUTS CommTimeOuts;
    CommTimeOuts.ReadIntervalTimeout = 0xFFFFFFFF;
    CommTimeOuts.ReadTotalTimeoutMultiplier = 0;
    CommTimeOuts.ReadTotalTimeoutConstant = 0;
    CommTimeOuts.WriteTotalTimeoutMultiplier = 0;
    CommTimeOuts.WriteTotalTimeoutConstant = 5000;
    SetCommTimeouts(m_hIDComDev, &CommTimeOuts);

    wsprintf(szComParams, L"COM%d:%d,n,8,1", nPort, nBaud);

    m_OverlappedRead.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    m_OverlappedWrite.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

    dcb.DCBlength = sizeof(DCB);
    GetCommState(m_hIDComDev, &dcb);
    dcb.BaudRate = nBaud;
    dcb.ByteSize = 8;
    unsigned char ucSet;
    ucSet = (unsigned char)((FC_RTSCTS & FC_DTRDSR) != 0);
    ucSet = (unsigned char)((FC_RTSCTS & FC_RTSCTS) != 0);
    ucSet = (unsigned char)((FC_RTSCTS & FC_XONXOFF) != 0);
    if (!SetCommState(m_hIDComDev, &dcb) ||
        !SetupComm(m_hIDComDev, 10000, 10000) ||
        m_OverlappedRead.hEvent == NULL ||
        m_OverlappedWrite.hEvent == NULL) {
        DWORD dwError = GetLastError();
        if (m_OverlappedRead.hEvent != NULL) CloseHandle(m_OverlappedRead.hEvent);
        if (m_OverlappedWrite.hEvent != NULL) CloseHandle(m_OverlappedWrite.hEvent);
        CloseHandle(m_hIDComDev);
        return(FALSE);
    }

    m_bOpened = TRUE;


    return(m_bOpened);

}
int Serial::ReadData(void* buffer, int limit)
{

    if (!m_bOpened || m_hIDComDev == NULL)
    {
        //std::cout << "Could not open data\n";
        return(0);
    }

    BOOL bReadStatus;
    DWORD dwBytesRead, dwErrorFlags;
    COMSTAT ComStat;

    ClearCommError(m_hIDComDev, &dwErrorFlags, &ComStat);
    if (!ComStat.cbInQue)
    {
        //std::cout << "No cbInQue\n";
        return(0);
    }

    dwBytesRead = (DWORD)ComStat.cbInQue;
    if (limit < (int)dwBytesRead)
    {
        dwBytesRead = (DWORD)limit;
        //std::cout << "Limit reached\n";
    }

    bReadStatus = ReadFile(m_hIDComDev, buffer, dwBytesRead, &dwBytesRead, &m_OverlappedRead);
    if (!bReadStatus)
    {
        //std::cout << "!bReadStatus\n";
        if (GetLastError() == ERROR_IO_PENDING)
        {
            WaitForSingleObject(m_OverlappedRead.hEvent, 2000);
            //std::cout << "dderror\n";
            return((int)dwBytesRead);
        }
        //std::cout << "Could not read file...\n";
        return(0);
    }
    //std::cout << "Data read!\n";

    return((int)dwBytesRead);

}

int Serial::SendData(const char* buffer, int size)
{

    if (!m_bOpened || m_hIDComDev == NULL) return(0);

    DWORD dwBytesWritten = 0;
    int i;
    for (i = 0; i < size; i++) {
        WriteCommByte(buffer[i]);
        dwBytesWritten++;
    }

    return((int)dwBytesWritten);

}
BOOL Serial::WriteCommByte(unsigned char ucByte)
{
    BOOL bWriteStat;
    DWORD dwBytesWritten;

    bWriteStat = WriteFile(m_hIDComDev, (LPSTR)&ucByte, 1, &dwBytesWritten, &m_OverlappedWrite);
    if (!bWriteStat && (GetLastError() == ERROR_IO_PENDING)) {
        if (WaitForSingleObject(m_OverlappedWrite.hEvent, 1000)) dwBytesWritten = 0;
        else {
            GetOverlappedResult(m_hIDComDev, &m_OverlappedWrite, &dwBytesWritten, FALSE);
            m_OverlappedWrite.Offset += dwBytesWritten;
        }
    }

    return(TRUE);

}
