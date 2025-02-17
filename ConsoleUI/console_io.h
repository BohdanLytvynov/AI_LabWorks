#ifndef CONSOLE_IO_H

#define CONSOLE_IO_H

#include<functional>

#pragma region Exporter Macro

#ifdef CONSOLE_IO_EXPORT

#define CONSOLE_IO_API __declspec(dllexport)

#else

#define CONSOLE_IO_API __declspec(dllimport)

#endif // CONSOLE_UI_EXPORT

#pragma endregion

namespace io
{        
    struct CONSOLE_IO_API ConsoleInputOutput
    {
        ConsoleInputOutput(HANDLE console, 
            WORD defForegroundColor,
            WORD defBackGroundColor, 
            WORD defErrorForegroundColor,
            WORD defErrorBackgroundColor);

        explicit ConsoleInputOutput(HANDLE console);

        ConsoleInputOutput(HANDLE console, WORD defForegroundColor, WORD defBackGroundColor);

        ConsoleInputOutput(HANDLE console, WORD defErrorForegroundColor, WORD defErrorBackgroundColor ...);

        void PrintLine(LPCTSTR value);   
        void ReadLine(LPTSTR value, size_t length);

        void PrintLine(const char* value);
        void ReadLine(char* value, size_t length);

        void PrintLine(LPCTSTR value, WORD foreground, WORD background);        

        void PrintLine(const char* value, WORD foreground, WORD background);

        template<class Tout>
        Tout Input(LPCTSTR msg, 
            std::function<Tout(LPTSTR result, LPCTSTR error, int& error_code)> converter, 
            std::function<bool(Tout result, LPCTSTR error)> validator = nullptr)
        {
            if (!converter)
                throw std::exception("converter parameter was not set!");

            Tout result;
            LPCTSTR error = nullptr;
            LPTSTR temp = nullptr;
            int error_code;
            do
            {
                error_code = 0;
                temp = new TCHAR[1024];
                error = new TCHAR[1024];
                if (msg)
                {
                    PrintLine(msg);
                }

                ReadLine(temp, sizeof(temp));

                result = converter(temp, error, error_code);

                if (error_code != 0)//Error during conversion!
                {
                    PrintLine(TEXT("Error during conversion!"), m_def_errorForeground, m_def_errorBackground);
                    PrintLine(error);
                    delete[] temp;
                    delete[] error;
                    continue;
                }

                if (validator && !validator(result, error))
                {
                    PrintLine(TEXT("Error during validation!"), m_def_errorForeground, m_def_errorBackground);
                    PrintLine(error);
                    delete[] temp;
                    delete[] error;
                    continue;
                }

                delete[] temp;
                delete[] error;

                break;

            } while (true);

            return result;
        }

        template<class Tout>
        Tout Input(const char* msg,
            std::function<Tout(char* result,const char* error, int& error_code)> converter,
            std::function<bool(Tout result,const char* error)> validator = nullptr)
        {
            if (converter)
                throw std::exception("converter parameter was not set!");

            Tout result;
            char* error = nullptr;
            char* temp = nullptr;
            int error_code;
            do
            {
                error_code = 0;
                temp = new char[1024];
                error = new char[1024];
                if (msg)
                {
                    PrintLine(msg);
                }

                ReadLine(temp, sizeof(temp));

                result = converter(temp, error, error_code);

                if (error_code != 0)//Error during convertion!
                {
                    PrintLine("Error during conversion!", m_def_errorForeground, m_def_errorBackground);
                    PrintLine(error);
                    delete[] temp;
                    delete[] error;
                    continue;
                }

                if (validator && !validator(result, error))
                {
                    PrintLine("Error during validation!", m_def_errorForeground, m_def_errorBackground);
                    PrintLine(error);
                    delete[] temp;
                    delete[] error;
                    continue;
                }

                delete[] temp;
                delete[] error;

                break;

            } while (true);

            return result;
        }

        void Input(LPCTSTR msg, LPTSTR buff, std::function<bool(LPTSTR result, LPCTSTR error)> validator = nullptr);

        void Input(const char* msg, char*& buff, std::function<bool(char* result, const char* error)> validator = nullptr);
        private:
            HANDLE m_consoleHandle;
            WORD m_def_foreground;
            WORD m_def_background;
            WORD m_def_errorForeground;
            WORD m_def_errorBackground;
    };    
}

#endif // !CONSOLE_IO_H



