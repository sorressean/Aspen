#include "../inc/ConsoleReporter.h"
#include <iostream>
#ifdef _WIN32
#include <Windows.h>
#endif

namespace Frisk
{
	ConsoleReporter::ConsoleReporter()
	{
		this->setColor("fail", CONSOLE_COLOR_RED);
		this->setColor("pass", CONSOLE_COLOR_GREEN);
		this->setColor("section", CONSOLE_COLOR_BLUE);
		this->setColor("text", CONSOLE_COLOR_DEFAULT);
		this->setOption("verbose", true);
    this->testsRun = 0;
    this->testsPassed = 0;
    this->testsFailed = 0;
    this->testsPending = 0;

#ifdef _WIN32
		this->hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
		GetConsoleScreenBufferInfo(this->hStdOut, &this->csbi);
#endif
	}

	ConsoleReporter::~ConsoleReporter()
	{

	}

	void ConsoleReporter::setConsoleTextColor(unsigned int color)
	{
		if (color > 3)
			return;

#ifdef _WIN32
		WORD real_colors[] = { FOREGROUND_RED | FOREGROUND_INTENSITY, FOREGROUND_GREEN | FOREGROUND_INTENSITY, 
			FOREGROUND_BLUE | FOREGROUND_INTENSITY, this->csbi.wAttributes };

		WORD attr = real_colors[color];
		SetConsoleTextAttribute(this->hStdOut, attr);
#else
    int real_colors[] = { 31, 32, 34, 39 };
    std::cout << "\033[" << real_colors[color] << "m";
#endif
	}

	void ConsoleReporter::setColor(const std::string scheme, unsigned int color)
	{
		this->colorScheme[scheme] = color;
	}

	void ConsoleReporter::setOption(const std::string option, bool value)
	{
		this->options[option] = value;
	}

	void ConsoleReporter::onStart(const Frisk::TestCollection &collection)
	{

	}

	void ConsoleReporter::onPreRun(const Frisk::Test &test)
	{
		
	}

	void ConsoleReporter::onPostRun(const Frisk::Test &test)
	{
    this->setConsoleTextColor(this->colorScheme["text"]);
		
    if (test.getOption(FRISK_OPTION_PENDING) != 0)
			std::cout << "Pending test '";
		else
			std::cout << "Running test '";
		std::cout.flush();
		this->setConsoleTextColor(this->colorScheme["section"]);
		std::cout << test.getName();
		std::cout.flush();
		this->setConsoleTextColor(this->colorScheme["text"]);
		std::cout << "'... ";
		if (test.getOption(FRISK_OPTION_PENDING) != 0)
		{
			this->setConsoleTextColor(this->colorScheme["pass"]);
			std::cout << "[SKIPPED]" << std::endl;
			this->setConsoleTextColor(this->colorScheme["text"]);
		}
		std::cout.flush();

    this->testsRun++;

    if (test.getOption(FRISK_OPTION_PENDING) != 0)
    {
      this->testsPending++;
    } 
    else if (test.getOption(FRISK_OPTION_PENDING) == 0 && test.getFailureCount() != 0) 
    {
      this->testsFailed++;
			this->setConsoleTextColor(this->colorScheme["fail"]);
			std::cout << "[FAILED]" << std::endl;
			this->setConsoleTextColor(this->colorScheme["text"]);

			if (this->options["verbose"] == true)
			{
				std::string filePath = test.getFileName();
				if (filePath.length() > 30) {
					filePath = filePath.substr(filePath.length() - 30);
					filePath = "...." + filePath;
				}
				std::cout << "\tFile '" << filePath << "': ";
				std::list<FailureInfo>::const_iterator it = test.getFailureIterator();
				this->setConsoleTextColor(this->colorScheme["text"]);
				std::cout << std::endl;
				while (it != test.getFailureIteratorEnd())
				{
					Frisk::FailureInfo info = (*it);
          if (this->options["description"])
          {
					  std::cout << "\t[" << info.description << " | Ln " << info.lineNo << "]: " << info.message << "\n";
          }
          else
          {
					  std::cout << "\t[Ln " << info.lineNo << "]: " << info.message << "\n";
          }
					it++;
				}
			}
		} else {
			this->setConsoleTextColor(this->colorScheme["pass"]);
			std::cout << "[PASSED]" << std::endl;
			this->setConsoleTextColor(this->colorScheme["text"]);
      this->testsPassed++;
		}
	}

	void ConsoleReporter::onComplete()
	{
		this->setConsoleTextColor(this->colorScheme["text"]);
		std::cout << "Test complete [" << this->testsRun << " TOTAL, ";
    this->setConsoleTextColor(this->colorScheme["pass"]);
    std::cout << this->testsPassed << " PASSED, ";
    if (this->testsFailed != 0) {
      this->setConsoleTextColor(this->colorScheme["fail"]);
    }
    std::cout << this->testsFailed << " FAILED, ";
    this->setConsoleTextColor(this->colorScheme["pass"]);
    std::cout << this->testsPending << " PENDING]\n";
    this->setConsoleTextColor(this->colorScheme["text"]);
	}
}
