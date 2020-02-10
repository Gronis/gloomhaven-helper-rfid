#ifndef __PRINT_H__
#define __PRINT_H__

#ifdef ARDUINO
#include <Arduino.h>
#else // on a pc
#include <iostream>
#endif // ARDUINO

namespace ghh
{

template <typename T>
void __print_dec__(const T &arg)
{
#ifdef ARDUINO
    Serial1.print(arg, DEC);
#else  // on a pc
    std::cout << arg;
#endif // ARDUINO
}

void print(const int &&arg)
{
    __print_dec__(arg);
}

void print(const unsigned int &&arg)
{
    __print_dec__(arg);
}

void print(const short &&arg)
{
    __print_dec__(arg);
}

void print(const unsigned short &&arg)
{
    __print_dec__(arg);
}

void print(const long &&arg)
{
    __print_dec__(arg);
}

void print(const unsigned long &&arg)
{
    __print_dec__(arg);
}

void print(const float &&arg)
{
    __print_dec__(arg);
}

void print(const double &&arg)
{
    __print_dec__(arg);
}

void print(const std::string &&arg)
{
#ifdef ARDUINO
    Serial1.print(arg.c_str());
#else  // on a pc
    std::cout << arg;
#endif // ARDUINO
}

void print(const char* arg)
{
#ifdef ARDUINO
    Serial1.print(arg.c_str());
#else  // on a pc
    std::cout << arg;
#endif // ARDUINO
}

template <typename T>
void print(const T &&arg)
{
#ifdef ARDUINO
    Serial1.print(arg);
#else  // on a pc
    std::cout << arg;
#endif // ARDUINO
}

template <typename T>
void print(const T &arg)
{
#ifdef ARDUINO
    Serial1.print(arg);
#else  // on a pc
    std::cout << arg;
#endif // ARDUINO
}

template <typename T, typename... Args>
void print(const T &arg, const Args &... args)
{
    print(arg);
    print(args...);
}
template <typename T, typename... Args>
void print(const T &&arg, const Args &&... args)
{
    print(arg);
    print(args...);
}

} // end namespace ghh

#endif // __PRINT_H_: