
#ifndef CONSOLE_GRAPHICS_H

#pragma region Exporter 

#ifdef CONSOLE_GRAPHICS_EXPORT

#define CONSOLE_GRAPHICS_API __declspec(dllexport)

#else

#define CONSOLE_GRAPHICS_API __declspec(dllimport)

#endif

#pragma endregion

namespace graphics
{
    /// <summary>
    /// Class that is used to use console graphics functions
    /// </summary>
    struct CONSOLE_GRAPHICS_API ConsoleGraphics
    {
        ConsoleGraphics(HANDLE consoleHandler);

        void Draw();

    private:
        HANDLE _consoleHandler;//Pointer to the console
    };
}

#endif // !CONSOLE_GRAPHICS_H


