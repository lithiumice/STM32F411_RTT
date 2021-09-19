#include "main.h"
#include <string>
using namespace std;

//////////////////////////////////////////////////////////
string operator+(string &content, int number)
{
  string temp = "";
  char t = 0;
  while (true)
  {
    t = number % 10 + '0';
    temp = t + temp;
    number /= 10;
    if (number == 0)
    {
      return content + temp;
    }
  }
}

string &operator+=(string &content, int number)
{
  return content = content + number;
}

//////////////////////////////////////////////////////////
void push_and_pull(float *buff, int len, float newdata)
{
  for (int i = len - 1; i > 0; i--)
  {
    *(buff + i) = *(buff + i - 1);
  }
  *buff = newdata;
}

void push_int_pull(int *buff, int len, int newdata)
{
  for (int i = len - 1; i > 0; i--)
  {
    *(buff + i) = *(buff + i - 1);
  }
  *buff = newdata;
}

int clip(int x, int low, int up)
{
  return x > up ? up : x < low ? low
                               : x;
}

float fclip(float x, float low, float up)
{
  return x > up ? up : x < low ? low
                               : x;
}
