#ifndef __UTIL__
#define __UTIL__
#include <string>
using namespace std;

float my_sqrt(float number);
string operator+(string &content, int number);
string &operator+=(string &content, int number);

#endif