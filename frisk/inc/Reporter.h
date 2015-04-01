#ifndef _REPORTER_H
#define _REPORTER_H

#include "frisk_config.h"
#include "Test.h"
#include "TestCollection.h"

namespace Frisk
{
	class TestCollection;

	class Reporter
	{
	public:
		Reporter() { }
		virtual ~Reporter() { }

		virtual void onStart(const Frisk::TestCollection &collection) = 0;
		virtual void onPreRun(const Frisk::Test &test) = 0;
		virtual void onPostRun(const Frisk::Test &test) = 0;
		virtual void onComplete() = 0;
	};
}

#endif /* _REPORTER_H */
