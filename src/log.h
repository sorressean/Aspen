#pragma once
#ifndef LOG_H
#define LOG_H
#include <string>
#include <fstream>
#include "mud.h"
#include "conf.h"

/*
*Logger class.
*Used to log messages to both console and files.
*/
class Log
{
    FILE* _out;
    std::string _name;
public:
    Log(const std::string &file);
    ~Log();
    /*
    *Writes the data to the console and the file if possible.
    *Param: [in] The message to write.
    */
    void Write(const std::string &data,LOG_LEVEL l=INFORM);
    /*
    *Checks to see if the size of the current log file
    *Is not over the set size.
    *If the file is larger than LOG_MAXSIZE, the files will be cycled out.
    */
    void CheckSize();
    /*
    *Cycles through the files, renaming older files up to the maximum limit.
    */
    void CycleFiles();
};
#endif
