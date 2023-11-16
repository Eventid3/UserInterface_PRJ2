#include <Windows.h>
#include <string>
#include <iostream>
#include <fstream>

class UserInterface {
public:
    UserInterface(int port, int baud);
    ~UserInterface();

    void Run();

private:
    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    void GetMenu(HWND hwnd);
    void ReadLog();
    void ChangeThreshold(HWND hwnd);
    void PrintBuffer(HWND hwnd);
    void ClearBuffer();
    void HandleInput(HWND hwnd, WPARAM wParam);

    HWND m_hwnd;
    int m_port;
    int m_baud;
    bool m_running;
    std::wstring m_buffer;
};

UserInterface::UserInterface(int port, int baud) : m_port(port), m_baud(baud), m_running(true), m_buffer(L"") {
    // Initialize the window
    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = L"UserInterfaceWindow";

    RegisterClass(&wc);

    m_hwnd = CreateWindowEx(
        0,                              // Optional window styles.
        wc.lpszClassName,               // Window class
        L"User Interface",              // Window text
        WS_OVERLAPPEDWINDOW,            // Window style

        // Size and position
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

        NULL,       // Parent window
        NULL,       // Menu
        GetModuleHandle(NULL),  // Instance handle
        this        // Additional application data
    );

    if (m_hwnd == NULL) {
        MessageBox(NULL, L"Window Creation Failed!", L"Error!", MB_ICONEXCLAMATION | MB_OK);
        exit(-1);
    }

    ShowWindow(m_hwnd, SW_SHOWNORMAL);
    UpdateWindow(m_hwnd);
}

UserInterface::~UserInterface() {
    DestroyWindow(m_hwnd);
}

void UserInterface::Run() {
    MSG msg = {};

    while (m_running) {
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
}

LRESULT CALLBACK UserInterface::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    UserInterface* pUI;

    if (uMsg == WM_NCCREATE) {
        CREATESTRUCT* pCreate = (CREATESTRUCT*)lParam;
        pUI = (UserInterface*)pCreate->lpCreateParams;
        SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)pUI);

        pUI->m_hwnd = hwnd;
    }
    else {
        pUI = (UserInterface*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
    }

    if (pUI) {
        switch (uMsg) {
        case WM_CREATE:
            pUI->GetMenu(hwnd);
            break;

        case WM_COMMAND:
            pUI->HandleInput(hwnd, wParam);
            break;

        case WM_DESTROY:
            pUI->m_running = false;
            PostQuitMessage(0);
            break;

        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
        }
    }

    return 0;
}

void UserInterface::GetMenu(HWND hwnd) {
    CreateWindow(
        L"BUTTON",                      // Predefined class; Unicode assumed
        L"Toggle LED",                  // Button text
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles
        10, 10, 100, 30,                 // x, y, width, height
        hwnd,                           // Parent window
        (HMENU)1,                       // Menu or control ID
        GetModuleHandle(NULL),          // Instance handle
        NULL);                          // Additional application data

    CreateWindow(
        L"BUTTON",                      // Predefined class; Unicode assumed
        L"Read Current Temperature",   // Button text
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles
        10, 50, 200, 30,                 // x, y, width, height
        hwnd,                           // Parent window
        (HMENU)2,                       // Menu or control ID
        GetModuleHandle(NULL),          // Instance handle
        NULL);                          // Additional application data

    CreateWindow(
        L"BUTTON",                      // Predefined class; Unicode assumed
        L"Change Temperature Threshold",// Button text
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles
        10, 90, 250, 30,                 // x, y, width, height
        hwnd,                           // Parent window
        (HMENU)3,                       // Menu or control ID
        GetModuleHandle(NULL),          // Instance handle
        NULL);                          // Additional application data

    CreateWindow(
        L"BUTTON",                      // Predefined class; Unicode assumed
        L"Read Temperature Threshold", // Button text
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles
        10, 130, 200, 30,                // x, y, width, height
        hwnd,                           // Parent window
        (HMENU)4,                       // Menu or control ID
        GetModuleHandle(NULL),          // Instance handle
        NULL);                          // Additional application data

    CreateWindow(
        L"BUTTON",                      // Predefined class; Unicode assumed
        L"Get Menu",                    // Button text
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles
        10, 170, 100, 30,                // x, y, width, height
        hwnd,                           // Parent window
        (HMENU)9,                       // Menu or control ID
        GetModuleHandle(NULL),          // Instance handle
        NULL);                          // Additional application data

    CreateWindow(
        L"BUTTON",                      // Predefined class; Unicode assumed
        L"Quit",                        // Button text
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles
        10, 210, 100, 30,                // x, y, width, height
        hwnd,                           // Parent window
        (HMENU)0,                       // Menu or control ID
        GetModuleHandle(NULL),          // Instance handle
        NULL);                          // Additional application data
}

void UserInterface::ReadLog() {
    std::wstring logLine;
    std::wifstream log(L"log.txt");

    while (std::getline(log, logLine)) {
        std::wcout << logLine << L"\n";
    }

    log.close();
}

void UserInterface::ChangeThreshold(HWND hwnd) {
    float input{};
    std::wcout << L"Enter new temperature threshold: ";
    std::wcin >> input;

    // TODO: check for valid input

    std::wstring buffer;
    buffer = std::to_wstring(input);
    std::wcout << buffer << L"\n";

    // Send data through serial port
    // m_Serial->SendData(buffer.c_str(), buffer.size());
}

void UserInterface::PrintBuffer(HWND hwnd) {
    // TODO: Print the buffer in the GUI (consider using a label or text box)
}

void UserInterface::ClearBuffer() {
    m_buffer.clear();
}

void UserInterface::HandleInput(HWND hwnd, WPARAM wParam) {
    switch (LOWORD(wParam)) {
    case 1:
        // Toggle LED
        // m_Serial->SendData("1", 1);
        break;
    case 2:
        // Read Current Temperature
        // m_Serial->SendData("2", 1);
        // m_Serial->ReadDataWaiting();
        // ClearBuffer();
        // m_Serial->ReadData(&m_Buffer, 10);
        // PrintBuffer(hwnd);
        break;
    case 3:
        // Change Temperature Threshold
        // m_Serial->SendData("3", 1);
        // ChangeThreshold(hwnd);
        break;
    case 4:
        // Read Temperature Threshold
        // m_Serial->SendData("4", 1);
        // m_Serial->ReadDataWaiting();
        // ClearBuffer();
        // m_Serial->ReadData(&m_Buffer, 10);
        // PrintBuffer(hwnd);
        break;
    case 9:
        // Get Menu
        // TODO: Clear the window content and redraw the menu
        break;
    case 0:
        // Quit
        m_running = false;
        break;
    default:
        break;
    }
}