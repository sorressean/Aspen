#include "../inc/TestCollection.h"
#include <stdexcept>

namespace Frisk
{
	/**
	* Initializes a new instance of TestCollection.
	*/
	TestCollection::TestCollection() : lastFailCount(0)
	{

	}

	/**
	* Destroys this instance of TestCollection.
	*/
	TestCollection::~TestCollection()
	{

	}

	/**
	* Adds a test to the test collection.
	* @param func The function that defines the tests.
	* @param name The name of the test, if it is not initialized or named in the function.
	*/
	void TestCollection::addTest(std::function<void(Frisk::Test &)> func, const std::string &name)
	{
		TestCollection::TestFuncPair pair;
		pair.func = func;
		pair.test.initialize(name);

		this->tests.push_back(pair);
	}

	/**
	* Runs the given tests and returns a list of the test objects.
	* @param failfast If true, then the tests will be run only until the first failure.
	* @returns A list of Test objects.
	*/
	std::list<Frisk::Test> TestCollection::runTests(bool failfast, Reporter *reporter)
	{
    this->lastFailCount = 0;

		std::list<Frisk::Test> result;
		if (reporter != nullptr)
			reporter->onStart(*this);

		std::list<TestCollection::TestFuncPair>::iterator it = this->tests.begin();
		while (it != this->tests.end())
		{
			if (reporter != nullptr)
				reporter->onPreRun((*it).test);

      try
      {
			  (*it).func((*it).test);
      }
      catch(std::exception &ex)
      {
        std::string msg = std::string(ex.what());
        (*it).test.addFailure((*it).test.getFileName(), std::string("Unexpected exception was caught: ") + msg, "Unexpected exception", (*it).test.getLineNo());
      }

			result.push_back((*it).test);

			if (reporter != nullptr)
				reporter->onPostRun((*it).test);
 
      this->lastFailCount += (*it).test.getFailureCount();

			// only quit if failfast AND the test is NOT pending.
			if (failfast && ((*it).test.getFailureCount() != 0) && ((*it).test.getOption(FRISK_OPTION_PENDING) == 0))
				break;

			it++;
		}

		if (reporter != nullptr)
			reporter->onComplete();

		return result;
	}

    /**
     * Returns the number of failures in the last run of tests.
     */
    int TestCollection::getLastFailCount() const
    {
      return this->lastFailCount;
    }
}
