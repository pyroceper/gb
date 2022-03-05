#include "util/logger.h"

namespace Logger 
{
    bool debug = false;
    void print(const std::string &str)
    {
        if(debug)
            fmt::print(str.c_str());
    }

}