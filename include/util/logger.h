#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <fmt/core.h>
namespace Logger
{
    extern bool debug;
    void print(const std::string &str);
}

#endif