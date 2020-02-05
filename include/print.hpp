#ifndef __PRINT_H__
#define __PRINT_H__

#ifdef ARDUINO
#include <Arduino.h>
#else // on a pc
#include <iostream>
#endif // ARDUINO

namespace ghr
{

template <typename T>
void __print_dec__(T &&arg)
{
#ifdef ARDUINO
    Serial1.print(arg, DEC);
#else  // on a pc
    std::cout << arg;
#endif // ARDUINO
}

void print(int &&arg)
{
    __print_dec__(arg);
}

void print(unsigned int &&arg)
{
    __print_dec__(arg);
}

void print(short &&arg)
{
    __print_dec__(arg);
}

void print(unsigned short &&arg)
{
    __print_dec__(arg);
}

void print(long &&arg)
{
    __print_dec__(arg);
}

void print(unsigned long &&arg)
{
    __print_dec__(arg);
}

void print(float &&arg)
{
    __print_dec__(arg);
}

void print(double &&arg)
{
    __print_dec__(arg);
}

void print(std::string &arg)
{
    printf("debug: %s", arg.c_str());
}

template <typename T>
void print(T &&arg)
{
#ifdef ARDUINO
    Serial1.print(arg);
#else  // on a pc
    // std::cout << arg;
#endif // ARDUINO
}

template <typename T, typename... Args>
void print(T &&arg, Args &&... args)
{
    print(arg);
    print(args...);
}

} // end namespace ghr

#endif // __PRINT_H_: