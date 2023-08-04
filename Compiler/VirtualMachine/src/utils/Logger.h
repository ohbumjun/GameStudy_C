#ifndef Logger_h
#define Logger_h

#include <sstream>

class ErrorLogMeesage : public std::basic_ostringstream<char>{
    public :
    ~ErrorLogMeesage()
    {
        fprintf(stderr, "Fatal error : %s\n", str().c_str());
        exit(EXIT_FAILURE);
    }
};

#endif