#ifndef Logger_h
#define Logger_h

#include <sstream>
#include <iostream>
#include <iomanip>

class ErrorLogMeesage : public std::basic_ostringstream<char>{
    public :
    ~ErrorLogMeesage()
    {
        fprintf(stderr, "Fatal error : %s\n", str().c_str());
        exit(EXIT_FAILURE);
    }
};

#define DIE ErrorLogMeesage()

#define log(value) std::cout << #value << " = " << (value) << "\n"

#endif