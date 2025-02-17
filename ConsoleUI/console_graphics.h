
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
    struct CONSOLE_GRAPHICS_API ConsoleGraphics
    {
        ConsoleGraphics(HANDLE consoleHandler);

        void Draw();

    private:
        HANDLE _consoleHandler;
    };
}

#endif // !CONSOLE_GRAPHICS_H


