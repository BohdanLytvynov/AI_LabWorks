#ifndef CONSOLE_IO_H//Header guard

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
    /// <summary>
    /// Class that encapsulates functions for Console IO operations
    /// </summary>
    struct CONSOLE_IO_API ConsoleInputOutput
    {
        /// <summary>
        /// Main ctor
        /// </summary>
        /// <param name="console">Pointer to the console</param>
        /// <param name="defForegroundColor">Default foreground color in console</param>
        /// <param name="defBackGroundColor">Default background color for console</param>
        /// <param name="defErrorForegroundColor">Default foreground color for displayed error messages in the console</param>
        /// <param name="defErrorBackgroundColor">Default background color for displayed error messages in the console</param>
        ConsoleInputOutput(HANDLE console, 
            WORD defForegroundColor,
            WORD defBackGroundColor, 
            WORD defErrorForegroundColor,
            WORD defErrorBackgroundColor);

        /// <summary>
        /// Ctor that excepts only the pointer to the console
        /// </summary>
        /// <param name="console">Pointer to the console</param>
        explicit ConsoleInputOutput(HANDLE console);

        /// <summary>
        /// Ctor when we want to set only the colors for console
        /// </summary>
        /// <param name="console">Pointer to the console</param>
        /// <param name="defForegroundColor">Default foreground color in console</param>
        /// <param name="defBackGroundColor">Default background color for console</param>
        ConsoleInputOutput(HANDLE console, WORD defForegroundColor, WORD defBackGroundColor);

        /// <summary>
        /// Ctor when we want to set only the colors of error messages for console
        /// </summary>
        /// <param name="console">Pointer to the console</param>
        /// <param name="defErrorForegroundColor">Default foreground color for displayed error messages in the console</param>
        /// <param name="defErrorBackgroundColor">Default background color for displayed error messages in the console</param>
        /// <param name="...">Don't use this part actualy. It is used to overcome the ctor overloading</param>
        ConsoleInputOutput(HANDLE console, WORD defErrorForegroundColor, WORD defErrorBackgroundColor, ...);

        /// <summary>
        /// Prints Line to the console
        /// </summary>
        /// <param name="value">pointer to the string</param>
        void PrintLine(LPCTSTR value);   

        /// <summary>
        /// Reads Line
        /// </summary>
        /// <param name="value">buffer, the line must be read to</param>
        /// <param name="length">length of the buffer</param>
        void ReadLine(LPTSTR value, size_t length);

        /// <summary>
        /// Prints Line to the console, this is used for compatibility with other libs that use c-strings inside
        /// </summary>
        /// <param name="value">pointer to the string</param>
        void PrintLine(const char* value);

        /// <summary>
        /// Reads Line. This is used for compatibility with other libs that use c-strings inside
        /// </summary>
        /// <param name="value">buffer, the line must be read to</param>
        /// <param name="length">length of the buffer</param>
        void ReadLine(char* value, size_t length);

        /// <summary>
        /// Prints line with certain background and foreground colors
        /// </summary>
        /// <param name="value">Pointer to the string</param>
        /// <param name="foreground">Foreground color</param>
        /// <param name="background">Background color</param>
        void PrintLine(LPCTSTR value, WORD foreground, WORD background);        

        /// <summary>
        /// Prints line with certain background and foreground colors, this is used for compatibility with other libs that use c-strings inside
        /// </summary>
        /// <param name="value">Pointer to the string</param>
        /// <param name="foreground">Foreground color</param>
        /// <param name="background">Background color</param>
        void PrintLine(const char* value, WORD foreground, WORD background);

        /// <summary>
        /// Function, that reads value from the console
        /// </summary>
        /// <typeparam name="Tout">The type that we want to get from the function</typeparam>
        /// <param name="msg">Message that we want to show to user before input</param>
        /// <param name="converter">Converter function, that is responsible for converting TCHAR string to the Tout value</param>
        /// <param name="validator">Validator function, that validates the value that comes from the converter</param>
        /// <returns>Some type that we want to get</returns>
        template<class Tout>
        Tout Input(LPCTSTR msg, 
            std::function<Tout(LPCTSTR result, LPCTSTR& error, int& error_code)> converter,
            std::function<bool(Tout result, LPCTSTR& error)> validator = nullptr)
        {
            if (!converter)
                throw std::exception("converter parameter was not set!");

            Tout result;
            LPCTSTR error = nullptr;
            TCHAR temp[1024];
            int error_code;
            do
            {
                error_code = 0;                
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
                    std::memset(temp, 0, sizeof(temp));
                    error = nullptr;
                    continue;
                }

                if (validator && !validator(result, error))
                {
                    PrintLine(TEXT("Error during validation!"), m_def_errorForeground, m_def_errorBackground);
                    PrintLine(error);
                    std::memset(temp, 0, sizeof(temp));
                    error = nullptr;
                    continue;
                }
                
                break;

            } while (true);
            
            return result;
        }

        /// <summary>
        /// Function, that reads value from the console, this is used for compatibility with other libs that use c-strings inside
        /// </summary>
        /// <typeparam name="Tout">The type that we want to get from the function</typeparam>
        /// <param name="msg">Message that we want to show to user before input</param>
        /// <param name="converter">Converter function, that is responsible for converting c-string to the Tout value</param>
        /// <param name="validator">Validator function, that validates the value that comes from the converter</param>
        /// <returns>Some type that we want to get</returns>
        template<class Tout>
        Tout Input(const char* msg,
            std::function<Tout(const char* result, char*& error, int& error_code)> converter,
            std::function<bool(Tout result,char*& error)> validator = nullptr)
        {
            if (converter)
                throw std::exception("converter parameter was not set!");

            Tout result;
            char* error= nullptr;
            char temp[1024];
            int error_code;
            do
            {
                error_code = 0;                
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
                    std::memset(temp, 0, sizeof(temp));
                    error = nullptr;
                    continue;
                }

                if (validator && !validator(result, error))
                {
                    PrintLine("Error during validation!", m_def_errorForeground, m_def_errorBackground);
                    PrintLine(error);
                    std::memset(temp, 0, sizeof(temp));
                    error = nullptr;
                    continue;
                }
                
                break;

            } while (true);

            return result;
        }
        /// <summary>
        /// Function, that gets the input from the console. Here is the version when we want to read TCHAR string without converting it.
        /// </summary>
        /// <param name="msg">Message that we want to show to user before input</param>
        /// <param name="buff">Buffer, where we can store the output</param>
        /// <param name="validator">Validator function, that validates the TCHAR string after input</param>
        void Input(LPCTSTR msg, LPTSTR& buff, std::function<bool(LPCTSTR result, LPTSTR& error)> validator = nullptr);

        /// <summary>
        /// Function, that gets the input from the console. Here is the version when we want to read c-string without converting it.
        /// </summary>
        /// <param name="msg">Message that we want to show to user before input</param>
        /// <param name="buff">Buffer, where we can store the output</param>
        /// <param name="validator">Validator function, that validates the c-string after input</param>
        void Input(const char* msg, char*& buff, std::function<bool(const char* result, char*& error)> validator = nullptr);

        private:
            HANDLE m_consoleHandle;//Pointer to the console
            WORD m_def_foreground;//Def foreground color for console messages
            WORD m_def_background;//Def background color for console messages
            WORD m_def_errorForeground;//Def foreground color for error messages
            WORD m_def_errorBackground;//Def background color for error messages
    };    
}

#endif // !CONSOLE_IO_H



