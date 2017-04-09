#ifndef UTILS_H
#define UTILS_H
#include <string>
#include <map>
#include <list>
#include <vector>
#include <sys/time.h>
#include "mud.h"
#include "conf.h"
#include "editor.h"

//misc
int tonum(const char* str);
bool isnum(const char* str);
BOOL FileExists(const std::string &name);
/*
*Checks to see if a directory exists.
*Useful for creating empty dirs after a git checkout.
*/
bool DirectoryExists(const std::string& path);
/*
*Gets the size of the file specified.
*Param: [in] the fd of the file.
*Return: the size of the file in bytes.
*/
UINT GetFileSize(int fd);
/*
*Retrieves the size of the specified file.
*Param: [in] the name of the file.
*return: the size of the file.
*/
UINT GetFileSize(const char* path);
UINT GetFileSize(const std::string &path);
/*
*Tokenizes the string into chunks based on the delimiter.
*Param: [in] the string to tokenize.
*[out] a vector of strings, where the parts will go.
*param: [in] The delimiter.
*Return: void
*note: The slices will be put into tokens. If no tokens are found, the vector will contain the string passed in.
*/
void Tokenize(const std::string &str, std::vector<std::string> &tokens, const std::string &del=" ");
/*
*Splits a string up into a vector based on columns.
*We split to the last word, rather than cutting a word in half and wrapping it to the next column.
*Param: [in] the string to split.
*Param: [in] a pointer to the vector that will accept the lines.
*Param: [in] the number of columns to split to.
*/
void SplitToVector(const std::string &line, std::vector<std::string>* output, int cols=80);
/*
*Converts a timeval struct into a readable string.
*format: h:m:s.ms (where ms is a percentage of a second).
*Param: [in] a pointer to the timeval struct.
*Return: A newly created string.
*/
std::string TimevalToString(struct timeval *tv);
//player validation functions
BOOL IsValidUserName(const std::string &input);
BOOL IsValidPassword(const std::string &input);
BOOL PlayerExists(const std::string &name);
BOOL IsFirstUser();
//string functions:
void Lower(std::string &str);
std::string Capitalize(const std::string &str);
std::string Repeat(const std::string &filler,const int count);
std::string Repeat(const char filler, const int count);
std::string Center(const std::string &str,const int width);
std::string CenterLines(const std::string &str, const int width);
std::string Explode(std::vector <std::string> &parts, const std::string &del = " ");
std::string Explode(const std::vector<std::string>& parts, std::vector<std::string>::const_iterator it, const std::string &del = " ");
std::string StripWhitespace(const std::string &str);
void NumberToString(char* buffer, int number);
//bitfield manipulation
FLAG BitSet(FLAG flag,int pos);
FLAG BitClear(FLAG flag,int pos);
BOOL BitIsSet(FLAG flag,int pos);
std::string GenerateUuid();
//minmax
template <typename T>
inline T Min(const T a,const T b)
{
    return (a<b?a:b);
}
template <typename T>
inline T Max(const T a,const T b)
{
    return (a>b?a:b);
}

/*
*converts a list of strings to an english list (a, b, c, and d).
*Param: [in] a pointer to a list of std::string.
*Return: the string converted.
*/
std::string EnglishList(std::vector <std::string> *in);
/*
*Simulates dice rolls, using x number of dice with a set number of sides.
*Param: [in] the number of dice.
*[in] the number of sides.
*Return: the sum of the result.
*/
struct Dice
{
    int num; //the number of dice
    int sides; //the number of sides per dice
};
int Roll(int n,int s);
int Roll(const Dice& d);
/*
*Gets the name of the opposite exit.
*Param: [in] the name of the exit.
*Return: The opposite exit from the one specified, or "" if the exit was unknown.
*For example, if you provide north, the exit will be south, east will be west, etc.
*/
std::string SwapExit(const std::string &exit);
/*
*Checks to see if the exit name given is a valid exit.
*Param: [in] the name or alias of the exit.\
*return: true on success, false on failure.
*/
BOOL IsValidExit(const std::string &name);
/*
*Returns the exit direction associated with each exit.
*Param: [in] the name of the exit.
*return: The number associated with that direction.
*/
ExitDirection GetDirectionByName(const std::string &name);
/*
*Returns a string with the inputted list of data columnized
*Param: [in] a vector of strings with the data to columnize.
*[in] The number of columns to create.
*[in] The max width of the columns.
*Return: A string with the columnized data.
*/
std::string Columnize(std::vector<std::string> *data, int cols, std::vector<std::string>* headers = NULL, int width = 80);
/*
*Does the math required to calculate a percentage.
*Param: the total value used.
*[in] the actual value (calculated as value/total).
*Param: [in] round: Rounds to the nearest whole percentage; a value of 1 rounds up, anything else will round down.
*Return: The percentage, as an integer.
*/
int Percentage(int total, int count, int round = 0);
//time functions:
class TimeInfo
{
    int hour;
    int minute;
    int second;

    void Initialize();
public:
    TimeInfo(time_t data);
    TimeInfo(UINT data);
    TimeInfo();
    void Calculate(time_t data);
    std::string ToString();
};

class point
{
public:
    int x;
    int y;
    int z;
    point();
    point(int X, int Y, int Z);
    BOOL operator ==(const point &p);
    point& operator =(point &p);
};

int RandomPercent();
int RandomRange(int bottom, int top);
bool iequals(const std::string& a, const std::string& b);
std::string GetPositionString(unsigned int position);
#endif
