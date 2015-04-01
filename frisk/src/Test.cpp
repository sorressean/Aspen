#include "../inc/Test.h"

namespace Frisk
{
	/**
	* Initializes a new instance of a Test.
	*/
	Test::Test()
	{
		this->initialize();
	}

	/**
	*
	*/
	Test::~Test()
	{

	}

	/**
	* Initializes this test with the given values.
	* @param name The name of the test.
	* @param fileName The name of the file in which the test is defined.
	* @param lineNo The line number on which the test is defined.
	* @param options A combination of options that change the behavior of this test.
	*/
	void Test::initialize(const std::string &name, const std::string &fileName,
		unsigned int lineNo, unsigned int options)
	{
		this->name = name;
		this->fileName = fileName;
		this->lineNo = lineNo;
		this->options = options;
	}

	std::string Test::getName() const
	{
		return this->name;
	}

	unsigned int Test::getLineNo() const
	{
		return this->lineNo;
	}

	std::string Test::getFileName() const
	{
		return this->fileName;
	}

	/**
	* Enables or disables the given option.
	* @param option The option to enable or disable.
	* @param value True to enable, false to disable.
	*/
	void Test::setOption(unsigned int option, bool value)
	{		
		unsigned int bval = (unsigned int)value;
		unsigned int pos = bval << option;
		this->options = (this->options & (1 << option)) ^ ((this->options ^ (pos)));
	}

	/**
	* Returns whether or not the specified option is set.
	*/
	bool Test::getOption(unsigned int option) const
	{
		return (bool)(this->options & (unsigned int)(1 << option));
	}

	/**
	* Adds a new failure to the existing queue of failures.
	*/
	void Test::addFailure(const std::string &fileName, const std::string &message, unsigned int lineNo)
	{
		FailureInfo failure;
		failure.fileName = fileName;
		failure.message = message;
		failure.testName = this->name;
		if (failure.testName == "")
			failure.testName = "<unknown>";
		failure.lineNo = lineNo;
		this->failureCount += 1;
		this->failures.push_back(failure);
	}

	/**
	* Adds a new failure to the existing queue of failures.
	*/
	void Test::addFailure(const std::string &fileName, const std::string &message, const std::string &desc, unsigned int lineNo)
	{
		FailureInfo failure;
		failure.fileName = fileName;
		failure.message = message;
		failure.testName = this->name;
		if (failure.testName == "")
			failure.testName = "<unknown>";
    failure.description = desc;
		failure.lineNo = lineNo;
		this->failureCount += 1;
		this->failures.push_back(failure);
	}

	/**
	* Gets the number of failures in the list of failures.
	*/
	unsigned int Test::getFailureCount() const
	{
		return this->failures.size();
	}

	/**
	* Gets an iterator for the failure list.
	*/
	std::list<FailureInfo>::const_iterator Test::getFailureIterator() const
	{
		return this->failures.cbegin();
	}

	/**
	* Gets the end iterator for the failure list.
	*/
	std::list<FailureInfo>::const_iterator Test::getFailureIteratorEnd() const
	{
		return this->failures.cend();
	}

	/**
	* Static function which executes a test and returns the created Test object.
	*/
	Test Test::RunTest(std::function<void(Frisk::Test &)> func)
	{
		Test testObj;
		func(testObj);

		return testObj;
	}
}
