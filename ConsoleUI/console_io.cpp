#include"pch.h"
#include"console_io.h"

io::ConsoleInputOutput::ConsoleInputOutput(HANDLE console, 
    WORD defForegroundColor, WORD defBackGroundColor,
    WORD defErrorForegroundColor, WORD defErrorBackgroundColor)
{
    m_consoleHandle = console;
    m_def_foreground = defForegroundColor;
    m_def_background = defBackGroundColor;
    m_def_errorForeground = defErrorForegroundColor;
    m_def_errorBackground = defErrorBackgroundColor;
}

io::ConsoleInputOutput::ConsoleInputOutput(HANDLE console)
    : ConsoleInputOutput(console, 
        FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY,
        0,
        FOREGROUND_RED,
        0
        )
{
}

io::ConsoleInputOutput::ConsoleInputOutput(HANDLE console, WORD defForegroundColor, WORD defBackGroundColor)
    :ConsoleInputOutput(console, defForegroundColor, defBackGroundColor, 
        FOREGROUND_RED, 0)
{
}

io::ConsoleInputOutput::ConsoleInputOutput(HANDLE console, WORD defErrorForegroundColor, WORD defErrorBackgroundColor ...)
    : ConsoleInputOutput(console, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY,
        0, defErrorForegroundColor, defErrorBackgroundColor)
{

}

void io::ConsoleInputOutput::PrintLine(LPCTSTR value)
{
#ifdef _UNICODE
	std::wcout << value << std::endl;
#else //Prj builds using ASCII
	std::cout << value << std::endl;
#endif
}

void io::ConsoleInputOutput::PrintLine(LPCTSTR value, WORD foreground, WORD background)
{
    SetConsoleTextAttribute(m_consoleHandle, foreground | background);
#ifdef _UNICODE
    std::wcout << value << std::endl;
#else //Prj builds using ASCII
    std::cout << value << std::endl;
#endif
    SetConsoleTextAttribute(m_consoleHandle, m_def_foreground | m_def_background);
}

void io::ConsoleInputOutput::PrintLine(const char* value, WORD foreground, WORD background)
{
    SetConsoleTextAttribute(m_consoleHandle, foreground | background);
    std::cout << value << std::endl;
    SetConsoleTextAttribute(m_consoleHandle, m_def_foreground | m_def_background);
}

void io::ConsoleInputOutput::ReadLine(LPTSTR value, size_t length)
{
#ifdef _UNICODE
	std::wcin.getline(value, length);
#else //Prj builds using ASCII
	std::cin.getline(value, length);
#endif
}

void io::ConsoleInputOutput::PrintLine(const char* value)
{
    std::cout << value << std::endl;
}

void io::ConsoleInputOutput::ReadLine(char* value, size_t length)
{
    std::cin.getline(value, length);
}

void io::ConsoleInputOutput::Input(LPCTSTR msg, LPTSTR buff, std::function<bool(LPTSTR result, LPCTSTR error)> validator)
{
    LPTSTR temp = nullptr;
    LPTSTR error = nullptr;
    size_t count = 1024;
    do
    {
        if (msg)
        {
            PrintLine(msg);
        }
        
        temp = new TCHAR[count];
        error = new TCHAR[count];
        
        ReadLine(temp, count);

        if (validator && !validator(temp, error))
        {
            PrintLine(TEXT("Error during validation!"), m_def_errorForeground, m_def_errorBackground);
            PrintLine(error);
            delete[] temp;
            delete[] error;
            continue;
        }

        break;

    } while (true);
    
    for (size_t i = 0; i < count; ++i)
    {
        buff[i] = temp[i];
    }

    delete[] temp;
    delete[] error;
}

void io::ConsoleInputOutput::Input(const char* msg, char*& buff, std::function<bool(char* result, const char* error)> validator)
{
    char* temp = nullptr;
    char* error = nullptr;
    size_t count = 1024;
    do
    {
        if (msg)
        {
            PrintLine(msg);
        }

        temp = new char[count];
        error = new char[count];

        ReadLine(temp, count);

        if (validator && !validator(temp, error))
        {
            PrintLine("Error during validation!", m_def_errorForeground, m_def_errorBackground);
            PrintLine(error);
            delete[] temp;
            delete[] error;
            continue;
        }

        break;

    } while (true);

    int act_count = std::strlen(temp);

    buff = new char[act_count];

    for (size_t i = 0; i < act_count; ++i)
    {
        buff[i] = temp[i];
    }

    delete[] temp;
    delete[] error;
}


