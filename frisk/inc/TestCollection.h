#ifndef _TEST_COLLECTION_H
#define _TEST_COLLECTION_H

#include "Test.h"
#include <functional>
#include "Reporter.h"

namespace Frisk
{
	class Reporter;
	class TestCollection
	{
		typedef struct _TestFuncPair {
			Test test;
			std::function<void(Frisk::Test &)> func;
		} TestFuncPair;

	protected:
		/**
		 * The list of tests that this collection should execute.
		 */
		std::list<TestCollection::TestFuncPair> tests;

    /**
     * The number of failures in the last run of tests.
     */
    int lastFailCount;

	public:
		/**
		 * Initializes a new instance of TestCollection.
		 */
		TestCollection();

		/**
		 * Destroys this instance of TestCollection.
		 */
		virtual ~TestCollection();

		/**
		 * Adds a test to the test collection.
		 * @param func The function that defines the tests.
		 * @param name The name of the test, if it is not initialized or named in the function.
		 */
		virtual void addTest(std::function<void(Frisk::Test &)> func, const std::string &name = "");

		/**
		 * Runs the given tests and returns a list of the test objects.
		 * @param failfast If true, then the tests will be run only until the first failure.
		 * @returns A list of Test objects.
		 */
		virtual std::list<Frisk::Test> runTests(bool failfast = true, Reporter *reporter = nullptr);

    /**
     * Returns the number of failures in the last run of tests.
     */
    virtual int getLastFailCount() const;
	};
}

#endif /* _TEST_COLLETION_H */
