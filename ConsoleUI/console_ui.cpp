#include "pch.h"
#include "console_ui.h"

ui::ConsoleUI::ConsoleUI(HANDLE consoleHandler, 
	io::ConsoleInputOutput* inputModule, 
	g::ConsoleGraphics* consoleGraphicsModule)
{
	_consoleHandler = consoleHandler;
	if (!inputModule)
	{
		//_inputModule = new io::ConsoleInputOutput;
		_inpModuleDefUsed = true;
	}
	else
	{
		_inputModule = inputModule;
		_inpModuleDefUsed = false;
	}
	
	if (consoleGraphicsModule)
	{
		_consoleGraphicsModule = consoleGraphicsModule;
		_graphModuleDefaultUsed = true;
	}
	else
	{
		_graphModuleDefaultUsed = false;
	}
	
}

ui::ConsoleUI::~ConsoleUI()
{
	if (_inpModuleDefUsed)
		delete _inputModule;
	if (_graphModuleDefaultUsed)
		delete _consoleGraphicsModule;
}

void ui::ConsoleUI::Print(LPTSTR value)
{
	_inputModule->PrintLine(value);
}

void ui::ConsoleUI::ReadLine(LPTSTR value, size_t size)
{
	_inputModule->ReadLine(value, size);
}

