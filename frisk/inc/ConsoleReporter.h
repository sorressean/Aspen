#ifndef _CONSOLE_REPORTER_H
#define _CONSOLE_REPORTER_H

#include "Reporter.h"
#include <map>

#ifdef _WIN32
#include <Windows.h>
#endif

#define CONSOLE_COLOR_RED	0x00
#define CONSOLE_COLOR_GREEN 0x01
#define CONSOLE_COLOR_BLUE	0x02
#define CONSOLE_COLOR_DEFAULT 0x03

namespace Frisk
{
	class ConsoleReporter : public Reporter
	{
	protected:
		std::map<std::string, unsigned int> colorScheme;
		std::map<std::string, bool> options;
    int testsRun;
    int testsPassed;
    int testsFailed;
    int testsPending;

	private:
		void setConsoleTextColor(unsigned int color);
#ifdef _WIN32
		CONSOLE_SCREEN_BUFFER_INFO csbi;
		HANDLE hStdOut;
#endif
	public:
		ConsoleReporter();
		virtual ~ConsoleReporter();

		virtual void setColor(const std::string scheme, unsigned int color);
		virtual void setOption(const std::string option, bool value);

		virtual void onStart(const Frisk::TestCollection &collection);
		virtual void onPreRun(const Frisk::Test &test);
		virtual void onPostRun(const Frisk::Test &test);
		virtual void onComplete();
	};
}

#endif /* _CONSOLE_REPORTER_H */
