/** @file test.h
 *  @brief Defines the Test class
*/
#ifndef _FRISK_TEST_H
#define _FRISK_TEST_H

#include "frisk_config.h"
#include "FailureInfo.h"
#include <functional>

/**
 * If this flag is set, then even if an expectation is not met, the next expectation will be executed.
 * Default value is not set.
 */
#define FRISK_OPTION_CONTINUE	0x00

/**
 * Specifies that the output of tests should very minimal (no message other than failed test).
 * Default value is not set.
 */
#define FRISK_OPTION_QUIET		0x01

/**
 * Specifies that this test should be skipped.
 */
#define FRISK_OPTION_PENDING	0x02



#define TEST_OPTION(test, opt, val)\
  (test).setOption( (opt), (val) );\
  if ( (opt) == FRISK_OPTION_PENDING && (val)) return;

/**
 * Defines a test using the given arguments. 
 */
#define DEF_TEST(name)\
	void name(Frisk::Test &self)

#define DEF_TEST_METHOD(name)\
  static void name(Frisk::Test &self)

#define BEGIN_TEST(test)\
	(test).initialize(__FUNCTION__, __FILE__, __LINE__, 0)

namespace Frisk
{
	class Test
	{
	protected:
		/**
		 * The queue of failed expectations.
		 */
		std::list<FailureInfo> failures;

		/**
		 * The total number of failures.
		 */
		unsigned int failureCount;
		
		/**
		 * The name of this test.
		 */
		std::string name;

		/**
		 * The file in which this test is defined.
		 */
		std::string fileName;

		/**
		 * The line number on which the test is defined. 
		 */
		unsigned int lineNo;

		/**
		 * Flags specifying options for this test. By default, no flags are set.
		 */
		unsigned int options;
	public:
		/**
		 * Initializes a new instance of a Test.
		 */
		Test();

		/**
		 * 
		 */
		virtual ~Test();

		/**
		 * Initializes this test with the given values.
		 * @param name The name of the test.
		 * @param fileName The name of the file in which the test is defined.
		 * @param lineNo The line number on which the test is defined.
		 * @param options A combination of options that change the behavior of this test.
		 */
		virtual void initialize(const std::string &name = "<unknown>", const std::string &fileName = "<unknown>", 
			unsigned int lineNo = -1, unsigned int options = 0);

		virtual std::string getName() const;
		virtual unsigned int getLineNo() const;
		virtual std::string getFileName() const;

		/**
		 * Enables or disables the given option.
		 * @param option The option to enable or disable.
		 * @param value True to enable, false to disable.
		 */
		virtual void setOption(unsigned int option, bool value);

		/**
		 * Returns whether or not the specified option is set.
		 */
		virtual bool getOption(unsigned int option) const;

		/**
		 * Adds a new failure to the existing queue of failures.
		 */
		virtual void addFailure(const std::string &fileName, const std::string &message, const std::string &desc, unsigned int lineNo);

    /**
		 * Adds a new failure to the existing queue of failures.
		 */
		virtual void addFailure(const std::string &fileName, const std::string &message, unsigned int lineNo);

		/**
		 * Gets the number of failures in the list of failures.
		 */
		virtual unsigned int getFailureCount() const;

		/**
		 * Gets an iterator for the failure list.
		 */
		virtual std::list<FailureInfo>::const_iterator getFailureIterator() const;

		/**
		 * Gets the end iterator for the failure list.
		 */
		virtual std::list<FailureInfo>::const_iterator getFailureIteratorEnd() const;

		/**
		 * Static function which executes a test and returns the created Test object.
		 */
		static Test RunTest(std::function<void(Frisk::Test &)> func);
	};
}

#endif /* _FRISK_TEST_H */
