/** @file FailureInfo.h
 *  @brief Contains structure definition that represents an expectation failure.
*/
#ifndef _FAILUREINFO_H
#define _FAILUREINFO_H

#include "frisk_config.h"

namespace Frisk
{
	typedef struct _FailureInfo {
		/**
		* The name of the file in which the test was defined.
		*/
		std::string fileName;

		/**
		* The name of the function in which the test was executed.
		*/
		std::string testName;

		/**
		* The message associated with the failure; typically the expectation message.
		*/
		std::string message;

    /**
    * User-provided description of the expectation that failed.
    */
    std::string description;

		/**
		* The line number on which the test failed.
		*/
		unsigned int lineNo;
	} FailureInfo;
}

#endif /* _FAILUREINFO_H */
