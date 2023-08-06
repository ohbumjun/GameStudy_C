<<<<<<< HEAD
=======
#ifndef Logger_h
#define Logger_h

#include <sstream>
#include <iostream>
#include <iomanip>
#include <cstdint>

class ErrorLogMeesage : public std::basic_ostringstream<char>{
    public :
    ~ErrorLogMeesage()
    {
        fprintf(stderr, "Fatal error : %s\n", str().c_str());
        exit(EXIT_FAILURE);
    }
};

#define DIE ErrorLogMeesage()

#define log(value) std::cout << #value << " = " << value << "\n";

#endif
>>>>>>> 4679352424279ce7aca40b30e46392b1d0be4a8a
