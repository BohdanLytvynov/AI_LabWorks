#ifndef CONSOLE_UI_H

#define CONSOLE_UI_H

#include"console_io.h"
#include"console_graphics.h"

#pragma region Exporter Macro

#ifdef CONSOLE_UI_EXPORT

#define CONSOLE_UI_API __declspec(dllexport)

#else

#define CONSOLE_UI_API __declspec(dllimport)

#endif // CONSOLE_UI_EXPORT

#pragma endregion

namespace ui 
{    
    namespace g = graphics;

    struct CONSOLE_UI_API ConsoleUI
    {
        ConsoleUI(HANDLE consoleHandler, 
            io::ConsoleInputOutput* inputModule = nullptr, 
            g::ConsoleGraphics* consoleGraphicsModule = nullptr);

        ~ConsoleUI();

        void Print(LPTSTR value);

        void ReadLine(LPTSTR value, size_t size);

        template<class Tout>
        Tout Input(LPCTSTR msg, std::function<Tout(LPTSTR result, LPTSTR error, int& error_code)> converter,
            std::function<bool(Tout result, LPSTR error)> validator)
        {
            return _inputModule->Input<Tout>(msg, converter, validator);
        }

    private:
        io::ConsoleInputOutput* _inputModule;        
        g::ConsoleGraphics* _consoleGraphicsModule;
        HANDLE _consoleHandler;

        bool _inpModuleDefUsed;
        bool _graphModuleDefaultUsed;
    };
}

#endif // !CONSOLE_UI_H


